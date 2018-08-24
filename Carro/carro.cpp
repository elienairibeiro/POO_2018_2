#include <iostream>
#include <stdio.h>
using namespace std;

struct Carro{
    int pass;
    int passMax;
    float gas;
    float gasMax;
    float km;

    Carro(int pass = 0, float gas = 0, float km = 0, int passMax = 2, float gasMax = 10){
        this->pass = pass;
        this->gas = gas;
        this->km = km;
        this->passMax = passMax;
        this->gasMax = gasMax;
    }

    bool in(){
        if(pass < 2){
            this->pass += 1;
            cout << "  done" << endl;
            return true;
        }
        puts("  fail: limite de pessoas atingido");
        return false;
    }
    bool out(){
        if(pass > 0){
            this->pass -= 1;
            cout << "  done" << endl;
            return true;
        }
        puts("  fail: nao ha ninguem no carro");
        return false;
    }
    void fuel(float gasolina){
        gas += gasolina;
        if(gas > gasMax){
            gas = gasMax;
        }
        cout << "  done" << endl;
    }
    bool drive(float distancia){
        if(pass > 0 ){
            if(gas > 0){
                distancia /= 10;
                if(gas - distancia >= 0){
                    this->gas -= distancia;
                    this->km = distancia*10;
                    return true;
                }
                puts("  fail: gasolina insuficiente");
                return false;
            }
            puts("  fail: gasolina insuficiente");
            return false;
        }
        puts("  fail: nao ha ninguem no carro");
        return false;
    }  
};

int main(){
    string op;
    Carro carro;
    while(true){
        cin >> op;
        if(op == "show"){
            cout << "  pass: " << carro.pass
                << ", gas: " << carro.gas
                << ", km: " << carro.km << endl;
        }
        else if(op == "in"){
            carro.in();
        }
        else if(op == "out"){
            carro.out();
        }
        else if(op == "fuel"){
            float n;
            cin >> n;
            carro.fuel(n);
        }
        else if(op == "drive"){
            float n;
            cin >> n;
            carro.drive(n);
        }
        else if(op == "fim"){
            break;
        }
    }
}