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

#include "condition.h"

Condition::Condition(MutexLock& mutex)
    :mutex_(mutex)
{   
    pthread_cond_init(&pcond_,NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&pcond_);
}

void Condition::wait(){
    pthread_cond_wait(&pcond_,mutex_.getPthreadMutex());
}

bool Condition::waitForSeconds(int seconds){
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME,&abstime);
    abstime.tv_sec += seconds;
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_,mutex_.getPthreadMutex(),&abstime);
}

void Condition::notify(){
    pthread_cond_signal(&pcond_);
}

void Condition::notifyAll(){
    pthread_cond_broadcast(&pcond_);
}   