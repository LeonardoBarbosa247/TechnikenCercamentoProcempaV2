#include "Watcher.hpp"


Watcher::Watcher(string _directory)
{
    this->_directory = _directory;
    try
    {
        for(auto& p: recursive_directory_iterator(_directory.c_str()))
        {
        if(p.is_directory())
        {
            string dir = (p.path().string());
            int size = 0;
            for(auto& s: directory_iterator(dir + "/")) size++;
            directories[dir] = size;
            directoriesKeys.push_back(dir);
        }
     }   
    }
    catch(const std::exception& e)
    {
        cout << endl;
        cout << "Diretório informado: " << this->_directory << " não foi reconhecido" << endl; 
        cout << "    - Este ERRO ocorre apenas se não foi encontrado este diretório" << endl;
        cout << "    - Se, porventura, o diretório informado foi escrito corretamente, entre em contato com a empresa Techniken" << endl;
        cout << "    -   Contato: suporte@techniken.com.br" << endl;
        cout << "    -   Assunto: Software Cercamento - ERRO (não reconhecendo diretório, de busca, informado ) " << endl;
        exit(0);
    }
}

Watcher::~Watcher()
{

}

string Watcher::watching()
{
    while(true)
    {
        for(int i = 0 ; i < directoriesKeys.size() ; i++)
        {
            int size = 0;
            for(auto& p : directory_iterator(directoriesKeys[i] + "/")) size++;
            if(directories[directoriesKeys[i]] != size) return directoriesKeys[i];
        }
    }
}
