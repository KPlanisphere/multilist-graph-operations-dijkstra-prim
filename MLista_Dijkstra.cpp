//BENEMERITA UNIVERSIDAD AUTONOMA DE PUEBLA
//FACULTAD DE CIENCIAS DE LA COMPUTACION
// - - - EQUIPO 6 "Los papus" - - -
#include <iostream>
#include <conio.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

//Variables globales
int pause;                  //Pausa
int totalnodos = 0;         //Cantidad de nodos actualmente registrados
int DirectedStatus = 1;     //1 = DIRIGIDO | 2 = NO DIRIGIDO

//Nodos de la lista principal
struct nodo {
    char    info;           //nombre del vertice o nodo
    bool    visitado;
    int     marca;
    struct  nodo* sig;
    struct  nodito* ady;    //puntero hacia la primera arista del nodo
};

//Uniones a otros nodos de la lista principal
struct nodito {
    bool    visitado;
    int     marca;
    int     suma;
    int     peso;
    struct  nodo* destino;  //puntero al nodo de llegada
    struct  nodito* sig;
};

//Redefiniciones
typedef struct nodo* Nodo;  //  Tipo Nodo
typedef struct nodito* Arista; //Tipo Arista

Nodo head; //puntero cabeza

// ░█▀▀█ ░█▀▀█ ░█▀▀▀█ ▀▀█▀▀ ░█▀▀▀█ ▀▀█▀▀ ▀█▀ ░█▀▀█ ░█▀▀▀█ ░█▀▀▀█
// ░█▄▄█ ░█▄▄▀ ░█──░█ ─░█── ░█──░█ ─░█── ░█─ ░█▄▄█ ░█──░█ ─▀▀▀▄▄
// ░█─── ░█─░█ ░█▄▄▄█ ─░█── ░█▄▄▄█ ─░█── ▄█▄ ░█─── ░█▄▄▄█ ░█▄▄▄█

// Menu
void menu();

// DIRIGIDO                                             // NO DIRIGIDO
void agregarArista(Nodo&, Nodo&, Arista&, int);         void agregarAristaND();
void insertarArista();                                  void enlaceNoDirigido(char, char);

// LIMPIEZA
void clearCola(queue<Nodo>&);
void clearTrueVisit(Nodo);

// BFS                                                  // DFS
void RecorridoAncho(char);                              void RecorridoProfundo(char);
void BPP(Nodo);

// PRIM                                                 // DIJKSTRA
void PRIM(char);                                        void dijkstra();
void update(Nodo&, Arista&);
void getMenorArco(Arista&, Arista&);
void PRIM_Aux(Nodo&, Arista&);
void adyacenciaPRIM(Arista&);
void pesoFinal(Arista&);

// OTROS
void insertarNodo();
void showMultilista();
void exit();

// - - > MAIN
int main() {
    head = NULL;
    int  op;
    char BFS;
    do {
        cout << "\n\tTIPO DE GRAFO A INGRESAR" << endl;
        cout << "\n [1] DIRIGIDO";
        cout << "\n [2] NO DIRIGIDO" << endl;
        cout << " >> ";
        cin >> DirectedStatus;
        if (DirectedStatus > 2 || DirectedStatus < 1) {
            cout << "\t[!] INGRESA UNA OPCION VALIDA [!]";
            pause = cin.get(); //Pausa
            pause = cin.get(); //Pausa
        }
        system("cls");
    } while (DirectedStatus > 2 || DirectedStatus < 1);

    do {
        menu();
        cin >> op;

        switch (op) {
        case 1:
            insertarNodo();
            break;
        case 2:
            if (DirectedStatus == 1) {
                insertarArista();
            }
            else {
                agregarAristaND();
            }
            break;
        case 3:
            showMultilista();
            break;
        case 4:
            cout << " NODO PARA INICIAR: ";
            cin >> BFS;
            RecorridoAncho(BFS);
            break;
        case 5:
            cout << " NODO PARA INICIAR: ";
            cin >> BFS;
            RecorridoProfundo(BFS);
            break;
        case 6:
            cout << " NODO PARA INICIAR: ";
            cin >> BFS;
            PRIM(BFS);
            break;
        case 7:
            dijkstra();
            break;
        case 8:
            exit();
            break;
        default:
            cout << "[!] ERROR: INGRESA UNA OPCION VALIDA [!]";
            break;
        }
        cout << endl << endl;
        system("pause");
        system("cls");

    } while (op != 8);
    pause = cin.get(); //Pausa
    return 0;
}

