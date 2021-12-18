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
#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include "eventLoop.h"

class EventLoop;

class Channel{
public:
    typedef  std::function<void()> callBack;

    Channel(EventLoop* loop_,int fd_);
    ~Channel();

    //获取所绑定的描述符
    int getFd() const;

    //获取关心的事件
    int getevents() const;

    //设置已经发生的事件
    void setRevents(int rev);

    //判断有没有事件发生
    bool isNoneEvent() const;

    //处理事件
    void handlerEvent();

    //设置三种回调方式
    void setReadCallBack(const callBack&);
    void setWriteCallBack(const callBack&);
    void setErrorCallBackk(const callBack&);

    //使能，添加所需要关心的事件
    void enableReadEvent();
    void enableWriteEvent();
    void disableAll();

    //获取编号
    int getIndex() const;

    //设置编号
    void setIndex(int i);

private:

    void update();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;
 
    //每个channel有且仅仅属于一个线程！！！
    EventLoop* which_loop;

    //绑定的文件描述符
    const int fd;

    //关心的事件
    int events;

    //已经发生的事件
    int revents; 

    //用来标记是否已经加入到pooler类中
    int index;

    //处理事件时调用的三种不同的函数
    callBack  read_Callback;
    callBack  write_Callback;
    callBack  error_Callback;
};

#endif
