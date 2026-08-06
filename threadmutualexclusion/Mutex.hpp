#pragma once
#include<iostream>
#include<pthread.h>


namespace MutexModule
{
    class Mutex
    {
    public:
        Mutex()
        {
            pthread_mutex_init(&_mutex, nullptr);
        }
        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;
        ~Mutex()
        {
            pthread_mutex_destroy(&_mutex);
        }
        void lock()
        {
            int n = pthread_mutex_lock(&_mutex);
            (void)n;
        }
        void unlock()
        {
            int n = pthread_mutex_unlock(&_mutex);
            (void)n;
        }
    private:
        pthread_mutex_t _mutex;
    };
    class LockGuard
    {
    public:
        LockGuard(Mutex &mutex)
            : _mutex(mutex)
        {
            _mutex.lock();
        }
        ~LockGuard()
        {
            _mutex.unlock();
        }
    private:
        Mutex &_mutex;
    };
}