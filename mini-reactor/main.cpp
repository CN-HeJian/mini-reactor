#include <iostream>
#include "eventLoop.h"
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/syscall.h>
#include "channel.h"
#include <cstring>
#include <string.h>
#include "thread.h"

using namespace std;

EventLoop* g_loop;
int g_flag = 0;

void print(){
    printf("print pid:%ld\n",syscall(SYS_gettid));
    //abort();
}

void* threadFunc(void* ){
    printf("threadFunc pid:%ld\n",syscall(SYS_gettid));
    g_loop->runAfter(8.0,print);
    return nullptr;
}

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
    
    std::cout<<"main begin: "<<Timestamp::now().toFormattedString()<<std::endl;

    printf("main pid:%ld\n",syscall(SYS_gettid));

    //printf("main pid:%ld\n",pthread_self());

    EventLoop loop;
    g_loop = &loop;

    pthread_t th;

    int ret = pthread_create(&th,NULL,threadFunc,NULL);
    if(ret!=0){
        std::cout<<"create thread failed\n"<<endl;
    }


    //Thread t(threadFunc);
    //t.start();
    
    loop.loop();






    // printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);

    // EventLoop loop;

    // g_loop= &loop;

    // loop.runAfter(2,run1);
    // loop.loop();

    // printf("main(): pid = %d, flag = %d\n", getpid(), g_flag);

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
