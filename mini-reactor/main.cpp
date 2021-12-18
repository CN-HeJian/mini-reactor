#include <iostream>
#include "eventLoop.h"
#include <pthread.h>

using namespace std;

void* threadFunc(void* arg){
    EventLoop loop;
    loop.loop();
    return nullptr;
}

int main(){
    EventLoop loop;

    pthread_t p1;

    pthread_create(&p1,nullptr,threadFunc,nullptr);
    
    loop.loop();

    pthread_exit(nullptr);
}