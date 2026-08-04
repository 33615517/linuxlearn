#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<cstdlib>

#include<sys/wait.h>


void handler(int sig)
{
    std::cout << "hello signal:" << sig << std::endl;
    exit(0);
}

int main()
{
    signal(SIGCHLD,handler);
    pid_t pid = fork();
    if(pid == 0)
    {
        std::cout << "I am child,exit"<< std::endl;sleep(3);
        exit(3);
    }
    waitpid(pid,NULL,0);
    return 0;
}




















//volatile int flag = 0;
// void handler(int sig)
// {
//     std::cout << "更改全局变量，" << flag  <<  "->1" << std::endl;
//    flag = 1;
// }
// int main()
// {




//     signal(2,handler);
//     while(!flag)
//     {

//     }
//     std::cout<< "process quit normal!" << std::endl;

//     return 0;
// }
// void handler(int sig)
// {
//     std::cout << "hello signal:" << sig << std::endl;
//     while(true)
//     {
//         sigset_t pending;
//         int m = sigpending(&pending);
//         for(int i = 31; i >= 0 ;i--)
//         {
//             if (sigismember(&pending, i))
//             {
//                 std::cout << "1";
//             }
//             else
//             {
//                 std::cout << "0";
//             }
//         }
//         std::cout << std::endl;
//         sleep(1);
//     }
//     exit(0);
// }

// int main()
// {
//     struct sigaction act,oact;
//     sigemptyset(&oact.sa_mask);
//     act.sa_handler = handler;
//     sigemptyset(&act.sa_mask);
//     sigaddset(&act.sa_mask, 3);
//     sigaddset(&act.sa_mask, 4);
//     act.sa_flags = 0;
//     sigaction(SIGINT,&act,&oact);

//     while(true)
//     {
//         std::cout << "hello" << "pid:" << getpid() << std::endl;
//         sleep(1);
//     }
//     return 0;
// }