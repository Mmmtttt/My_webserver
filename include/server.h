#ifndef SEVER_H
#define SEVER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <threadpoll.h>

template <typename T>
class server{
    private:
        bool _close=false;
        int epoll_size=50;

        int port;
        int server_sock;
        int epoll_fd;
        epoll_event *events;
        epoll_event temp;
        threadPoll<T> *threadpoll;

    public:
        server(int po):port(po){
            initial();

            T t(epoll_loop,(void*)this);
            threadpoll=new threadPoll<T>(&t);
        }
        ~server(){delete threadpoll;}

    private:
        void initial(){
            sockaddr_in serv_addr;
            memset(&serv_addr,0,sizeof(serv_addr));
            serv_addr.sin_family=AF_INET;
            serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
            serv_addr.sin_port=htons(port);
            bind(server_sock,(sockaddr*)&serv_addr,sizeof(serv_addr));
            listen(server_sock,5);

            epoll_initial();
        }
        void epoll_initial(){
            epoll_fd=epoll_create(50);
            events=new epoll_event[epoll_size];
            temp.data.fd=server_sock;
            temp.events=EPOLLIN;
            epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_sock,NULL);
        }

        static void* epoll_loop(void*arg){
            server<T> *s=(server<T> *)arg;
            s->_epoll_loop(NULL);
            return NULL;
        }
        void* _epoll_loop(void*){
            int event_count=epoll_wait(epoll_fd,events,epoll_size,-1);
            for(int i=0;i<event_count;i++){
                if(events[i].data.fd==server_sock){
                    sockaddr* clnt_addr;
                    socklen_t adr_sz=sizeof(clnt_addr);
                    int clnt_sock=accept(server_sock,clnt_addr,&adr_sz);
                    temp.events=EPOLLIN;
                    temp.data.fd=clnt_sock;
                    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,clnt_sock,&temp);
                }
                else{
                    int *temp_fd=new int(events[i].data.fd);
                    std::pair<void*,void*>*ptr=new std::pair<void*,void*>((void*)this,(void*)temp_fd);
                    T *t=new T(handler,(void*)ptr);
                    threadpoll->append(t);
                }
            }
            if(_close){
                close(server_sock);
                close(epoll_fd);
                return NULL;
            }
            return NULL;
        }

        static void* handler(void*arg){
            server<T> *s=(server<T> *)(((std::pair<void*,void*>*)arg)->first);
            s->_handler(((std::pair<void*,void*>*)arg)->second);
            return NULL;
        }
        void*_handler(void*temp_fd){
            char buf[50];
            int str_len=read(*(int*)temp_fd,buf,50);
            write(*(int*)temp_fd,buf,str_len);
            epoll_ctl(epoll_fd,EPOLL_CTL_DEL,*(int*)temp_fd,NULL);
            close(*(int*)temp_fd);
            return NULL;
        }


    public:
        void clo(){_close=true;}

};




#endif