#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

struct Passageiro{

    //Atributos
    string nome;
    int idade;

    //Metodos
    Passageiro(string nome = "Desconhecido", int idade = 0){
        this->nome = nome;
        this->idade = idade;
    }

    string toString(){
        stringstream saida;
        saida << this->nome << ":" << this->idade;
        return saida.str();
    }
};

struct Topic{

    //Atributos
    int nPref;
    vector<Passageiro*> cadeiras;

    //Metodos

    Topic(int qtd = 0, int nPref = 0):
        cadeiras(qtd, nullptr),
        nPref(nPref)
    {
    }

    ~Topic(){
        for(Passageiro *passageiro : cadeiras){
            delete(passageiro);
        }
    }

    /*int find(string nome){
        for(int i = 0; i < cadeiras.size(); i++){
            if(cadeiras[i]->nome == nome){
                return -1;
            }
        }
        return false;
    }*/

    bool inserir(string nome, int idade){
        int a = 0;
        for(int i = 0; i < cadeiras.size(); i++){
            if(cadeiras[i] == nullptr){
                a++;
            }
        }

        if(a = 0){
            cout << "A topic ja esta cheia" << endl;
            return false;
        }

        Passageiro *passageiro;
        passageiro = new Passageiro(nome, idade);

        if(passageiro->idade > 60){
            for(int i = 0; i < nPref; i++){
                if(cadeiras[i] == nullptr){
                    cadeiras[i] = passageiro;
                    return true;
                }
            }

            for(int i = nPref; i < cadeiras.size(); i++){
                if(cadeiras[i] == nullptr){
                    cadeiras[i] = passageiro;
                    return true;
                }
            }
        }else{
            for(int i = nPref; i < cadeiras.size(); i++){
                if(cadeiras[i] == nullptr){
                    cadeiras[i] = passageiro;
                    return true;
                }
            }
        }

        return false;
    }

    /*bool retirar(string nome){
        
        int pos = find(nome);
        if(pos == -1){
            return false;
        }
        delete cadeiras[pos];
        cadeiras[pos] = nullptr;
        return true;
    }*/
        

    string toString(){
        stringstream saida;
        saida << "[ ";
            int i;
            for(i = 0; i < cadeiras.size(); i++){
                if(cadeiras[i] != nullptr){
                    saida << cadeiras[i]->toString() << " ";
                }else if(i < nPref){
                    saida << "@ ";
                }else{
                    saida << "= ";
                }
            }
        saida << "]";
        return saida.str();
    }
};

struct Controller{
    Topic topic;

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;
        if(op == "help")
            out << "init; show; inserir; retirar;";
        else if(op == "init"){      
            int qtd, nPref;
            in >> qtd >> nPref;
            topic = Topic(qtd, nPref);
            out << "done";
        }else if(op == "show"){
            out << topic.toString();
        }else if(op == "inserir"){
            string nome;
            int idade;
            in >> nome >> idade;
            if(topic.inserir(nome, idade)){
                out << "done";
            }
        }else if(op == "retirar"){
            string nome;
            in >> nome;
            if(topic.retirar(nome)){
                out << "done";
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
    Controller c;
    c.exec();
    return 0;
}