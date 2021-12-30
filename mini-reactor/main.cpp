#include <iostream>
#include "eventLoop.h"
#include <pthread.h>
#include <sys/timerfd.h>
#include "channel.h"
#include <cstring>
#include <string.h>

using namespace std;

EventLoop* g_loop;
int g_flag = 0;

void timeout(){
    printf("Timeout!\n");
    g_loop->setQuit();
}

void run4(){
    printf("run4(): pid = %d, flag = %d\n",getpid(),g_flag);
    g_loop->setQuit();
}

void run3(){
    printf("run3(): pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->runAfter(8, run4);
    g_flag = 3;
}

void run2(){
    printf("run2(): pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->queueInLoop(run3);
}

void run1(){
    g_flag = 1;
    printf("run1(): pid = %d, flag = %d\n", getpid(), g_flag);
    g_loop->runInLoop(run2);
    g_flag = 2;
}



int main(){
    printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);

    EventLoop loop;

    g_loop= &loop;

    loop.runAfter(2,run1);
    loop.loop();

    printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);

    // int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    // Channel channel(&loop,timerfd);
    // channel.setReadCallBack(timeout);
    // channel.enableReadEvent();
    
    // struct itimerspec howlong;
    // //bzero(&howlong, sizeof howlong);
    // //memset(&howlong, sizeof howlong,0);
    // bzero(&howlong, sizeof howlong);
    // howlong.it_value.tv_sec = 5;
    // timerfd_settime(timerfd, 0, &howlong, NULL);

    // loop.loop();

    // close(timerfd);
    //pthread_exit(nullptr);
}
