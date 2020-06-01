#include "Watcher.hpp"


Watcher::Watcher(string _directory)
{
    _directory[_directory.size() - 1] = '\0';
    this->_directory = _directory;
    try
    {
        //cout << _directory[_directory.size() - 1] << endl;
        string dir = _directory;
        int size = 0;
        for(auto& s: directory_iterator(dir + "/")) size++;
        directories[dir] = size; 
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
        int size = 0;
        for(auto& p : directory_iterator(this->_directory + "/")) size++;
        if(directories[this->_directory] != size) return this->_directory;
    }
}