// - - > MENU DE OPCIONES
void menu() {
    cout << "\n\t - - - - LISTA DE LISTAS - MULTILISTAS - - - -" << endl;
    cout << "\t\t\t   (+ PRIM)\n" << endl;

    cout << " [1] INSERTAR UN NODO" << endl;
    cout << " [2] INSERTAR UNA ARISTA" << endl;
    cout << " [3] MOSTRAR  GRAFO" << endl;
    cout << " [4] RECORRIDO A LO ANCHO" << endl;
    cout << " [5] RECORRIDO A LO PROFUNDO" << endl;
    cout << " [6] PRIM" << endl;
    cout << " [7] DIJKSTRA" << endl;
    cout << " [8] SALIR" << endl;

    cout << "\n INGRESE OPCION: ";
}

// ░█▀▀▄ ▀█▀ ░█▀▀█ ▀█▀ ░█▀▀█ ▀█▀ ░█▀▀▄ ░█▀▀▀█
// ░█─░█ ░█─ ░█▄▄▀ ░█─ ░█─▄▄ ░█─ ░█─░█ ░█──░█
// ░█▄▄▀ ▄█▄ ░█─░█ ▄█▄ ░█▄▄█ ▄█▄ ░█▄▄▀ ░█▄▄▄█

// - - > AGREGAR UNIONES
void agregarArista(Nodo& aux, Nodo& aux2, Arista& nuevo, int peso) {
    Arista saveNoditos;
    if (aux->ady == NULL) { //Si el nodo no tiene nodos adyacentes...
        aux->ady = nuevo; //Agregamos un nodito al nodo actual
        nuevo->destino = aux2; //El nombre del nodito sera el mismo que uno de los nodos principales
        nuevo->peso = peso;
    }
    else { //Si el nodo ya tiene 1 o mas nodos adyacentes...
        saveNoditos = aux->ady; //Guardamos todos los noditos del nodo en saveNoditos
        while (saveNoditos->sig != NULL) { //Mientras no estemos en el ultimon nodito...
            saveNoditos = saveNoditos->sig; //Pasamos al siguiente nodito
        }
        //Cuando estamos en el ultimo nodito...
        nuevo->destino = aux2; //El nombre del nodito sera el mismo que uno de los nodos principales
        nuevo->peso = peso;
        saveNoditos->sig = nuevo; //Guardamos el nuevo nodito en la posicion siguiente al actual
    }
    cout << "\n [!] UNION AGREGADA CORRECTAMENTE [!]";
}

// - - > INSERTAR UNION
// Busca un nodo padre (primer dato) y le asigna un hijo (segundo dato)
void insertarArista() {
    char inicio, fin;
    int peso;
    Arista nuevo = new struct nodito;
    Nodo aux, aux2;
    //Verificar lista vacia
    if (head == NULL) {
        cout << "\n [!] LA LISTA ESTA VACIA [!]";
        return;
    }
    nuevo->sig = NULL;
    //Entrada de nodos a ligar
    cout << "\n NODO INICIAL: ";
    cin >> inicio;
    cout << " NODO FINAL: ";
    cin >> fin;
    cout << " PESO: ";
    cin >> peso;
    //Apuntadores auxiliares
    aux = head;
    aux2 = head;
    //Recorremos desde la cabezera hasta encontrar el valor FINAL
    while (aux2 != NULL) {
        if (aux2->info == fin) {
            break;
        }
        aux2 = aux2->sig;
    }
    //Recorremos desde la cabezera hasta encontrar el valor INICIO
    while (aux != NULL) {
        if (aux->info == inicio) {
            agregarArista(aux, aux2, nuevo, peso);
            return;
        }
        aux = aux->sig;
    }
}


