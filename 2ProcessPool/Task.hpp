#pragma once
#include<iostream>
#include<vector>
#include<ctime>
typedef void (*task_t)();
void PrintLog()
{
    std::cout<<"我是一个打印日志的任务"<<std::endl;

}
void Download()
{
    std::cout<<"我是一个下载的任务" << std::endl;
}
void Upload()
{
    std::cout<<"我是一个上传的任务" << std::endl;
}
class TaskManger
{
public:
    TaskManger()
    {
        srand(time(nullptr));

    }
    void Register(task_t t)
    {
        _tasks.push_back(t);


    }
    int Code()
    {
        return rand() % _tasks.size();

    }
    void Execute(int code)
    {
        if(code>=0 && code < _tasks.size())
        {
            _tasks[code]();
        }

    }
    ~TaskManger(){};

private:
    std::vector<task_t> _tasks;

};