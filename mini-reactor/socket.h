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
    explicit Socket(int sockfd);
    ~Socket();

    int getfd()const;

    void bindAddress(const InetAddress& localaddr);

    void listen();

    int accept(InetAddress* peeraddr);

    void setReuseAddr(bool on);

private:

    const int sockfd_;
};

#endif