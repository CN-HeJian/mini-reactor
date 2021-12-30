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

#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <string>
#include <netinet/in.h>

class InetAddress{
public:
    //三种不同的构造函数
    explicit InetAddress(uint16_t port);
    InetAddress(const std::string &ip,uint16_t port);
    InetAddress(const struct sockaddr_in& addr);

    std::string toHostPort()const;

    const struct sockaddr_in& getSockAddrInet() const ;

    void setSockAddrInet(const struct sockaddr_in& addr);

private:
    struct sockaddr_in addr_;
};

#endif