// ░█▄─░█ ░█▀▀▀█ 　 ░█▀▀▄ ▀█▀ ░█▀▀█ ▀█▀ ░█▀▀█ ▀█▀ ░█▀▀▄ ░█▀▀▀█
// ░█░█░█ ░█──░█ 　 ░█─░█ ░█─ ░█▄▄▀ ░█─ ░█─▄▄ ░█─ ░█─░█ ░█──░█
// ░█──▀█ ░█▄▄▄█ 　 ░█▄▄▀ ▄█▄ ░█─░█ ▄█▄ ░█▄▄█ ▄█▄ ░█▄▄▀ ░█▄▄▄█

// - - > AGREGAR UNIONES (NO DIRIGIDO)
void agregarAristaND() {
    Nodo aux, aux2;
    Arista auxArista;
    char nodin, nodin2;
    int band, band2, band3;
    if (head != NULL) {
        aux = head;
        while (aux != NULL) {
            cout << aux->info << "  ";
            aux = aux->sig;
        }
        cout << "\n NODO A ENLAZAR: ";
        cin >> nodin;
        band = 0;
        while (band == 0) {
            aux = head;
            while (aux != NULL && band == 0) {
                if (aux->info == nodin) {
                    band = 1;
                }
                aux = aux->sig;
            }
            if (band == 0) {
                cout << "\n [!] EL NODO NO EXISTE [!]" << endl;
                cout << " INGRESA UNO NUEVO: ";
                cin >> nodin;
            }
        }
        cout << "\n NODOS DISPONIBLES PARA ENLAZAR:" << endl;
        aux = head;
        aux2 = head;
        while (aux2->info != nodin) {
            aux2 = aux2->sig;
        }
        while (aux != NULL) {
            if (aux->info != nodin) {
                auxArista = aux2->ady;
                band2 = 0;
                while (auxArista != NULL && band2 == 0) {
                    if (aux->info == auxArista->destino->info) {
                        band2 = 1;
                    }
                    auxArista = auxArista->sig;
                }
                if (band2 == 0) {
                    cout << aux->info << "  ";
                }
            }
            aux = aux->sig;
        }
        cout << "\n ENLAZAR CON..." << endl;
        cout << " >> ";
        cin >> nodin2;
        band = 0;
        while (band == 0) {
            aux = head;
            while (nodin2 == nodin) {
                cout << "\n [!] ENLACE NO VALIDO [!]" << endl;
                cout << " INGRESA UNO NUEVO: ";
                cin >> nodin2;
            }
            while (aux != NULL && band == 0) {
                if (aux->info == nodin2) {
                    band = 1;
                }
                aux = aux->sig;
            }
            if (band == 0) {
                cout << "\n [!] EL NODO NO EXISTE [!]" << endl;
                cout << " INGRESA UNO NUEVO: ";
                cin >> nodin2;
            }
        }
        aux = head;
        while (aux->info != nodin) {
            aux = aux->sig;
        }
        auxArista = aux->ady;
        band3 = 0;
        while (auxArista != NULL && band3 == 0) {
            if (auxArista->destino->info == nodin2) {
                band3 = 1;
            }
            auxArista = auxArista->sig;
        }
        if (band3 == 0) {
            //FUNCION GIGAPRO
            enlaceNoDirigido(nodin, nodin2);
            cout << "\n [!] ENLACE CREADO CORRECTAMENTE [!]";
        }
        else {
            cout << "\n [!] EL ENLACE YA EXISTE [!]" << endl;
        }
    }
}


