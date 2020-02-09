#include <iostream>
using namespace std;
#include "ListaDoble.h"

int main(){
	ListaDoble lista;
	Nodo nuevo("hola");
	Nodo gei("que pasa :C");
	lista.Insertar(nuevo);
	lista.Insertar(gei);
	lista.Imprimir();
return 0;
};
