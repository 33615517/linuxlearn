#include <iostream>
#include <time.h>
#include <ctime>
#include <unistd.h> 
#include <signal.h>
#include <unistd.h>
void handlerSig(int sig)
{
    std::cout << "获得了一个信号" << sig << std::endl;
    exit(10);
}
int main()
{
    for(int i = 1; i< 32 ;i++)
    signal(i,handlerSig);
    // for(int i = 1; i< 32 ;i++)
    // {
    //     sleep(1);
    //     if(i==9)continue;
    //     //raise(i);
    // }
    int cnt = 0;
    while (true)
    {
        
        sleep(1);

        std::cout << "Hello, World!" << cnt++ << "pid" << getpid() << std::endl;
        int a = 10;
        a /= 0;//除0错误
       // abort();
    }
    return 0;
}