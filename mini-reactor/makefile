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

objects = eventLoop.o main.o

GPP = g++
CFLAGS = -lm -Wall -g 

all: clean mini-muduo 
 
mini-muduo:$(objects)
	$(GPP) $(CFLAGS) -o mini-muduo $(objects)  -lpthread

eventLoop.o : eventLoop.cpp
	$(GPP) $(CFLAGS) -c eventLoop.cpp 

main.o :  main.cpp
	$(GPP) $(CFLAGS) -c main.cpp 


.PHONY : clean
clean :
	-rm -f mini-muduo $(objects)

run :
	./mini-muduo