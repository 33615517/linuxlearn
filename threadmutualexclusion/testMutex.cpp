
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
#include "Mutex.hpp"
int ticket = 100;
// pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;

// std::mutex cpp_lock;

using namespace MutexModule;

class ThreadDate
{
public:
    ThreadDate(const std::string &n, Mutex &lock)
        : name(n),
          lockp(&lock)
    {
    }
    std::string name;
    Mutex *lockp;
};
void *route(void *arg)
{
    ThreadDate *td = static_cast<ThreadDate *>(arg);
    while (1)
    {
        LockGuard guard(*td->lockp);//加锁完成，RAII风格的互斥锁，离开作用域自动解锁
        if (ticket > 0)
        { 
            usleep(1000);
            printf("%s sells ticket:%d\n", td->name.c_str(), ticket);
            ticket--;
        }
        else
        {
            break;
        }
        // pthread_mutex_lock(td->lockp);
        // pthread_mutex_lock(&glock);
        // cpp_lock.lock();
        //  td->lockp->lock();
        //  if (ticket > 0)
        //  {
        //      usleep(1000);
        //      printf("%s sells ticket:%d\n", td->name.c_str(), ticket);
        //      ticket--;
        //       // pthread_mutex_unlock(td->lockp);
        //      // pthread_mutex_unlock(&glock);
        //      //cpp_lock.unlock();
        //      td->lockp->unlock();
        //  }

        // else
        // {
        //     //pthread_mutex_unlock(td->lockp);
        //     //pthread_mutex_unlock(&glock);
        //     //cpp_lock.unlock();
        //     td->lockp->unlock();
        //     break;
        // }
    }
    return nullptr;
}
int main(void)
{
    // pthread_mutex_t lock;
    // pthread_mutex_init(&lock, nullptr);//初始化互斥锁

    Mutex lock;

    pthread_t t1, t2, t3, t4;
    ThreadDate *td1 = new ThreadDate("thread 1", lock);
    pthread_create(&t1, NULL, route, td1);
    ThreadDate *td2 = new ThreadDate("thread 2", lock);
    pthread_create(&t2, NULL, route, td2);
    ThreadDate *td3 = new ThreadDate("thread 3", lock);
    pthread_create(&t3, NULL, route, td3);
    ThreadDate *td4 = new ThreadDate("thread 4", lock);
    pthread_create(&t4, NULL, route, td4);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    // pthread_mutex_destroy(&lock);//销毁互斥锁
    return 0;
}