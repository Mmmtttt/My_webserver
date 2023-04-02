#ifndef THREADPOOL_H
#define THREADPOLL_H

#include <thread>
#include <list>
#include <semaphore.h>
#include <vector>
#include <utility>
#include "signal.h"
#include <stdlib.h>
#include <deque>
#include <unistd.h>
#include <functional>



template  <typename T>
class threadPoll{
    private:
        bool close=false;
        int threads_num;
        int core_threads_num=3;
        int max_threads_num=20;

        std::vector<pthread_t>  threads;
        std::list<T *> request_list;
        sem_t request_count;
        pthread_mutex_t request_list_locker;
        //pthread_mutex_t threads_vector_locker;

    public:
        threadPoll(){
            close=false;

            threads_num=core_threads_num;
            pthread_mutex_init(&request_list_locker,NULL);
            //pthread_mutex_init(&threads_vector_locker,NULL);
            sem_init(&request_count,0,0);

            for(int i=0;i<threads_num;i++){
                pthread_t thread_id;
                pthread_create(&thread_id,NULL,work,(void*)this);
                threads.push_back(thread_id);
                pthread_detach(threads[i]);
            }

            pthread_t monitor_t;
            pthread_create(&monitor_t,NULL,Monitor,(void*)this);
            pthread_detach(monitor_t);

        }
        
        ~threadPoll(){
            close=true;
        }
        
        static void* Monitor(void*a);
        void* _Monitor(void*a);
        
        static void* work(void*a);
        void* _work(void*a);

        void append(T *t){
            pthread_mutex_lock(&request_list_locker);
            request_list.push_back(t);
            sem_post(&request_count);
            pthread_mutex_unlock(&request_list_locker);
        }
};

template <typename T>
void* threadPoll<T>::_work(void*){
    
    bool close2=false;
    std::function<void(int)> handler = [&close2](int signum){close2=!close2;};
    signal(SIGUSR1, handler.target<void(int)>());
    
    while(true){
        if(close||close2){exit(0);}
        sem_wait(&request_count);
        pthread_mutex_lock(&request_list_locker);
        T *temp =request_list.front();
        request_list.pop_front();
        pthread_mutex_unlock(&request_list_locker);
        temp->first(temp->second);
    }
    return nullptr;
}

template <typename T>
void* threadPoll<T>::_Monitor(void*){
    while(true){
        if(close){exit(0);}
        sleep(1);
        //pthread_mutex_lock(&threads_vector_locker);
        if(threads_num<threads.size()){continue;}
        else if((threads_num>core_threads_num)&&(threads_num<threads.size())){
            pthread_kill(threads.back(),SIGUSR1);
            threads.pop_back();
        }
        else if((threads_num>threads.size())&&(threads_num<max_threads_num)){
            threads.push_back(pthread_t(1ul));
            pthread_create(&threads.front(),NULL,work,(void*)this);
            pthread_detach(threads.back());
        }
        else{}
        //pthread_mutex_unlock(&threads_vector_locker);
    }
    return nullptr;
}

template <typename T>
void* threadPoll<T>::Monitor(void*a){
    threadPoll *temp=(threadPoll*)a;
    temp->_Monitor(a);
    return temp;
}

template <typename T>
void* threadPoll<T>::work(void*a){
    threadPoll *temp=(threadPoll*)a;
    temp->_work(a);
    return temp;
}


#endif