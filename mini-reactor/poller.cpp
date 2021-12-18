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

#include <poller.h>
#include <assert.h>

Poller::Poller(EventLoop* loop)
    :which_loop(loop)
{   
}

Poller::~Poller()
{

}

void Poller::poll(int timeOutMs,ChannelList* activeChannelList){
    printf("poll size of fd： %d\n",pollfds.size());
    int numEvents = ::poll(&*pollfds.begin(),pollfds.size(),timeOutMs);
    printf("numEvents: %d\n",numEvents);
    if(numEvents>0){
        findActiveChannel(numEvents,activeChannelList);
    }
}

void Poller::updateChannel(Channel *channel){
   //assertInLoopThread();

   //此处更新日志！！！LOG_TRANCE

    if(channel->getIndex()<0){
        printf("a new channel add to poller\n");
        assert(channels.find(channel->getFd()) == channels.end());
        struct pollfd pfd;
        pfd.fd = channel->getFd();
        pfd.events = static_cast<short>(channel->getevents());
        pfd.revents = 0;
        pollfds.push_back(pfd);
        int idx = static_cast<int>(pollfds.size())-1;
        channel->setIndex(idx);
        channels[pfd.fd] = channel;
    }else{
        assert(channels.find(channel->getFd())!=channels.end());
        assert(channels[channel->getFd()]==channel);
        int idx = channel->getIndex();
        assert(0<=idx && idx <static_cast<int>(pollfds.size()));
        struct pollfd& pfd = pollfds[idx];
        assert(pfd.fd==channel->getFd() || pfd.fd==-1);
        pfd.events = static_cast<short>(channel->getevents());
        pfd.revents = 0;
        if(channel->isNoneEvent()){
            pfd.fd = -1;
        }
    }
}

void Poller::findActiveChannel(int NumOfFd,ChannelList* activeChannels) const{
    for(PollFd_vec::const_iterator pfd = pollfds.begin();pfd!=pollfds.end()&&NumOfFd>0;++pfd){
        if(pfd->revents>0){
            NumOfFd--;
            ChannelMap::const_iterator chan = channels.find(pfd->fd);
            assert(chan != channels.end());
            Channel* channel = chan->second;
            assert(channel->getFd()==pfd->fd);
            channel->setRevents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}


