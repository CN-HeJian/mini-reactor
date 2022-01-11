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

class TcpConnection : public std::enable_shared_from_this<TcpConnection>{
public: 
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

    // Tcp连接
    TcpConnection(EventLoop* loop,const std::string &name,int sockfd,const InetAddress& localAddr,const InetAddress &peerAddr);
    ~TcpConnection();

    //获取事件循环
    EventLoop* getLoop() const ;
    //获取连接的名字
    const std::string& name() const;
    //
    const InetAddress& localAddress();
    //
    const InetAddress& peerAddress();
    //获取连接状态
    bool connected() const ;

    //设置连接的回调函数
    void setConnectionCallback(const ConnectionCallback& cb);
    //设置发送消息的回调函数
    void setMessageCallback(const MessageCallback& cb);
    //设置关闭连接的回调
    void setCloseCallBack(const CloseCallback& cb);

    //连接建立
    void connectEstablished();  //仅仅能被调用一次
    //
    void connectDestroyed();

private:
    enum StateE{
        kConnecting,
        kConnected,
        kDisconnected,
    };

    void setState(StateE s);
    //接受数据
    void handleRead();

    void handleWrite();

    void handleClose();

    void handleError();


    EventLoop* loop_;
    std::string name_;
    StateE state_;

    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;

    InetAddress localAddr_;
    InetAddress peerAddr_;

    ConnectionCallback ConnectionCallback_;
    MessageCallback MessageCallback_;
    CloseCallback closeCallback_;
};





#endif