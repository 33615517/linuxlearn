#include<iostream>
#include<pthread.h>
#include <semaphore.h>

namespace SemModule
{
    const int defaultSemValue = 0;
    class Sem
    {
    public:
        Sem(unsigned int value = defaultSemValue)
        {
            //构造函数中初始化信号量
            sem_init(&_sem, 0, value);
        }
        ~Sem() 
        {
            //析构函数中销毁信号量
            sem_destroy(&_sem);
        }
        void P()
        {
            //等待信号量
            sem_wait(&_sem);
        }
        void V()
        {
            //释放信号量
            sem_post(&_sem);
        }
    private:
        sem_t _sem;
    };
}