#include <iostream>
#include "../include/threadpoll.h"

void *fun(void *);

int main(){
    int arg;
    typedef std::pair<void*(*)(void*),void*> T;
    T t[100];
    int x[100];

    threadPoll<T>poll;
    for(int i=0;i<100;i++){
        x[i]=i;
        t[i]=T(fun,(void*)&x[i]);
        poll.append(&t[i]);
    }
    // threadPoll<T>poll;
    // pthread_mutex_lock(&poll.request_list_locker);
    // for(int i=0;i<100;i++){
    //     x[i]=i;
    //     t[i]=T(fun,(void*)&x[i]);
    //     poll.request_list.push_back(&t[i]);
    //     sem_post(&poll.request_count);
    // }
    // pthread_mutex_unlock(&poll.request_list_locker);
    getchar();
}

void *fun(void*arg){
    std::cout<<"thread id:"<<gettid()<<"       deal with number "<<*(int*)arg<<std::endl;
    sleep(0.1);
    return nullptr;
}