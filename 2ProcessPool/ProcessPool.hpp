#ifndef PROCESS_POOL_HPP__
  
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/wait.h>
#include "Task.hpp"
// 先描述
class Channel
{
public:
    Channel(int fd, pid_t id)
        : _wfd(fd), _subid(id)
    {
        _name = "chanenl-" + std::to_string(_wfd) + "-" + std::to_string(_subid);
    }
    ~Channel()
    {
    }
    int Fd() const
    {
        return _wfd;
    }

    pid_t Subid() const
    {
        return _subid;
    }
    bool Send(int task)
    {
        ssize_t n = write(_wfd, &task, sizeof(task));
        return n == sizeof(task);
    }
    const std::string &Name() const
    {
        return _name;
    }
    void Close()
    {
        close(_wfd);
    }
     void Wait()
    {
        pid_t rid =waitpid(_subid,nullptr,0);
        (void)rid;
    }


private:
    int _wfd;
    pid_t _subid;
    std::string _name;
};
// 再组织
class ChannelManager
{
public:
    ChannelManager() : _next(0)
    {
    }
    void BuildChannel(int wfd, pid_t subid)
    {
        Channel c(wfd, subid);
        _channels.push_back(c);
    }
    ~ChannelManager()
    {
    }
    int Size() const
    {
        return _channels.size();
    }
    Channel &operator[](int index)
    {
        return _channels[index];
    }
    Channel &Select()
    {
        auto &c = _channels[_next];
        _next++;
        _next %= _channels.size();
        return c;
    }
    void StopSubPocess()
    {
        for (auto &channel : _channels)
        {
            channel.Close();
            std::cout<< "关闭："<<channel.Name()<<std::endl;
        }
    }
    void WaitSubProcess()
    {
        for (auto &channel : _channels)
        {
            channel.Close();
            std::cout<< "回收："<<channel.Name()<<std::endl;
        }
    }

private:
    std::vector<Channel> _channels;
    int _next;
};
const int gdefaultnum = 5;
class ProcessPool
{
public:
    ProcessPool(int num = gdefaultnum)
        : _process_num(num)

    {
        _tm.Register(PrintLog);
        _tm.Register(Download);
        _tm.Register(Upload);
    }
    void work(int rfd)
    {
        while (true)
        {
            int command = 0;

            ssize_t n = read(rfd, &command, sizeof(command));

            if (n == sizeof(command))
            {
                // 根据 command 执行任务
                std::cout << "子进程[" << getpid() << "]收到一个任务码：" << command << std::endl;
                _tm.Execute(command);
            }
            else if (n == 0)
            {
                // 父进程关闭了管道写端
                std::cout << "子进程退出" << std::endl;
                break;
            }
            else
            {
                // 读取出错
                std::cout << "读取出错" << std::endl;
                break;
            }
        }
    }
    bool Create()
    {
        for (int i = 0; i < _process_num; i++)
        {
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
            {
                return false;
            }
            pid_t subid = fork();
            if (subid < 0)
                return false;
            else if (subid == 0)
            {
                close(pipefd[1]);
                work(pipefd[0]);
                close(pipefd[0]);
                exit(0);
            }
            else
            {
                close(pipefd[0]);
                _cm.BuildChannel(pipefd[1], subid);
            }
        }
        return true;
    }

    ~ProcessPool()
    {
    }
    void Run()
    {
        int taskcode = _tm.Code();
        auto &c = _cm.Select();
        std::cout << "选择了一个子进程：" << c.Name() << std::endl;
        c.Send(taskcode);
    }
    void Stop()
    {
        _cm.StopSubPocess();
        _cm.WaitSubProcess();
    }

private:
    ChannelManager _cm;
    int _process_num;
    TaskManger _tm;
};

#endif