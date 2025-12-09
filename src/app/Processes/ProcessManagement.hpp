#ifndef PROCESSMANAGEMENT_HPP
#define PROCESSMANAGEMENT_HPP

#include "Task.hpp"
#include <queue>
#include <memory>
#include <atomic>
#include <semaphore.h>
#include <mutex>

using namespace std;
class ProcessManagement{


    public:
        ProcessManagement();
        ~ProcessManagement();
        bool submitToQueue(std::unique_ptr<Task> task);
        void executeTasks();
    private:
        struct SharedMemory{
            std::atomic<int> size;
            char tasks[1000][256];
            atomic<int> front;
            atomic<int> rear;

            void printSharedMemory(){
                cout<<"Size:"<<size.load()<<endl;
                cout<<"Front:"<<front.load()<<endl; 
                cout<<"Rear:"<<rear.load()<<endl;
            }
        };
       SharedMemory* sharedMem;
       int shmFd;

       sem_t* itemsSemaphore;
       sem_t* emptySlotsSemaphore;
       
       const char* SHM_NAME = "/my_queue";
       std::mutex queueLock;
};

#endif