// - - > INSERTAR UNION (NO DIRIGIDO)
void enlaceNoDirigido(char inicio, char final) {
    Nodo aux, aux2;
    Arista new_arista, auxArista;
    int peso;
    aux = head;
    while (aux->info != inicio) {
        aux = aux->sig;
    }
    aux2 = head;
    while (aux2->info != final) {
        aux2 = aux2->sig;
    }
    if (aux->ady == NULL) {
        new_arista = new nodito;
        new_arista->destino = aux2;
        cout << "\n PESO: ";
        cin >> peso;
        new_arista->peso = peso;
        new_arista->visitado = false;
        new_arista->suma = 0;
        new_arista->sig = NULL;
        aux->ady = new_arista;
        if (aux2->ady == NULL) {
            new_arista = new nodito;
            new_arista->destino = aux;
            new_arista->peso = peso;
            new_arista->visitado = false;
            new_arista->suma = 0;
            new_arista->sig = NULL;
            aux2->ady = new_arista;
        }
        else {
            new_arista = new nodito;
            new_arista->destino = aux;
            new_arista->peso = peso;
            new_arista->visitado = false;
            new_arista->suma = 0;
            new_arista->sig = NULL;
            auxArista = aux2->ady;
            while (auxArista->sig != NULL) {
                auxArista = auxArista->sig;
            }
            auxArista->sig = new_arista;
        }
    }
    else {
        new_arista = new nodito;
        new_arista->destino = aux2;
        cout << "\n PESO: ";
        cin >> peso;
        new_arista->peso = peso;
        new_arista->visitado = false;
        new_arista->suma = 0;
        new_arista->sig = NULL;
        auxArista = aux->ady;
        while (auxArista->sig != NULL) {
            auxArista = auxArista->sig;
        }
        auxArista->sig = new_arista;
        if (aux2->ady == NULL) {
            new_arista = new nodito;
            new_arista->destino = aux;
            new_arista->peso = peso;
            new_arista->visitado = false;
            new_arista->suma = 0;
            new_arista->sig = NULL;
            aux2->ady = new_arista;
        }
        else {
            new_arista = new nodito;
            new_arista->destino = aux;
            new_arista->peso = peso;
            new_arista->visitado = false;
            new_arista->suma = 0;
            new_arista->sig = NULL;
            auxArista = aux2->ady;
            while (auxArista->sig != NULL) {
                auxArista = auxArista->sig;
            }
            auxArista->sig = new_arista;
        }
    }
}


// ░█─── ▀█▀ ░█▀▄▀█ ░█▀▀█ ▀█▀ ░█▀▀▀ ░█▀▀▀█ ─█▀▀█ 
// ░█─── ░█─ ░█░█░█ ░█▄▄█ ░█─ ░█▀▀▀ ─▄▄▄▀▀ ░█▄▄█
// ░█▄▄█ ▄█▄ ░█──░█ ░█─── ▄█▄ ░█▄▄▄ ░█▄▄▄█ ░█─░█

// - - > CLEAN COLA
void clearCola(queue<Nodo>& cola) {
    queue<Nodo> vacio; //Creamos una cola vacia (de tipo Nodo)
    swap(cola, vacio); //Invertimos su contenido
}

//PASAR TODOS LOS TRUE A FALSE
void clearTrueVisit(Nodo aux) {
    while (aux != NULL) { //Mientras no lleguemos al final...
        if (aux->visitado == true) { //Si esta visitado...
            aux->visitado = false; //Marcamos como no visitado
        }
        aux = aux->sig; //Pasamos al siguiente nodo
    }
}


// ░█▀▀█ ░█▀▀▀ ░█▀▀▀█
// ░█▀▀▄ ░█▀▀▀ ─▀▀▀▄▄
// ░█▄▄█ ░█─── ░█▄▄▄█
// RECORRIDO A LO ANCHO (BFS)

