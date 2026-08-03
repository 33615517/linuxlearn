#include <iostream>
#include <time.h>
#include <functional>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{

    pid_t pid = fork();
    if (pid == 0)
    {
        sleep(2);
        int a = 10;
        a /= 0;
        exit(1);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    printf("signal:%d,exit code:%d,core dump:%d\n", (status >> 8) & 0x7f, (status >> 8) & 0xff, (status >> 7) & 0x1);
    return 0;
}

// void PrintPending(sigset_t &pending)
// {
//     std::cout << "我是一个进程，我的pid是:" << getpid() << std::endl;
//     for (int signo = 1; signo < 32; signo++)
//     {
//         if (sigismember(&pending, signo))
//         {
//             std::cout << "1";
//         }
//         else
//         {
//             std::cout << "0";
//         }
//     }
//     std::cout << std::endl;
// }
// void handler(int sig)
// {
//     std::cout << "抵达2号信号处理函数" << std::endl;

//     sigset_t pending;
//     int m = sigpending(&pending);
//     PrintPending(pending);
//     std::cout << "###############################################" << std::endl;
// }
// int main()
// {

//     signal(SIGINT, handler);
//     // 1.屏蔽信号
//     sigset_t block, oblock;
//     sigemptyset(&block);
//     sigemptyset(&oblock);

//     sigaddset(&block, SIGINT);
//     int n = sigprocmask(SIG_BLOCK, &block, &oblock);
//     (void)n;

//     int cnt = 0;
//     // 4.重复获取和打印的过程
//     while (true)
//     {
//         // 2.获取pending信号集
//         sigset_t pending;
//         int m = sigpending(&pending);

//         // 3.打印
//         PrintPending(pending);
//         if (cnt == 10)
//         {
//             // 解除屏蔽
//             sigprocmask(SIG_SETMASK, &oblock, nullptr);
//             std::cout << "解除屏蔽" << std::endl;
//         }
//         cnt++;
//         sleep(1);
//     }

//     return 0;
// }
// void handlerSig(int sig)
// {
//     std::cout << "获得了一个信号" << sig << std::endl;
//     signal(2,SIG_DFL);//恢复默认行为
//     std::cout << "恢复默认行为" << std::endl;
// }
// int main()
// {
//     signal(2,handlerSig);

//     sigset_t set;
//     while(true)
//     {
//         sleep(1);
//         std::cout<<"."<<std::endl;
//     }

//     return 0;
// }
// struct task_struct
// {
//     pid_t pid;
//     int count = 10;//时间片,本质就是一个计数器
//     void(*code)();
// }t;
// std::vector<task_struct> task_list;
// //////////function<void()> func;
// void sche()
// {
//     std::cout << "我是进程调度器" << std::endl;
//     t.count--;
//     if(t.count == 0)
//     {
//         std::cout << "时间片用完了，切换进程" << std::endl;
//         t.count = 10;
//     }
// }
// void MemManger()
// {
//     std::cout << "我是周期性的内存管理，正在检查有没有内存问题" << std::endl;
// }
// void Fflush()
// {
//     std::cout << "我是周期性的刷新器，正在刷新数据到磁盘" << std::endl;
// }
// using func_t =std::function<void()>;
// std::vector<func_t> funcs;
// int timestamp = 0;
// //每隔1s完成一些任务
// void handlerSig(int sig)
// {
//     timestamp++;
//     std::cout << "##############################################" << std::endl;
//     for(auto& f : funcs)
//     {
//         f();
//     }
//      std::cout << "##############################################" << std::endl;
//     int n = alarm(1); // 设定闹钟1秒后，1s后当前进程会收到一个14号SIGALRM信号
//    // exit(10);
//    alarm(1);
// }
// int main()
// {
//     funcs.push_back(sche);
//     funcs.push_back(MemManger);
//     funcs.push_back(Fflush);
//      signal(SIGALRM, handlerSig);
//      alarm(1); // 设定闹钟1秒后，1s后当前进程会收到一个14号SIGALRM信号
//     while (true)
//     {
//          pause();

//     }

//     return 0;
// }

// int main()
// {
//     signal(SIGALRM, handlerSig);
//     alarm(1); // 设定闹钟1秒后，1s后当前进程会收到一个14号SIGALRM信号
//     while (true)
//     {
//         //什么都不做，就想让进程暂停
//         //一旦来一个信号，就唤醒一次执行方法

//          std::cout << "." << "pid:" << getpid() << std::endl;
//          pause();
//         // sleep(1);

//     }

//     return 0;
// }

// int main()
// {
// //     for (int i = 1; i < 32; i++)
// //         signal(i, handlerSig);
//      signal(SIGALRM, handlerSig);
//     alarm(1); // 设定闹钟1秒后，1s后当前进程会收到一个14号SIGALRM信号

//     while (true)
//     {
//         //std::cout << "count:" << cnt++ << std::endl;
//         cnt++;

//     }

//     return 0;
// }

// void handlerSig(int sig)
// {
//     std::cout << "获得了一个信号" << sig << std::endl;
//     exit(10);
// }
// int main()
// {
//     for(int i = 1; i< 32 ;i++)
//     signal(i,handlerSig);
//     // for(int i = 1; i< 32 ;i++)
//     // {
//     //     sleep(1);
//     //     if(i==9)continue;
//     //     //raise(i);
//     // }
//     int cnt = 0;
//     while (true)
//     {

//         sleep(1);

//         std::cout << "Hello, World!" << cnt++ << "pid" << getpid() << std::endl;
//         int a = 10;
//         a /= 0;//除0错误
//        // abort();
//     }
//     return 0;
// }