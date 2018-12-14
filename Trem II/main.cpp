#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

class Emb{ 
public:
    virtual string getId() = 0;
    virtual string toString() = 0;
    virtual ~Emb(){};
};

class Pass: public Emb {
    string id;
public:
    Pass(string id = ""):
        id(id){}

    virtual ~Pass(){}

    virtual string getId(){
        return id;
    }

    virtual string toString(){
        return id;
    }
};

class Carga : public Emb {
    string id;
    float peso;
public:
    Carga(string id = "", float peso = 0.0):
        id(id), peso(peso)
    {}

    ~Carga(){}

    virtual string getId(){ 
        return id;
    }
    
    float getPeso(){
        return peso;
    }

    virtual string toString(){
        stringstream out;
        out << fixed << setprecision(1);        
        out << id << ":" << peso;
        return out.str();
    }
};

class Vagao{ 
public:
    virtual bool embarcar(Emb * emb) = 0;
    virtual ~Vagao(){};
    virtual bool desembar(string idPass) = 0;
    virtual bool exists(string idPass) = 0;
    virtual string toString() = 0;
};

class VagaoPessoas : public Vagao {
    vector<Pass*> passageiros;
public:
    VagaoPessoas(int capacidade):
        passageiros(capacidade, nullptr)
    {}
    ~VagaoPessoas(){
        for(size_t i = 0; i < passageiros.size(); i++){
            if(passageiros[i] != nullptr)
                delete passageiros[i];
        }
    }

    virtual bool embarcar(Emb * emb){
        if(Pass * pass = dynamic_cast<Pass*>(emb)){
            for(size_t i = 0; i < passageiros.size(); i++){
                if(passageiros[i] == nullptr){
                    passageiros[i] = pass;
                    return true;
                }
            }
        }
        return false;
    }

    virtual bool desembar(string idPass){        
        for(auto *& cadeira : passageiros){
            if(cadeira != nullptr && cadeira->getId() == idPass){
                delete cadeira;
                cadeira = nullptr;
                return true;
            }
        }
        return false;
    }

    virtual bool exists(string idPass){
        for(auto *& passageiro : passageiros){
            if((passageiro != nullptr) && (passageiro->getId() == idPass)){
                return true;
            }
        }
        return false;
    }

    virtual string toString(){
        stringstream ss;
        ss << "[ ";
        for(auto* pass : passageiros){
            if(pass == nullptr)
                ss << "- ";
            else
                ss << pass->toString() << " ";
        }
        ss << "]";
        return ss.str();
    }
};

class VagaoCargas : public Vagao{
    float capacidade;
     map<string, Carga*> cargas;
public:
    VagaoCargas(float capacidade = 0.0):
        capacidade(capacidade)
    {}
    ~VagaoCargas(){
        for(auto& coisa : cargas){
            delete coisa.second;
        }
    }

    virtual bool embarcar(Emb * emb){
        if(Carga * carga = dynamic_cast<Carga*>(emb)){
            if(carga->getPeso() <= capacidade){
                cargas[carga->getId()] = carga;
                capacidade -= carga->getPeso();
                return true;
            }
        }
        return false;
    }

    virtual bool desembar(string idCarga){
        if(exists(idCarga)){
            auto it = cargas.find(idCarga);
            capacidade += it->second->getPeso();
            delete it->second;
            cargas.erase(it);
            return true;
        }
        return false;
    }

    virtual bool exists(string idCarga){
        return cargas.find(idCarga) != cargas.end();
    }

    virtual string toString(){
        stringstream out;
        out << fixed << setprecision(1);            
        out << "( ";
        for(auto & coisa : cargas){
            out << coisa.second->toString() << " ";
        }
        out << "_" << capacidade << " )";
        return out.str();
    }
};

class Trem{
    vector<Vagao*> vagoes;
    int forca;
public:
    Trem(int forca = 0){
        this->forca = forca;
    }
    ~Trem(){
        for(auto * vagao : vagoes)
            delete vagao;
    }
    bool addVagao(Vagao * vagao){
        if((int) vagoes.size() < forca){
            vagoes.push_back(vagao);
            return true;
        }
        return false;
    }
    bool embarcar(Emb * emb){
        for(auto* vagao : vagoes){
            if(vagao->embarcar(emb))
                return true;
        }
        return false;
    }
    bool desembar(string idPass){
        for(auto* vagao : vagoes){
            if(vagao->desembar(idPass))
                return true;
        }
        return false;
    }

    bool exists(string id){
        for(auto * vagao : vagoes){
            if(vagao->exists(id)){
                return true;
            }
        }
        return false;
    }
    
    string toString(){
        stringstream ss;
        ss << "Trem: ";
        for(auto * vagao : vagoes)
            ss << vagao->toString() << " ";
        return ss.str();
    }
};

template<class T>
T read(stringstream& ss){
    T t;
    ss >> t;
    return t;
}

class Controller{
    Trem trem;
public:
    Controller(){}

    void shell(string line){
        stringstream ss(line);
        string op;
        ss >> op;
        if(op == "init"){ //_forca
            trem = Trem(read<int>(ss));
        }else if(op == "addvp"){//_cadeiras
            Vagao * vagao = new VagaoPessoas(read<int>(ss));
            if(!trem.addVagao(vagao)){
                delete vagao;
                throw "  fail: limite de vagoes atingido";
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "embp"){//nome
            Pass * pass = new Pass(read<string>(ss));
            if(trem.exists(pass->getId()))
                throw "  fail: Ja esta no trem";
            if(!trem.embarcar(pass)){
                delete pass;
                throw "  fail: Trem ja esta lotado";
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "show"){
            cout << trem.toString() << endl;
        }else if(op == "dembp"){//_nome
            string idPass;
            ss >> idPass;
            if(!trem.exists(idPass))
                throw "  fail: Nao esta no trem";
            if(trem.desembar(idPass))
                cout << "  done" << endl;
        }else if(op == "addvc"){//_capacidade
            Vagao * vagao = new VagaoCargas(read<float>(ss));
            if(!trem.addVagao(vagao)){
                delete vagao;
                throw "  fail: limite de vagoes atingido";
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "embc"){//_nome _peso
            string idCarga;
            ss >> idCarga;
            Carga * carga = new Carga(idCarga, read<float>(ss));
            if(trem.exists(idCarga))
                throw "  fail: Ja esta no trem";
            if(!trem.embarcar(carga)){
                delete carga;
                throw "  fail: Trem lotado";
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "dembc"){//_nome
            string idCarga;
            ss >> idCarga;
            if(trem.desembar(idCarga)){
                cout << "  done" << endl;
            }else{
                throw "  fail: Nao esta no trem";
            }
        }else
            cout << "  fail: comando invalido" << endl;
    }

    void exec(){
        string line;
        while(true){
            try{
                getline(cin, line);
                cout << "$" << line << endl;
                if(line == "end")
                    return;
                shell(line);
            }catch(const char * fail){
                cout << fail << endl;
            }
        }
    }
};

int main(){
    Controller cont;
    cont.exec();
}

/*
init 3
addvp 2
addvc 100
addvp 1
show
addvp 3
embp davi
embp rui
embp rufus
show
embc arroz 80
embc feijao 20
show
embp chico
embc chinela 10
dembp davi
dembp rufus
show
dembp andre
dembc arroz
show
dembc chinela
show
*/