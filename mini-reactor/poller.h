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

#ifndef POLLER_H
#define POLLER_H

#include <vector>
#include <map>
#include "eventLoop.h"
#include "channel.h"
#include <assert.h>
#include <poll.h>

struct pollfd;
class EventLoop;
class Channel;

class Poller
{
public:
    Poller(EventLoop *loop);
    ~Poller();

    typedef std::vector<Channel*> ChannelList;

    void poll(int timeOutMs,ChannelList* activeChannelList);

    void updateChannel(Channel *channel);

private:
    
    void findActiveChannel(int NumofFd,ChannelList* activeChannels) const;
    
    typedef std::vector<struct pollfd> PollFd_vec;
    typedef std::map<int,Channel*> ChannelMap;

    EventLoop* which_loop;
    PollFd_vec pollfds;
    ChannelMap channels;

};


#endif