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
#include "inetAddress.h"
#include "socket.h"
#include "socketOps.h"
#include "accept.h"

using namespace std;

EventLoop* g_loop;
int g_flag = 0;

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
         getpid(), CurrentThread::tid());
}

void newConnection(int sockfd,const InetAddress& peerAddr){
    printf("newConnection(): accept a new connection from %s\n",peerAddr.toHostPort().c_str());
    ::write(sockfd,"how are you?\n",13);
    socketOps::close(sockfd);
}


int main(){
    // printf("main() pid = %d, tid = %ld\n",getpid(),syscall(SYS_gettid));
    
    // EventLoopThread loopThread;

    // EventLoop* loop = loopThread.startLoop();

    // loop->runInLoop(runInThread);
    // sleep(1);

    // loop->runAfter(2,runInThread);
    // sleep(3);

    // loop->setQuit();

    // printf("exit main().\n");

    printf("main(): pid = %ld\n",syscall(SYS_gettid));

    InetAddress listenAddr(9981);

    EventLoop loop;

    Acceptor accept(&loop,listenAddr);

    accept.setNewConnectionCallBack(newConnection);

    accept.listen();

    loop.loop();
}   
