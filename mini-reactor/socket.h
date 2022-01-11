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

#ifndef SOCKET_H
#define SOCKET_H

class InetAddress;

class Socket{
public:
    //限制只有一个没有默认值的成员参数的构造
    explicit Socket(int sockfd);
    ~Socket();

    //获取文件描述符号
    int getfd()const;

    //给socket绑定端口号和地址
    void bindAddress(const InetAddress& localaddr);

    //监听
    void listen();

    //接受连接
    int accept(InetAddress* peeraddr);

    //对这个连接做额外的设置
    void setReuseAddr(bool on);

private:

    //连接的文件描述符号
    const int sockfd_;
};

#endif
