#include"BlockQueue.hpp"
#include"Task.hpp"
#include<iostream>
#include<ctime>
#include<pthread.h>
#include<unistd.h>

void *consumer(void *arg)
{
        BlockQueue<task_t> *bq = static_cast<BlockQueue<task_t> *>(arg);
    while(1)
    {
        task_t t = bq->pop();
        t();
        //std::cout << "消费者消费数据：" << t.X() << " + " << t.Y() << " = " << t.Result() << std::endl;

       // sleep(1);
    }
 
    return nullptr;
}
void *producter(void *arg)
{
    //int x = 1,y = 1;
    BlockQueue<task_t> *bq = static_cast<BlockQueue<task_t> *>(arg);
    while(1)
    {
           sleep(1);
        std::cout << "生产了一个任务" << std::endl;
        bq->Equeue(Download);  

        // std::cout << "生产者生产数据：" << x << " + " << y << " = " << (x + y) << std::endl;  
        // x++;
        //y++;
     
    }
    return nullptr;
}

int main()
{

    //申请阻塞队列
    BlockQueue<task_t> *bq = new BlockQueue<task_t>();

    
    //构建生产者和消费者
    pthread_t c,p;
    pthread_create(&c,nullptr,consumer,bq);
    pthread_create(&p,nullptr,producter,bq);

    pthread_join(c,nullptr);
    pthread_join(p,nullptr);

    delete bq;
    return 0;
}





















// #include"BlockQueue.hpp"
// #include"Task.hpp"
// #include<iostream>
// #include<ctime>
// #include<pthread.h>
// #include<unistd.h>

// void *consumer(void *arg)
// {
//         BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(arg);
//     while(1)
//     {
//         Task t = bq->pop();
//         t.Execute();
//         std::cout << "消费者消费数据：" << t.X() << " + " << t.Y() << " = " << t.Result() << std::endl;
//         sleep(1);
//     }
 
//     return nullptr;
// }
// void *producter(void *arg)
// {
//     int x = 1,y = 1;
//     BlockQueue<Task> *bq = static_cast<BlockQueue<Task> *>(arg);
//     while(1)
//     {
//            sleep(1);
        
//         bq->Equeue(Task(x, y));  
//         std::cout << "生产者生产数据：" << x << " + " << y << " = " << (x + y) << std::endl;  
//         x++;
//         y++;
     
//     }
//     return nullptr;
// }

// int main()
// {

//     //申请阻塞队列
//     BlockQueue<Task> *bq = new BlockQueue<Task>();

    
//     //构建生产者和消费者
//     pthread_t c,p;
//     pthread_create(&c,nullptr,consumer,bq);
//     pthread_create(&p,nullptr,producter,bq);

//     pthread_join(c,nullptr);
//     pthread_join(p,nullptr);

//     delete bq;
//     return 0;
// }