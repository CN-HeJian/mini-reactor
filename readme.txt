makefile :https://www.bilibili.com/video/BV1Mx411m7fm?from=search&seid=14540049951182321444

EventLoop:
1) 一个IO线程一个事件循环，Loop为关键结构


Channel:
1) 绑定一个文件描述符
2）绑定回调方法，事件触发之后执行什操作...


Poller:
1) 封装给事件循环使用、一个IO线程一个事件循环，一个事件循环中包含一个Poller实例
2）Poll()系统调用在事件循环loop中调用
3）存放Channel的实例，Poll()监听这些文件描述符号上的事件

TimerId
1) 对Timer多做了一层封装，对用户开放使用
2) 存在疑惑，在哪儿使用呢？

Timer
1）设置到期时间
2）并且绑定到对应的回调函数

TimerQueue
1）提供插入时间以及取消时间的接口
2）

socketOps
1) 转换主机地址、网络地址
2) 封装系统调用::bind  ::listen  ::accpet  ::inet_pton  ::inet_ntop 等系统调用

socket
1) 包含一个监听端口的文件描述符号
2）包含bind listen accpet 

INetAddress
1) 创建一个addr..
2）封装了一下sockaddr_in