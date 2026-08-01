#ifndef SEM_HPP
#define SEM_HPP
#include <memory>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sstream>
#include <vector>

const std::string SEM_PATH = "/tmp";
const int SEM_PROJ_ID = 0x1234;
const int defaultnum = 1;
#define GET_SEM (IPC_CREAT)
#define BUILD_SEM (IPC_CREAT | IPC_EXCL)

// 一个把整数转成十六进制的函数
std::string int_to_hex(int num)
{
    char hex[64];
    snprintf(hex, sizeof(hex), "0x%x", num);
    return std::string(hex);
}

// class Semaphore
// {
//     // struct sembuf
//     //  {
//     //      unsigned short sem_num; /* semaphore index in array */
//     //      short sem_op;           /* semaphore operation */
//     //      short sem_flg;          /* operation flags */
//     //  };
// public:
//     Semaphore(int semid) : _semid(semid)
//     {
//     }
//     int Id() const
//     {
//         return _semid;
//     }
//     void P()
//     {
//         PV(-1);
//     }
//     void V()
//     {
//         PV(1);
//     }

//     ~Semaphore()
//     {
//         if (_semid > 0)
//         {
//             int n = semctl(_semid, 0, IPC_RMID);
//             if (n == -1)
//             {
//                 perror("semctl failed");
//             }
//         }
//         std::cout << "Semaphore destroyed: " << _semid << std::endl;
//     }

// private:
//     void PV(int date)
//     {
//         struct sembuf sem_buf;
//         sem_buf.sem_num = 0;        // 第一个信号量
//         sem_buf.sem_op = date;      // 信号量操作，-1表示P操作，+1表示V操作
//         sem_buf.sem_flg = SEM_UNDO; // 设置信号量操作标志
//         int n = semop(_semid, &sem_buf, 1);
//         if (n == -1)
//         {
//             perror("semop failed");
//         }
//         // std::cout << "Semaphore P: " << _semid << std::endl; // 打印信号量操作信息
//     }

// private:
//     int _semid;
// };

// // 简单的建造者模式
// class SemaphoreBuilder
// {
// public:
//     SemaphoreBuilder()
//     {
//     }
//     ~SemaphoreBuilder()
//     {
//     }
//     SemaphoreBuilder &SetVar(int val)
//     {
//         _val = val;
//         return *this;
//     }
//     std::shared_ptr<Semaphore> build(int flag, int num = defaultnum)
//     {
//         // 1、构建键值
//         std::cout << "Building a semaphore" << std::endl;
//         key_t key = ftok(SEM_PATH.c_str(), SEM_PROJ_ID);
//         if (key == (key_t)-1)
//         {
//             perror("ftok failed");
//             return nullptr;
//         }
//         std::cout << "Semaphore key: " << int_to_hex(key) << std::endl;

//         // 2、创建信号量集合
//         int semid = semget(key, num, flag);
//         if (semid == (int)-1)
//         {
//             perror("semget failed");
//             return nullptr;
//         }
//         std::cout << "Semaphore id: " << semid << std::endl;

//         // 3、初始化操作
//         if (flag == BUILD_SEM)
//         {
//             if (!Init(semid, 0, _val))
//             {
//                 return nullptr;
//             }
//         }
//         return std::make_shared<Semaphore>(semid);
//     }

// private:
//     bool Init(int semid, int num, int val)
//     {
//         union semun
//         {
//             int val;               /* Value for SETVAL */
//             struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
//             unsigned short *array; /* Array for GETALL, SETALL */
//             struct seminfo *__buf; /* Buffer for IPC_INFO
//                                       (Linux-specific) */
//         } un;
//         un.val = val; // 设置信号量的初始值
//         int n = semctl(semid, num, SETVAL, un);
//         if (n == -1)
//         {
//             perror("semctl failed");
//             return false;
//         }
//         return true;
//     }

// private:
//     int _val; // 信号量的初始值
// };

/////////////////////////////////////////// 先设计建设者模式的代码结构

