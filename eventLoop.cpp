/**
 * Copyright 2021 icf
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "eventLoop.h"

__thread EventLoop* t_loopInThisThread=0;

EventLoop::EventLoop()
    :looping(false),
    threadId(getpid())
{
    if(t_loopInThisThread){
        std::cout<<"this thread already exit eventloop"<<std::endl;
    }else{
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop(){
    assert(!looping);
    t_loopInThisThread = NULL;
}

EventLoop* EventLoop::getEventLoopOfCurThread(){
    return t_loopInThisThread;
}

void EventLoop::loop(){
    assert(!looping);
    assertInLoopThread();
    looping = true;

    poll(nullptr,0,5*1000);

    looping = false;
}

bool EventLoop::isInThread() const{
    printf("currentThreadId id %d,threadId is %d\n",getpid(),threadId);
    return threadId == getpid();
}

void EventLoop::assertInLoopThread(){
    if(!isInThread()){
        abortNotInLoopThread();
    }
}

void EventLoop::abortNotInLoopThread(){
    std::cout<<"abortNotInLoopThread"<<std::endl;
}
