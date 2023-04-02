#include "../include/threadpoll.h"

// template <typename T>
// void* threadPoll<T>::_work(void*){
    
//     bool close2=false;
//     std::function<void(int)> handler = [&close2](int signum){close2=!close2;};
//     signal(SIGUSR1, handler.target<void(int)>());
    
//     while(true){
//         if(close||close2){exit(0);}
//         sem_wait(&request_count);
//         pthread_mutex_lock(&request_list_locker);
//         T *temp =request_list.front();
//         request_list.pop_front();
//         pthread_mutex_unlock(&request_list_locker);
//         temp->first(temp->second);
//     }
//     return nullptr;
// }

// template <typename T>
// void* threadPoll<T>::_Monitor(void*){
//     while(true){
//         if(close){exit(0);}
//         sleep(1);
//         //pthread_mutex_lock(&threads_vector_locker);
//         if(threads_num<threads.size()){continue;}
//         else if((threads_num>core_threads_num)&&(threads_num<threads.size())){
//             pthread_kill(threads.back(),SIGUSR1);
//             threads.pop_back();
//         }
//         else if((threads_num>threads.size())&&(threads_num<max_threads_num)){
//             threads.push_back(pthread_t(1ul));
//             pthread_create(&threads.front(),NULL,work,(void*)this);
//             pthread_detach(threads.back());
//         }
//         else{}
//         //pthread_mutex_unlock(&threads_vector_locker);
//     }
//     return nullptr;
// }

// template <typename T>
// void* threadPoll<T>::Monitor(void*a){
//     threadPoll *temp=(threadPoll*)a;
//     temp->_Monitor(a);
//     return temp;
// }

// template <typename T>
// void* threadPoll<T>::work(void*a){
//     threadPoll *temp=(threadPoll*)a;
//     temp->_work(a);
//     return temp;
// }