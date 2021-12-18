#include <iostream>
#include "eventLoop.h"
#include <pthread.h>
#include <sys/timerfd.h>
#include "channel.h"
#include <cstring>
#include <string.h>

using namespace std;

EventLoop* g_loop;

void timeout(){
    printf("Timeout!\n");
    g_loop->setQuit();
}

int main(){
    EventLoop loop;

    g_loop= &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel channel(&loop,timerfd);
    channel.setReadCallBack(timeout);
    channel.enableReadEvent();
    
    struct itimerspec howlong;
    //bzero(&howlong, sizeof howlong);
    //memset(&howlong, sizeof howlong,0);
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 5;
    timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    close(timerfd);
    //pthread_exit(nullptr);
}
