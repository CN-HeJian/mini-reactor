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

//什么时候监听读事件，在这里我们只会将最早到期的时刻设置为过期时刻，减少系统调用次数
void resetTimerfd(int timerfd ,Timestamp expiration){
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue,sizeof(newValue));
    bzero(&oldValue,sizeof(oldValue));
    newValue.it_value = howMuchTimeFromNow(expiration);
    int ret = ::timerfd_settime(timerfd,0,&newValue,&oldValue);
    std::cout<<"ret: the return of timerfd_settime: "<<ret<<std::endl;
    if(ret){
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
    //次处不需要delete channel
    for(TimerLists::iterator it = timers_.begin();it!=timers_.end();it++){
        delete it->second;
    }
}

TimerId TimerQueue::addTimer(const TimerCallback& cb,Timestamp when,double interval){
    Timer* timer = new Timer(cb,when,interval);
    bool earliestChanged = insert(timer);
    //什么意思？？？添加的时间已经过去了
    if(earliestChanged){
        resetTimerfd(timefilefd_,timer->expiration());
    }

    return TimerId(timer);
}

void TimerQueue::handleRead(){
    loop_->assertInLoopThread();
    Timestamp now(Timestamp::now());
    std::vector<Entry> expired = getExpired(now);
    for (std::vector<Entry>::iterator it = expired.begin();it != expired.end(); ++it)
    {
        it->second->run();
    }
    //why???,处理完之后更新啊！！！
    reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now){
    std::vector<Entry> expired;
    //learn 
    Entry sentry = std::make_pair(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
    //查找第一个大于等于sentry的位置
    TimerLists::iterator it = timers_.lower_bound(sentry);
    assert(it==timers_.end()||now<it->first);
    std::copy(timers_.begin(),it,back_inserter(expired));
    //to learn
    timers_.erase(timers_.begin(),it);
    return expired;
}

//处理过期事件之后执行
void TimerQueue::reset(const std::vector<Entry>& expired,Timestamp now){
    Timestamp nextExpire;
    for(std::vector<Entry>::const_iterator it = expired.begin();it!=expired.end();it++){
        if(it->second->repeat()){
            it->second->restart(now);
            insert(it->second);
        }else{
            delete it->second;
        }
    }
    if(!timers_.empty()){
        nextExpire = timers_.begin()->second->expiration();
    }
    //why??
    if(nextExpire.valid()){
        resetTimerfd(timefilefd_,nextExpire);
    }
}

bool TimerQueue::insert(Timer* timer)
{
  bool earliestChanged = false;
  Timestamp when = timer->expiration();
  TimerLists::iterator it = timers_.begin();
  if (it == timers_.end() || when < it->first)
  {
    earliestChanged = true;
  }
  std::pair<TimerLists::iterator, bool> result =
          timers_.insert(std::make_pair(when, timer));
  assert(result.second);
  return earliestChanged;
}