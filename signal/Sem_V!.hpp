#ifndef SEM_HPP
#define SEM_HPP
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sstream>

const std::string SEM_PATH = "/tmp";
const int SEM_PROJ_ID = 0x1234;
const int defaultnum = 1;
#define GET_SEM (IPC_CREAT)
#define BUILD_SEM (IPC_CREAT | IPC_EXCL | 0666)

// 一个把整数转成十六进制的函数
std::string int_to_hex(int num)
{
    char hex[64];
    snprintf(hex, sizeof(hex), "0x%x", num);
    return std::string(hex);
}

class Semaphore  
{
    //struct sembuf
    // {
    //     unsigned short sem_num; /* semaphore index in array */
    //     short sem_op;           /* semaphore operation */
    //     short sem_flg;          /* operation flags */
    // };
public:
    Semaphore(int semid) : _semid(semid)
    {
    }
    int Id() const
    {
        return _semid;
    }
    void P()
    {
        PV(-1);
    }
    void V()
    {
        PV(1);
    }
    
    ~Semaphore()
    {
        if(_semid > 0)
        {
            int n = semctl(_semid, 0, IPC_RMID);
            if(n == -1)
            {
                perror("semctl failed");
            }
        }
        std::cout << "Semaphore destroyed: " << _semid << std::endl;
    }

private:
    void PV(int date)
    {
        struct sembuf sem_buf;
        sem_buf.sem_num = 0; // 第一个信号量
        sem_buf.sem_op = date; // 信号量操作，-1表示P操作，+1表示V操作
        sem_buf.sem_flg = SEM_UNDO; // 设置信号量操作标志
        int n = semop(_semid, &sem_buf, 1);
        if (n == -1)
        {
            perror("semop failed");
        }
       // std::cout << "Semaphore P: " << _semid << std::endl; // 打印信号量操作信息
    }
private:
    int _semid;
};

// 简单的建造者模式
class SemaphoreBuilder
{
public:
    SemaphoreBuilder()
    {
    }
    ~SemaphoreBuilder()
    {
    }
    SemaphoreBuilder &SetVar(int val)
    {
        _val = val;
        return *this;
    }
    std::shared_ptr<Semaphore> build(int flag, int num = defaultnum)
    {
        // 1、构建键值
        std::cout << "Building a semaphore" << std::endl;
        key_t key = ftok(SEM_PATH.c_str(), SEM_PROJ_ID);
        if (key == (key_t)-1)
        {
            perror("ftok failed");
            return nullptr;
        }
        std::cout << "Semaphore key: " << int_to_hex(key) << std::endl;

        // 2、创建信号量集合
        int semid = semget(key, num, flag);
        if (semid == (int)-1)
        {
            perror("semget failed");
            return nullptr;
        }
        std::cout << "Semaphore id: " << semid << std::endl;

        // 3、初始化操作
        if (flag == BUILD_SEM)
        {
            if (!Init(semid, 0 , _val))
            {
                return nullptr;
            }
        }
        return std::make_shared<Semaphore>(semid);
    }

private:
    bool Init(int semid, int num , int val)
    {
        union semun
        {
            int val;               /* Value for SETVAL */
            struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
            unsigned short *array; /* Array for GETALL, SETALL */
            struct seminfo *__buf; /* Buffer for IPC_INFO
                                      (Linux-specific) */
        } un;
        un.val = val; // 设置信号量的初始值
        int n = semctl(semid, num, SETVAL, un);
        if (n == -1)
        {
            perror("semctl failed");
            return false;
        }
        return true;
    }
private:
    int _val;// 信号量的初始值
};

#endif