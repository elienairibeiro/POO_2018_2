#include <iostream>
#include <sstream>

using namespace std;

struct Pet{
    int Energy;
    int energyMax;
    int Hungry;
    int hungryMax;
    int Clean;
    int cleanMax;
    int Diamonds;
    int Age;
    bool Vivo;

    // Metodos

    // Construtor

    Pet(int energyMax = 0, int hungryMax = 0, int cleanMax = 0, int Diamonds = 0, int Age = 0, bool Vivo = true):
        Energy(energyMax), energyMax(energyMax),
        Hungry(hungryMax), hungryMax(hungryMax),
        Clean(cleanMax), cleanMax(cleanMax),
        Diamonds(Diamonds),
        Age(Age),
        Vivo(Vivo)
    {}

    string toString(){
        stringstream fluxo;
        fluxo << "E:" << Energy << "/" << energyMax << ", "
            << "H:" << Hungry << "/" << hungryMax << ", "
            << "C:" << Clean << "/" << cleanMax << ", "
            << "D:" << Diamonds << ", "
            << "A:" << Age;
        return fluxo.str();
    }

    bool play(){
        if(!Vivo){
            cout << "  fail: pet esta morto";
            return false;
        }
        if((Energy -= 2) < 0){
            Energy = 0;
            cout << "  fail: pet morreu de fraqueza";
            Vivo = false;
            return false;
        }
        if(Hungry-- < 0){
            Hungry = 0;
            cout << "  fail: pet morreu de fome";
            Vivo = false;
            return false;
        }
        if((Clean -= 3) < 0){
            Clean = 0;
            cout << "  fail: pet morreu  de sujeira";
            Vivo = false;
            return false;
        }

        Diamonds++;
        Age++;

        return true;
    }

    bool eat(){
        if(!Vivo){
            cout << "  fail: pet esta morto";
            return false;
        }
        if(Energy-- < 0){
            Energy = 0;
            cout << "  fail: pet morreu de fraqueza";
            Vivo = false;
            return false;
        }
        if((Clean -= 2) < 0){
            Clean = 0;
            cout << "  fail: pet morreu  de sujeira";
            Vivo = false;
            return false;
        }

        if(Hungry += 4 > hungryMax){
            Hungry = hungryMax;
        }

        Age++;

        return true;
    }

    bool sleep(){
        if(!Vivo){
            cout << "  fail: pet esta morto";
            return false;
        }
        if(energyMax - Energy < 5){
            cout << "  fail: nao esta com sono";
            return false;
        }
        if(Hungry-- < 0){
            Hungry = 0;
            cout << "  fail: pet morreu de fome";
            Vivo = false;
            return false;
        }

        Age += energyMax - Energy;
        Energy = energyMax;

        return true;
    }

    bool clean(){
        if(!Vivo){
            cout << "  fail: pet esta morto";
            return false;
        }
        if(Hungry-- < 0){
            Hungry = 0;
            cout << "  fail: pet morreu de fome";
            Vivo = false;
            return false;
        }
        if((Energy -= 3) < 0){
            Energy = 0;
            cout << "  fail: pet morreu de fraqueza";
            Vivo = false;
            return false;
        }

        Age += 2;
        Clean = cleanMax;

        return true;
    }
};

struct Controller{
    Pet pet;

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;
        if(op == "init"){
            int Energy, Hungry, Clean;
            in >> Energy >> Hungry >> Clean;
            pet = Pet(Energy, Hungry, Clean);
            out << "success";
        }else if(op == "show"){
            out << pet.toString();
        }else if(op == "play"){
            if(pet.play()){
                out << "success";
            }
        }else if(op == "eat"){
            if(pet.eat()){
                out << "success";
            }
        }else if(op == "sleep"){
            if(pet.sleep()){
                out << "success";
            }
        }else if(op == "clean"){
            if(pet.clean()){
                out << "success";
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
            //cout << line << endl;
            cout << "  " << shell(line) << endl;
        }
    }
};

int main(){
    Controller controller;
    controller.exec();

    return 0;
}