#include <iostream>
#include "../include/server.h"

// void *fun(void *);
// void *mfun(void *);

int main(){
    // int arg;
    // typedef std::pair<void*(*)(void*),void*> T;
    // T t[100];
    // int x[100];

    // T temp(mfun,NULL);
    // threadPoll<T>poll(&temp);
    // for(int i=0;i<100;i++){
    //     x[i]=i;
    //     t[i]=T(fun,(void*)&x[i]);
    //     poll.append(&t[i]);
    // }



    // threadPoll<T>poll;
    // pthread_mutex_lock(&poll.request_list_locker);
    // for(int i=0;i<100;i++){
    //     x[i]=i;
    //     t[i]=T(fun,(void*)&x[i]);
    //     poll.request_list.push_back(&t[i]);
    //     sem_post(&poll.request_count);
    // }
    // pthread_mutex_unlock(&poll.request_list_locker);


    typedef std::pair<void*(*)(void*),void*> T;
    server<T> ser(9191);
    char q;
    // sleep(5);
    // ser.thread_num()=2;
    while(true){
        std::cin>>q;
        if(q=='q'){
            ser.clo();
            sleep(5);
            return 0;
        }
    }
    
    





    getchar();
    return 0;
}

// void *fun(void*arg){
//     std::cout<<"thread id:"<<gettid()<<"       deal with number "<<*(int*)arg<<std::endl;
//     sleep(0.2);
//     std::cout<<"number "<<*(int*)arg<<" done"<<std::endl;
//     return nullptr;
// }
// void *mfun(void*arg){
//     for(int i=0;i<110;i++){
//         std::cout<<"main"<<std::endl;
//         sleep(0.2);
//     }
//     return nullptr;
// }