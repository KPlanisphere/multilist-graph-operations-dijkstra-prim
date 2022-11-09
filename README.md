# Multilist Graph Operations with Dijkstra's and Prim's Algorithms

This project implements a multilayer graph structure in C++ with various operations to manipulate and traverse the graph. The program includes functions to insert nodes and edges, display the graph, perform breadth-first and depth-first searches, and implement Prim's and Dijkstra's algorithms. It also features a menu-driven interface for user interaction.

## Features

- **Insert Node**: Adds a new node to the graph.
- **Insert Edge**: Adds an edge between two nodes with a specified weight.
- **Display Graph**: Prints the adjacency list of the graph.
- **Breadth-First Search (BFS)**: Traverses the graph using BFS starting from a specified node.
- **Depth-First Search (DFS)**: Traverses the graph using DFS starting from a specified node.
- **Prim's Algorithm**: Implements Prim's algorithm to find the minimum spanning tree of the graph.
- **Dijkstra's Algorithm**: Implements Dijkstra's algorithm to find the shortest path from a source node.
- **Menu Interface**: Provides a user-friendly menu to perform the above operations.

## Usage

Compile the program using a C++ compiler and run the executable. You will be presented with a menu to choose from various operations.

### Example Commands

1. **Insert Node**: Prompts the user to enter a value for the new node.
2. **Insert Edge**: Prompts the user to enter the start node, end node, and weight for the edge.
3. **Display Graph**: Displays the adjacency list of the graph.
4. **Breadth-First Search**: Prompts the user to enter the starting node for BFS.
5. **Depth-First Search**: Prompts the user to enter the starting node for DFS.
6. **Prim's Algorithm**: Prompts the user to enter the starting node for Prim's algorithm.
7. **Dijkstra's Algorithm**: Executes Dijkstra's algorithm to find the shortest path.
8. **Exit**: Terminates the program.

## Code Snippets

### Insert Node
```cpp
void insertarNodo() {
    Nodo aux, new_nodo = new struct nodo;
    cout << "\n VALOR DEL NODO: ";
    cin >> new_nodo->info;
    new_nodo->sig = NULL;
    new_nodo->ady = NULL;
    new_nodo->visitado = false;

    if (head == NULL) {
        head = new_nodo;
    } else {
        aux = head;
        while (aux->sig != NULL) {
            aux = aux->sig;
        }
        aux->sig = new_nodo;
    }
    totalnodos++;
}
```

### Insert Edge

```cpp
void agregarArista(Nodo& aux, Nodo& aux2, Arista& nuevo, int peso) {
    Arista saveNoditos;
    if (aux->ady == NULL) {
        aux->ady = nuevo;
        nuevo->destino = aux2;
        nuevo->peso = peso;
    } else {
        saveNoditos = aux->ady;
        while (saveNoditos->sig != NULL) {
            saveNoditos = saveNoditos->sig;
        }
        nuevo->destino = aux2;
        nuevo->peso = peso;
        saveNoditos->sig = nuevo;
    }
    cout << "\n [!] UNION AGREGADA CORRECTAMENTE [!]";
}
```

### Dijkstra's Algorithm

```cpp
void dijkstra() {
    cout << "\n\tAqui va el algoritmo de Dijkstra..." << endl;
}
```

### Menu Interface

```cpp
void menu() {
    cout << "\n\t - - - - LISTA DE LISTAS - MULTILISTAS - - - -" << endl;
    cout << "\t\t\t   (+ PRIM + DIJKSTRA)\n" << endl;

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
```

## How to Run

1. Clone the repository:
    ```bash
    git clone https://github.com/KPlanisphere/multilist-graph-operations-dijkstra-prim.git
    ```
2. Navigate to the project directory:
    ```bash
    cd multilist-graph-operations-dijkstra-prim
    ```
3. Compile the code:
    ```bash
    g++ MLista_Dijkstra.cpp -o multilist_graph
    ```
4. Run the executable:
    ```bash
    ./multilist_graph
    ```
