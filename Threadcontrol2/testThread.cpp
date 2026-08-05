#include<iostream>
#include<string>
#include<pthread.h>
#include<unistd.h>
#include<cstdio>
#include<vector>
#include<cstring>















































// const int NUM = 10;

// void *routinue(void *arg)
// {
//     std::string name = static_cast<char *>(arg);
//     int cnt = 5;
//     while(cnt--)
//     {
//         std::cout<<"我是新线程：name = "<<name<<", id = "<<pthread_self()<<std::endl;
//         sleep(1);
//     }
//     return nullptr;
// }
// //创建多线程
// int main()
// {
//     std::vector<pthread_t> tids;
//     for(int i=0; i<NUM; i++)
//     {
//         pthread_t tid;
//         char id[64];
//         snprintf(id, sizeof(id), "thread-%d", i);
//         int n = pthread_create(&tid, nullptr, routinue, (void *)id);
//         if(n==0)
//         {
//             tids.push_back(tid);
//         }
//         else continue;
//     }

//     for(int i=0; i<NUM; i++)
//     {
//         int n = pthread_join(tids[i], nullptr);
//         if(n == 0)
//         {
//             std::cout << "pthread_join success" << std::endl;
//         }
//         else
//         {
//             std::cout << "pthread_join error, n = " << strerror(n) << std::endl;
//         }
//     }
//     return 0;
// }


























// class Task
// {
// public:
//     Task(int a,int b):_a(a),_b(b)
//     {
//     }
//     int Execute()
//     {
//         return _a+_b;
//     }
//     ~Task()
//     {
//     }
// private:
//     int _a;
//     int _b;

// };
// class Result
// {
// public:
//     Result(int result):_result(result)
//     {
//     }
//     int GetResult()
//     {
//         return _result;
//     }
//     ~Result()
//     {
//     }
// private:
//     int _result;
// };
// void *routinue(void *arg)
// {
//     pthread_detach(pthread_self());
//     int cnt = 5;
//     while(cnt--)
//     {
//         std::cout<<"我是新线程：name = "<<static_cast<char *>(arg)<<", id = "<<pthread_self()<<std::endl;
//         sleep(1);
//     }



//     return nullptr;
//     // Task *t = static_cast<Task *>(arg);
//     // sleep(1); 
//     // int ret = t->Execute();
//     // Result *r = new Result(ret);
//     // sleep(100);
//     // //return (void *)r;
//     // pthread_exit((void *)r);
//     // std::cout<<"haha,新线程不应该看到我"<<std::endl;
// }

// // 如果主线程不想再关心新线程，而是当新线程结束的时候，让他自己释放？？
// // 设置新线程为分离状态
// // 技术层面：线程默认是需要被等待的，joinable，如果不想让主线程等待新线程
// // 想让新线程结束之后，自己退出，设置为分离状态(!joinable or detach)  // TODO
// // 理解层面：线程分离，主分离新，新把自己分离。
// // 分离的线程，依旧在进程的地址空间中，进程的所有资源，被分离的线程，依旧可以访问，可以操作。
// // 主不等待新线程。
// // 分离操作
// //如果线程被设置为分离状态，不需要进行join,join会失败

// int main()
// {
//     pthread_t tid;
//     //Task *t = new Task(10,20);
//     pthread_create(&tid,nullptr,routinue,(void*)"thread-1");

//     // 设置新线程为分离状态
//     // pthread_detach(tid);
//     // std::cout<<"主线程分离了新线程"<<std::endl;


//     int cnt=5;
//     while(cnt--)
//     {
//         std::cout << "我是main线程：name = " << "main thread" << ", id = " << pthread_self() << std::endl;
//         sleep(1);
//     }

//     // int n = pthread_join(tid,nullptr);
//     // if(n!=0)
//     // {
//     //     std::cout << "pthread_join error, n = " << strerror(n) << std::endl;
//     // }
//     // else
//     // {
//     //     std::cout << "pthread_join success" << std::endl;
//     // }
//     // sleep(3);
//     // pthread_cancel(tid);

//     // std::cout<<"主线程取消了新线程"<<std::endl;
//     // void *ret=nullptr;
//     // pthread_join(tid,&ret );

//     // Result *ret=nullptr;
//     // pthread_join(tid,(void **)&ret);
//     // int n = ret->GetResult();
//     // std::cout << "线程返回值: " << n << std::endl;
//     // delete t;
//     // delete ret;

//     return 0;
// }