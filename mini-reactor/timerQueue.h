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

/*
    需要实现的功能
    1）管理所有的time
    2）存在一个channel绑定时间描述符
    3）需要有所属的时间循环线程
    4）当可读事件发生时，epool返回调用channel对应的回调函数
    5）依据当前时间找到所有到期时间，处理单个到期时间绑定的回调函数
    6）执行完之后将最新到器的时间作为下一次到期时间
*/

#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include "timestamp.h"
#include "timer.h"
#include "channel.h"
#include "timerId.h"
#include  "callback.h"

#include <set>
#include <vector>
#include <assert.h>



//class EventLoop;
//class Timer;
//class TimerId;

class TimerQueue{
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    //用户调用的接口，添加一个时间接口
    TimerId addTimer(const TimerCallback& cb,Timestamp when,double interval);
    //void addTimerInLoop(Timer* timer);
private:
    void addTimerInLoop(Timer* timer);
    typedef  std::pair<Timestamp,Timer*> Entry;
    typedef  std::set<Entry> TimerLists;
    //Channel读事件对应的回调函数
    void handleRead();
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired,Timestamp now);
    bool insert(Timer *timer);
    //事件循环
    EventLoop *loop_;
    //时间描述符
    const int timefilefd_;
    //文件描述符对应的读写错误事件
    Channel* channelOfTimerfd_;
    //所有的timer
    TimerLists timers_;
};

#endif
