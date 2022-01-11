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

#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <arpa/inet.h>
#include <endian.h>

namespace socketOps{

     inline uint64_t hostToNetwork64(uint64_t host64)
    {
        return htobe64(host64);
    }

     inline uint32_t hostToNetwork32(uint32_t host32)
     {
         return htonl(host32);
     }

     inline uint16_t hostToNetwork16(uint16_t host16)
     {
         return htons(host16);
     }

     inline uint64_t networkToHost64(uint64_t net64)
     {
         return be64toh(net64);
     }

     inline uint32_t networkToHost32(uint32_t net32)
     {
         return ntohl(net32);
     }

     inline uint16_t networkToHost16(uint16_t net16)
     {
         return ntohs(net16);
     }

    int createNonblockingOrDie();

    void bindOrDie(int sockfd,const struct sockaddr_in& addr);
    void listenOrDie(int sokcfd);
    int accpet(int sockfd,struct sockaddr_in* addr);
    void close(int sockfd);

    void toHostPort(char* buf,size_t size,const struct sockaddr_in& addr);
    void fromHostPort(const char* ip,uint16_t port,struct sockaddr_in*addr);

    struct sockaddr_in getLocalAddr(int sockfd);
};
#endif
