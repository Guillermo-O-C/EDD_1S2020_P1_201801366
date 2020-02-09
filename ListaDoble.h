#include <iostream>
#include "Nodo.h"
using namespace std;


class ListaDoble{
	private:
		Nodo *cabeza;
		int size;
	public:
		ListaDoble(){
			this->cabeza = NULL;
			this->size=0;
		}
		void Insertar(Nodo value){
			if(cabeza==NULL){
				cabeza = &value;
			}else{
				Nodo *aux;
				aux = cabeza;
				while(aux->getNext() != NULL){
					aux = aux->getNext();
				}
				aux->setNext(&value);
				value.setPrevious(aux);
				
			}
			
		}
		
		void Imprimir(){
				Nodo *aux;
				aux = cabeza;
				while(aux->getNext() != NULL){
					cout << aux->getValue();
					aux = aux->getNext();
				}
				cout << aux->getValue();
		}
};
