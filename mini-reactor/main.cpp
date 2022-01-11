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
#include "tcpServer.h"

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

void onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    printf("onConnection(): new connection [%s] from %s\n",
           conn->name().c_str(),
           conn->localAddress().toHostPort().c_str());
  }
  else
  {
    printf("onConnection(): connection [%s] is down\n",
           conn->name().c_str());
  }
}

void onMessage(const  TcpConnectionPtr& conn,
               const char* data,
               ssize_t len)
{
  printf("onMessage(): received %zd bytes from connection [%s]\n",
         len, data);
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


    TcpServer server(&loop,listenAddr);

    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();



    //Acceptor accept(&loop,listenAddr);

    //accept.setNewConnectionCallBack(newConnection);

    //accept.listen();

    loop.loop();
}   
