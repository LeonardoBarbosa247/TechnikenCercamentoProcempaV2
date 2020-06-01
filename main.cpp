#include <iostream>
#include <sstream>
#include <filesystem>
#include <dirent.h>
#include <fstream>
#include "Watcher.hpp"
#include "Plate.hpp"
#include <map>
#include <chrono>
#include <thread>

int main(int argc, char** argv)
{
    /*
    pair<string,string> a("29052020","062357");
    pair<string,string> b("29052020","062356");

    cout << ( a == b ? "iquais" : "difference") << endl;
    return 0;
    */
    if(argc < 3)
    {
        cout << "ERROR: Não foram informados todos os parâmetros " << endl;
        return 0;
    }
    path destiny = argv[2];  
    try
    {
        for(auto& p: recursive_directory_iterator(destiny.c_str())) break;
    }
    catch(exception& e)
    {
        cout << endl;
        cout << "Diretório informado: " << destiny.string() << " não foi reconhecido" << endl; 
        cout << "    - Este ERRO ocorre apenas se não foi encontrado este diretório" << endl;
        cout << "    - Se, porventura, o diretório informado foi escrito corretamente, entre em contato com a empresa Techniken" << endl;
        cout << "    -   Contato: suporte@techniken.com.br" << endl;
        cout << "    -   Assunto: Software Cercamento - ERRO (não reconhecendo diretório, de destino, informado ) " << endl;
        exit(0);
    }
    

    map<string,vector<Plate*>> valid;                                   // Para cada placa válida, lista de placas iguais
    vector<string> keysValid;                                           // Todas placas válidas encontradas
    map<pair<string,string>,vector<Plate*>> unvalid;                    // Para cada data e hora, lista de placa zeradas
    vector<pair<string,string>> keysUnvalid;                            // Todas os pares data e hora encontrados de placa zeradas
    
    map<pair<string,string>, bool> pairDataAndHourUnvalidProcessed;     // Pares data e hora de placas zeradas já processadas
    map<pair<string,string>,bool>  pairDataAndHourValidProcessed; ;     // Pares data e hora de placas válidas já processadas

    pair<string,string> memoryDateAndHour;                              // Últuma data e hora salva na memória
    string              memoryPlate;                                    // Última placa salva na memória

    pair<string,string> dateAndHourLastPlate;                           // Data e hora da última placa
    Plate*              lastPlate;                                      // Última placa

    map<string,bool>    deleteUnvalid;                                  // Placa não válidas já excluídas


    Watcher* w = new Watcher(argv[1]);

    path origin = w->watching();
    
    this_thread::sleep_for(chrono::milliseconds(1000));

    string ip = origin.filename();
    path originPath = (origin.string() + origin.preferred_separator);

    if(argc > 3)
    {
        /* 
            Se argc > 3, significa que há um terceiro parâmetro, este que só é recebido se o programa chama ele recursivamente, informando qual a última placa processada
            Logo, se houver última placa processada, salvar informações referente à mesma.
        */
        Plate* pt = new Plate(argv[3]);
        dateAndHourLastPlate = pt->getDateAndHour();
        memoryPlate = pt->getPlate();
        memoryDateAndHour = pt->getDateAndHour();
        lastPlate = pt;
    }    

    for(auto& p : recursive_directory_iterator(originPath.string().c_str()))
    {  
        if(p.path().extension() == ".jpg")
        {
            /*
                Criar objeto apenas para arquivos jpg
            */
            Plate* pt = new Plate(p.path().filename());
            if(dateAndHourLastPlate.first.size() > 0)
            {
                /*
                    Apenas realizar o if abaixo de houver algum dado na variável dateAndHourLastPlate, pois se não houver, o programa executaria com erro
                */
                if(pt->getDateAndHour() > dateAndHourLastPlate)
                {
                    /*
                        Caso a data e hora da placa analisada seja maior do que a data e hora mais atual salva
                    */
                    dateAndHourLastPlate = pt->getDateAndHour();
                    lastPlate = pt;
                }
            }else
            {
                /*
                    Se dateAndHourLastPlate ainda não obtem informação, salvar as informações desta placa que está sendo analisada, como default
                */
                dateAndHourLastPlate = pt->getDateAndHour();
                lastPlate = pt;
            }
            
            if(isValid(pt->getPlate()))
            {
                /*
                    Se a placa for válida, isto é, não for 0000000
                */
                if(valid[pt->getPlate()].size() == 0) keysValid.push_back(pt->getPlate()); 
                valid[pt->getPlate()].push_back(pt);
            }else
            {
                /*
                    Se a placa for inválida, isto é, for 0000000
                */
                if(unvalid[pt->getDateAndHour()].size() == 0) keysUnvalid.push_back(pt->getDateAndHour());
                unvalid[pt->getDateAndHour()].push_back(pt);
            }
        }
    }


    pair< string, string > lastDateAndHour;
    if(memoryDateAndHour.first.size() > 0) lastDateAndHour = memoryDateAndHour;

    for(int i = 0 ; i < keysValid.size(); i++)
    {
        /*
            Para cada placa válida encontrada ... 
        */
        int lastNumber;
        for(int j = 0 ; j < valid[keysValid[i]].size() ; j++)
        {
            /*
                Correr todos arquivos que possuem esta placa, eles podem conter números diferentes, ou até mesmo, data e hora diferentes, porém, são com placa identicas!
            */
            if(lastDateAndHour.first.size() == 0)
            {
                    /*
                        Se a variável lastDateAndHour estiver vazia:
                             - Mover placa
                             - Salvar data, hora
                             - Salvar data e hora em pairDataAndHourValidProcessed
                    */
                    bool flag = true;
                    if(memoryPlate.size() > 0)
                    {
                        /*
                            Se a variável memoryPlate conter informações
                        */
                        if(memoryPlate == valid[keysValid[i]][j]->getPlate())
                        {
                            /*
                                Se a última placa processada for igual à placa que está sendo, agora, analisada, deve-se:
                                    - Excluir esta placa, pois já foi movida uma outra versão da mesma
                            */
                            flag = false;
                            string rm = "lixo";
                            create_directory(destiny/rm);
                            rename(originPath/valid[keysValid[i]][j]->getFile(), destiny/rm/valid[keysValid[i]][j]->getFile());
                        } 
                    }
                    if(flag)
                    {
                        /*
                            Apenas move esta placa se lastDateAndHour estiver vázio, ou seja, é a primeira placa que está sendo processada e é a primeira vez que o programa é executada
                            Quando mover, deve-se salvar os dados de data e hora da placa, assim como salvar que esta fora a última placa processada
                        */
                        create_directory(destiny/ip);
                        rename(originPath/valid[keysValid[i]][j]->getFile(), destiny/ip/valid[keysValid[i]][j]->getFile() );
                        pairDataAndHourValidProcessed[valid[keysValid[i]][j]->getDateAndHour()] = true;
                        //lastDateAndHour.first = valid[keysValid[i]][j]->getDate();
                        //lastDateAndHour.second = valid[keysValid[i]][j]->getHour();
                        lastDateAndHour = valid[keysValid[i]][j]->getDateAndHour();
                        lastPlate = valid[keysValid[i]][j];
                        //cout << 3 << endl;
                    }
            }else if(!(valid[keysValid[i]][j]->getDateAndHour() == lastDateAndHour)) // Apenas se a diferença de segundos for maior que 1
            { 

                //cout << valid[keysValid[i]][j]->getDateAndHour().first << endl;
                //cout << valid[keysValid[i]][j]->getDateAndHour().second << endl;
                //cout << "com " << endl;
                //cout << lastDateAndHour.first << endl;
                //cout << lastDateAndHour.second << endl;

                //cout << (valid[keysValid[i]][j]->getDateAndHour() == lastDateAndHour ? "igual" : "differenteee" ) << endl;
                /*
                    Apenas se a diferença de data e hora entre a placa válida que está sendo analisada e a data e hora da última placa forem maior a 1
                    Isto significa que as placas são iguais, porém possuem data e hora com diferença maior que 1 seg
                    
                */
                bool flag = true;
                if(memoryPlate.size() > 0)
                {
                    /*
                        memoryPlate.size > 0, serve para conferir se há informações válidas em memoryPlate, afim de evitar que o programa finalize sua execução por erro
                    */
                    if(memoryPlate == valid[keysValid[i]][j]->getPlate())
                    {
                        /*
                            Se a placa da memória (última placa processada no programa executado anteriormente) for igual à placa que estou analisando:
                            Deve-se excluir, então, a placa que estou analisando
                        */
                        flag = false;
                        string rm = "lixo";
                        create_directory(destiny/rm);
                        rename(originPath/valid[keysValid[i]][j]->getFile(), destiny/rm/valid[keysValid[i]][j]->getFile());
                    }
                }
                /*
                    move valid[keysValid[i]][j], pois a placa, que é igual, mas que já foi movida é de horário diferente
                    Salvar data e hora em pairDataAndHourValidProcessed
                */
                if(flag)
                {
                    /*
                        Apenas se não encontrei placa equivalente na memória, posso, então, mover 
                    */
                    create_directory(destiny/ip);
                    rename(originPath/valid[keysValid[i]][j]->getFile(), destiny/ip/valid[keysValid[i]][j]->getFile() );
                    pairDataAndHourValidProcessed[valid[keysValid[i]][j]->getDateAndHour()] = true;
                    //lastDateAndHour.first = valid[keysValid[i]][j]->getDate();
                    //lastDateAndHour.second = valid[keysValid[i]][j]->getHour();
                    lastDateAndHour = valid[keysValid[i]][j]->getDateAndHour();
                    lastPlate = valid[keysValid[i]][j];
                    //cout << 4 << endl;
                }
            }else if(valid[keysValid[i]][j]->getPlate() == memoryPlate or valid[keysValid[i]][j]->getPlate() == lastPlate->getPlate())
            {
                    /*
                        Caso eu encontre a placa que estou processando igual à placa na memória ou igual à última placa que eu processei
                        exclui valid[keysValid[i]][j], pois já foi movido esta placa, neste horário e data
                    */
                //cout << "1" <<endl;
                string rm = "lixo";
                create_directory(destiny/rm);
                rename(originPath/valid[keysValid[i]][j]->getFile(), destiny/rm/valid[keysValid[i]][j]->getFile());
            }else
            {
                //cout << "2" <<endl;
                create_directory(destiny/ip);
                rename(originPath/valid[keysValid[i]][j]->getFile(), destiny/ip/valid[keysValid[i]][j]->getFile() );
                pairDataAndHourValidProcessed[valid[keysValid[i]][j]->getDateAndHour()] = true;
                lastDateAndHour.first = valid[keysValid[i]][j]->getDate();
                lastDateAndHour.second = valid[keysValid[i]][j]->getHour();
                lastPlate = valid[keysValid[i]][j];
            }
            
        }
    }

    for(int i = 0 ; i < keysUnvalid.size() ; i++)
    {
        if(!pairDataAndHourValidProcessed[keysUnvalid[i]]) // Quer dizer que não tem nenhuma placa válida movida neste horário
        {
            if(unvalid[keysUnvalid[i]].size() > 1)
            {
                for(int j = 0 ; j < unvalid[keysUnvalid[i]].size() ; j++)
                {
                    if(j == 0)
                    {
                        // Move
                        create_directory(destiny/ip);
                        rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/ip/unvalid[keysUnvalid[i]][j]->getFile() );   
                    }else 
                    {
                        // Deleta
                        if(!deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()])
                        {
                            //cout << "1" << endl;
                            string rm = "lixo";
                            create_directory(destiny/rm);
                            rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][j]->getFile() );
                            deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()] = true;
                        }
                    }
                }
            }else if(unvalid[keysUnvalid[i]][0]->getDateAndHour() == memoryDateAndHour)
            {
                if(!deleteUnvalid[unvalid[keysUnvalid[i]][0]->getFile()])
                {
                    //cout << "2" << endl;
                    string rm = "lixo";
                    create_directory(destiny/rm);
                    rename(originPath/unvalid[keysUnvalid[i]][0]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][0]->getFile() ); 
                    deleteUnvalid[unvalid[keysUnvalid[i]][0]->getFile()] = true;
                }    
                
            }
            pairDataAndHourUnvalidProcessed[keysUnvalid[i]] = true;
            unvalid[keysUnvalid[i]].clear();

            /*
                Salvar keysUnvalid[i] na lista de já processadas  --> pairDataAndHourUnvalidProcessed[keysUnvalid[i]] = true;
                Limpa vector<Plate*>  de unvalid[keysUnvalid[i]] -- > unvalid[keysUnvalid[i]].clear();
            */
        }else
        {
            for(int j = 0 ; j < unvalid[keysUnvalid[i]].size() ;  j++)
            {
                if(!deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()])
                {
                    //cout << "3" << endl;
                    string rm = "lixo";
                    create_directory(destiny/rm);
                    rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][j]->getFile() );     
                    deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()] = true;
                }
            }
        }
        
    }

    for(int i = 0 ; i < keysUnvalid.size() ; i++)
    {
        if(!(pairDataAndHourUnvalidProcessed[keysUnvalid[i]])) // Se for verdade, já foi processada
        {
            bool flag = false;
            for(int j = 0 ; j < keysValid.size(); j++)
            {
                for(int k = 0 ; k < valid[keysValid[j]].size(); k++)
                {
                    if( valid[keysValid[j]][k]->getDateAndHour() == keysUnvalid[i] ) // Diferença entre 0 e 1
                    {
                        /*
                            Excluir unvalid[keysUnvalid[i]], pois já teve placa válida processada
                        */
                       for(int l = 0 ; l < unvalid[keysUnvalid[i]].size(); l++)
                       {
                           if(!deleteUnvalid[unvalid[keysUnvalid[i]][l]->getFile()])
                           {
                            //cout << "4" << endl;
                            string rm = "lixo";
                            create_directory(destiny/rm);
                            rename(originPath/unvalid[keysUnvalid[i]][l]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][l]->getFile()); 
                            deleteUnvalid[unvalid[keysUnvalid[i]][l]->getFile()] = true;
                           }
                       }
                       flag = true;
                       //break;
                    }
                    //if(flag) break;
                }
                //if(flag) break;
            }
            if(!flag)
            {
                if( unvalid[keysUnvalid[i]].size() > 1)
                {   
                    for(int j = 0 ; j < unvalid[keysUnvalid[i]].size() ; j++)
                    {
                        if( j == 0)
                        {
                            if(unvalid[keysUnvalid[i]].size() > 1)
                            {
                                create_directory(destiny/ip);
                                rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/ip/unvalid[keysUnvalid[i]][j]->getFile() );  
                            }else if(unvalid[keysUnvalid[i]][j]->getDateAndHour() == memoryDateAndHour )
                            {
                                if(!deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()])
                                {
                                    // << "5" << endl;
                                    string rm = "lixo";
                                    create_directory(destiny/rm);
                                    rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][j]->getFile()); 
                                    lastPlate = unvalid[keysUnvalid[i]][j];   
                                    deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()] = true;
                                }
                            }
                        }else
                        {
                            if(!deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()])
                            {
                                //cout << "6" << endl;
                                string rm = "lixo";
                                create_directory(destiny/rm);
                                rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][j]->getFile());    
                                deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()] = true;
                            }
                        }
                    }
                }else if(unvalid[keysUnvalid[i]][0]->getDateAndHour() == memoryDateAndHour)
                {
                    if(!deleteUnvalid[unvalid[keysUnvalid[i]][0]->getFile()])
                    {
                        // cout << "7" << endl;
                        string rm = "lixo";
                        create_directory(destiny/rm);
                        rename(originPath/unvalid[keysUnvalid[i]][0]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][0]->getFile());    
                        deleteUnvalid[unvalid[keysUnvalid[i]][0]->getFile()] = true;
                    }
                }else
                {
                    if(!deleteUnvalid[unvalid[keysUnvalid[i]][0]->getFile()])
                    {
                        //cout << "7" << endl;
                        string rm = "lixo";
                        create_directory(destiny/rm);
                        rename(originPath/unvalid[keysUnvalid[i]][0]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][0]->getFile());    
                        deleteUnvalid[unvalid[keysUnvalid[i]][0]->getFile()] = true;
                    }
                }
            }else
            {   
                for(int j = 0 ; j < unvalid[keysUnvalid[i]].size(); j++)
                {
                    if(!deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()])
                    {
                        //cout  << "8" << endl;
                        string rm = "lixo";
                        create_directory(destiny/rm);
                        rename(originPath/unvalid[keysUnvalid[i]][j]->getFile(), destiny/rm/unvalid[keysUnvalid[i]][j]->getFile());    
                        deleteUnvalid[unvalid[keysUnvalid[i]][j]->getFile()] = true;
                    }
                }
            }
            

            /*
                Apenas se ainda não foi processada
                Procurar, na lista de keysValid, alguma placa processada com horário de diferença no máximo de 1s
                    Se encontrar, excluir unvalid[keysUnvalid[i]]
                    Se não encontrar, 
                        Se unvalid[keysUnvalid[i]].size() == 1, move 
                        Se > 1, move a primeira e deleta as demais
            */

        }
    }

     for(auto& p : recursive_directory_iterator(originPath.string().c_str()))
    {  
        if(p.path().extension() == ".jpg")
        {
            /*
                Criar objeto apenas para arquivos jpg
            */
            Plate* pt = new Plate(p.path().filename());
            if(!isValid(pt->getPlate()))
            {
                // cout << "8" << endl;
                string rm = "lixo";
                create_directory(destiny/rm);
                rename(originPath/p.path().filename(), destiny/rm/p.path().filename());    
            }
        }
    }

    stringstream cmd;
    cmd << argv[0] << " " << argv[1] << " " << argv[2] << " " << lastPlate->getFile();
    //cout << cmd.str() << endl;
    system(cmd.str().c_str());

}
