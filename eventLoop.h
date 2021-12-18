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
//事件循环，一个线程有且仅有一个事件循环
1)创建的时候需要判断有没有
2)使用的时候需要判断是不是在当前线程
//事件循环是不可拷贝的
*/
#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <pthread.h>
#include <assert.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>  


class EventLoop{
public:
    EventLoop();
    ~EventLoop();

    //主循环事件
    void loop();
    
    //假设当前在所属线程
    void assertInLoopThread();

private:
    //判断自身是否在所属线程里面
    bool isInThread() const;

    //不在当前线程触发警报
    void abortNotInLoopThread();

    //判断是否处于事件循环状态，初始化时设置为false
    bool looping;

    //所属线程id
    const pid_t threadId; 

    //返回线程专属的loopEvent,如果不是io线程，则返回的指针可能是空指针
    EventLoop* getEventLoopOfCurThread();
};

#endif
