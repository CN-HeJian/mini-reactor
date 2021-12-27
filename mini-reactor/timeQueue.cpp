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


#include <sys/timerfd.h>
#include "timeQueue.h"

int createTimerfd(){
    int timefd = timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK | TFD_CLOEXEC);
    if(timefd<0){
        std::cout<<"create timefd failed"<<std::endl;
    }
    return timefd;
}

struct timespec howMuchTimeFromNow(Timestamp when){
    int64_t microseconds = when.getMicroSecondsSinceEpoch()-Timestamp::now().getMicroSecondsSinceEpoch();
    if(microseconds<100){
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec =static_cast<time_t>(microseconds/Timestamp::kMicroSecondsPerSeconds);
    ts.tv_nsec = static_cast<long>((microseconds%Timestamp::kMicroSecondsPerSeconds)*1000);
    return ts;
}

void readTimerfd(int timefd,Timestamp now){
    uint64_t howmany;
    ssize_t n = read(timefd,&howmany,sizeof(howmany));
    std::cout<<"TimeQueue::handerRead(): "<<howmany<<" at "<<now.toString()<<std::endl;
    std::cout<<"what is n: "<<n<<" sizeof howmany: "<<sizeof(howmany)<<std::endl;
    if(n!=sizeof(howmany)){
        std::cout<<"failed: readTimefd\n"<<std::endl;
    }
}

/*  函数功能：重置文件描述符号
 *  什么时候监听读事件，在这里我们只会将最早到期的时刻设置为过期时刻，减少系统调用次数
 *  timefd_settime: https://linux.die.net/man/2/timerfd_settime
 *  由于timefd_settime系统调用是需要开销的,每次都只会把第一个最早到期的时间设置为监听定时
 */
void resetTimerfd(int timerfd ,Timestamp expiration){
    struct itimerspec newValue;
    struct itimerspec oldValue;
    //清空整块
    bzero(&newValue,sizeof(newValue));
    bzero(&oldValue,sizeof(oldValue));
    //设置新的时间
    newValue.it_value = howMuchTimeFromNow(expiration);
    //通过文件描述符通知的定时事件，优点是通过select、poll、epoll监视文件描述符号
    int ret = ::timerfd_settime(timerfd,0,&newValue,&oldValue);
    //异常记录，此处需要转化为日志
    if(ret){
        std::cout<<"ret: the return of timerfd_settime: "<<ret<<std::endl;
        std::cout<<"timefd_settime failed" <<std::endl;
    }
}

TimerQueue::TimerQueue(EventLoop* loop)
    :loop_(loop),
    timefilefd_(createTimerfd()),
    channelOfTimerfd_(loop,timefilefd_),
    timers_()
{
    channelOfTimerfd_.setReadCallBack(std::bind(&TimerQueue::handleRead,this));
    channelOfTimerfd_.enableReadEvent();
}

TimerQueue::~TimerQueue(){
    ::close(timefilefd_);
    //此处不需要delete channel
    for(TimerLists::iterator it = timers_.begin();it!=timers_.end();it++){
        delete it->second;
    }
}

/*
  往时间队列里面添加一个Timer,返回一个TimerId:
  Timer 成员： 
       1): TimeCallBack callback_
       2): TimeStamp expiration_
       3): double interval_
       4): bool repeat_
*/
TimerId TimerQueue::addTimer(const TimerCallback& cb,Timestamp when,double interval){
    //回调函数绑定到Timer上
    Timer* timer = new Timer(cb,when,interval);
    bool earliestChanged = insert(timer);
    //更新文件描述符、将最早过期的时间作为系统调用
    if(earliestChanged){
        resetTimerfd(timefilefd_,timer->getExpiration());
    }
    return TimerId(timer);
}

/**
 * @brief 当有定时事件发生时、执行绑定的回调函数、注意这个是Channel的回调函数
 *        找到所有的过期时间、执行过期时间对应的回调函数
 *         
 */
void TimerQueue::handleRead(){
    loop_->assertInLoopThread();
    //获取当前时间
    Timestamp now(Timestamp::now());
    //获取截止到现在的所有时间，使用二分法
    std::vector<Entry> expired = getExpired(now);
    for (std::vector<Entry>::iterator it = expired.begin();it != expired.end(); ++it)
    {
        it->second->run();
    }
    //处理expired
    reset(expired, now);
}

/**
 * @brief 使用二分法获取到期的时间
 *        同时删除TimeLists中已经到期的时间
 */
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now){
    std::vector<Entry> expired;
    //https://www.cnblogs.com/xiangtingshen/p/11064315.html
    Entry sentry = std::make_pair(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
    //查找第一个大于等于sentry的位置,据说效率贼低
    //https://www.cnblogs.com/lipoicyclic/p/15115118.html
    TimerLists::iterator it = timers_.lower_bound(sentry);
    assert(it==timers_.end()||now<it->first);
    //copy不负责分配空间
    //https://www.cnblogs.com/silentNight/p/5508605.html
    std::copy(timers_.begin(),it,back_inserter(expired));
    //[begin，it),左闭右开区间
    timers_.erase(timers_.begin(),it);
    return expired;
}

/**
 * @brief 处理完到期的时间之后、需要重新设置时间
 *        比如将原来的重复定时情况
 */
void TimerQueue::reset(const std::vector<Entry>& expired,Timestamp now){
    Timestamp nextExpire;
    for(std::vector<Entry>::const_iterator it = expired.begin();it!=expired.end();it++){
        if(it->second->repeat()){
            //注意这儿仅仅是inser、并没有使用insert的返回值
            it->second->restart(now);
            insert(it->second);
        }else{
            delete it->second;
        }
    }
    if(!timers_.empty()){
        nextExpire = timers_.begin()->second->getExpiration();
    }
    //存在一个最新到期的时间，更新文件描述符号、使用系统调用添加最新的文件描述符
    if(nextExpire.valid()){
        resetTimerfd(timefilefd_,nextExpire);
    }
}

/**
 * @brief   插入时间戳
 *          如果时间链表是空的、或者新插入的时间是第一个即将过期的时间,则需要重置文件描述符
 *          否则插入即可
 */
bool TimerQueue::insert(Timer* timer){
    bool earliestChanged = false;
    Timestamp when = timer->getExpiration();
    //TimeLists中没有或者新加的是第一个到期的时间戳
    TimerLists::iterator it = timers_.begin();
    if (it == timers_.end() || when < it->first){                                                                                     
        earliestChanged = true;
    }
    //管理Timer的TimeLists中插入Entry
    std::pair<TimerLists::iterator, bool> result = timers_.insert(std::make_pair(when, timer));
    assert(result.second);
    return earliestChanged;
}
