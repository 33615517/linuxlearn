#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <pthread.h>
int flag = 100;
void showtid(pthread_t tid)
{
    std::cout << "新线程的id = " << tid << std::endl;
}
std::string FormatId(const pthread_t &tid)
{
    char id[64];
    snprintf(id , sizeof(id) ,"0x%lx",tid);
    return id;
}
void *routinue(void *arg)
{
    std::string str = static_cast<char *>(arg);
    pthread_t tid = pthread_self();
    int cnt = 5;
    while (cnt--)
    {
        std::cout << "我是新线程：name = " << str << ", id = " << FormatId(tid) << std::endl;
        sleep(1);
        flag++;
    }
    return nullptr;
}
int main()
{
    pthread_t tid;
    int n = pthread_create(&tid, nullptr, routinue, (void *)"thread-1");
    (void)n;

    
    showtid(tid);
    int cnt = 5;
    while (true)
    {
        std::cout << "我是main线程：name = " << "main thread" << ", id = " << FormatId(pthread_self()) << ", flag = " << flag << std::endl;
        sleep(1);
    }
    void *ret=nullptr;
    pthread_join(tid, &ret);
    std::cout << "线程返回值: " << ret << std::endl;
    return 0;
}













// #include<thread>

// void hello()
// {
//     while(true)
//     {
//         std::cout<<"新线程 hello world"<<std::endl;
//         sleep(1);
//     }
// }

// int main()
// {
//     std::thread t(hello);
//     while(true)
//     {
//         std::cout<<"主线程 hello world,pid:"<<getpid()<<std::endl;
//         sleep(1);
//     }
//     t.join();
//     return 0;
// }

// void *threadrun(void *arg)
// {
//     std::string str = (char*)arg;
//     while(1)
//     {
//         std::cout << "我是新线程：name = " << str << std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }
// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid,nullptr,threadrun,(void*)"thread-1");
//     while(1)
//     {
//         std::cout << "我是主线程" << std::endl;
//         sleep(1);
//     }
//     return 0;
// }