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

#include "tcpConnection.h"
#include "socket.h"
#include "channel.h"
#include "eventLoop.h"

TcpConnection::TcpConnection(EventLoop* loop,const std::string& nameArg,int sockfd,const InetAddress& localAddr,const InetAddress& peerAddr)
   :loop_(loop),
    name_(nameArg),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop,sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr)
{
    channel_->setReadCallBack(std::bind(&TcpConnection::handleRead,this));
}

TcpConnection::~TcpConnection(){

}

void TcpConnection::connectEstablished(){
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->enableReadEvent();

    //ConnectionCallback_(shared_from_this());
}

void TcpConnection::handleRead(){
    char buf[65536];
    ssize_t n = ::read(channel_->getFd(),buf,sizeof buf);
    
    if(n){

    }

    //MessageCallback_(shared_from_this(),buf,n);
}

EventLoop* TcpConnection::getLoop() const {
    return loop_;
}

const std::string& TcpConnection::name() const{
    return name_;
}

const InetAddress& TcpConnection::localAddress(){
    return localAddr_;
}

const InetAddress& TcpConnection::peerAddress(){
    return peerAddr_;
}

bool TcpConnection::connected() const {
    return state_ == kConnected;
}

void TcpConnection::setConnectionCallback(const ConnectionCallback& cb){
    ConnectionCallback_= cb;
}

void TcpConnection::setMessageCallback(const MessageCallback& cb){
    MessageCallback_ = cb;
}

void TcpConnection::setState(StateE s){
    state_ = s;
}
