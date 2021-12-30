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

#include <channel.h>
#include <poll.h>
#include <iostream>
#include "eventLoop.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN|POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop_,int fd_)
    :which_loop(loop_),
    fd(fd_),
    events(0),
    revents(0),
    index(-1)
{

}

Channel::~Channel(){

}

int Channel::getFd() const{
    return fd;
}

int Channel::getevents() const{
    return events;
}

void Channel::setRevents(int rev){
    revents = rev;
}

bool Channel::isNoneEvent()const{
    return events == kNoneEvent; 
}

void Channel::handlerEvent(){
    std::cout<<"handler"<<std::endl;
    if (revents& POLLNVAL) {
        std::cout<< "Channel::handle_event() POLLNVAL"<<std::endl;
        //LOG_WARN << "Channel::handle_event() POLLNVAL";
    }
    if (revents & (POLLERR | POLLNVAL)) {
        if (error_Callback) 
            error_Callback();
    }
    if (revents & (POLLIN | POLLPRI | POLLRDHUP)) {
        if (read_Callback) 
            read_Callback();
    }
    if (revents & POLLOUT) {
        if (write_Callback) 
            write_Callback();
    }
}

void Channel::setReadCallBack(const callBack& cb){
    read_Callback = cb;
}

void  Channel::setWriteCallBack(const callBack& cb){
    write_Callback = cb;
}

void Channel::setErrorCallBackk(const callBack& cb){
    error_Callback = cb;
} 

void Channel::enableReadEvent(){
    events = events | kReadEvent;
    update();
}

void Channel::enableWriteEvent(){
    events = events | kWriteEvent;
    update();
}

void Channel::disableAll(){
    events = kNoneEvent;
    update();
}

void Channel::update(){
    printf("update a channel\n");
    which_loop->updateChannel(this);
}

int Channel::getIndex() const{
    return index;
}

void Channel::setIndex(int i){
    index = i;
}

