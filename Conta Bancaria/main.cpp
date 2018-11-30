#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;

struct Operacao{
    int indice;
    string descricao;
    float valor;
    float saldo;

    //Construtor
    Operacao(int indice = 0, string descricao = "", float valor = 0, float saldo = 0):
        indice(indice),
        descricao(descricao),
        valor(valor),
        saldo(saldo)
    {}

    string toString(){
        stringstream fluxo;
        fluxo << indice
            << ":" << setw(10) << descricao
            << ":" << setw(5) << valor
            << ":" << setw(5) << saldo << endl;
        return fluxo.str();
    }
};

struct Conta{
    int nextId{0};
    int numero;
    float saldo;
    vector<Operacao> operacoes;

    Conta(int numero = 0, float saldo = 0):
        numero(numero),
        saldo(saldo)
    {
        pushOperation("abertura", 0, 0);
    }

    string toString(){
        stringstream fluxo;
        fluxo << "conta: " << numero << " saldo: " << saldo;
        return fluxo.str();
    }

    void pushOperation(string descricao, float valor, float saldo){
        operacoes.push_back(Operacao(nextId, descricao, valor, saldo));
        nextId++;
    }

    bool debitar(string op, float valor){
        if(valor < 0){
            cout << "  fail: valor invalido";
            return false;
        }

        if(op == "saque"){
            if(valor > saldo){
                cout << "  fail: saldo insuficiente";
                return false;
            }
        }
        
        saldo -= valor;
        pushOperation(op, valor*(-1), saldo);
        return true;
    }

    bool creditar(string op, float valor){
        if(valor < 0){
            cout << "  fail: valor invalido";
            return false;
        }

        saldo += valor;
        pushOperation(op, valor, saldo);
        return true;
    }

    void extornar(string line){
        stringstream fluxo(line);
        int indiceExtornar;
        while(fluxo >> indiceExtornar){
            if(indiceExtornar < 0 || indiceExtornar >= (int)(operacoes.size())){
                cout << "  fail: indice " << indiceExtornar << " invalido" << endl;
            }else if(operacoes[indiceExtornar].descricao != "tarifa"){
                cout << "  fail: indice " << indiceExtornar << " nao e tarifa" << endl;
            }else{
                float valor = operacoes[indiceExtornar].valor * (-1);
                saldo += valor;
                pushOperation("extorno", valor, saldo);
            }
        }
    }

    void extrato(){
        for(int i = 0; i < (int)(operacoes.size()); i++){
            cout << operacoes[i].indice
            << ":" << setw(10) << operacoes[i].descricao
            << ":" << setw(5) << operacoes[i].valor
            << ":" << setw(5) << operacoes[i].saldo << endl;
        }
    }

    void extratoN(int n){
        int i = (int)(operacoes.size()) - n;
        for(; i < (int)(operacoes.size()); i++){
            cout << operacoes[i].indice
            << ":" << setw(10) << operacoes[i].descricao
            << ":" << setw(5) << operacoes[i].valor
            << ":" << setw(5) << operacoes[i].saldo << endl;
        }
    }

};

struct Controller{
    Conta conta;

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;
        if(op == "init"){
            int numero;
            in >> numero;
            conta = Conta(numero);
            out << "  success";
        }else if(op == "show"){
            out << conta.toString();
        }else if(op == "saque" || op == "tarifa"){
            float valor;
            in >> valor;
            if(conta.debitar(op, valor))
                out << "  success";
        }else if(op == "deposito"){
            float valor;
            in >> valor;
            if(conta.creditar(op, valor))
                out << "  success";
        }else if(op == "extornar"){
            string line;
            getline(in, line);
            conta.extornar(line);
        }else if(op == "extrato"){
            conta.extrato();
        }else if(op == "extratoN"){
            int n;
            in >> n;
            conta.extratoN(n);
        }else{
            out << "  fail: comando invalido";
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
            cout << shell(line) << endl;
        }
    }
};

int main(){
    Controller controller;
    controller.exec();

    return 0;
}