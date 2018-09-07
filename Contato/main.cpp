#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Fone{
public:
    string operadora;
    string numero;

    Fone(string operadora = "", string numero = ""):
        operadora(operadora), numero(numero)
    {
    }

    string toString(){
        stringstream saida;
        saida << operadora << ":" << numero;
        return saida.str();
    }
};

struct Contato{
private:
    string nome;
    vector<Fone> fones;

public:
    Contato(string nome = "Vazio"):
        nome(nome)
    {
    }

    bool validar(string numero){
        for (int i = 0; i < (int)numero.size(); i++){
            if(!isdigit(numero[i]) && numero[i] != '(' && numero[i] != ')'){
                return false;
            }
        }
        return true;
    }

    bool adicionar(Fone fone){
        for(auto telefone : fones){
            if(telefone.numero == fone.numero){
                cout << "  fail: ID duplicado";
                return false;
            }
        }
        
        if(validar(fone.numero)){
            fones.push_back(fone);
            return true;
        }else{
            cout << "  fail: Numero invalido";
            return false;
        }
    }

    bool remover(int foneId){
        for(int i = 0; i < (int)fones.size(); i++){
            if(i == foneId){
                fones.erase(fones.begin() + i);
                return true;
            }
        }
        return false;
    }

    void atualizar(string line){
        stringstream in(line);
        string palavra;

        in >> palavra;
        if (palavra == nome){
            fones.clear();
            while (in >> palavra){
                stringstream ss(palavra);
                string operadora, numero;
                getline(ss, operadora, ':');
                getline(ss, numero);
                if (validar(numero))
                    fones.push_back(Fone(operadora, numero));
                else{
                    cout << "  fail: o numero da " << operadora << " nao e valido";
                }
            }
        } else{
            cout << "  fail: update invalido" << endl;
        }
    }

    string toString(){
        stringstream saida;
        saida << nome << "=>";
        for(int i = 0; i < fones.size(); i++){
            saida << "[" << i << ":" << fones[i].toString() << "]";
        }
        return saida.str();
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
            string nome, fone;
            in >> nome >> fone;
            if(contato.adicionar(Fone(nome, fone))){
                out << "done";
            }
        }else if(opcao == "remover"){
            int id;
            in >> id;
            if(contato.remover(id)){
                out << "done";
            }else{
                out << "fail: ID nao encontrado";
            }
        }else if(opcao == "atualizar"){
            string linha;
            getline(in, linha);
            contato.atualizar(linha);
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