void RecorridoAncho(char raiz) {
    queue<Nodo> cola;
    Nodo aux, x, y;
    Arista aux2, ndito = NULL;
    aux = head;
    //LIMPIEZA DE LA COLA (xd)
    clearCola(cola);
    //PASAR TODOS LOS TRUE A FALSE
    clearTrueVisit(aux);
    //Detecta el char en la multilista
    while (aux != NULL) {
        if (aux->info == raiz) {
            break;
        }
        aux = aux->sig;
    }
    y = head;
    //Marcamos como visitado el nodo de entrada
    aux->visitado = true;
    cola.push(aux); //Lo metemos a la cola
    cout << "\n BFS: ";

    while (!cola.empty()) {
        x = cola.front(); //Guardamos el frente de la cola (NO SE ELIMINA)
        //Guardamos la direccion del inicio de los nodos adyacentes
        aux2 = x->ady;
        //Imprimimos el frente de la cola
        cout << x->info << "  ";
        cola.pop(); //Eliminamos el primero

        //Para cada nodo Y adyacente a X
        while (x->ady != NULL) {
            ndito = x->ady;
            //Recorremos todos los noditos del Nodo
            while (ndito != NULL) {
                //Recorremos todos los nodos buscando un valor igual a Nodito
                while (y != NULL) {
                    //Si el nodo tiene el mimso nombre que el nodito...
                    if (y->info == ndito->destino->info) {
                        if (y->visitado == false) { //Verificamos si no esta visitado
                            y->visitado = true; //En el NODO marcamos visitado
                            cola.push(y); //Entra a la cola
                        }
                    }
                    y = y->sig; //Pasamos al siguiente NODO (del iterador)
                }
                ndito = ndito->sig; //Pasamos al siguiente NODITO
                y = head; // Reestablecemos el inicio de los NODOS
            }
            x->ady = x->ady->sig; //Pasamos al siguiente NODITO
        }
        //Reestablecemos la direccion del inicio de los nodos adyacentes
        x->ady = aux2;
    }


}

// ░█▀▀▄ ░█▀▀▀ ░█▀▀▀█
// ░█─░█ ░█▀▀▀ ─▀▀▀▄▄
// ░█▄▄▀ ░█─── ░█▄▄▄█
// RECORRIDO A LO PROFUNDO

// Busqueda primero en profundidad
void RecorridoProfundo(char raiz) {
    Nodo aux;
    Arista ndito = NULL;
    aux = head;
    //PASAR TODOS LOS TRUE A FALSE
    clearTrueVisit(aux);
    //Detecta el char en la multilista
    while (aux != NULL) {
        if (aux->info == raiz) {
            break;
        }
        aux = aux->sig;
    }
    //Si el nodo no esta visitado...
    if (aux->visitado == false) {
        cout << "\n DFS: ";
        BPP(aux); //Entramos al recorrido a lo profundo
    }
}

// - - > BPP
void BPP(Nodo in) {
    Nodo y = head;
    Arista aux2, ndito = NULL;
    //Marcamos como visitado el nodo de entrada
    in->visitado = true;
    //Imprimimos el nodo de entrada
    cout << in->info << "  ";
    //Guardamos el apuntador al inicio de sus nodos (noditos) adyacentes
    aux2 = in->ady;
    //RECORRIDO
    //Para cada nodo Y adyacente a X
    while (in->ady != NULL) {
        ndito = in->ady;
        //Recorremos todos los noditos del Nodo
        while (ndito != NULL) {
            //Recorremos todos los nodos buscando un valor igual a Nodito
            while (y != NULL) {
                //Si el nodo tiene el mismo nombre que el nodito...
                if (y->info == ndito->destino->info) {
                    if (y->visitado == false) { //Verificamos si no esta visitado
                        BPP(y);
                    }
                }
                y = y->sig; //Pasamos al siguiente NODO (del iterador)
            }
            ndito = ndito->sig; //Pasamos al siguiente NODITO
            y = head; // Reestablecemos el inicio de los NODOS
        }
        in->ady = in->ady->sig; //Pasamos al siguiente NODITO
    }
    //Reestablecemos la direccion del inicio de los nodos adyacentes
    in->ady = aux2;
}


