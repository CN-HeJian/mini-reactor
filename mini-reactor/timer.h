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

#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include "timestamp.h"
#include "callback.h"

class Timer
{
public:
    Timer(const TimerCallback& cb,Timestamp expiration,double interval);

    ~Timer();
    
    Timestamp expiration()const;
    bool repeat()const;
    
    //超时回调函数
    void run()const;
    //
    void restart(Timestamp now);

    //typedef std::function(void()) TimerCallBack;
private:
    //超时的回调函数
    const TimerCallback callback_;
    //到期时间
    Timestamp expiration_;
    //时间间隔
    const double interval_;
    //是否是重复的
    const bool repeat_;
};

#endif
