#include<iostream>
#include<signal.h>
#include<unistd.h>
#include<string>
#include<sys/types.h>

//./myskill signumber pid
int main(int argc , char *argv[])
{
    if(argc !=3)
    {
        std::cout << "参数错误" << std::endl;
        return 1;
    }
    int signum = std::stoi(argv[1]);
    pid_t target = std::stoi(argv[2]);
    int ret = kill(target,signum);
    if(ret == -1)
    {
        std::cout << "发送信号失败" << std::endl;
    }
    else
    {
        std::cout << "send " << signum << " to " << target << std::endl;
    }

    return 0;
}
