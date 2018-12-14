#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
#include <iomanip>

using namespace std;

class Funcionario{
protected:
    string nome;
    string profissao;  //para identificar a profissao nas saidas de dados
    int maxDiarias;    //o maximo de diarias que ele pode receber
    int qtdDiarias{0}; //a qtd de diarias que ja recebeu
    float bonus{0};    //o quanto de bonus ele recebeu
    float salario;     //o salario do funcionario

public:
    Funcionario(string nome = ""):
        nome(nome)
    {}

    virtual ~Funcionario(){}

    virtual string getNome() = 0;
    virtual string getProfissao() = 0;
    virtual int getQtdDiarias() = 0;
    virtual int getMaxDiarias() = 0;
    virtual float getBonus() = 0;
    virtual float getSalario() = 0;

    virtual void setBonus(float bonus){
        this->bonus = bonus;
    }

    virtual void addDiaria(){
        this->qtdDiarias++;
    }

    //calculo parcial das diarias e o bonus
    virtual void calcSalario() = 0;

    virtual string toString() = 0;
};

class Professor : public Funcionario{
    char classe;
    int maxDiarias{2};
    string profissao{"Prof"};

public:
    Professor(string nome = "", char classe = ' '):
        Funcionario(nome), classe(classe)
    {}

    virtual ~Professor(){}

    virtual string getNome(){
        return nome;
    }
    virtual string getProfissao(){
        return profissao;
    }
    virtual int getQtdDiarias(){
        return qtdDiarias;
    }
    virtual int getMaxDiarias(){
        return maxDiarias;
    }
    virtual float getBonus(){
        return bonus;
    }
    virtual float getSalario(){
        return salario;
    }

    //sobrescreve adicionando o calculo do salario
    void calcSalario(){
        int aux = 0;
        switch(classe){
            case 'A':
                aux = 3000;
                break;
            case 'B':
                aux = 5000;
                break;
            case 'C':
                aux = 7000;
                break;
            case 'D':
                aux = 9000;
                break;
            case 'E':
                aux = 11000;
                break;
        }
        salario = aux + 100 * qtdDiarias + bonus;
    }

    string toString(){
        stringstream out;
        out << fixed << setprecision(1);
        out << "  " << profissao << " " << nome << " classe " << classe << endl;
        out << "  salario " << salario << endl;
        return out.str(); 
    }
};

class STA : public Funcionario{
    int nivel;
    string profissao{"Sta"};
    int maxDiarias{1};

public:
    STA(string nome = "", int nivel = 0):
        Funcionario(nome), nivel(nivel)
    {}

    virtual ~STA(){}
    
    virtual string getNome(){
        return nome;
    }
    virtual string getProfissao(){
        return profissao;
    }
    virtual int getQtdDiarias(){
        return qtdDiarias;
    }
    virtual int getMaxDiarias(){
        return maxDiarias;
    }
    virtual float getBonus(){
        return bonus;
    }
    virtual float getSalario(){
        return salario;
    }

    //sobrescreve adicionando o calculo do salario
    void calcSalario(){
        salario = 3000 + (300 * nivel) + (100 * qtdDiarias) + bonus;
    }

    string toString(){
        stringstream out;
        out << fixed << setprecision(1);
        out << "  " << profissao << " " << nome << " nivel " << nivel << endl;
        out << "  salario " << salario << endl;
        return out.str();
    }
};

class Terceirizado : public Funcionario{
    int horasTrab;
    bool insalubre;
    string profissao{"Ter"};

public:
    Terceirizado(string nome = "", int horasTrab = 0, bool insalubre = false):
        Funcionario(nome), horasTrab(horasTrab), insalubre(insalubre)
    {}

    virtual ~Terceirizado(){}

    virtual string getNome(){
        return nome;
    }
    virtual string getProfissao(){
        return profissao;
    }
    virtual int getQtdDiarias(){
        return qtdDiarias;
    }
    virtual int getMaxDiarias(){
        return maxDiarias;
    }
    virtual float getBonus(){
        return bonus;
    }
    virtual float getSalario(){
        return salario;
    }

