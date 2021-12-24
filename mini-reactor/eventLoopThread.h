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

#ifndef EVENTLOOPTHREAD_H
#define EVENTLOOPTHREAD_H

#include "eventLoop.h"


#include "mutex.h"
#include "condition.h"
#include "thread.h"


class EventLoopThread{
public:
    EventLoopThread();
    ~EventLoopThread();

    EventLoop* startLoop();

private:

    void threadFunc();

    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};



#endif