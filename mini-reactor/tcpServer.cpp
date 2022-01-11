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

#include "tcpServer.h"
#include "inetAddress.h"
#include "accept.h"
#include <functional>
#include "socketOps.h"

using namespace std::placeholders;

TcpServer::TcpServer(EventLoop* loop,const InetAddress& listenAddr)
    :loop_(loop),
     name_(listenAddr.toHostPort()),
     acceptor_(new Acceptor(loop,listenAddr)),
     started_(false),
     nextConId_(1)
{
    acceptor_->setNewConnectionCallBack(bind(&TcpServer::newConnection,this,_1,_2));
}


TcpServer::~TcpServer(){

}


void TcpServer::start(){
    if(!started_){
        started_  = true;
    }
    if(!acceptor_->listenning()){
        loop_->runInLoop(std::bind(&Acceptor::listen,acceptor_));
    }
}


void TcpServer::newConnection(int sockfd,const InetAddress& peerAddr){
    loop_->assertInLoopThread();

    char buf[32];
    snprintf(buf,sizeof(buf),"#%d",nextConId_);
    ++nextConId_;
    std::string connName = name_ + buf;
    InetAddress localAddr(socketOps::getLocalAddr(sockfd));

    TcpConnectionPtr conn(new TcpConnection(loop_,connName,sockfd,localAddr,peerAddr));
    connections_[connName] = conn;

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setCloseCallBack(bind(&TcpServer::removeConnection,this,_1));
    conn->connectEstablished();
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
  loop_->assertInLoopThread();
  size_t n = connections_.erase(conn->name());
  assert(n == 1); (void)n;
  loop_->queueInLoop(bind(&TcpConnection::connectDestroyed, conn));
}

void TcpServer::setConnectionCallback(const ConnectionCallback &cb){
    connectionCallback_ = cb;
}

void TcpServer::setMessageCallback(const MessageCallback &cb){
    messageCallback_ = cb;
}
