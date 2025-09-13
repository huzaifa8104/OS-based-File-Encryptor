#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "Cryption.hpp"
#include "../Processes/Task.hpp"
#include "../fileHandling/ReadEnv.cpp"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <chrono>

using namespace std;

int executeCryption(const string &taskData){
    Task task = Task::fromString(const_cast<string&>(taskData));
    ReadEnv env;
    
    string envKey = env.getenv();
    int key = stoi(envKey);
    
    if(task.action == Action::ENCRYPT){
        char ch;
        while(task.f_stream.get(ch)){
            ch = (ch+key)%256;
            task.f_stream.seekg(-1,ios::cur);
            task.f_stream.put(ch);
        }
        task.f_stream.close();
        
    }else{
        char ch;
        while(task.f_stream.get(ch)){
            ch = (ch-key+256)%256;
            task.f_stream.seekg(-1,ios::cur);
            task.f_stream.put(ch);
        }
       task.f_stream.close();
    }
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    cout<<"Exiting the encryption/decryption process at "<<put_time(now, "%Y-%m-%d %H:%M:%S")<<endl;
    return 0;
    
}