// 产品类,只需要关心自己的使用
// 这里Semaphore不是一个信号量，而是一个信号量集合，要指明要PV哪个信号量
class Semaphore
{

public:
    explicit Semaphore(int semid)
        : _semid(semid)
    {
    }
    void P(int who)
    {
        PV(who, -1);
    }
    void V(int who)
    {
        PV(who, 1);
    }
    int Id() const
    {
        return _semid;
    }

    ~Semaphore()
    {
        if (_semid > 0)
        {
            int n = semctl(_semid, 0, IPC_RMID);
            if (n == -1)
            {
                perror("semctl failed");
            }
        }
        std::cout << "Semaphore destroyed: " << _semid << std::endl;
    }

private:
    void PV(int who, int date)
    {
        struct sembuf sem_buf;
        sem_buf.sem_num = who;      // 第who个信号量
        sem_buf.sem_op = date;      // 信号量操作，-1表示P操作，+1表示V操作
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

// 建造者接口
class SemaphoreBuilder
{
public:
    virtual ~SemaphoreBuilder() = default;
    virtual void BuildKey() = 0;                           // 构建键值
    virtual void SetPermissions(int perm) = 0;             // 设置权限
    virtual void SetSemNum(int num) = 0;                   // 创建信号量集合
    virtual void SetInitVal(std::vector<int> initVal) = 0; // 初始化操作
    virtual void build(int flag) = 0;                      // 完成建造过程
    virtual void InitSem() = 0;                            // 初始化信号量

    virtual std::shared_ptr<Semaphore> GetSem() = 0; // 获取建造好的信号量对象
};

// 具体建造者类
class ConcreteSemaphoreBuilder : public SemaphoreBuilder
{
public:
    ConcreteSemaphoreBuilder()
    {
    }
    virtual void BuildKey() override // 构建键值
    {
        // 1、构建键值
        std::cout << "Building a semaphore" << std::endl;
        _key = ftok(SEM_PATH.c_str(), SEM_PROJ_ID);
        if (_key == (key_t)-1)
        {
            perror("ftok failed");
            exit(1);
        }
        std::cout << "Semaphore key: " << int_to_hex(_key) << std::endl;
    }
    virtual void SetPermissions(int perm) override // 设置权限
    {
        _perm = perm;
    }
    virtual void SetSemNum(int num) override // 创建信号量集合
    {
        _num = num;
    }
    virtual void SetInitVal(std::vector<int> initVal) override // 初始化操作
    {
        _initVal = initVal;
    }
    virtual void build(int flag) override // 完成建造过程
    {
        // 2、创建信号量集合
        int semid = semget(_key, _num, flag | _perm);
        if (semid == (int)-1)
        {
            perror("semget failed");
            exit(2);
        }
        std::cout << "Semaphore id: " << semid << std::endl;

        _sem = std::make_shared<Semaphore>(semid);
    }
    virtual void InitSem() override
    {
        // 3、初始化操作,初始化信号量集合
        if (_num > 0 && _initVal.size() == _num)
        {
            for (int i = 0; i < _num; i++)
            {
                if (!Init(_sem->Id(), i, _initVal[i]))
                {
                    std::cerr << "Failed to initialize semaphore " << i << std::endl;
                    exit(3);
                }
            }
        }
    }

    std::shared_ptr<Semaphore> GetSem() override { return _sem; } // 获取建造好的产品对象
    virtual ~ConcreteSemaphoreBuilder() override
    {
    }
private:
    bool Init(int semid, int num, int val)
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
    key_t _key;                      // 键值
    int _perm;                       // 权限
    int _num;                        // 信号量集合的数量
    std::vector<int> _initVal;       // 初始化操作的值
    std::shared_ptr<Semaphore> _sem; // 我们具体要建造的产品对象
};

// 指挥者类
class Director
{
public:
    void construct(
        std::shared_ptr<SemaphoreBuilder> builder,
        int flag,
        int perm = 0666,
        int num = defaultnum,
        std::vector<int> initVal = {1})
    {
        builder->BuildKey();
        builder->SetPermissions(perm);
        builder->SetSemNum(num);
        builder->SetInitVal(initVal);
        builder->build(flag);

        if (flag == BUILD_SEM)
        {
            builder->InitSem();
        }
    }
};

#endif