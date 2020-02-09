#include <iostream>
using namespace std;

class Nodo{
	private:
		string value;
		Nodo *next;
		Nodo *previous;
	public:
		Nodo(string value){
			this->value = value;
			this->next = NULL;
			this->previous = NULL;
		}
		void setValue(string value){
			this->value = value;
		}
		string getValue(){
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
