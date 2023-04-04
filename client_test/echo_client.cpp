#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../include/threadpoll.h"


typedef std::pair<void*(*)(void*),void*> T;
char s[40]="message from socket id : ";
void*handle(void*arg);
sockaddr_in serv_addr;

#define NUM 127995

int sock[NUM];

int main(){
    
    T*t[NUM];

    //sock=socket(PF_INET,SOCK_STREAM,0);
    

    memset(&serv_addr,0,sizeof(serv_addr));

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(9191);
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");


    threadPoll<T> poll(NULL);
    for(int i=0;i<NUM;i++){
        sock[i]=socket(PF_INET,SOCK_STREAM,0);
        if(sock[i]==-1){std::cout<<"socket create error "<<i<<std::endl;return 0;}
        
        //std::pair<int,int> *arg=new std::pair<int,int>(sock[i],i);
        int *arg=new int (i);
        
        t[i]=new T(handle,(void*)arg);
    }

    for(int i=0;i<NUM;i++){
        

        poll.append(t[i]);
    }


    std::cout<<"done"<<std::endl;
    getchar();
    // for(int i=0;i<NUM;i++){
    //     delete t[i];
    // }
    

    return 0;

}
void*handle(void*arg){
    std::cout<<sock[*(int*)arg]<<std::endl;
    if(connect(sock[*(int*)arg],(sockaddr*)&serv_addr,sizeof(serv_addr))==-1){std::cout<<"connect error"<<std::endl;return NULL;}

    char ss[40];
    strcpy(ss,s);
    char num[10];
    sprintf(num,"%d",*(int*)arg);
    strcat(ss,num);
    std::cout<<ss<<std::endl;
    write(sock[*(int*)arg],num,10);
    //std::cout<<"write ok"<<std::endl;
    char p[40];
    read(sock[*(int*)arg],p,40);
    p[39]='\0';
    std::cout<<p<<std::endl;
    return NULL;
}