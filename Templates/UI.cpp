#include <ncurses.h>
#include <string>
#include <fstream>
#include "Nodo_t.h"
#include "ListaDoble_t.h"
#include "ListaSimple_t.h"
#include "ListaCircular_t.h"
#include "Pila_t.h"
#include "Cambio.h"

using namespace std;

void Menu();
ListaDoble<char> BuscaryReemplazar(ListaDoble<char> lista, string buscado, string remplazo, int x);
ListaDoble<char> StringToChar(string content);
string CharToString(ListaDoble<char> lista);
//changed all lista for list
string  GraphDoubleList(ListaDoble<char> lista){
    std::string graph ("digraph ReporteLD { graph [dpi=300]\n rankdir =LR; \n size=\"5\" \n node [shape = circle]; \n");
    Nodo<char> *aux = lista.GetCabeza();
    Nodo<char> *previous = aux;
    for(int i =0; i<lista.GetSize(); i++){
        string number = "_"+ std::to_string(i);
        string prevNumber = "_"+std::to_string((i-1));
        //NULL pointers to the graph edges
        if(i==0){
            graph +=  aux->getValue() + std::string( number + " -> NULL_;\n");
        }
        //The rest of the nodes
        else{
            previous = aux;
            aux=aux->getNext();
            graph += previous->getValue() + std::string(prevNumber + " -> " + aux->getValue() + number +";\n");
            graph += aux->getValue() + std::string( number + " -> " + previous->getValue() + prevNumber +";\n");
            if(i==lista.GetSize()-1){
                    graph +=  aux->getValue() + std::string( number + " -> _NULL;\n}");
            }
        }
    }
    return graph;
}



void Buscar(){
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * searchWin = newwin(2, xMax-5, yMax-5, 1);
    getbegyx(searchWin, yBeg, xBeg);
    //box(searchWin, 0,0);
    wrefresh(searchWin);
    keypad(searchWin, TRUE);
    string buscado, reemplazo = "";
    ListaDoble<char> temp ;
    wattron(searchWin,A_REVERSE);
    wattroff(searchWin,A_REVERSE);
    wrefresh(searchWin);
    wmove(searchWin, 1,1);
    while(true){
        int character = wgetch(searchWin);
        //wprintw(searchWin, "%c", character);
        if(character==10){
            wprintw(searchWin, "ENTER");
            Nodo<char> *aux = temp.GetCabeza();
            wprintw(searchWin, "%c", aux->getValue());
            bool first = true;
            int u = 0;
            while(aux->getNext()!=NULL){
                /*
                if(aux->getValue()==';'){
                first=false;
                }
                if(first){
                    char c = aux->getValue();
                    wprintw(searchWin, "%c", aux->getValue());
                    buscado += aux->getValue();
                }else{
                    char c = aux->getValue();
                    wprintw(searchWin, "%c", aux->getValue());
                    reemplazo += aux->getValue();
                }
                aux = aux->getNext();
                */
                u++;
                break;
                clear();
                wrefresh(searchWin);
            }
            wprintw(searchWin, "%d", u);
        }else if(character==KEY_BACKSPACE){
            temp.DeleteLast();
        }else if(character==24){
            //Salir
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);
            clear();
            box(searchWin, 0, 0);
            wrefresh(searchWin);
            mvprintw(yMax/2, xMax/3, "¡Hasta la proximaaaa!");
            break;
        }else{
            temp.Insertar(character);
        }
        if(!temp.Empty()){
            wclear(searchWin);
            refresh();
            wrefresh(searchWin);
            wmove(searchWin, yBeg+1, xBeg+1);
            Nodo<char> *aux = temp.GetCabeza();
            while(aux->getNext()!=NULL)
            {
                char c = aux->getValue();
                wprintw(searchWin, "%c", c);
                aux = aux->getNext();
            }
            char c = aux->getValue();
                wprintw(searchWin, "%c", c);
        }
        int palabrasAfectadas;
        //BuscaryReemplazar(lista, "", "", palabrasAfectadas);
    }
}