// ░█▀▀█ ░█▀▀█ ▀█▀ ░█▀▄▀█
// ░█▄▄█ ░█▄▄▀ ░█─ ░█░█░█
// ░█─── ░█─░█ ▄█▄ ░█──░█
// - - > Algoritmo de PRIM
void PRIM(char raiz) {
    Nodo aux = NULL;
    Arista auxArista = NULL;
    char band, band2;

    if (head != NULL) { //Si el apuntador de cabeza tiene información...
        //FUNCION DE ACTUALIZACION
        update(aux, auxArista);
        band = 0;
        while (band == 0) {
            aux = head;
            //Detecta el char en la multilista
            while (aux != NULL) {
                if (aux->info == raiz) { //Cuando el nodo actual en info sea igual al char...
                    band = 1; //Bandera se activa terminando el ciclo
                }
                aux = aux->sig; //Pasamos al siguiente nodo
            }
            /*
            AQUI VA UNA EXCEPCION CUANDO LA RAIZ NO ESTA
            EN LA LISTA DE NODOS [!!!!]
            if (band == 0) {
                despues lo termino xxd
            }*/

        }
        aux = head; //Apuntando al inicio de los nodos
        while (aux->info != raiz) { //Ubicamos aux con el valor de raiz
            aux = aux->sig;
        }
        aux->marca = 1; //Marcamos como 1 el nodo actual
        auxArista = aux->ady; //En una variable auxiliar, guardamos todos los adyacentes del nodo actual
        while (auxArista != NULL) { //Mientras no lleguemos al ultimo adyacente...
            auxArista->marca = 1; //Marcamos el adyacente actual con 1
            auxArista = auxArista->sig; //Pasamos al siguiente nodo adyacente
        }

        band2 = 1;
        while (band2 == 1) {
            //FUNCION AUXILIAR PARA PRIM
            PRIM_Aux(aux, auxArista);
            band2 = 0;
            aux = head; //Apuntando al inicio de los nodos
            while (aux != NULL) {
                if (aux->marca == 0) { //Si en el nodo actual, la marca es cero...
                    band2 = 1; //bandera 2 se activa, rompiendo el ciclo y conservando el nodo actual
                }
                aux = aux->sig; //Pasamos al siguiente nodo
            }
        }
        //LISTA DE ADYACENCIA PARA PRIM
        adyacenciaPRIM(auxArista);
        //PESO TOTAL DEL CAMINO
        pesoFinal(auxArista);
    }
}


// - - > UPDATE
// Función que resetea los valores de una arista de entrada.
void update(Nodo& aux, Arista& auxArista) {
    aux = head; //Apuntador al inicio de los nodos
    while (aux != NULL) { //Mientras no estemos en el ultimo...
        auxArista = aux->ady; //Guardamos los nodos adyacentes al Nodo principal en una variable auxiliar
        while (auxArista != NULL) { //Mientras no estemos en el ultimo nodito...
            auxArista->marca = 0; //Resetamos marca
            auxArista->suma = 0; //Resetamos suma
            auxArista = auxArista->sig; //Pasamos al siguiente nodito
        }
        aux->marca = 0; //Marcamos como 0 la marca del NODO actual
        aux = aux->sig; //Pasamos al siguiente NODO
    }
}

// - - > OBTENER MENOR ARCO
//Funcion que guarda el arco con menor peso de una lista de noditos adyacentes a un NODO de entrada
//      auxArista sera la arista a evaluar y menorArco sera el registro de la arista que haya pasado por
//      la funcion y haya tenido menor peso que sus predecesoras,
void getMenorArco(Arista& auxArista, Arista& menorArco) {
    Nodo aux2 = head; //Apuntador al inicio de los nodos
    menorArco = NULL;
    while (aux2 != NULL) { //Mientras no estemos en el NODO final
        if (aux2->marca == 1) { //Si la marca del nodo actual es 1...
            auxArista = aux2->ady; //Guardamos los noditos adyacentes en otra variable auxiliar
            while (auxArista != NULL) { //Mientras no estemos en el NODITO final
                if (auxArista->marca == 1) { //Si la marca del NODITO actual es 1...
                    if (menorArco == NULL) { //Si es el primer arco...
                        menorArco = auxArista; //La primer arista a evaluar sera la primera
                    }
                    else if (auxArista->peso < menorArco->peso) { //Si la  arista pesa menos que el arco menor...
                        menorArco = auxArista; //el menor arco actualiza su información a un nuevo menor arco
                    }
                }
                auxArista = auxArista->sig; //Pasamos al siguiente NODITO
            }
        }
        aux2 = aux2->sig; //Pasamos al siguiente NODO
    }
    menorArco->marca = 3; //actualizamos la marca (ORIGEN)
    menorArco->destino->marca = 1;//actualizamos la marca (DESTINO)
}

