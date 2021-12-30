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

#include "inetAddress.h"

#include <string.h>
#include <netinet/in.h>
#include "socketOps.h"
#include <assert.h>

static const in_addr_t kInaddrAny = INADDR_ANY;

static_assert(sizeof(InetAddress)==sizeof(sockaddr_in));

//only port
InetAddress::InetAddress(uint16_t port){
    bzero(&addr_,sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = socketOps::hostToNetwork32(kInaddrAny);
    addr_.sin_port = socketOps::hostToNetwork16(port);
}

//port and ip[网络]
InetAddress::InetAddress(const std::string &ip,uint16_t port){
    bzero(&addr_,sizeof addr_);
    socketOps::fromHostPort(ip.c_str(),port,&addr_);
}

//sockaddr_in
InetAddress::InetAddress(const struct sockaddr_in& addr)
    :addr_(addr){
}

//将网络地址转换为主机地址
std::string InetAddress::toHostPort()const{
    char buf[32];
    //转换为主机地址
    socketOps::toHostPort(buf,sizeof(buf),addr_);
    return buf;
}

//获取主机地址
const struct sockaddr_in& InetAddress::getSockAddrInet() const{
    return addr_;
}

//改变主机地址
void InetAddress::setSockAddrInet(const struct sockaddr_in& addr){
    addr_ = addr;
}
