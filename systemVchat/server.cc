#include<iostream>
#include<cstring>
#include<cerrno>
#include"comm.hpp"

int main()
{
    key_t key = ftok(PATHNAME,PROJ_ID);

    if(key == (key_t)-1){
        perror("ftok");
        return 1;
    }
    
    int msgid = msgget(key,IPC_CREAT | IPC_EXCL | 0666);

    if(msgid==-1)
    {
        perror("msgget");
        return 1;
    }

    Message message{};

    while(true)
    {
        ssize_t n = msgrcv(msgid,&message,sizeof(message.mtext)-1,1,0);
        if(n==-1)
        {
            if(errno==EINTR)
            {
                continue;
            }
            perror("msgrcv");
            break;
        }
         message.mtext[n] = '\0';
        std::cout << "client:" << message.mtext << std::endl;
        if(strcmp(message.mtext,"quit") == 0)
        {
            break;
        }
    }
    if(msgctl(msgid,IPC_RMID,nullptr) == -1)
    {
        perror("msgctl");
    }
    
    return 0;
}