#include<iostream>
#include<cstring>
#include"comm.hpp"

int main()
{
    key_t key = ftok(PATHNAME,PROJ_ID);

    if(key==(key_t)-1)
    {
        perror("ftok");
        return 1;
    }
     int msgid = msgget(key,0);

     if(msgid==-1)
     {
        perror("msgget");
        return 1;
     }

     Message message{};

     message.mtype = 1;

     while(std::cin.getline(message.mtext,sizeof(message.mtext)))
     {
        if(msgsnd(msgid,&message,strlen(message.mtext),0)==-1)
        {
            perror("msgsnd");
            break;

        }
        if(strcmp(message.mtext,"quit")==0)
        {
            break;
        }
     }


    return 0;
}