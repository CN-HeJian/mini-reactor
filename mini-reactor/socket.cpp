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

//返回文件描述符
int Socket::getfd()const{
    return sockfd_;
}

//绑定端口
void Socket::bindAddress(const InetAddress& addr){
    //将文件描述符号与端口绑定
    socketOps::bindOrDie(sockfd_,addr.getSockAddrInet());
}

//监听端口
void Socket::listen(){
    //
    socketOps::listenOrDie(sockfd_);
}

//接受连接
int Socket::accept(InetAddress* peeraddr){
    struct sockaddr_in addr;
    bzero(&addr,sizeof addr);
    //addr是用来保存客户端套接字对应的地方，包括客户端IP和端口信息
    int connfd = socketOps::accpet(sockfd_,&addr);
    if(connfd>=0){
        peeraddr->setSockAddrInet(addr);
    }
    return connfd;
}

void Socket::setReuseAddr(bool on){
    int optval = on?1:0;
    //SOL_SOCKET： 套接字级别上设置选项
    //SO_REUSEDADDR: 打开或关闭地址复用功能
    //socket 关闭之后，操作系统不会立即收回对端口的控制权，而是要经历一个等待阶段
    //此时对这个端口绑定就会出错，设置了SO_REUSEDADDR之后才会消除等待时间
    ::setsockopt(sockfd_,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof optval);
}
