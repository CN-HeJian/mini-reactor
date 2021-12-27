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
#include "timer.h"

Timer::Timer(const TimerCallback& cb,Timestamp expiration,double interval)
   :callback_(cb),
    expiration_(expiration),
    interval_(interval),
    repeat_(interval>0)
{

}

Timer::~Timer(){

}

/**
 * @brief 返回到期时间
 * 
 */
Timestamp Timer::getExpiration()const{
    return expiration_;
}

/**
 * @brief 获取是否是重复定时
 * 
 */
bool Timer::repeat()const{
    return repeat_;
}

/**
 * @brief 启动绑定的回调函数
 * 
 */
void Timer::run()const{
    callback_();
}

/**
 * @brief 如果是周期性的定时,则刷新该定时器的到期时间
 *        否则将定时到期时间设置为0
 * 
 */
void Timer::restart(Timestamp now){
    if(repeat_){
        expiration_ = addTime(now,interval_);
    }else{
        expiration_ = Timestamp::invalid();
    }
}
