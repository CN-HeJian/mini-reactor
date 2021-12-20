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
#include "timestamp.h"
#include <inttypes.h>
#include <sys/time.h>

//to learn
static_assert(sizeof(Timestamp)==sizeof(uint64_t));


Timestamp:: Timestamp(/* args */)
    :microSecondsSinceEpoch(0)
{
}

Timestamp::Timestamp(int64_t microSecondSinceEpoch_)
    :microSecondsSinceEpoch(microSecondSinceEpoch_)
{

}

std::string Timestamp::toString() const{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch/kMicroSecondsPerSeconds;
    int64_t microseconds = microSecondsSinceEpoch%kMicroSecondsPerSeconds;
    snprintf(buf,sizeof(buf)-1,"%" PRId64 ".%06" PRId64 "",seconds,microseconds);
    return buf;
}


std::string Timestamp::toFormattedString() const{
    char buf[64] = {0};
    time_t seconds  = static_cast<time_t>(microSecondsSinceEpoch/kMicroSecondsPerSeconds);
    int64_t microSecond = static_cast<int>(microSecondsSinceEpoch%kMicroSecondsPerSeconds);
    //to learn
    struct tm tm_time;
    //to learn
    gmtime_r(&seconds,&tm_time);
    //to learn
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06ld",
      tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
      tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
      microSecond);
    return buf;
}

bool Timestamp::valid()const{
    return microSecondsSinceEpoch>0;
}

int64_t Timestamp::getMicroSecondsSinceEpoch()const{
    return microSecondsSinceEpoch;
}

Timestamp Timestamp::now(){
    //to learn
    struct timeval tv;
    //to learn
    gettimeofday(&tv,NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds*kMicroSecondsPerSeconds+tv.tv_usec);
}

Timestamp Timestamp::invalid(){
    return Timestamp();
}

Timestamp::~Timestamp(){

}


/*
demo
int main(){
    Timestamp ti(1639986827555555);
    ti.now();
    
    int64_t _microSecondsSinceEpoch =  ti.getMicroSecondsSinceEpoch();
    std::cout<<"microSecondsSinceEpoch: "<< _microSecondsSinceEpoch<<std::endl;

    bool isValid = ti.valid();
    std::cout<<"Valid: "<<isValid<<std::endl;

    std::string _s_f = ti.toString();
    std::cout<<"_s_f: "<<_s_f<<std::endl;

    std::string _s_format = ti.toFormattedString();
    std::cout<<"_s_format: "<<_s_format<<std::endl;

}
*/