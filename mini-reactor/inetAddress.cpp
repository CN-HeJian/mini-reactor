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

static const in_addr_t kInaddrAny = INADDR_ANY;

static_assert((sizeof(InetAddress)==sizeof(struct sockaddr_in)));

InetAddress::InetAddress(uint16_t port){
    bzero(&addr_,sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = sockets::hostToNetwork32(kInaddrAny);
    addr_.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string &ip,uint16_t port){
    bzero(&addr_,sizeof addr_);
    sockets::fromHostPort(ip.c_str(),port,&addr_);
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
    :addr_(addr){
}

std::string InetAddress::toHostPort()const{
    char buf[32];
    sockets::toHostPort(buf,sizeof(buf),addr_);
    return buf;
}

const struct sockaddr_in& InetAddress::getSockAddrInet() const{
    return addr_;
}

void InetAddress::setSockAddrInet(const struct sockaddr_in& addr){
    addr_ = addr;
}
