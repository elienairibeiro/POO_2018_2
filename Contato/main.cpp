#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Fone{

    //Atributos
    string id;
    string fone;

    //Metodos
    Fone(string id = "", string fone = ""):
        id(id), fone(fone)
    {
    }

    string toString(){
        stringstream ss;
        ss << "[" << id << ":" << fone << "]";
        return ss.str();
    }
};

struct Contato{

    //Atributos
    string id;
    vector<Fone> fones;

    //Metodos
    Contato(string id = "Zazen"):
        id(id)
    {
    }

    bool adicionar(Fone fone){
        for(auto telefone : fones)
            if(telefone.id == fone.id){
                return false;
            }
        fones.push_back(fone);
        return true;
    }

    bool deletar(string foneId){
        for(int i = 0; i < fones.size(); i++){
            if(fones[i].id == foneId){
                fones.erase(fones.begin() + i);
                return true;
            }
        }
        return false;
    }

    string toString(){
        stringstream ss;
        ss << id << " ";
        for(auto fone : fones)
            ss << fone.toString();
        return ss.str();
    }
};

struct Controller{
    Contato contato;

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string opcao;
        in >> opcao;

        if(opcao == "help"){
            out << "iniciar _nome; show; adicionar _operadora_fone; deletar _operadora; end;";
        }else if(opcao == "iniciar"){
            string nome;
            in >> nome;
            contato = Contato(nome);
            out << "done";
        }else if(opcao == "show"){
            out << contato.toString();
        }else if(opcao == "adicionar"){
            string id, fone;
            in >> id >> fone;
            if(contato.adicionar(Fone(id, fone))){
                out << "done";
            }else{
                out << "fail: id duplicado";
            }
        }else if(opcao == "deletar"){
            string id;
            in >> id;
            if(contato.deletar(id)){
                out << "done";
            }else{
                out << "fail: id nao encontrado";
            }
        }

        return out.str();
    }

    void exec(){
        string line;
        while(true){
            getline(cin, line);
            if(line == "end"){
                return;
            }
            cout << "  " << shell(line) << endl;
        }
    }
};

int main(){
    Controller controller;
    controller.exec();

    return 0;
}