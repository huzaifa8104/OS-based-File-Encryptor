#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <fstream>
#include <string>
#include "IO.hpp"
#include <string>
#include <sstream>

using namespace std;

class ReadEnv{
    public:
        string getenv(){
            string env_path = ".env";
            IO io(env_path);
            fstream f_stream = io.getFileStream();
            stringstream buffer;
            buffer << f_stream.rdbuf();
            string content = buffer.str();
            return content;
        }
};