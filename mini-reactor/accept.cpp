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

#include "accept.h"

#include "eventLoop.h"
#include "inetAddress.h"
#include "socketOps.h"

#include <boost/bind.hpp>


Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
  : loop_(loop),
    acceptSocket_(socketOps::createNonblockingOrDie()),
    acceptChannel_(loop, acceptSocket_.getfd()),
    listening_(false)
{
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.bindAddress(listenAddr);

  acceptChannel_.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

//套接字监听
void Acceptor::listen()
{
  loop_->assertInLoopThread();
  listening_ = true;
  acceptSocket_.listen();

  //只有监听了才会设置读事件
  acceptChannel_.enableReadEvent();
}

//出现了读事件、表明有新的连接
void Acceptor::handleRead()
{
  loop_->assertInLoopThread();

  //peerAddr 客户端的...
  InetAddress peerAddr(0);
  
  int connfd = acceptSocket_.accept(&peerAddr);

  if (connfd >= 0) {
    if (newConnextionCallBack_) {
      //新的连接：：
      newConnextionCallBack_(connfd, peerAddr);
    } else {
      socketOps::close(connfd);
    }
  }
}

//设置回调函数
void Acceptor::setNewConnectionCallBack(const newConnectCallBack& cb){
      newConnextionCallBack_ = cb;
}

//判断是否正在监听
bool Acceptor::listenning()const{
  return listening_;
}