#ifndef CAMBIO_H
#define CAMBIO_H
#include <iostream>
using namespace std;

class Cambio
{
    public:
        string Getbuscar() { return buscar; }
        void Setbuscar(string val) { buscar = val; }
        string Getreemplazar() { return reemplazar; }
        void Setreemplazar(string val) { reemplazar = val; }
        bool Getestado() { return estado; }
        void Setestado(bool val) { estado = val; }
        string Getpalabra() { return palabra; }
        void Setpalabra(string val) { palabra = val; }
        int Getposicion() { return posicion; }
        void Setposicion(int val) { posicion = val; }
        string Getcadena() { return cadena; }
        void Setcadena(string val) { cadena = val; }
        bool Getbusqueda(){return busqueda;}
        void Setbusqueda(bool val) { busqueda = val;}
        /*Cambio(string buscar, string reemplazar, bool estado, string palabra, int posicion, string cadena){
            this-> buscar = buscar;
            this-> reemplazar = reemplazar;
            this-> estado = estado;
            this-> palabra = palabra;
            this-> posicion = posicion;
            this->cadena = cadena;
        }
        */

    private:
        string buscar;
        string reemplazar;
        bool estado;
        string palabra;
        int posicion;
        string cadena;
        bool busqueda;
};

#endif // CAMBIO_H
