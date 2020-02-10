#include <iostream>
#include "Nodo_t.h"
#include "ListaDoble_t.h"
#include "ListaSimple_t.h"
#include "ListaCircular_t.h"
using namespace std;

int main(){
	ListaDoble<char> lista;
	lista.Insertar('c');
	lista.Insertar('d');
	lista.Insertar('e');
	lista.Imprimir();
	lista.DeleteLast();
	lista.Imprimir();
	lista.Insertar('a');
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
/*	Nodo<char> nuevo3('e');
	lista.InsertAfter(nuevo, nuevo3);
	lista.Imprimir();
*/	return 0;
};
