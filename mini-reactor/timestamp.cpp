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

//静态断言
static_assert(sizeof(Timestamp)==sizeof(uint64_t));

/**
 * @brief 默认构造函数
 *        
 */
Timestamp::Timestamp(/* args */)
    :microSecondsSinceEpoch(0){

}

/**
 * @brief 隐式构造函数
 *        
 */
Timestamp::Timestamp(int64_t microSecondSinceEpoch_)
    :microSecondsSinceEpoch(microSecondSinceEpoch_){

}

/**
 * @brief 析构函数
 *        
 */
Timestamp::~Timestamp(){

}

/**
 * @brief 获取此处的时间
 *        返回TimeStamp
 */
Timestamp Timestamp::now(){
    struct timeval tv;
    //获取此刻的时间
    gettimeofday(&tv,NULL);
    int64_t seconds = tv.tv_sec;
    return Timestamp(seconds*kMicroSecondsPerSeconds+tv.tv_usec);
}

/**
 * @brief 判断时间是否合法
 * 
 */
bool Timestamp::valid()const{
    return microSecondsSinceEpoch>0;
}

/**
 * @brief 返回时间戳的微秒数
 * 
 */
int64_t Timestamp::getMicroSecondsSinceEpoch()const{
    return microSecondsSinceEpoch;
}

/**
 * @brief 返回一个不合法的时间，起始时间
 * 
 */
Timestamp Timestamp::invalid(){
    return Timestamp();
}

//将微秒表示的时间转换为秒表示的时间,并以字符串的形式返回
//1639986827555555--->1639986827.555555
std::string Timestamp::toString() const{
    char buf[32] = {0};
    int64_t seconds = microSecondsSinceEpoch/kMicroSecondsPerSeconds;
    int64_t microseconds = microSecondsSinceEpoch%kMicroSecondsPerSeconds;
    //int snprintf(char* str,size_t size,const char *format,......)
    snprintf(buf,sizeof(buf)-1,"%" PRId64 ".%06" PRId64 "",seconds,microseconds);
    return buf;
}

//将微秒表示的时间转换为日期,并以字符串的形式返回
//1639986827555555--->20211220 07:53:47.555555
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


// int main(){
//     //Timestamp ti(1639986827555555);
//     Timestamp ti(0); //19700101 00:00:00.000000

//     ti.now();
    
//     int64_t _microSecondsSinceEpoch =  ti.getMicroSecondsSinceEpoch();
//     std::cout<<"microSecondsSinceEpoch: "<< _microSecondsSinceEpoch<<std::endl;

//     bool isValid = ti.valid();
//     std::cout<<"Valid: "<<isValid<<std::endl;

//     std::string _s_f = ti.toString();
//     std::cout<<"_s_f: "<<_s_f<<std::endl;

//     std::string _s_format = ti.toFormattedString();
//     std::cout<<"_s_format: "<<_s_format<<std::endl;
// }
