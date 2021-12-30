// Copyright 2021 icf
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <pthread.h>
#include <assert.h>
#include <iostream>
#include "poller.h"
#include <unistd.h>  
#include <memory>
#include <functional>

#include "timerId.h"
#include "timestamp.h"
#include "callback.h"
#include "timerQueue.h"
#include "mutex.h"

class Channel;
class Poller;
//class TimerQueue;

class EventLoop{
public:
    EventLoop();
    ~EventLoop();

    typedef std::function<void()> Functor;

    //主循环事件
    void loop();

    //退出循环
    void setQuit();

    //假设当前在所属线程
    void assertInLoopThread();

    //往事件循环里面添加一个Channel
    void updateChannel(Channel* channel);

    //添加定时事件
    TimerId runAt(const Timestamp& time,const TimerCallback& cb);
    TimerId runAfter(double delay,const TimerCallback& cb);
    TimerId runEvery(double interval,const TimerCallback& cb);

    //IO线程调用...
    void runInLoop(const Functor& cb);
    void queueInLoop(const Functor& cb);

private:
    //判断自身是否在所属线程里面
    bool isInThread() const;

    //不在当前线程触发警报
    void abortNotInLoopThread();

    //判断是否处于事件循环状态，初始化时设置为false
    bool looping;

    //所属线程id
    const pid_t threadId; 

    //退出标志位
    bool quit; /* atomic */

    //IO复用
    Poller* poller;

    //管理所有的句柄
    typedef std::vector<Channel*> ChannelList;
    ChannelList activeChannels;

    //是否正在执行doPendingFunctors_,减少唤醒次数
    bool callingPendingFunctors_;

    //执行回调函数
    void doPendingFunctors();

    //返回线程专属的loopEvent,如果不是io线程，则返回的指针可能是空指针
    EventLoop* getEventLoopOfCurThread();

    //定时队列
    TimerQueue* timerQueue_;

    //唤醒文件描述符
    int wakeupFd;   

    //
    Channel* wakeupChannel;
    
    //锁
    MutexLock mutex_;

    //唤醒
    void wakeup();

    //象征性读一下
    void handleRead();

    //回调函数集合
    std::vector<Functor> pendingFunctors_;
};

#endif
