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

#include "thread.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <assert.h>

namespace CurrentThread{
    __thread const char* t_threadName = "unnamedThread";
}

namespace{

__thread pid_t t_cachedTid = 0;

void afterFork(){

}

class ThreadNameInitilizer{

};

ThreadNameInitilizer init;

struct ThreadData{
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    std::weak_ptr<pid_t> wkTid_;

    ThreadData (const ThreadFunc& func,
                const std::string& name,
                const std::shared_ptr<pid_t>& tid)
        :func_(func),
         name_(name),
         wkTid_(tid){

    }

    void runInThread(){
        pid_t tid = CurrentThread::tid();
        std::shared_ptr<pid_t> ptid = wkTid_.lock();
        if(ptid){
            *ptid = tid;
            ptid.reset();
        }
        if(!name_.empty()){
            CurrentThread::t_threadName = name_.c_str();
        }
        ::prctl(PR_SET_NAME, CurrentThread::t_threadName);
        func_();
        CurrentThread::t_threadName = "finished";
    }
};


void* startThread(void* obj){
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->runInThread();
    delete data;
    return NULL;
}


}

pid_t CurrentThread::tid(){
    if(t_cachedTid==0){
        t_cachedTid = gettid();
    }
    return t_cachedTid;
}

const char* CurrentThread::name(){
    return t_threadName;
}

bool CurrentThread::isMainThread(){
    return tid()==::getpid();
}

//AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func,const std::string&n)
    :started_(false),
     joined_(false),
     pthreadId_(0),
     tid_(new pid_t(0)),
     func_(func),
     name_(n)
{
    //numCreated_.increment();
}

Thread::~Thread(){
    if(started_&&!joined_){
        pthread_detach(pthreadId_);
    }
}

void Thread::start(){
    assert(!started_);
    started_ = true;

    ThreadData* data = new ThreadData(func_,name_,tid_);
    if(pthread_create(&pthreadId_,NULL,&startThread,data))
    {
        started_ = false;
        delete data;
        abort();
    }
}

void Thread::join(){
    assert(started_);
    assert(!joined_);
    joined_ = true;
    pthread_join(pthreadId_,NULL);
}
