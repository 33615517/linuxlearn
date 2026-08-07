// 阻塞队列的实现

#pragma once
#include <iostream>
#include <pthread.h>
#include <string>
#include <queue>
#include"Mutex.hpp"
#include"Cond.hpp"
const int defaultCap = 5;

using namespace MutexModule;
using namespace CondModule;


template <typename T>
class BlockQueue
{
private:
    bool isFull()
    {
        return _queue.size() == _cap;
    }
    bool isEmpty()
    {
        return _queue.size() == 0;
    }

public:
    BlockQueue(int cap = defaultCap)
        : _cap(cap), _csleep_num(0), _psleep_num(0)
    {

    }
    void Equeue(const T &data)
    {
        LockGuard lockguard(_mutex);
        // 生产者调用
        while(isFull())
        { // 让生产者线程等待
            // 1、pthread_cond_wait调用成功，挂起当前进程前，会自动解锁互斥锁，等待条件变量的通知
            // 2、当收到条件变量的通知后，pthread_cond_wait会自动加锁
            _psleep_num++;
           // pthread_cond_wait(&_full_cond, &_mutex); // 等待条件变量的通知，等待时会自动解锁，收到通知后会自动加锁
           _full_cond.Wait(_mutex);
            _psleep_num--;
        }
        _queue.push(data);

        if (_csleep_num > 0) // 如果有消费者线程在等待条件变量的通知，则唤醒一个消费者线程
           { 
            _empty_cond.Signal();
            std::cout << "唤醒消费者线程" << std::endl;
        }
    }
    T pop()
    {
        // 消费者调用
        LockGuard lockguard(_mutex);
        while (isEmpty())
        {
            _csleep_num++;
            _empty_cond.Wait(_mutex); // 等待条件变量的通知，等待时会自动解锁，收到通知后会自动加锁
            _csleep_num--;
        }
        T data = _queue.front();
        _queue.pop();
        if (_psleep_num > 0) // 如果有生产者线程在等待条件变量的通知，则唤醒一个生产者线程
           { 
            _full_cond.Signal();
            std::cout << "唤醒生产者线程" << std::endl;
        }
        return data;
    }
    ~BlockQueue()
    {
        
    }

private:
    std::queue<T> _queue;
    int _cap; // 容量大小

    Mutex _mutex;
    Cond _full_cond;
    Cond _empty_cond;

    int _csleep_num; // 消费者休眠的个数
    int _psleep_num; // 生产者休眠的个数
};