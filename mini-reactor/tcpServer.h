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

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>
#include <map>
#include <iostream>
#include <memory>

class EventLoop;
class Acceptor;
class TcpConnection;
class InetAddress;

#include "callback.h"
#include "tcpConnection.h"

class TcpServer{
public:
    TcpServer(EventLoop* loop,const InetAddress& listenAddr);

    ~TcpServer();

    void start();

    void setConnectionCallback(const ConnectionCallback &cb);

    void setMessageCallback(const MessageCallback &cb);

private:

    void newConnection(int sockfd,const InetAddress& peerAddr);

    typedef std::map<std::string,TcpConnectionPtr> connectionMap;

    EventLoop* loop_;
    const std::string name_;
    //std::auto_ptr<Acceptor> acceptor_;
    //std::unique_ptr<Acceptor>  acceptor_;
    Acceptor* acceptor_;

    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    
    bool started_;
    int nextConId_;
    connectionMap connections_;
};



#endif
