#pragma once
#include<iostream>
#include<functional>
using task_t =std::function<void()>;
void Download()
{
    std::cout << "我是一个下载任务" << std::endl;
}



























//任务形式1
class Task
{
public:
    Task(){}
    Task(int a,int b):_x(a),_y(b)
    {

    }
    ~Task(){}
    void Execute()
    {
        _result = _x + _y;
        std::cout << "计算结果：" << _result << std::endl;
    }
    int Result()
    {
        return _result;
    }
    int X()
    {
        return _x;
    }
    int Y()
    {
        return _y;  
    }


private:
    int _x;
    int _y;
    int _result;
};