#pragma once

#include <vector>
#include <iostream>
#include "Sem.hpp"
#include "Mutex.hpp"
static const int gcap = 5;
using namespace SemModule;
using namespace MutexModule;
template <typename T>
class RingQueue
{
public:
    RingQueue(int cap = gcap) : _cap(cap),
                                _rq(cap),
                                _blank_sem(cap),
                                _date_sem(0),
                                _p_step(0),
                                _c_step(0) {};
    void Equeue(const T &in)
    {
        // 生产者
        // 1.申请信号量，空位置信号量
        _blank_sem.P();
        {
            LockGuard lockguard(_pmutex);
            // 2.生产
            _rq[_p_step] = in;
            // 3.更新下标
            _p_step++;
            // 4.维持环形特性
            _p_step %= _cap;
            // 5.释放信号量，数据位置信号量
        }
        _date_sem.V();
    }
    void Pop(T *out)
    {
        // 消费者
        // 1.申请信号量，数据位置信号量
        _date_sem.P();
        {
            LockGuard lockguard(_cmutex);
            // 2.消费

            *out = _rq[_c_step];
            // 3.更新下标
            _c_step++;
            // 4.维持环形特性
            _c_step %= _cap;
        }
        // 5.释放信号量，空位置信号量
        _blank_sem.V();
    }
    ~RingQueue() {};

private:
    std::vector<T> _rq;
    int _cap;
    // 生产者
    Sem _blank_sem; // 空位置
    int _p_step;

    // 消费者
    Sem _date_sem; // 数据位置
    int _c_step;

    // 维护多生产，多消费，2把锁
    Mutex _pmutex;
    Mutex _cmutex;
};