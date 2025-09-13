#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <filesystem>
#include "./src/app/Processes/ProcessManagement.hpp"
#include "./src/app/Processes/Task.hpp"
#include <ctime>
#include <iomanip>

using namespace std;

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
	string directory;
    string action;

    cout<<"Enter the directory path:"<<endl;
    getline(cin, directory);

    
    cout<<"Enter the action(Encrypt/Decrypt):"<<endl;
    getline(cin, action);

    try
    {
        if(fs::exists(directory) && fs::is_directory(directory)){
            ProcessManagement processManagement;
                for(const auto &entry: fs::recursive_directory_iterator(directory)){
                    if(entry.is_regular_file()){
                        string filePath = entry.path().string();
                        IO io(filePath);
                        fstream f_stream = io.getFileStream();
                        if(f_stream.is_open()){
                            Action taskAction = (action == "ENCRYPT") ? Action::ENCRYPT : Action::DECRYPT;
                            auto task = make_unique<Task>(move(f_stream), taskAction, filePath);

                            time_t t = time(nullptr);
                            tm* now = localtime(&t);    
                            cout<<"Entering the encryption/decryption process at "<<put_time(now, "%Y-%m-%d %H:%M:%S")<<endl;
                            processManagement.submitToQueue(move(task));
                        }else{
                        cout<<"Unable to open file: "<<filePath<<endl;
                        }
                    }
                }
        
            processManagement.executeTasks();
        }else{
            cout<<"Invalid Directory Path"<<endl;
        }
    }
    catch(const fs::filesystem_error& ex)
    {
       cout<<"File System Error: "<<ex.what()<<endl;
    }
    
}

