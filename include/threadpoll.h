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
#include <iostream>

template <typename T>
class threadPoll
{
private:
    bool close = false;
    int threads_num = 6;
    int core_threads_num = 3;
    int max_threads_num = 20;

    std::vector<pthread_t> threads;
    std::list<T *> request_list;
    sem_t request_count;
    pthread_mutex_t request_list_locker;
    // pthread_mutex_t threads_vector_locker;
    T *mainthread;

public:
    threadPoll(T *maint = NULL) : mainthread(maint), close(false)
    {

        //*****************************initialize
        // threads_num=core_threads_num;
        pthread_mutex_init(&request_list_locker, NULL);
        // pthread_mutex_init(&threads_vector_locker,NULL);
        sem_init(&request_count, 0, 0);

        //*****************************create main thread (alternative)
        if (maint != NULL)
        {
            pthread_t main_tid;
            pthread_create(&main_tid, NULL, mainThread, (void *)this);
            pthread_detach(main_tid);
            int priority = sched_get_priority_max(SCHED_FIFO);
            pthread_setschedprio(main_tid, priority);
        }
        //*****************************create work thread
        for (int i = 0; i < core_threads_num; i++)
        {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, work, (void *)this);
            threads.push_back(thread_id);
            // std::cout<<"create thread "<<threads.back()<<std::endl;
            std::cout << "create thread " << thread_id << std::endl;
            pthread_detach(threads[i]);
        }

        //*****************************create monitor
        pthread_t monitor_t;
        pthread_create(&monitor_t, NULL, Monitor, (void *)this);
        pthread_detach(monitor_t);
    }
    void clos()
    {
        
        close = true;
    }

    ~threadPoll()
    {
        delete mainthread;
        clos();
        sleep(1);
    }

    void append(T *t)
    {
        pthread_mutex_lock(&request_list_locker);
        request_list.push_back(t);
        sem_post(&request_count);
        pthread_mutex_unlock(&request_list_locker);
    }

    int &thread_num() { return threads_num; }

private:
    static void *Monitor(void *a);
    void *_Monitor(void *a);

    static void *work(void *a);
    void *_work(void *a);

    static void *mainThread(void *a);
    void *_mainThread(void *a);
};

//*********************************************************************
template <typename T>
void *threadPoll<T>::_work(void *)
{

    int oldstate;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
    // 线程的具体工作
    pthread_setcancelstate(oldstate, NULL);

    while (true)
    {
        if (close)
        {
            std::cout << gettid() << " closed" << std::endl;
            exit(0);
        }
        sem_wait(&request_count);
        pthread_mutex_lock(&request_list_locker);
        T *temp = request_list.front();
        request_list.pop_front();
        pthread_mutex_unlock(&request_list_locker);
        temp->first(temp->second);
        delete temp;
    }
    return nullptr;
}

//*********************************************************************
template <typename T>
void *threadPoll<T>::_Monitor(void *)
{

    while (true)
    {
        if (close)
        {
        }
        sleep(1);
        // pthread_mutex_lock(&threads_vector_locker);
        if (threads_num == threads.size())
        {
            continue;
        }
        else if ((threads_num < threads.size()))
        {
            std::cout << "Monitor kill thread " << threads.back() << std::endl;
            pthread_cancel(threads.back());
            threads.pop_back();
        }
        else if ((threads_num > threads.size()) && (threads_num < max_threads_num))
        {
            threads.push_back(pthread_t(1ul));
            pthread_create(&threads.back(), NULL, work, (void *)this);
            std::cout << "Monitor create thread " << threads.back() << std::endl;
            pthread_detach(threads.back());
        }
        else
        {
        }
        // pthread_mutex_unlock(&threads_vector_locker);
    }
    return nullptr;
}

//*********************************************************************
template <typename T>
void *threadPoll<T>::_mainThread(void *a)
{
    while (true)
    {
        if (close)
        {
            exit(0);
        }
        mainthread->first(mainthread->second);
    }

    return a;
}

//*********************************************************************
template <typename T>
void *threadPoll<T>::mainThread(void *a)
{
    std::cout << "main thread id : " << gettid() << std::endl;
    threadPoll *temp = (threadPoll *)a;
    temp->_mainThread(a);
    return temp;
}

//*********************************************************************
template <typename T>
void *threadPoll<T>::Monitor(void *a)
{
    std::cout << "Monitor id : " << gettid() << std::endl;
    threadPoll *temp = (threadPoll *)a;
    temp->_Monitor(a);
    return temp;
}

template <typename T>
void *threadPoll<T>::work(void *a)
{
    threadPoll *temp = (threadPoll *)a;
    temp->_work(a);
    return temp;
}

#endif