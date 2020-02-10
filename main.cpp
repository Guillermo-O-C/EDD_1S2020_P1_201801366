#include <iostream>
using namespace std;
#include "ListaDoble.h"
#include "DL_Char.h"

int main(){
	ListaDoble lista;
	Nodo nuevo("hola");
	Nodo gei("que pasa :C");
	lista.Imprimir();
	lista.Insertar(nuevo);
	lista.Insertar(gei);
	lista.Imprimir();
	
	DL_Char lista_Char;
	lista_Char.Insertar('f');
	lista_Char.Insertar('e');
	lista_Char.Insertar('o');
	lista_Char.Imprimir();
return 0;
};