// - - > AUX PARA PRIM
void PRIM_Aux(Nodo& aux, Arista& auxArista) {
    Nodo aux2;
    Arista menorArco = NULL;
    Arista auxArista2;
    //FUNCION PARA OBTENER EL MENOR ARCO
    getMenorArco(auxArista, menorArco);
    //Mostramos los valores del arco
    cout << " ARCO MENOR: " << menorArco->destino->info << endl;
    cout << " VALOR: " << menorArco->peso;
    pause = cin.get(); //Pausa
    pause = cin.get(); //Pausa
    aux = menorArco->destino; //Guardamos el nodo de conexion final del arco menor
    auxArista = aux->ady; //Guardamos los nodos adyacentes (noditos) en una variable auxilar
    while (auxArista != NULL) { //Mientras no estemos en la ultima arista
        if (auxArista->destino->marca == 0) {//Si la marca del nodito de destino es 0...
            aux2 = head; //Apuntador al inicio de los nodos
            while (aux2 != NULL) { //Mientras no estemos en el ultimo nodo
                if (aux2 != aux && aux2->marca == 1) { //Si el nodo actual es distinto al nodo con peso menor y su marca sea igual a 1
                    auxArista2 = aux2->ady; //En otra variable auxiliar guardamos los adyacentes del NODO de entrada
                    while (auxArista2 != NULL) { //Mientras no estemos en la ultima arista
                        if (auxArista->destino->info == auxArista2->destino->info) { //Comparamos las 2 variables auxiliares para los arcos (NODITOS)
                            if (auxArista->peso < auxArista2->peso) { //Si el peso de menor arco es menor al peso del arco actual...
                                //Ajustamos las marcas
                                auxArista2->marca = 2;
                                auxArista->marca = 1;
                            }
                            else {//Si el peso de menor arco es menor al peso del arco actual...
                                //Ajustamos las marcas
                                auxArista->marca = 2;
                                auxArista2->marca = 1;
                            }
                        }
                        auxArista2 = auxArista2->sig; //Pasamos al siguiente nodo adyacente (NODITO)

                    }
                }
                aux2 = aux2->sig; //Pasamos al siguiente NODO de la lista
            }
            //Reajustamos las marcas del auxiliar
            if (auxArista->marca == 0) {
                auxArista->marca = 1; //Marcamos con una vuelta al NODITO
            }
        }
        else {
            auxArista->marca = 2; //Marca de segunda vuelta para NODITO
        }
        auxArista = auxArista->sig; //Pasamos al siguiente nodo adyacente del ARCO MENOR
    }
}

// - - > IMPRIMIR LISTA DE ADYACENCIA
//Imprime la lista de adyacencia final despues de ejecutarse el algoritmo de PRIM
//Recibe como parametro la arista que almacena todas las adyacencias
void adyacenciaPRIM(Arista& auxArista) {
    Nodo aux; 
    if (head != NULL) { //Si el apuntador tiene información...
        aux = head; //Apuntador al inicio de los nodos
        cout << "\nNODO | LISTA DE ADYACENCIA\n";
        while (aux != NULL) { //Pasamos NODO por NODO
            auxArista = aux->ady; //Guardamos los noditos adyacentes en una variable auxiliar
            cout << "\t" << aux->info << " | "; //Mostramos el NODO actual
            while (auxArista != NULL) { //Mientras no estemos en el ultimo NODITO
                if (auxArista->marca == 3) { //Si la marca es 3 esta listo para ser mostrado
                    cout << auxArista->destino->info << " "; //mostramos el NODITO
                }
                auxArista = auxArista->sig; //Pasamos al siguiente NODITO
            }
            cout << endl;
            aux = aux->sig; //Pasamos al siguiente NODO
        }
    }
}

