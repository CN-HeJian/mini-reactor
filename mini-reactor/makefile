# Copyright 2021 icf
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#eventLoop.o main.o channel.o poller.o   timestamp.o  mutex.o timer.o

INCLUDE := .

objects = timerQueue.o  eventLoop.o main.o channel.o poller.o   timestamp.o  mutex.o timer.o thread.o eventLoopThread.o\
condition.o

GPP = g++
CFLAGS = -lm -Wall -g 

 
mini-muduo:$(objects)
	$(GPP) $(CFLAGS) -o mini-muduo $(objects)  -lpthread

all: clean mini-muduo 

eventLoop.o : eventLoop.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c eventLoop.cpp 

main.o :  main.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c main.cpp 

channel.o : channel.cpp channel.h
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c channel.cpp  

poller.o : poller.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c poller.cpp

timerQueue.o : timerQueue.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c timerQueue.cpp

timestamp.o : timestamp.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c timestamp.cpp

timer.o : timer.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c timer.cpp

mutex.o : mutex.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c mutex.cpp

thread.o : thread.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c thread.cpp

eventLoopThread.o : eventLoopThread.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c eventLoopThread.cpp

condition.o : condition.cpp
	$(GPP) $(CFLAGS) -I$(INCLUDE) -c condition.cpp


.PHONY : clean
clean :
	-rm -f mini-muduo $(objects)

run :
	./mini-muduo