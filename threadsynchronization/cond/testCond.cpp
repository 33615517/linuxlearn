#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<condition_variable>
#include<mutex>
#include<string>
#include<vector>

#define NUM 5
int cnt = 10000;

pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;//定义锁
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;//定义条件变量

void *threadrun(void *arg)
{
    std::string name = static_cast<char *>(arg);
    while(1)
    {
        pthread_mutex_lock(&glock);//加锁
        //直接让对应的线程进行等待
        pthread_cond_wait(&gcond,&glock);//等待条件变量的通知，等待时会自动解锁，收到通知后会自动加锁 
        std::cout << name << " 计算：" << cnt << std::endl;
        cnt++;
        pthread_mutex_unlock(&glock);//解锁
    }
    return nullptr;
}

int main()
{

    std::vector<pthread_t> threads;
    for(int i =0 ;i< NUM; i++)
    {
        pthread_t tid;
        char *name = new char[64];
        snprintf(name,64,"thread %d",i);
        int n = pthread_creater(&tid,nullptr,threadrun,name);
        if(n != 0)
        {
            std::cout<<"pthread_creater error"<<std::endl;
            return -1;
        }
        threads.push_back(tid);
    }
    //每隔1s唤醒一个线程
    while(true)
    {
        std::cout << "唤醒所有线程" << std::endl;
        pthread_cond_broadcast(&gcond);//唤醒所有线程
       // pthread_cond_signal(&gcond);//唤醒一个线程
        sleep(1);
    }

    for(auto &id : threads)
    {
        pthread_join(id,nullptr);
    }
    return 0;
}