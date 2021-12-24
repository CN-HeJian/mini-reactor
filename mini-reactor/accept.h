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

#ifndef ACCEPTOR_H
#define ACCEPTOR_H


#include <functional>
#include "eventLoop.h"
#include "channel.h"
#include "socket.h"

class InetAddress;

class Acceptor{
public:
    typedef std::function<void(int sockfd,const InetAddress&)> newConnectCallBack;

    Acceptor(EventLoop* loop,const InetAddress& listenAddr);

    void setNewConnectionCallBack(const newConnectCallBack& cb);

    bool listenning()const;
    void listen();

private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    newConnectCallBack newConnextionCallBack_;
    bool listening_;
};

#endif