    //sobrescreve adicionando o calculo do salario
    void calcSalario(){
        salario = 4 * horasTrab;
        if(insalubre)
            salario += 500;
    }

    virtual string toString(){
        stringstream out;
        out << fixed << setprecision(1);
        out << "  " << profissao << " " << nome << " " << horasTrab << "h ";
        if(insalubre){
            out << "insalubre" << endl;
        }else{
            out << endl;
        }
        out << "  salario " << salario << endl;
        return out.str();
    }
};

template<typename Y>
class Repositorio{
    map<string, Y*> funcionarios;

public:
    ~Repositorio(){
        for(auto& coisa : funcionarios){
            delete coisa.second;
        }
    }

    bool addFun(string id, Y* funcionario){
        if(!existe(id)){
            funcionarios[id] = funcionario;
            return true;
        }
        cout << "  funcionario " << id << " ja existe" << endl;
        return false;
    }

    void rmFun(string id){
        if(existe(id)){
            auto it = funcionarios.find(id);
            cout << "  " + it->second->getProfissao() << " " << it->second->getNome() << " foi removido" << endl;

            delete it->second;
            funcionarios.erase(it);
            return;
        }
        
        cout << "  funcionario " << id << " nao existe"<< endl;
    }

    bool addDiaria(string id){
        if(existe(id)){
            auto it = funcionarios.find(id);
            if(it->second->getProfissao() == "Ter"){
                throw "  ter nao pode receber diaria";
            }else if(it->second->getQtdDiarias() >= it->second->getMaxDiarias()){
                throw "  fail: limite de diarias atingido";
            }else{
                it->second->addDiaria();
            }
            return true;
        }
        return false;
    }

    void setBonus(int bonus){
        bonus = bonus / funcionarios.size();
        for(auto& funcionario : funcionarios){
            funcionario.second->setBonus(bonus);
        }
    }

    Y* getUser(string id){
        auto it = funcionarios.find(id);
        if(it != funcionarios.end()){
            return it->second;
        }
        throw "fail: funcionario nao existe";
    }

    void atualizarSalario(Y* fun){
        fun->calcSalario();
    }

    bool existe(string id){
        return funcionarios.find(id) != funcionarios.end();
    }
};

class Controller{
    Repositorio<Funcionario> repFun;
public:
    void shell(string line){
        stringstream in(line);
        string op;
        in >> op;
        if(op == "addProf"){
            string nome;
            char classe;
            in >> nome >> classe;
            Professor* prof = new Professor(nome, classe);
            if(!repFun.addFun(nome, prof)){
                delete prof;
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "addSta"){
            string nome;
            int nivel;
            in >> nome >> nivel;
            STA* sta = new STA(nome, nivel);
            if(!repFun.addFun(nome, sta)){
                delete sta;
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "addTer"){
            string nome, aux;
            int horasTrab;
            bool insalubre;
            in >> nome >> horasTrab >> aux;
            if(aux == "sim")
                insalubre = true;
            Terceirizado* ter = new Terceirizado(nome, horasTrab, insalubre);
            if(!repFun.addFun(nome, ter)){
                delete ter;
            }else{
                cout << "  done" << endl;
            }
        }else if(op == "show"){
            in >> op;
            Funcionario* fun = repFun.getUser(op);
            repFun.atualizarSalario(fun);
            cout << repFun.getUser(op)->toString();
        }else if(op == "rm"){
            string nome;
            in >> nome;
            repFun.rmFun(nome);
        }else if(op ==  "addDiaria"){
            string nome;
            in >> nome;
            repFun.addDiaria(nome);
            cout << "  done" << endl;
        }else if(op == "setBonus"){
            int bonus;
            in >> bonus;
            repFun.setBonus(bonus);
            cout << "  done" << endl;
        }else{
            cout << "  fail: comando invalido" << endl;
        }
    }

    void exec(){
        string line;
        while(true){
            try{
                getline(cin, line);
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
    Controller controller;
    controller.exec();
}



/*
addProf david C
addProf elvis D
addSta gilmario 3
addTer helder 40 sim

show david
show gilmario
show helder

show pinoquio

rm elvis

rm batman

addDiaria david
addDiaria david
addDiaria david

addDiaria helder

setBonus 600
*/