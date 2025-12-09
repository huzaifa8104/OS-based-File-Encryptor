# File Encryptor using Multi-Threading
A multiprocessing + multithreading task-execution engine using shared memory, semaphores, and a background worker thread.  
Encrypty is designed to queue, manage, and execute system-level tasks securely and efficiently, separating orchestration from execution through a well-defined process and memory model.

## Features

### Multithreaded Task Execution
A background **worker thread** continuously polls for tasks and executes them without blocking the main program flow.

### Multiprocessing With Shared Memory
Tasks are executed in isolated **child processes** created via `fork()`, enabling safer execution of external commands and preventing crashes from affecting the main service.

### Synchronization & Safety
The system uses:
- POSIX shared memory (`shm_open`, `mmap`)
- POSIX semaphores (producer/consumer model)
- `std::atomic` variables for state tracking  

This ensures proper coordination between threads and processes.

### Structured Task Queue
A shared-memory queue stores:
- Commands  
- Parameters  
- Execution states  
- Results or exit codes  

## Project Structure
```
Encrypty/
│
├── src/
│ ├── ProcessManagement.cpp
│ ├── ProcessManagement.h
│ ├── SharedMemory.h
│ ├── main.cpp
│ └── utilities...
│
├── include/
│ └── headers used across modules
│
├── CMakeLists.txt
└── README.md
```

## Building the Project

### Requirements
- C++17 compiler (GCC / Clang)
- CMake 3.10+
- Linux or macOS (for POSIX shared memory)

### Build Steps

```bash
mkdir build
cd build
cmake ..
make
```
### Running
```
./encrypty
```

## License
This File Encryptor is open source and released under the MIT License. Feel free to use, modify, and distribute it as per the terms of the license.