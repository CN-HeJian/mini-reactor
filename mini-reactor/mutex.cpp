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

#include "mutex.h"
#include <pthread.h>
#include <unistd.h>

MutexLock::MutexLock()
    :holder_(0)
{
    pthread_mutex_init(&mutex_,NULL);
}

MutexLock::~MutexLock(){
    assert(holder_==0);
    pthread_mutex_destroy(&mutex_);
}

bool MutexLock::isLockedByThisThread(){
    return holder_ == getpid();
}

void MutexLock::lock(){
    pthread_mutex_lock(&mutex_);
    holder_ = getpid();
}

void MutexLock::unlock(){
    holder_ = 0;
    pthread_mutex_unlock(&mutex_);
}

pthread_mutex_t* MutexLock::getPthreadMutex(){
    return &mutex_;
}

//MutexLockGuard

MutexLockGuard::MutexLockGuard(MutexLock& mutex)
    :mutex_(mutex)
{
    mutex_.lock();
}

MutexLockGuard::~MutexLockGuard(){
    mutex_.unlock();
}
