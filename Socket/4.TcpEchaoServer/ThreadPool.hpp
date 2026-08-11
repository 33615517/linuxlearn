#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include "Log.hpp"
#include <queue>
#include "Thread.hpp"
#include "Cond.hpp"
#include "Mutex.hpp"

namespace ThreadPoolModule
{
    using namespace LogModule;
    using namespace ThreadModlue;
    using namespace CondModule;
    using namespace MutexModule;
    static const int gnum = 15;
    template <typename T>
    class ThreadPool
    {
    private:
        void WakeUpAllThread()
        {
            LockGuard lock(_mutex);
            if (_sleeprnum > 0)
            {
                _cond.Broadcast();
                LOG(Loglevel::INFO) << "唤醒所有的休眠线程";
            }
        }

        void WakeUpOne()
        {

            _cond.Signal();
            LOG(Loglevel::INFO) << "唤醒一个休眠线程";
        }

        ThreadPool(int num = gnum)
            : _num(num), _isrunning(false), _sleeprnum(0)
        {
            for (int i = 0; i < num; i++)
            {
                _threads.emplace_back(
                    [this]()
                    {
                        HandlerTask();
                    });
            }
        };
        void Start()
        {
            if (_isrunning)
                return;
            _isrunning = true;
            for (int i = 0; i < _num; i++)
            {
                _threads[i].Start();
                LOG(Loglevel::INFO) << "strat new thread success:" << _threads[i].GetName();
            }
        }
        ThreadPool(const ThreadPool<T> &) = delete;
        ThreadPool<T> &operator=(const ThreadPool<T> &) = delete;

    public:
        static ThreadPool<T> *GetInstance(int num = gnum)
        {
            if (inc == nullptr)
            {
                LockGuard lock(_lock);
                LOG(Loglevel::DEBUG) << "获取线程池单例...";
                if (inc == nullptr)
                {
                    LOG(Loglevel::DEBUG) << "首次创建单例，创建之....";
                    inc = new ThreadPool<T>(num);
                    inc->Start();
                } 
            }
            return inc;
        }
        void Stop()
        {
            if (!_isrunning)
                return;
            _isrunning = false;
            // 唤醒所有的线程
            WakeUpAllThread();
        }
        void Join()
        {
            for (auto &t : _threads)
            {
                t.Join();
            }
        }
        void HandlerTask()
        {
            char name[1024];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while (true)
            {
                T task;
                {
                    LockGuard lock(_mutex);
                    // 1.a.队列是否为空 b.线程池是否运行
                    while (_task_queue.empty() && _isrunning)
                    {
                        _sleeprnum++;
                        _cond.Wait(_mutex);
                        _sleeprnum--;
                    }
                    // 2.内部的线程被唤醒
                    if (!_isrunning && _task_queue.empty())
                    {
                        LOG(Loglevel::INFO) << name << " 退出了，线程池退出&&任务队列为空";
                        break;
                    }
                    task = _task_queue.front();
                    _task_queue.pop();
                }
                task();
            }
        }
        bool Equeue(const T &task)
        {
            if (_isrunning)
            {
                LockGuard lock(_mutex);
                _task_queue.push(task);
                if (_threads.size() == _sleeprnum)
                    WakeUpOne();
                return true;
            }
            return false;
        }
        ~ThreadPool() {};

    private:
        std::vector<Thread> _threads;
        int _num;                  // 线程池中，线程的数量
        std::queue<T> _task_queue; // 任务队列
        Cond _cond;                // 条件变量
        Mutex _mutex;              // 互斥锁
        bool _isrunning;           // 线程池是否运行
        int _sleeprnum;            // 休眠的线程数量

        static ThreadPool<T> *inc; // 单例指针
        static Mutex _lock;        // 单例锁
    };
    template <typename T>
    ThreadPool<T> *ThreadPool<T>::inc = nullptr;
    template <typename T>
    Mutex ThreadPool<T>::_lock;
}