// - - > MOSTRAR PESO FINAL
//Funcion para mostrar el peso final segun el camino indicado por el algoritmo PRIM
void pesoFinal(Arista& auxArista) {
    Nodo aux;
    aux = head; //Apuntador al inicio de los nodos
    int suma = 0; //Variable donde se guardara la suma final
    while (aux != NULL) { //mientras no estemos en el ultimo nodo
        auxArista = aux->ady; //Guardamos todos los NODITOS adyacentes
        while (auxArista != NULL) { //Mientras no estemos en el ultimo NODITO
            if (auxArista->marca == 3) { //Si la marca es 3 esta listo para ser sumado
                suma = suma + auxArista->peso; //sumamos el peso de la arista a evaluar
            }
            auxArista = auxArista->sig; //Pasamos al siguiente NODITO
        }
        aux = aux->sig; //Pasamos al siguiente NODO
    }
    //Mostramos la suma final
    cout << "\n SUMA FINAL: " << suma;
}

// ░█▀▀▄ ▀█▀ ───░█ ░█─▄▀ ░█▀▀▀█ ▀▀█▀▀ ░█▀▀█ ─█▀▀█
// ░█─░█ ░█─ ─▄─░█ ░█▀▄─ ─▀▀▀▄▄ ─░█── ░█▄▄▀ ░█▄▄█
// ░█▄▄▀ ▄█▄ ░█▄▄█ ░█─░█ ░█▄▄▄█ ─░█── ░█─░█ ░█─░█
void dijkstra() {

    cout << "\n\tAqui va el algoritmo de Dijkstra..." << endl;
}


// ░█▀▀▀█ ▀▀█▀▀ ░█▀▀█ ░█▀▀▀█ ░█▀▀▀█
// ░█──░█ ─░█── ░█▄▄▀ ░█──░█ ─▀▀▀▄▄
// ░█▄▄▄█ ─░█── ░█─░█ ░█▄▄▄█ ░█▄▄▄█

// - - > INSERTAR NUEVO NODO
void insertarNodo() {
    Nodo aux, new_nodo = new struct nodo;
    //Definiciones para el nuevo nodo
    cout << "\n VALOR DEL NODO: ";
    cin >> new_nodo->info;
    new_nodo->sig = NULL;
    new_nodo->ady = NULL;
    new_nodo->visitado = false;

    //Si la lista esta vacia...
    if (head == NULL) {
        head = new_nodo; //PRIMER NODO EN EL PUNTERO DE CABEZA
        //Con este nodo podemos recorrer todos los siguientes
        //nodos hasta llegar a NULL (osea el ultimo)
    }
    else {
        //Recorremos todos los nodos hasta llegar al final
        aux = head; //Reestablecemos el puntero al incio de la multilista
        while (aux->sig != NULL) { //Mientras el siguiente nodo no sea NULL...
            aux = aux->sig; //Pasamos al siguiente nodo
        }
        //Cuando el siguiente sea NULL...
        aux->sig = new_nodo; //Insertamos el nuevo nodo
    }
    totalnodos++; //Aumenta contador de nodos totales
}

// - - > MOSTRAR LISTA DE LISTAS
void showMultilista() {
    Nodo ndo;
    Arista ndito;
    ndo = head;
    //Verificar lista vacia
    if (head == NULL) {
        cout << "\n [!] LA LISTA ESTA VACIA [!]";
        pause = cin.get(); //Pausa
        return;
    }
    //Mostrar tabla
    cout << "\n\tNODO | LISTA DE ADYACENCIA\n";
    while (ndo != NULL) {
        cout << "\t   " << ndo->info << " | ";
        if (ndo->ady != NULL) {
            ndito = ndo->ady;
            while (ndito != NULL) {
                cout << " " << ndito->destino->info;
                ndito = ndito->sig; //Pasamos al siguiente nodito
            }
        }
        ndo = ndo->sig; //Pasamos al siguiente nodo
        cout << endl;
    }
    pause = cin.get(); //Pausa
}

// - - > Salir
void exit() {
    cout << "\tGRACIAS POR USAR NUESTROS SERVICIOS =)" << endl;
    cout << "\t *ASDASDASDAS* - BUAP / FCC";
}