void CrearArchivo(){
    box(stdscr, 0, 0);
    noecho();
    cbreak();
    raw();
    ListaDoble<char> lista;
    Pila<Cambio> ctrlZ, ctrlY;
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * inputwin = newwin(yMax-3, xMax-2, 1, 1);
    getbegyx(inputwin, yBeg, xBeg);
    wrefresh(inputwin);
    move(yMax-1,xMax/6);
    printw("^w (Buscar y Reemplazar)    ^c(Reportes)    ^s(Guardar)");
    refresh();
    keypad(inputwin, TRUE);
    refresh();
    wrefresh(inputwin);
    wmove(inputwin, yBeg+1, xBeg+1);
    while(true){
        wrefresh(inputwin);
        int character = wgetch(inputwin);
        if(character==23){
            //Buscar y Reemplazar
            mvwprintw(inputwin, yMax-5, 3, "Buscando...");
            char busqueda[100];
            echo();
            mvwgetnstr(inputwin, yMax-4, 3, busqueda, 99);
            wrefresh(inputwin);
            string buscado, reemplazo;
            bool first = true;
            for(int i = 0 ; i<=99;i++){
                if(busqueda[i]!=0){
                    if(busqueda[i]==' '){
                        first = false;
                    }
                    if(first){
                        buscado+=busqueda[i];
                    }else{
                        reemplazo+=busqueda[i];
                    }
                }
            }
            mvwprintw(inputwin, yMax-5, 3, "CAMBIANDO...");
            wrefresh(inputwin);
            int x=0;
            lista = BuscaryReemplazar(lista, buscado, reemplazo, x);
            if(x>0){
                mvwprintw(stdscr, 0, 0,"hay match");
                refresh();
            }
            mvwprintw(inputwin, yMax-5, 3, "%s",reemplazo);
            wrefresh(inputwin);
        }else if(character==3){
            //Reportes
            noecho();
            wattron(inputwin, A_REVERSE);
            mvwprintw(inputwin, yMax-4, 3, "Reportes: 1)Lista   2)Palabras Buscadas 3)Palabras Ordenadas");
            wattroff(inputwin, A_REVERSE);
            wrefresh(inputwin);
            int option = wgetch(inputwin);
            mvwprintw(inputwin, yMax-4, 3, "%d", option);
            wrefresh(inputwin);
            getch();
            if(option==49){
                //Reporte Lista
                if(!lista.Empty()){
                    ofstream graphFile;
                    string name = "Consola";
                    graphFile.open("SavedFiles/"+name+".txt");
                    graphFile << GraphDoubleList(lista);
                    graphFile.close();
                    std::string filePath="dot -Tpng SavedFiles/"+name+".txt -o SavedFiles/"+name+".png";
                    system(filePath.c_str());
                    mvwprintw(inputwin, yMax-5, xMax-(xMax-3), "GENERANDO GRAFICO...");
                    wrefresh(inputwin);
                }else{
                    wprintw(inputwin, "No se ha introducido texto, intentalo de nuevo");
                    wrefresh(inputwin);
                }
            }else if(option==50){
                //Reporte Buscada

            }else if(option==51){
                //Reporte Ordenada

            }
        }else if(character==19){
            //Guardar
            char nombre[100];
            echo();
            mvwprintw(inputwin, yMax-5, 3, "Introduce el nombre del documento");
            wrefresh(inputwin);
            mvwgetnstr(inputwin, yMax-4, 3, nombre, 99);
            mvwprintw(inputwin, yMax-5, xMax-(xMax-3), "GUARDANDO ARCHIVO...");
            wrefresh(inputwin);
            string content = "";
            ofstream myfile;
            myfile.open(nombre);
            Nodo<char> *aux = lista.GetCabeza();
            while(aux->getNext()!=NULL)
            {
                char c = aux->getValue();
                content += aux->getValue();
                aux = aux->getNext();
            }
            content += aux->getValue();
            myfile << content;   //content of the new file
            myfile.close();
        }else if(character==24){
            //Salir
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);
            clear();
            box(inputwin, 0, 0);
            wrefresh(inputwin);
            mvprintw(yMax/2, xMax/3, "¡Hasta la proximaaaa!");
            break;
        }else if(character==KEY_BACKSPACE){
            Cambio nuevo;
            nuevo.Setbuscar("");
            nuevo.Setreemplazar("");
            nuevo.Setestado(false);
            string palabra="";
            palabra+=lista.GetLast()->getValue();
            nuevo.Setpalabra(palabra);
            nuevo.Setposicion(lista.GetSize());
            nuevo.Setcadena(CharToString(lista));
            //("", "", false, lista.GetLast()->getValue(), lista.GetSize(), CharToString(lista));
            ctrlZ.Push(nuevo);
            lista.DeleteLast();
        }else if(character==25){
            //CTRL+Y
            if(!ctrlY.Empty()){
                Nodo<Cambio> *change = ctrlY.Pop();
                ctrlZ.Push(change->getValue());
                lista = StringToChar(change->getValue().Getcadena());
            }
        }else if(character==26){
            //CTRL+Z
            if(!ctrlZ.Empty()){
                Nodo<Cambio> *change = ctrlZ.Pop();
                ctrlY.Push(change->getValue());
                lista = StringToChar(change->getValue().Getcadena());
            }
        }else{
            //creo que se debe de vaciar la pila cada vez que se hace una nueva inserción.
            ctrlY.SuperPop();
            lista.Insertar(character);
            wrefresh(inputwin);
            Cambio nuevo;
            nuevo.Setbuscar("");
            nuevo.Setreemplazar("");
            nuevo.Setestado(false);
            string palabra="";
            palabra+=character;
            nuevo.Setpalabra(palabra);
            nuevo.Setposicion(lista.GetSize());
            nuevo.Setcadena(CharToString(lista));
            ctrlZ.Push(nuevo);
        }
        if(!lista.Empty()){
            wclear(inputwin);
            refresh();
            wrefresh(inputwin);
            wmove(inputwin, yBeg+1, xBeg+1);
            Nodo<char> *aux = lista.GetCabeza();
            while(aux->getNext()!=NULL)
            {
                char c = aux->getValue();
                wprintw(inputwin, "%c", c);
                aux = aux->getNext();
            }
            char c = aux->getValue();
                wprintw(inputwin, "%c", c);
        }
    }
}

