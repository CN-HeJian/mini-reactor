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

#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

class EventLoop; 
class Socket;
class Channel;

#include <string>
#include <iostream>
#include <memory>


#include "inetAddress.h"
#include "callback.h"


class TcpConnection{

public: 
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

    // 999
    TcpConnection(EventLoop* loop,const std::string &name,int sockfd,const InetAddress& localAddr,const InetAddress &peerAddr);
    ~TcpConnection();

    EventLoop* getLoop() const ;
    const std::string& name() const;
    const InetAddress& localAddress();
    const InetAddress& peerAddress();
    bool connected() const ;

    void setConnectionCallback(const ConnectionCallback& cb);

    void setMessageCallback(const MessageCallback& cb);
    
    void connectEstablished();

private:
    enum StateE{
        kConnecting,
        kConnected,
    };

    void setState(StateE s);
    void handleRead();

    EventLoop* loop_;
    std::string name_;
    StateE state_;

    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    InetAddress localAddr_;
    InetAddress peerAddr_;

    ConnectionCallback ConnectionCallback_;
    MessageCallback MessageCallback_;
};





#endif