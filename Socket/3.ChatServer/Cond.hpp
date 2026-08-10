#pragma once

#include<iostream>
#include<pthread.h>
#include<queue>
#include<mutex>
#include<condition_variable>
#include"Mutex.hpp"
using namespace MutexModule;
namespace CondModule
{
    class Cond
    {
    public:
        Cond()
        {
            //构造函数中初始化条件变量
            pthread_cond_init(&_cond, nullptr);
        }
        ~Cond()
        {
            //析构函数中销毁条件变量
            pthread_cond_destroy(&_cond);
        }
        void Wait(Mutex &mutex)
        {
            //等待条件变量
           int n = pthread_cond_wait(&_cond, mutex.get());
            (void)n;
        }
        void Signal()
        {
            //唤醒一个等待条件变量的线程
            int n = pthread_cond_signal(&_cond);
            (void)n;
        }
        void Broadcast()
        {
            //唤醒所有等待条件变量的线程
            int n = pthread_cond_broadcast(&_cond);
            (void)n;
        }
    private:
        pthread_cond_t _cond;
    };
}