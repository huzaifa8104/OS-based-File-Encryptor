#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include "ProcessManagement.hpp"

#include <cstring>
#include <unistd.h>
#include "../encryptDecrypt/Cryption.hpp"
#include <sys/mman.h>
#include <sys/wait.h>
#include <atomic>
#include <fcntl.h> 
#include <semaphore.h>
#include <thread>
#include <mutex>

using namespace std;

ProcessManagement::ProcessManagement() {
    // Open semaphores
    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);

    if (itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    // Open shared memory
    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd == -1) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    // Set size of shared memory
    if (ftruncate(shmFd, sizeof(SharedMemory)) == -1) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    sharedMem = static_cast<SharedMemory*>(
        mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0)
    );
    if (sharedMem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Initialize shared memory
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySlotsSemaphore);
    unique_lock<mutex> lock(queueLock);

    if (sharedMem->size.load() >= 1000) {
        cout << "Queue is full" << endl;
        lock.unlock();
        sem_post(emptySlotsSemaphore);
        return false;
    }

    strncpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str(), sizeof(sharedMem->tasks[sharedMem->rear]) - 1);
    sharedMem->tasks[sharedMem->rear][sizeof(sharedMem->tasks[sharedMem->rear]) - 1] = '\0';  // ensure null termination

    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);
    lock.unlock();

    sem_post(itemsSemaphore);

   
    thread thread_1(&ProcessManagement::executeTasks, this);
    thread_1.detach();

    return true;
}

void ProcessManagement::executeTasks() {
    sem_wait(itemsSemaphore);
    unique_lock<mutex> lock(queueLock);

    char taskStr[256];
    strncpy(taskStr, sharedMem->tasks[sharedMem->front], sizeof(taskStr) - 1);
    taskStr[sizeof(taskStr) - 1] = '\0';

    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    sem_post(emptySlotsSemaphore);
    lock.unlock();

    executeCryption(taskStr);
}

ProcessManagement::~ProcessManagement() {
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);

    sem_close(itemsSemaphore);
    sem_close(emptySlotsSemaphore);

    sem_unlink("/items_semaphore");
    sem_unlink("/empty_slots_semaphore");
}