void Menu(){
    wborder(stdscr, 0, 0, 0, 0, 0, 0, 0, 0);
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * inputwin = newwin(yMax-2, xMax-2, 1, 1);
    keypad(inputwin, TRUE);
    //box(inputwin, 0, 0);
    refresh();
    wrefresh(stdscr);
	//wborder(inputwin, 0, 0, 0, 0, 0, 0, 0, 0);
	wattron(inputwin, A_REVERSE);
	mvwprintw(inputwin, 1, 5, "UNIVERSIDAD DE SAN CARLOS DE GUATEMALA");			//moves wprintw(window, y, x, text);
    mvwprintw(inputwin, 2, 5, "FACULTAD DE INGENNIERIA");
    mvwprintw(inputwin, 3, 5, "ESTRUCTURAS DE DATOS");
    mvwprintw(inputwin, 4, 5, "PRACTICA 1");
    mvwprintw(inputwin, 5, 5, "JOSUE GUILLERMO ORELLANA CIFUENTES");
    mvwprintw(inputwin, 6, 5, "201801366");
    wattroff(inputwin, A_REVERSE);
    mvwprintw(inputwin, 10, 5, "MENU");
    mvwprintw(inputwin, 11, 5, "1. Crear Archivo");
    mvwprintw(inputwin, 12, 5, "2. Abrir Archivo");
    mvwprintw(inputwin, 13, 5, "3. Archivos recientes");
    mvwprintw(inputwin, 14, 5, "4. Salir");
	refresh();
	wrefresh(inputwin);
        int d = getch();
        if(d==49){
            //Crear Archivo
            clear();
            refresh();
            CrearArchivo();
            //mvprintw(15, 5,"%d", d);

        }else if(d==50){
            //Abrir Archivo
            clear();
            refresh();
        }else if(d==51){
            //Archivos Recientes
            clear();
            refresh();
        }else if(d==52){
            //Salir
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);
            noecho();
            clear();
            //box(inputwin, 0, 0);
            mvprintw(yMax/2, xMax/3, "¡Hasta la proximaaaa!");
           // wrefresh(inputwin);
        }
}

int main(int argc, char ** argv)
{
	initscr();
	cbreak();
    raw();
    //int yMax, xMax;
    //getmaxyx(stdscr, yMax, xMax);
    Menu();

	getch();
	endwin();
	return 0;
}

ListaDoble<char> BuscaryReemplazar(ListaDoble<char> lista, string buscado, string remplazo, int x){
    cout << "\n Method Starts\n qry: "<<buscado<<"\n";
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
                                anterior->setNext(temporal);
                                temporal->getNext()->setPrevious(temporal);
                            }
                            anterior = temporal;
                        }
                        i=0;
                        x++;
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

ListaDoble<char> StringToChar(string content){
    ListaDoble<char> newList;
    for(int i=0; i<content.size();i++){
        newList.Insertar(content[i]);
    }
    return newList;
}

string CharToString(ListaDoble<char> lista){
    string content;
    Nodo<char> *aux = lista.GetCabeza();
    while(aux->getNext()!=NULL){
        char c = aux->getValue();
        content+=c;
        aux= aux->getNext();
    }
    char c = aux->getValue();
    content+=c;
    return content;
}



