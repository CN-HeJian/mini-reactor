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
// limiations under the License.

#ifndef CONDITION_H
#define CONDITION_H 

#include <pthread.h>
#include <errno.h>
#include "mutex.h"

class Condition
{
public:
    explicit Condition(MutexLock&mutex);
    ~Condition();
    void wait();
    bool waitForSeconds(int seconds);
    void notify();
    void notifyAll();
private:
    /* data */
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};




#endif // !1