#include <ncurses.h>
#include <string>
#include "Nodo_t.h"
#include "ListaDoble_t.h"
#include "ListaSimple_t.h"
#include "ListaCircular_t.h"
#include "Pila_t.h"

using namespace std;

string WinPrint(WINDOW win, ListaDoble<char> lista){
    if(!lista.Empty())
    {

    }
}

void CrearArchivo(){
    noecho();
    ListaDoble<char> lista;
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * inputwin = newwin(21, 78, 1, 1);
    getbegyx(inputwin, yBeg, xBeg);
    wrefresh(inputwin);
    move(22,xMax/6);
    printw("^w (Buscar y Reemplazar)    ^c(Reportes)    ^s(Guardar)");
    refresh();
    keypad(inputwin, TRUE);
    box(inputwin, 0, 0);
    refresh();
	wborder(inputwin, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(inputwin);
    wmove(inputwin, yBeg+1, xBeg+1);
    while(true){
        refresh();
        wrefresh(inputwin);
        int character = wgetch(inputwin);
        if(character==23){
            //Buscar y Reemplazar
            wprintw(inputwin, "BUSCAR");
        }else if(character==3){
            //Reportes
            wprintw(inputwin, "REPORTES");
        }else if(character==19){
            //Guardar
            wprintw(inputwin, "GUARDAR");
        }else if(character==24){
            //Salir
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);
            clear();
            //box(inputwin, 0, 0);
            mvprintw(yMax/2, xMax/3, "¡Hasta la proximaaaa!");
           // wrefresh(inputwin);
           break;
        }else if(character==KEY_BACKSPACE){
            lista.DeleteLast();
        }else{
            lista.Insertar(character);
            wrefresh(inputwin);
            //printw("%d",character);
            //refresh();
        }
        if(!lista.Empty()){
            wrefresh(inputwin);
            wmove(inputwin, yBeg+1, xBeg+1);
            Nodo<char> *aux = lista.GetCabeza();
            while(aux->getNext()!=NULL)
            {
                char c = aux->getValue();
                wprintw(inputwin, "%c", c);
                aux = aux->getNext();
            }
        }
    }
}

void Menu(){
    WINDOW * inputwin = newwin(22, 78, 1, 1);
    keypad(inputwin, TRUE);
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);
	wborder(inputwin, 0, 0, 0, 0, 0, 0, 0, 0);
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


