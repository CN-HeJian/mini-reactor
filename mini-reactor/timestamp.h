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
#ifndef  TIMESTAMP_H
#define  TIMESTAMP_H

#include <iostream>
#include <stdint.h>
#include <string.h> 

class  Timestamp
{
private:
    /* data */
    int64_t microSecondsSinceEpoch;

public:
    Timestamp(/* args */);
    explicit Timestamp(int64_t microSecondSinceEpoch_);
    void swap(Timestamp& other){
        std::swap(microSecondsSinceEpoch,other.microSecondsSinceEpoch);
    }
    std::string toString() const;
    std::string toFormattedString()const;

    bool valid()const;
    int64_t getMicroSecondsSinceEpoch()const;
    static Timestamp now();
    static Timestamp invalid();

    static const int kMicroSecondsPerSeconds = 1000*1000;

    ~ Timestamp();
};


inline  bool operator<(Timestamp lhs,Timestamp rhs){
    return lhs.getMicroSecondsSinceEpoch() < rhs.getMicroSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs,Timestamp rhs){
    return lhs.getMicroSecondsSinceEpoch() == rhs.getMicroSecondsSinceEpoch();
}

inline double timeDifference(Timestamp high,Timestamp low){
    int diff = high.getMicroSecondsSinceEpoch()-low.getMicroSecondsSinceEpoch();
    return static_cast<double>(diff)/Timestamp::kMicroSecondsPerSeconds;
}

inline Timestamp addTime(Timestamp timestamp,double seconds){
    int delta = static_cast<int64_t>(seconds*Timestamp::kMicroSecondsPerSeconds);
    return Timestamp(timestamp.getMicroSecondsSinceEpoch()+delta);
}

#endif

