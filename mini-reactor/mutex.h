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

#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <assert.h>

class MutexLock{
public:
    MutexLock();
    ~MutexLock();

    bool isLockedByThisThread();
    void assertLocked();
    void lock();
    void unlock();
    pthread_mutex_t* getPthreadMutex();

private:

    pthread_mutex_t mutex_;
    pid_t holder_;    
};

//外部封装！！！！

class MutexLockGuard{
public:
    explicit MutexLockGuard(MutexLock &mutex);
    ~MutexLockGuard();
private:
    MutexLock &mutex_;
};


#endif //  MUTEX_H
