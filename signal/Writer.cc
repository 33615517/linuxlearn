#include "Sem_V2.hpp"
#include <time.h>
#include <unistd.h>
#include <ctime>
int main()
{
    // 基于抽象接口类的具体建造者
    std::shared_ptr<SemaphoreBuilder> builder = std::make_shared<ConcreteSemaphoreBuilder>();
    // 指挥者对象
    std::shared_ptr<Director> director = std::make_shared<Director>();

    // 在指挥者的指导下，完成建造者的建造过程
    director->construct(builder, BUILD_SEM,0666, 3, {1, 2, 3});

    // 完成了对象的创建过程，获取对象
    auto sem = builder->GetSem();


    // SemaphoreBuilder sb;
    auto fsem =builder->GetSem();
    srand(time(NULL) ^ getpid());
    pid_t pid = fork(); 
    if (pid == 0)
    {
         director->construct(builder, GET_SEM);
         auto csem = builder->GetSem();
        // 子进程
        while(true)
        {
            csem->P(0);
            printf("C");
            usleep(rand() % 95270);
            fflush(stdout);
            printf("C");
            usleep(rand() % 43990);
            fflush(stdout);
            csem->V(0);
        }
    }
    else if (pid > 0)
    {
        // 父进程
        while(true)
        {
            fsem->P(0);
            printf("P");
            usleep(rand() % 95270);
            fflush(stdout);
            printf("P");
            usleep(rand() % 43990);
            fflush(stdout);
            fsem->V(0);
        }
    }
    else
    {
        perror("fork failed");
        return -1;
    }
    return 0;
}