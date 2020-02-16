#include <iostream>
using namespace std;

class Palabra{
    private:
        Nodo<char> *inicio;
        Nodo<char> *fin;
    public:
        Palabra(Nodo<char> *inicio, Nodo<char> *fin){
            this->inicio = inicio;
            this->fin = fin;
        }
    Nodo<char>* GetInicio(){
        return this->inicio;
    }
    Nodo<char>* GetFin(){
        return this->fin;
    }
    void SetInicio(Nodo<char> *cambio){
        this->inicio = cambio;
    }
    void SetFin(Nodo<char> *cambio){
        this->fin = cambio;
    }
};
