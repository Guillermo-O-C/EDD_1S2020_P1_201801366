#include <iostream>
#include "Nodo_t.h"
#include "ListaDoble_t.h"
#include "ListaSimple_t.h"
#include "ListaCircular_t.h"
#include "Pila_t.h"
#include "Palabra.h"
using namespace std;



ListaDoble<char> BuscaryReemplazar(ListaDoble<char> lista, string buscado, string remplazo){
    cout << "\n Method Starts\n qry: "<<buscado<<"\n";
    lista.Imprimir();
    ListaSimple<Palabra> coincidencias;
    Nodo<char> *aux = lista.GetCabeza();
    Nodo<char> *inicio;
    int i=0;
    cout << "\n While Starts\n";
    while(aux->getNext()!=NULL){
        if(aux->getPrevious()== NULL || aux->getPrevious()->getValue()==' ' || i>0){    //Verifica que no sean caracteres dentro de una palabra

        cout <<"\n"<<aux->getValue() <<"compares"<<buscado[i]<<"\n";
            if(aux->getValue()==buscado[i]){
                if(i==0){
                    inicio = aux;
                }
                i++;
                if(aux->getNext()== NULL || aux->getNext()->getValue()==' ' || aux->getNext()->getValue()=='\n' || i>0){ //Verifica que sea el final de la palabra
                    if(i==buscado.size()){
    cout << buscado.size()<< "\n match " <<remplazo.size() <<"\n";
                        Nodo<char> *anterior;
                        for(int e = 0; e<=remplazo.size();e++){
                            Nodo<char> *temporal = new Nodo<char>(remplazo[e]);
                            temporal->setNext(aux->getNext());
                            if(e==0){
                                if(inicio->getPrevious()==NULL){
                                    lista.SetCabeza(temporal);
                                }else{
                                    inicio=inicio->getPrevious();
                                    inicio->setNext(temporal);
                                    temporal->setPrevious(inicio);
                                }
                                    anterior = temporal;
                            }else if(e==remplazo.size()){
                                if(aux->getNext()==NULL){
                                    temporal->setNext(NULL);
                                }else{
                                    cout <<"\n el ultimo nodo conincede " <<aux->getValue() <<"\n";
                                    aux=aux->getNext();
                                    temporal->setNext(aux);
                                    aux->setPrevious(temporal);
                                }
                            }else{
                                cout <<"\n anterior is: "<<anterior->getValue() <<" Folowed by "<<anterior->getNext()->getValue() <<"\n";
                                temporal->setPrevious(anterior);
                                //temporal->setNext(anterior->getNext());
                                anterior->setNext(temporal);
                                temporal->getNext()->setPrevious(temporal);

                                //Nodo<char> *last = anterior->getNext();
                                //last->setPrevious(temporal);

                                //anterior->setNext(temporal);
                                //temporal->setPrevious(last);
                                //anterior=temporal;
                            }
                            anterior = temporal;
                        }
                        i=0;
                    }
                }else{
                    i=0;
                }
            }else{
                i=0;
            }

        }
        aux= aux->getNext();
    }
    cout << "\n Method Stops\n";
    return lista;
}


int main(){
	ListaDoble<char> lista;
	lista.Insertar('H');
	lista.Insertar('o');
	lista.Insertar('l');
	lista.Insertar('a');
	lista.Insertar(' ');
	lista.Insertar('j');
	lista.Insertar('o');
	lista.Insertar('v');
	lista.Insertar('e');
	lista.Insertar('n');
	lista.Insertar('e');
	lista.Insertar('s');
	lista.Insertar(' ');
	lista.Insertar('H');
	lista.Insertar('o');
	lista.Insertar('l');
	lista.Insertar('a');
	lista.Insertar(' ');
	lista.Insertar(' ');
	lista.Insertar('c');
	lista.Insertar('i');
	lista.Insertar('a');
	lista.Insertar(' ');
	lista.Insertar('H');
	lista.Insertar('o');
	lista.Insertar('l');
	lista.Insertar('a');
	lista.Imprimir();
	lista = BuscaryReemplazar(lista, "Hola", "Bueno Beno");
	cout <<"new string: ";
	lista.Imprimir();
	ListaSimple<string> list;
	list.Insertar(" hola ");
	list.Insertar(" hijos ");
	list.Insertar(" de ");
	list.Insertar(" puta ");
	list.Insertar(", besos ");
	cout <<list.GetSize();
	list.Imprimir();
	list.Imprimir();
	ListaCicular<int> calendario;
	calendario.Insertar(1);
	calendario.Insertar(2);
	calendario.Insertar(4);
	calendario.Imprimir();
	calendario.DeleteLast();
	calendario.Insertar(3);
	calendario.Imprimir();

	Pila<string> pila;
    pila.Push("hey");
    cout <<pila.ReturnTop()->getValue();
    pila.Push("bro");
    cout <<pila.ReturnTop()->getValue();
    pila.Pop();
    cout <<pila.ReturnTop()->getValue();
    pila.Push("bitch");
    cout <<pila.ReturnTop()->getValue();





	return 0;
};
