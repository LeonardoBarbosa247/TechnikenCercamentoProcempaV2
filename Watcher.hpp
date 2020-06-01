#ifndef Watcher_hpp
#define Watcher_hpp

#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>


using namespace std;
using namespace std::filesystem;


class Watcher
{
    private:
        /*
        vector<string> path;
        vector<int> size;
        */
        vector<pair<string,int>> path;
        string _directory;
        //bool running;
        //void threadPrint();
        //void threadWatching(string* ss);
    public:
        Watcher(string directory);
        ~Watcher();
        string watching();
};

#endif