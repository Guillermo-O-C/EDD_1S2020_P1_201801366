#include <iostream>
using namespace std;

class Nodo{
	private:
		char value;
		Nodo *next;
		Nodo *previous;
	public:
		Nodo(char value){
			this->value = value;
			this->next = NULL;
			this->previous = NULL;
		}
		void setValue(char value){
			this->value = value;
		}
		char getValue(){
			return value;
		}
		void setNext(Nodo *next){
			this->next = next;
		}
		void setPrevious(Nodo *previous){
			this->previous = previous;
		}
		Nodo* getNext(){
			return next;
		}
		Nodo* getPrevious(){
			return previous;
		}
};

class DL_Char{
	private:
		Nodo *cabeza;
		int size;
	public:
		DL_Char(){
			this->cabeza = NULL;
			this->size=0;
		}
		void Insertar(char new_char){
			Nodo value(new_char);
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
