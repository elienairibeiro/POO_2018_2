#include <iostream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Aluno{
    string id, curso, nome; //id é a matricula

public:
    Aluno(string id = "", string curso = "", string nome = ""):
        id(id), curso(curso), nome(nome)
        {}
    
    string toString(){
        stringstream out;
        out << id << ":" << curso << ":" << nome;
        return out.str();
    }
};

class Professor{
    string id, nome; //id é siape

public:
    Professor(string id = "", string nome = ""):
        id(id), nome(nome)
        {}

    string toString(){
        stringstream out;
        out << id << ":" << nome;
        return out.str();
    }
};

class Disciplina{
    string id, nome; //id é o codigo da disciplina

public:
    Disciplina(string id = "", string nome = ""):
        id(id), nome(nome)
        {}

    string toString(){
        stringstream out;
        out << id << ":" << nome;
        return out.str();
    }
};

template <typename K, typename V>
class Repositorio{
public:
    map<K, V> repositorio;

    bool existe(K key){
        return (repositorio.find(key) != repositorio.end());
    }

    void adicionar(K key, V elemento){
        if(!existe(key)){
            repositorio[key] = elemento;
        }else{
            throw "fail: Ja esta cadastrado no sistema";
        }
    }

    void remover(K key){
        if(existe(key)){
            repositorio.erase(key);
        }else{
            throw "fail: Nao existe no sistema";
        }
    }

    V& get(K key){
        auto it = repositorio.find(key);
        
        if (it != repositorio.end()){
            return it->second;
        }else{
            throw "fail: Nao existe no sistema";
        }
    }
    
    vector<V> getValor(){
        vector<V> valor;
        
        for(pair<K, V> par : repositorio){
            valor.push_back(par.second);
        }

        return valor;
    }
    
    vector<K> getKeys(){
        vector<K> keys;
        
        for(pair<K, V> par : repositorio){
            keys.push_back(par.first);
        }

        return keys;
    }

    string toString(vector<V> vetor){
        stringstream out;
        for(auto& coisa : vetor){
            out << coisa.toString() << endl;
        }
        return out.str();
    }
};

class Controller{
    Repositorio<string, Aluno> repAlu;
    Repositorio<string, Professor> repPro;
    Repositorio<string, Disciplina> repDis;

public:
    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;

        if(op == "addAlu" || op == "addPro" || op == "addDis"){
            string id, nome, curso;

            if(op == "addAlu"){
                in >> id >> curso;
                getline(in, nome);
                repAlu.adicionar(id, Aluno(id, curso, nome));
            }else if(op == "addPro"){
                in >> id;
                getline(in, nome);
                repPro.adicionar(id, Professor(id, nome));
            }else{
                in >> id;
                getline(in, nome);
                repDis.adicionar(id, Disciplina(id, nome));
            }
        }else if(op == "rmAlu" || op == "rmPro" || op == "rmDis"){
            string id;
            in >> id;
            
            if(op == "rmAlu"){
                repAlu.remover(id);
            }else if(op == "rmPro"){
                repPro.remover(id);
            }else{
                repDis.remover(id);
            }
        }else if(op == "getAlu" || op == "getPro" || op == "getDis"){
            string id;
            in >> id;

            if(op == "getAlu"){
                Aluno& alu = repAlu.get(id);
                out << alu.toString();
            }else if(op == "getPro"){
                Professor& pro = repPro.get(id);
                out << pro.toString();
            }else{
                Disciplina& dis = repDis.get(id);
                out << dis.toString();
            }
        }else if(op == "showAlu" || op == "showPro" || op == "showDis"){
            if(op == "showAlu"){
                out << repAlu.toString(repAlu.getValor());
            }else if(op == "showPro"){
                out << repPro.toString(repPro.getValor());
            }else{
                out << repDis.toString(repDis.getValor());
            }
        }
        return out.str();
    }

    void exec(){
        string line;
        while(true){
            getline(cin, line);
            if(line == "end")
                return;
            try{
                cout << shell(line) << endl;
            }catch(const char * fail){
                cout << fail << endl;
            }
        }
    }
};

int main(){
    Controller controller;
    controller.exec();

    return 0;
}

/*
addAlu 0023 ES rui almeida
addAlu 0021 RC bia barbosa
addAlu 0024 EC eva galdino
addAlu 0020 SI may parkerr

addPro 1323 david sena
addPro 1312 elvis caio
addPro 1542 josue luno
addPro 2542 almir bico

addDis QXD01 estrutura de dados
addDis QXD02 fundamentos de programação
addDis QXD03 banco de dados

getAlu 0021

getPro 1542

getDis QXD03

showAlu

showPro

showDis

rmAlu 0020
rmPro 1542
rmDis QXD02

showAlu

showPro

showDis

addAlu 0023 mia balike

addPro 1312 ari toleto

addDis QXD01 fundamentos da enrolacao

rmAlu 8888

rmPro 9999

rmDis QXD04

getAlu 8888

getPro 9999

getDis QXD04

showAlu

showPro

showDis

end
*/