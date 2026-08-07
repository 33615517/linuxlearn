#include <iostream>
#include <ctime>
#include <pthread.h>
#include <unistd.h>
#include "RingQueue.hpp"
#include "Mutex.hpp"

struct threaddata
{

    RingQueue<int> *rq;
    std::string name;
} ;
void *consumer(void *arg)
{
        threaddata *td = static_cast<threaddata *>(arg);
    while (1)
    {
        // sleep(3);
        // 1.消费队列
        int data = 0;
        td->rq->Pop(&data);

        // 2.处理任务 -- 处理任务的时候，这个任务，已经被拿到线程的上下文中了，不属于队列了
        std::cout << td->name << "消费者拿到一个数据：" << data << std::endl;
        sleep(3);
    }
    return nullptr;
}
void *producter(void *arg)
{
    // int x = 1,y = 1;
    threaddata *td = static_cast<threaddata *>(arg);
    int data = 1;
    while (1)
    {
         sleep(1);
        std::cout << "生产了一个任务:" << data << std::endl;
        td->rq->Equeue(data++);

        // std::cout << "生产者生产数据：" << x << " + " << y << " = " << (x + y) << std::endl;
        // x++;
        // y++;
    }
    return nullptr;
}

int main()
{

    // 申请阻塞队列
    RingQueue<int> *rq = new RingQueue<int>();

    // 构建生产者和消费者
    pthread_t c[2], p[3];
    struct threaddata *td1 = new threaddata();
    td1->name = "cthread-1";
    td1->rq = rq;
    pthread_create(c, nullptr, consumer, td1);
    struct threaddata *td2 = new threaddata();
    td2->name = "cthread-2";
    td2->rq = rq;
    pthread_create(c + 1, nullptr, consumer, td2);
    struct threaddata *td3 = new threaddata();
    td3->name = "pthread-3";
    td3->rq = rq;
    pthread_create(p, nullptr, producter, td3);
    struct threaddata *td4 = new threaddata();
    td4->name = "pthread4-";
    td4->rq = rq;
    pthread_create(p + 1, nullptr, producter, td4);
    struct threaddata *td5 = new threaddata();
    td5->name = "pthread-5";
    td5->rq = rq;
    pthread_create(p + 2, nullptr, producter, td5);

    pthread_join(c[0], nullptr);
    pthread_join(c[1], nullptr);
    pthread_join(p[0], nullptr);
    pthread_join(p[1], nullptr);
    pthread_join(p[2], nullptr);
    delete rq;
    return 0;
}
