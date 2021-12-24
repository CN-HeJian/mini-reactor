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

#include "socket.h"
#include "socketOps.h"
#include "inetAddress.h"
#include <strings.h>

Socket::Socket(int sockfd)
    :sockfd_(sockfd)
{

}

Socket::~Socket(){

}

int Socket::getfd()const{
    return sockfd_;
}

void Socket::bindAddress(const InetAddress& addr){
    sockets::bindOrDie(sockfd_,addr.getSockAddrInet());
}

void Socket::listen(){
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr){
    struct sockaddr_in addr;
    bzero(&addr,sizeof addr);
    int connfd = sockets::accpet(sockfd_,&addr);
    if(connfd>=0){
        peeraddr->setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on){
    int optval = on?1:0;
    ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof optval);
}
