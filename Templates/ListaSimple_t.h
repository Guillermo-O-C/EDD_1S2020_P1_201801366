#include <iostream>
using namespace std;
#include "Nodo_t.h"
#pragma once
template <class T>
class ListaSimple{
	Nodo<T> *cabeza;
	int size;
	public:
		ListaSimple(){
			this->cabeza = NULL;
			this->size=0;
		}
		void Insertar(T value);
		void Imprimir();
		int GetSize();
		bool Empty();
};
template <class T>
void ListaSimple<T>:: Insertar(T value){
			Nodo<T> *nuevo = new Nodo<T>(value);
			if(cabeza==NULL){
				this->cabeza = nuevo;
				size++;
			}else{
				Nodo<T> *aux = this->cabeza;
				while(aux->getNext()!=NULL){
					aux = aux->getNext();
				}
				aux->setNext(nuevo);
				nuevo->setNext(NULL);
				size++;
			}

		}
template <class T>
int ListaSimple<T>:: GetSize(){
	return size;
}
template <class T>
bool ListaSimple<T>:: Empty(){
	if(size==0){
		return true;
	}
	return false;
}

template <class T>
void ListaSimple<T>:: Imprimir(){
				Nodo<T> *aux = this->cabeza;
				if(Empty()){
					 cout << "La lista está vacía \n" ;
				}else{
					for(int i = 0; i<size;i++){
						cout << aux->getValue();
						aux = aux->getNext();
					}
					cout << "\n";
				}
		}
