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
static ListaCicular<string> *circular = new ListaCicular<string>();
void Menu();
ListaDoble<char> BuscaryReemplazar(ListaDoble<char> lista, string buscado, string remplazo, int x, Pila<Cambio> *ctrlZ);
ListaDoble<char> StringToChar(string content);
string CharToString(ListaDoble<char> lista);
int Searching(WINDOW *win, ListaDoble<char> lista, int yMax, Pila<Cambio> *ctrlZ);
string GraphLog(Pila<Cambio> *Z, Pila<Cambio> *Y);
void InsertarEnPila(Pila<Cambio> *ctrlZ, Cambio change);
void ArchivosRecientes();
string RecentFiles(ListaCicular<string> *circle);
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

void DuplicatePila(Pila<Cambio> *original, Pila<Cambio> *copia){
    if(!original->Empty()){
        Nodo<Cambio> *aux = original->Pop();
        DuplicatePila(original, copia);
        original->Push(aux->getValue());
        copia->Push(aux->getValue());
    }
}
void CrearArchivo(string origin, bool creado){
    box(stdscr, 0, 0);
    noecho();
    cbreak();
    raw();
    ListaDoble<char> lista;
    if(creado){
        lista = StringToChar(origin);
    }
    Pila<Cambio> *ctrlZ = new Pila<Cambio>();
    Pila<Cambio> *ctrlY = new Pila<Cambio>();
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
    bool busqueda=false;
    bool salida = false;
    while(true){
        wrefresh(inputwin);
        int character = wgetch(inputwin);
        if(character==23){
            //Buscar y Reemplazar
            int e = Searching(inputwin, lista, yMax, ctrlZ);
            wclear(inputwin);
            mvwprintw(inputwin, 1, 1, "Se encontraron %d", e);
            wgetch(inputwin);
            /*
            mvwprintw(inputwin, yMax-5, 3, "Palabra Buscada; Palabra De Reemplazo");
            int c;
            string entrada="";
            while(c!=10){
                wrefresh(inputwin);
                c=mvwgetch(inputwin, yMax-4, 3);
                entrada+=c;
                refresh();
                wrefresh(inputwin);
            }
            string buscado, reemplazo;
            bool first = true;
            for(int i = 0 ; i<entrada.size()-1;i++){
                    if(entrada[i]==';'){
                        first = false;
                    }else if(first){
                        buscado+=entrada[i];
                    }else{
                        reemplazo+=entrada[i];
                    }
            }
            mvwprintw(inputwin, yMax-5, 3, "CAMBIANDO...");
            wrefresh(inputwin);
            int x=0;
            lista = BuscaryReemplazar(lista, buscado, reemplazo, x);
            if(x>0){
                mvwprintw(inputwin, 0, 0,"La cantida de coincidencias es: ", "%d", x);
                wrefresh(inputwin);
            }else{
                mvwprintw(inputwin, 0, 0,"No hay coincidencias", "%d", x);
                wrefresh(inputwin);
            }
            wrefresh(inputwin);
            */
        }else if(character==3){
            //Reportes
            noecho();
            wattron(inputwin, A_REVERSE);
            mvwprintw(inputwin, yMax-4, 3, "Reportes: 1)Lista   2)Palabras Buscadas 3)Palabras Ordenadas");
            wattroff(inputwin, A_REVERSE);
            wrefresh(inputwin);
            int option = wgetch(inputwin);
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
                    mvwprintw(inputwin, yMax-5,3, "GENERANDO GRAFICO...");
                    wgetch(inputwin);
                    wrefresh(inputwin);
                }else{
                    mvwprintw(inputwin,1,1, "No se ha introducido texto, intentalo de nuevo");
                    wrefresh(inputwin);
                }
            }else if(option==50){
                //Reporte Buscada
                ofstream graphFile;
                graphFile.open("SavedFiles/Pilas.txt");
                graphFile << GraphLog(ctrlZ, ctrlY);
                graphFile.close();
                std::string filePath="dot -Tpng SavedFiles/Pilas.txt -o SavedFiles/Pilas.png";
                system(filePath.c_str());
                mvwprintw(inputwin, yMax-5,3, "GENERANDO GRAFICO...");
                wgetch(inputwin);
                wrefresh(inputwin);
            }else if(option==51){
                //Reporte Ordenada
            }
        }else if(character==19){
            //Guardar
            char nombre[100];
            echo();
            mvwprintw(inputwin, yMax-5, 3, "Introduce el nombre del documento\n\t");
            wrefresh(inputwin);
            //mvwgetnstr(inputwin, yMax-4, 3, nombre, 99);
            echo();
            int c;
            string entrada="SavedFiles/";
            while(c!=10){
                wrefresh(inputwin);
                c=wgetch(inputwin);
                entrada+=c;
                refresh();
                wrefresh(inputwin);
            }
            mvwprintw(inputwin, yMax-5, xMax-(xMax-3), "GUARDANDO ARCHIVO...");
            wrefresh(inputwin);
            string content = "";
            ofstream myfile;
            myfile.open(entrada);
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
            circular->Insertar(entrada);
        }else if(character==24){
            //Salir
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);
            clear();
            box(inputwin, 0, 0);
            wrefresh(inputwin);
            mvprintw(yMax/2, xMax/3,  "¡Esperamos que te haya sido util! \n                                  adios :)");
            break;
        }else if(character==KEY_BACKSPACE){
            if(!lista.Empty()){
                Cambio nuevo;
                nuevo.Setbuscar("");
                nuevo.Setreemplazar("");
                nuevo.Setestado(false);
                string palabra="";
                palabra+=lista.GetLast()->getValue();
                nuevo.Setpalabra(palabra);
                nuevo.Setposicion(lista.GetSize());
                nuevo.Setcadena(CharToString(lista));
                ctrlZ->Push(nuevo);
                lista.DeleteLast();
            }
        }else if(character==25){
            //CTRL+Y
            if(!ctrlY->Empty()){
                Nodo<Cambio> *change = ctrlY->Pop();
                change->getValue().Setestado(true);
                ctrlZ->Push(change->getValue());
                lista = StringToChar(change->getValue().Getcadena());
            }
        }else if(character==26){
            //CTRL+Z
            if(!ctrlZ->Empty()){
                Nodo<Cambio> *change = ctrlZ->Pop();
                change->getValue().Setestado(false);
                ctrlY->Push(change->getValue());
                lista = StringToChar(change->getValue().Getcadena());
            }
        }else if(character==27){
            //Tecla ESC
            salida = true;
            break;
        }else{
            //creo que se debe de vaciar la pila cada vez que se hace una nueva inserción.
            ctrlY->SuperPop();
            lista.Insertar(character);
            wrefresh(inputwin);
            Cambio nuevo;
            nuevo.Setbuscar("NULL");
            nuevo.Setreemplazar("NULL");
            nuevo.Setestado(false);
            string palabra="";
            palabra+=character;
            nuevo.Setpalabra(palabra);
            nuevo.Setbusqueda(false);
            nuevo.Setposicion(lista.GetSize());
            nuevo.Setcadena(CharToString(lista));
            ctrlZ->Push(nuevo);
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
    if(salida){
        clear();
        refresh();
        getch();
        Menu();
    }
}

