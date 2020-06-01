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
            ifstream file((p.path().string() + "/").c_str(), ios::binary | ios::ate );
            pair<string, int> myPair;
            myPair.first = (p.path().string() + "/");
            myPair.second = file.tellg();
            path.push_back(myPair);
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
        int i = 0;
        for(auto& p: recursive_directory_iterator(this->_directory.c_str()))
        {
            if(p.is_directory())
            {
                ifstream file((p.path().string() + "/").c_str(), ios::binary | ios::ate );
                if(path[i++].second != file.tellg())
                {
                    return (p.path().string());
                }
            }
        }
    }
}
