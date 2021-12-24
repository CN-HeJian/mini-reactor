// Copyright 2021 icf
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef THREAD_H
#define THREAD_H

#include <functional>
#include <memory>

class Thread{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();

    void start();
    void join();

    bool started()const;
    pid_t tid() const;
    const std::string& name()const;

    static int numCreated();

private:
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    std::shared_ptr<pid_t> tid_;
    ThreadFunc func_;
    std::string name_;

   // static AtomicInt32 numCreated_;
};

namespace CurrentThread{
    pid_t tid();
    const char* name();
    bool isMainThread();
}

#endif