void AbrirArchivo(){
    box(stdscr, 0, 0);
    noecho();
    cbreak();
    raw();
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * inputwin = newwin(yMax-3, xMax-2, 1, 1);
    getbegyx(inputwin, yBeg, xBeg);
    mvprintw(3, 1, "Ingrese la ruta del archivo...\t");
    wrefresh(inputwin);
    string content;
    string line;
    echo();
    int c;
    string nombre;
    while(c!=10){
        c= wgetch(stdscr);
        if(c==10){
        }else{
            nombre+=c;
        }
    }
    ifstream myfile(nombre);
    if(myfile.is_open()){
        while( getline(myfile, line)){
            content += line +"\n";
        }
        myfile.close();
        circular->Insertar(nombre);
        CrearArchivo(content, true);
    }else{
        content="No se ha encontrado el archivo"+nombre;
        wprintw(stdscr, "\nNo se ha podido abrir el archivo");
        wrefresh(stdscr);
        getch();
        Menu();
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
        int d = wgetch(inputwin);
        refresh();
        if(d==49){
            //Crear Archivo
            clear();
            refresh();
            CrearArchivo("", false);
        }else if(d==50){
            //Abrir Archivo
            clear();
            refresh();
            AbrirArchivo();
        }else if(d==51){
            //Archivos Recientes
            clear();
            refresh();
            ArchivosRecientes();
        }else if(d==52){
            //Salir
            int yMax, xMax;
            getmaxyx(stdscr, yMax, xMax);
            noecho();
            clear();
            mvprintw(yMax/2, xMax/3, "¡Esperamos que te haya sido util! \n                                  adios :)");
        }
}

int main(int argc, char ** argv)
{
	initscr();
	cbreak();
    raw();
    Menu();

	getch();
	endwin();
	return 0;
}

ListaDoble<char> BuscaryReemplazar(ListaDoble<char> lista, string buscado, string remplazo, int x, Pila<Cambio> *ctrlZ){
    cout << "\n Method Starts\n qry: "<<buscado<<"\n";
    Nodo<char> *aux = lista.GetCabeza();
    Nodo<char> *inicio;
    int i=0;
    cout << "\n While Starts\n";
    while(aux!=NULL){
        if(aux->getPrevious() == NULL || aux->getPrevious() == 0 || aux->getPrevious()->getValue()==' ' ||aux->getPrevious()->getValue()=='\n'||aux->getPrevious()->getValue()=='\t'|| i>0){    //Verifica que no sean caracteres dentro de una palabra
            cout <<"\n"<<aux->getValue() <<"compares"<<buscado[i]<<"\n";
            if(aux->getValue()==buscado[i]){
                if(i==0){
                    inicio = aux;
                }
                i++;
                if(aux->getNext()== NULL || aux->getNext()->getValue()==' ' || aux->getNext()->getValue()=='\n'){ //Verifica que sea el final de la palabra
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
                                x++;
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
                    }else{
                    i=0;
                    }
                }
            }else{
                i=0;
            }
        }
        aux= aux->getNext();
    }
    if(x>0){
        Cambio nuevo;
        nuevo.Setbuscar(buscado);
        nuevo.Setreemplazar(remplazo);
        nuevo.Setestado(false);
        nuevo.Setpalabra("NULL");
        nuevo.Setbusqueda(true);
        nuevo.Setposicion(x);
        nuevo.Setcadena(CharToString(lista));
        InsertarEnPila(ctrlZ, nuevo);
        cout << "\n Method Stops\n";
    }
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

int Searching(WINDOW *inputwin, ListaDoble<char> lista, int yMax, Pila<Cambio> *ctrlZ){
    mvwprintw(inputwin, yMax-5, 3, "Palabra Buscada; Palabra De Reemplazo\n\t");
            echo();
            int c;
            string entrada="";
            while(c!=10){
                wrefresh(inputwin);
                c=wgetch(inputwin);
                entrada+=c;
                refresh();
                wrefresh(inputwin);
            }
            string buscado, reemplazo;
            bool first = true;
            for(int i = 0 ; i<entrada.size()-1;i++){
                    if(entrada[i]==';'){
                        first = false;
                    }else if(first){
                        buscado+=entrada[i];
                    }else{
                        reemplazo+=entrada[i];
                    }
            }
            mvwprintw(inputwin, yMax-5, 3, "CAMBIANDO...");
            wrefresh(inputwin);
            int x;
            lista.InsertarInicio(' ');
            lista = BuscaryReemplazar(lista, buscado, reemplazo, x, ctrlZ);
            lista.DeleteFirst();
            if(x>0){
                mvwprintw(inputwin, 0, 0,"La cantida de coincidencias es: %d", x);
                wrefresh(inputwin);
            }else{
                mvwprintw(inputwin, 0, 0,"No hay coincidencias");
                wrefresh(inputwin);
            }
            wrefresh(inputwin);
            return x;
}

string GraphLog(Pila<Cambio> *Z, Pila<Cambio> *Y){
    string head = "digraph G{ \n rankdir=LR \n node[shape=box]; ";
    string pilaRealizados = "subgraph realizados { \n";
    string pilaRevertidos = "subgraph revertidos { \n";

    Pila<Cambio> *realizados = new Pila<Cambio>;
    Pila<Cambio> *revertidos = new Pila<Cambio>;
    Pila<Cambio> *testRealizados = new Pila<Cambio>;
    Pila<Cambio> *testRevertidos = new Pila<Cambio>;
     DuplicatePila(Z, testRealizados);
     DuplicatePila(Y, testRevertidos);
     while(!testRealizados->Empty()){
        Nodo<Cambio> *aux = testRealizados->Pop();
        Cambio contenido = aux->getValue();
         if(contenido.Getbusqueda()){
             realizados->Push(contenido);
         }
     }
     while(!testRevertidos->Empty()){
        Nodo<Cambio> *aux = testRevertidos->Pop();
        Cambio contenido = aux->getValue();
         if(contenido.Getbusqueda()){
             revertidos->Push(contenido);
         }
     }
     int i=1;     
     while(!realizados->Empty()){
        Nodo<Cambio> *aux = realizados->Pop();
        Cambio contenido = aux->getValue();
        string value = "\"";
        value+=to_string(i);
        value+=" Palabra Buscada:&#92;n";
        value+=contenido.Getbuscar();
        value+="&#92;nReemplazada por:&#92;n";
        value+=contenido.Getreemplazar();
        value+="&#92;nEstado:&#92;n No revertido";
        value+="&#92;nPalabra:";
        value+=contenido.Getpalabra();
        value+="&#92;nPosicion:&#92;";
        value+=to_string(contenido.Getposicion());
        i++;
        if(!realizados->Empty()){
        value+="\" -> ";
        }else{
        value+="\"; \n";  
        }
        pilaRealizados+=value;        
     }     
     pilaRealizados+=" label =\" Cambios Realizados\";}";     
     i=1;
     while(!revertidos->Empty()){
        Nodo<Cambio> *aux = revertidos->Pop();
        Cambio contenido = aux->getValue();
        std::string test("NULL");
            string value = "\"";
            value+=to_string(i);
            value+=" Palabra Buscada:&#92;n";
            value+=contenido.Getbuscar();
            value+="&#92;nReemplazada por:&#92;n";
            value+=contenido.Getreemplazar();
            value+="&#92;nEstado:&#92;n Revertido";
            value+="&#92;nPalabra: ";
            value+=contenido.Getpalabra();
            value+="&#92;nPosicion:&#92;";
            value+=to_string(contenido.Getposicion());
            i++;
            if(!revertidos->Empty()){
            value+="\" -> ";
            }else{
            value+="\"; \n";  
            }
            pilaRevertidos+=value;        
     }
     pilaRevertidos+=" label =\" Cambios Revertidos\";}";
     head += pilaRealizados+pilaRevertidos+"}";
     return head;
}

void InsertarEnPila(Pila<Cambio> *ctrlZ, Cambio change){
    ctrlZ->Push(change);
}

void ArchivosRecientes(){
    box(stdscr, 0, 0);
    noecho();
    cbreak();
    raw();
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(stdscr, yMax, xMax);
    WINDOW * inputwin = newwin(yMax-3, xMax-2, 1, 1);
    getbegyx(inputwin, yBeg, xBeg);
    mvprintw(3, 20, "Archivos Recientes");
    mvprintw(yMax-7, 20, "Presione X para generar Reporte");
    wrefresh(inputwin);
    
    keypad(inputwin, TRUE);
    refresh();
    wrefresh(inputwin);
    if(!circular->Empty()){
        Nodo<string> *aux = circular->GetCabeza();
        int y = 5;
        int x = 1;
        for(int i =0; i<circular->GetSize();i++){
            string val = aux->getValue();
            mvprintw(y, 19, "%d .\t ", i+1);
            string name;
            bool flag=false;
            int last = 0;
            for(int e=0; e<val.size();e++){
                mvprintw(y, 20+e, "%c", val[e]);
                if(flag){
                    name+=val[e];
                }
                if(val[e]=='/'){
                    name="";
                    flag=true;
                }
                
                last=20+e;
            }
            for(int u =0; u<name.size();u++){
                mvprintw(y, last+5+u, "%c", name[u]);
            }
            refresh();
            wrefresh(inputwin);
            y++;
            x++;
            aux=aux->getNext();
        }   
        int c = wgetch(inputwin);
        if(c==120){
            ofstream graphFile;
            string name = "RecentFiles";
            graphFile.open("SavedFiles/"+name+".txt");
            graphFile << RecentFiles(circular);
            graphFile.close();
            std::string filePath="dot -Tpng SavedFiles/"+name+".txt -o SavedFiles/"+name+".png";
            system(filePath.c_str());
            mvwprintw(inputwin, yMax-5,3, "GENERANDO GRAFICO...");
            wgetch(inputwin);
            wrefresh(inputwin);
            getch();
            Menu();
        }else if(circular->GetElementAt(c)!=0){
            ifstream myfile(circular->GetElementAt(c)->getValue());
            string content, line;
            if(myfile.is_open()){
                while( getline(myfile, line)){
                    content += line +"\n";
                }
                myfile.close();
                CrearArchivo(content, true);
            }else{
                content="No se ha encontrado el archivo";
                wprintw(stdscr, "No se ha podido abrir el archivo");
                wrefresh(stdscr);
                getch();
                Menu();
            }            
        }
        refresh();
        wrefresh(inputwin);
        wrefresh(stdscr);
    }else{
        mvprintw(4, 20, "No hay Archivos Recientes");
        refresh();
        wrefresh(inputwin);
        getch();
        Menu();
    }
}

string RecentFiles(ListaCicular<string> *circle){    
    string head = "digraph G{ \n rankdir=LR \n node[shape=box]; ";
    string content;
    string firstname;
    Nodo<string> *aux = circle->GetCabeza();
    for(int i =0;i<circle->GetSize(); i++){
        if(i==circle->GetSize()-1){            
            string val = aux->getValue();
            string path = "";
            string name="";
            bool flag = false;
            for(int e=0; e<val.size();e++){
                if(val[e]=='.'){
                    flag=false;
                }
                if(flag){
                    name+=val[e];
                }
                if(val[e]=='/'){
                    name="";
                    flag=true;
                }
            }
            content+="\""+to_string(i)+" "+name+"&#92;n"+aux->getValue()+"\"->"+"\"0 "+firstname+"&#92;n"+circle->GetCabeza()->getValue()+"\";";
        }else{
            string val = aux->getValue();
            string path = "";
            string name="";
            bool flag = false;
            for(int e=0; e<val.size();e++){
                if(val[e]=='.'){
                    flag=false;
                }
                if(flag){
                    name+=val[e];
                }
                if(val[e]=='/'){
                    name="";
                    flag=true;
                }
            }
            content+="\""+to_string(i)+" "+name+"&#92;n"+aux->getValue()+"\"->";
            if(i==0){
                firstname=name;
            }
        }
        aux=aux->getNext();
    }
    head+=content+"}";
    return head;
}