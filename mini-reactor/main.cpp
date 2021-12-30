#include <iostream>
#include "eventLoop.h"
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/syscall.h>
#include "channel.h"
#include <cstring>
#include <string.h>
#include "thread.h"
#include "eventLoopThread.h"

using namespace std;

EventLoop* g_loop;
int g_flag = 0;

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(), CurrentThread::tid());
}

int main(){
    printf("main() pid = %d, tid = %ld\n",getpid(),syscall(SYS_gettid));
    
    EventLoopThread loopThread;

    EventLoop* loop = loopThread.startLoop();

    loop->runInLoop(runInThread);
    sleep(1);

    loop->runAfter(2,runInThread);
    sleep(3);

    loop->setQuit();

    printf("exit main().\n");

}
