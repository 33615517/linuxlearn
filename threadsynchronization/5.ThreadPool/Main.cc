#include "Log.hpp"
#include"Thread.hpp"
#include"ThreadPool.hpp"
#include <iostream>
#include <memory>
#include"Task.hpp"
using namespace LogModule;
using namespace ThreadPoolModule;
int main()
{
    Enable_Console_log_strategy();
    LOG(Loglevel::INFO) << "主线程开始执行";
    
    //ThreadPool<task_t> *tp = ThreadPool<task_t>::GetInstance();
    //tp->Equeue(t);

    //tp->Start();
    int count=10;
    while(true)
    {
        sleep(1);
        ThreadPool<task_t>::GetInstance()->Equeue(Download);
        count--;
        if(count==0)
            break;
    }
   ThreadPool<task_t>::GetInstance()->Stop();
    ThreadPool<task_t>::GetInstance()->Join();
    //delete tp;
    sleep(5);
    // Enable_Console_log_strategy();
    // LOG(Loglevel::INFO) << "Hello, World!"<<3.14;




    // std::unique_ptr<LogStrategy> strategy= std::make_unique<ConsoleLogStrategy>();
    //  std::unique_ptr<LogStrategy> strategy= std::make_unique<FileLogStrategy>();

    // strategy->SyncLog("Hello, World!");
    // logger.EnableConsoleLogStrategy();
    //    logger.EnableFileLogStrategy();

    //     logger(Loglevel::INFO, "main.cc", 10) << "Hello, World!";

    return 0;
}