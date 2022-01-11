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

#include "socketOps.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

// uint64_t socketOps::hostToNetwork64(uint64_t host64)
// {
//     return htobe64(host64);
// }

// uint32_t socketOps::hostToNetwork32(uint32_t host32)
// {
//     return htonl(host32);
// }

// uint16_t socketOps::hostToNetwork16(uint16_t host16)
// {
//     return htons(host16);
// }

// uint64_t socketOps::networkToHost64(uint64_t net64)
// {
//     return be64toh(net64);
// }

// uint32_t socketOps::networkToHost32(uint32_t net32)
// {
//     return ntohl(net32);
// }

// uint16_t socketOps::networkToHost16(uint16_t net16)
// {
//     return ntohs(net16);
// }

template<typename To, typename From>
inline To implicit_cast(From const &f) {
    return f;
}

const sockaddr* sockaddr_cast(const struct sockaddr_in*addr){
    return static_cast<const sockaddr*>(implicit_cast<const void*>(addr));
}

sockaddr* sockaddr_cast(struct sockaddr_in* addr){
    return static_cast<sockaddr*>(implicit_cast<void*>(addr));
}

void setNonBlockAndCloseOnExec(int sockfd){
    //设置O_NONBLOCK
    int flags = ::fcntl(sockfd,F_GETFL,0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(sockfd,F_SETFL,flags);
    if(ret){

    }
    //设置FD_CLOEXEC
    flags = ::fcntl(sockfd,F_GETFD,0);
    flags |= FD_CLOEXEC;
    ret =::fcntl(sockfd,F_SETFD,flags);
}

//创建一个套接字
int socketOps::createNonblockingOrDie (){
    //字节流、非阻塞、exe开启一个新的进程会关闭刚才创建的文件描述符
    int sockfd = ::socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC,IPPROTO_TCP);
    if(sockfd<0){
        std::cout<<"create NonBlockingOrDie failed"<<std::endl;
    }
    return sockfd;
}

//绑定一个套接字，通常为套接字命名
void socketOps::bindOrDie (int sockfd,const struct sockaddr_in& addr){
    //通常是使用sockaddr_in,然后强转为sockaddr,这两个结构体的大小是一样的，所以可以直接强转，均为16个字节
    int ret = ::bind(sockfd,sockaddr_cast(&addr),sizeof(addr));
    //失败的原因一般有两种:
    //1)一种是被绑定的地址是锁定的、仅超级权限可以使用
    //2)当前地址正在使用
    if(ret<0){
        std::cout<<"socket::bindOrDie"<<std::endl;
    }
}

//监听
void socketOps::listenOrDie(int sockfd){
    //SOMAXCONN  4096  等待队列的最大长度
    //listen 主要作用是将套接字变成被动的连接监听套接字，listen函数不会阻塞
    int ret = ::listen(sockfd,SOMAXCONN);
    if(ret<0){
        std::cout<<"sockets:listenOrDie"<<std::endl;
    }
}

//接收一个套接字中已经建立的连接
int socketOps::accpet (int sockfd,struct sockaddr_in* addr){
    socklen_t addrlen = sizeof(*addr);
    int connfd = ::accept4(sockfd,sockaddr_cast(addr),&addrlen,SOCK_NONBLOCK|SOCK_CLOEXEC);
    if (connfd < 0)
  {
    int savedErrno = errno;
    std::cout<<"Socket::not accept "<<std::endl;
    //LOG_SYSERR << "Socket::accept";
    switch (savedErrno)
    {
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO: // ???
      case EPERM:
      case EMFILE: // per-process lmit of open file desctiptor ???
        // expected errors
        errno = savedErrno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP:
        // unexpected errors
        std::cout<<"unexpected error of ::accept "<<std::endl;
        //LOG_FATAL << "unexpected error of ::accept " << savedErrno;
        break;
      default:
        std::cout<<"unknown error of ::accept "<<std::endl;
        //LOG_FATAL << "unknown error of ::accept " << savedErrno;
        break;
    }
  }
  return connfd;
}

//关闭套接字
void socketOps::close(int sockfd){
    //close关闭连接的原理是将fd的引用计数减1，只有当fd的引用计数为0时连接才会真正的关闭
    //shutdown(sockfd,flag) 直接关闭
    if(::close(sockfd)){
        std::cout<<"close failed"<<std::endl;
    }
}

//网络地址变成主机地址
//rec addr   des: buf
void socketOps::toHostPort (char *buf,size_t size,const struct sockaddr_in&addr){
    char host[INET_ADDRSTRLEN] = "INVALID";

    //ip: 网络端口号转为主机端口号
    ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
    //port :网络端口号转为主机端口号
    uint16_t port = socketOps::networkToHost16(addr.sin_port);

    snprintf(buf, size, "%s:%u", host, port);
}

//转为网络地址
//rec: ip  des: addr
void socketOps::fromHostPort(const char*ip,uint16_t port,struct sockaddr_in* addr){
    addr->sin_family = AF_INET;
    //port: 主机端口号转为网络端口号
    addr->sin_port = hostToNetwork16(port);
    //ip: 主机地址转为网络地址  p refer presentation, n refer numeric
    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
    {
        std::cout<<"sockets::fromHostPort failed"<<std::endl;
    }
}

//获取套接字的连接信息
struct sockaddr_in socketOps::getLocalAddr(int sockfd)
{
  struct sockaddr_in localaddr;
  bzero(&localaddr, sizeof localaddr);
  socklen_t addrlen = sizeof(localaddr);
  if(::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
  {
    std::cout<<"get local Addr failed"<<std::endl;
    //LOG_SYSERR << "sockets::getLocalAddr";
  }
  return localaddr;
}

int socketOps::getSocketError(int sockfd){
    int optval;

    socklen_t optlen = sizeof optval;

    if(::getsockopt(sockfd,SOL_SOCKET,SO_ERROR,&optval,&optlen)){
        return errno;
    }else{
        return optval;
    }
}