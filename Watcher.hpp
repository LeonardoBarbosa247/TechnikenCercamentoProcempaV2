#ifndef Watcher_hpp
#define Watcher_hpp

#include <vector>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <map>


using namespace std;
using namespace std::filesystem;


class Watcher
{
    private:
        map<string, int> directories;
        string _directory;
        vector<string> directoriesKeys;
    public:
        Watcher(string directory);
        ~Watcher();
        string watching();
};

#endif