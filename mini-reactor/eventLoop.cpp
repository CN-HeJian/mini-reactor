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
#include <sys/eventfd.h>

static int createEventfd(){
    int evtfd = ::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
    if(evtfd<0){
        std::cout<<"EventLoop.cpp :创建文件描述符失败"<<std::endl;
    }
    return evtfd;
}


__thread EventLoop* t_loopInThisThread=0;

EventLoop::EventLoop()
    :looping(false),
    threadId(getpid()),
    quit(false),
    poller(new Poller(this)),
    callingPendingFunctors_(false),
    timerQueue_(new TimerQueue(this)),
    wakeupFd(createEventfd()),
    wakeupChannel(new Channel(this,wakeupFd)) 
{
    if(t_loopInThisThread){
        std::cout<<"this thread already exit eventloop"<<std::endl;
    }else{
        t_loopInThisThread = this;
    }

    wakeupChannel->setReadCallBack(std::bind(&EventLoop::handleRead,this));
    //注意，此处在Channel中调用了update,将Channel添加进了poller.
    wakeupChannel->enableReadEvent();
}

EventLoop::~EventLoop(){
    assert(!looping);
    ::close(wakeupFd);
    t_loopInThisThread = NULL;
}

EventLoop* EventLoop::getEventLoopOfCurThread(){
    return t_loopInThisThread;
}

void EventLoop::loop(){
    assert(!looping);
    assertInLoopThread();
    looping = true;
    quit  = false;

    while(!quit){
        printf("loop\n");
        activeChannels.clear();
        poller->poll(2000, &activeChannels);
        for (ChannelList::iterator it = activeChannels.begin();it != activeChannels.end(); ++it)
        {
            printf("findOne\n");
            (*it)->handlerEvent(); //处理事件完成之后再进行终止操作...
        }
        doPendingFunctors();
    }

    looping = false;
}

void EventLoop::wakeup(){
    uint64_t one = 1;
    ssize_t n = ::write(wakeupFd,&one,sizeof one);
    if(n!=sizeof(one)){
        std::cout<<"Eventloop.cpp:write wakeupFd failed"<<std::endl;
    }
}

void EventLoop::handleRead()
{
    uint64_t one = 1;
    ssize_t n = ::read(wakeupFd, &one, sizeof one);
    if(n!=sizeof(one)){ 
        std::cout<<"Eventloop.cpp:Read wakeupFd failed"<<std::endl;
    }
}


void EventLoop::runInLoop(const Functor& cb){
    if(isInThread()){
        cb();
    }else{
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor& cb){
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }
    //如果当前调用queueloop调用不是I/O线程，那么唤醒I/O线程,以便IO线程及时处理
    //如果是当前线程、但是还没有执行到doPendingFunctors(),按照正常逻辑，就算不wakeup，也会被唤醒执行doPendingFunctors()
    //可以减少对IO的读写...
    if(!isInThread() || callingPendingFunctors_){
        wakeup();
    }
}

//执行其他线程的回调函数
void EventLoop::doPendingFunctors(){
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    //锁范围的减少
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(size_t i=0;i<functors.size();i++){
        functors[i](); 
    }

    callingPendingFunctors_ = false;
}

/**
 * @brief  新加一个监听的文件描述符
 * 
 */
void EventLoop::updateChannel(Channel* channel){
    printf("eventloop add a channel\n");
    assertInLoopThread();
    poller->updateChannel(channel);
}

void EventLoop::assertInLoopThread(){
    if(!isInThread()){
        abortNotInLoopThread();
    }
}

void EventLoop::abortNotInLoopThread(){
    std::cout<<"abortNotInLoopThread"<<std::endl;
}

bool EventLoop::isInThread() const{
    //printf("currentThreadId id %d,threadId is %d\n",getpid(),threadId);
    return threadId == getpid();
}

//该函数可以跨线程调用！！！
void EventLoop::setQuit(){
    quit = true;
    if(!isInThread()){
        wakeup();
    }
}

TimerId EventLoop::runAt(const Timestamp& time,const TimerCallback& cb){
    return timerQueue_->addTimer(cb,time,0.0);
    //return  TimerId();
}

TimerId EventLoop::runAfter(double delay,const TimerCallback& cb){
    Timestamp time(addTime(Timestamp::now(),delay));
    return runAt(time,cb);
}

TimerId EventLoop::runEvery(double interval,const TimerCallback& cb){
    Timestamp time(addTime(Timestamp::now(),interval));
    return timerQueue_->addTimer(cb,time,interval);
}
