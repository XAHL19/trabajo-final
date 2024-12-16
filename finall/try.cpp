#include <iostream>
#include <cstdlib>
#include <algorithm> // Para std::max

using namespace std;

struct nodo {
    nodo *izq;
    int valor;
    int altura;
    nodo *der;
};

// Raíz del árbol AVL
nodo *raiz = nullptr;

// Función para obtener la altura de un nodo
int altura(nodo *n) {
    return (n == nullptr) ? 0 : n->altura;
}

// Función para crear un nuevo nodo
nodo* nuevoNodo(int valor) {
    nodo* nuevo = (nodo*)malloc(sizeof(nodo));
    nuevo->valor = valor;
    nuevo->izq = nuevo->der = nullptr;
    nuevo->altura = 1;
    return nuevo;
}

// Función para calcular el factor de equilibrio
int obtenerBalance(nodo *n) {
    return (n == nullptr) ? 0 : altura(n->izq) - altura(n->der);
}

// Rotación simple a la derecha
nodo* rotacionDerecha(nodo* y) {
    nodo* x = y->izq;
    nodo* T2 = x->der;

    // Rotación
    x->der = y;
    y->izq = T2;

    // Actualizar alturas
    y->altura = 1 + max(altura(y->izq), altura(y->der));
    x->altura = 1 + max(altura(x->izq), altura(x->der));

    cout << "Rotación simple a la derecha realizada." << endl;
    return x;
}

// Rotación simple a la izquierda
nodo* rotacionIzquierda(nodo* x) {
    nodo* y = x->der;
    nodo* T2 = y->izq;

    // Rotación
    y->izq = x;
    x->der = T2;

    // Actualizar alturas
    x->altura = 1 + max(altura(x->izq), altura(x->der));
    y->altura = 1 + max(altura(y->izq), altura(y->der));

    cout << "Rotación simple a la izquierda realizada." << endl;
    return y;
}

// Función para insertar un nodo en el árbol AVL
nodo* insertarNodo(nodo* raiz, int valor) {
    // Caso base: si el árbol está vacío
    if (raiz == nullptr)
        return nuevoNodo(valor);

    // Insertar en el subárbol izquierdo o derecho
    if (valor < raiz->valor)
        raiz->izq = insertarNodo(raiz->izq, valor);
    else if (valor > raiz->valor)
        raiz->der = insertarNodo(raiz->der, valor);
    else {
        cout << "El valor ya existe en el árbol." << endl;
        return raiz; // No se permiten duplicados
    }

    // Actualizar altura del nodo actual
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    // Obtener el factor de equilibrio
    int balance = obtenerBalance(raiz);

    // Casos de desbalance

    // Caso Izquierda-Izquierda
    if (balance > 1 && valor < raiz->izq->valor)
        return rotacionDerecha(raiz);

    // Caso Derecha-Derecha
    if (balance < -1 && valor > raiz->der->valor)
        return rotacionIzquierda(raiz);

    // Caso Izquierda-Derecha
    if (balance > 1 && valor > raiz->izq->valor) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }

    // Caso Derecha-Izquierda
    if (balance < -1 && valor < raiz->der->valor) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

// Función para encontrar el nodo con el valor mínimo en un subárbol
nodo* nodoMinimo(nodo* nodo) {
    while (nodo->izq != nullptr)
        nodo = nodo->izq;
    return nodo;
}

// Función para eliminar un nodo del árbol AVL
nodo* eliminarNodo(nodo* raiz, int valor) {
    if (raiz == nullptr) {
        cout << "El nodo no existe en el árbol." << endl;
        return raiz;
    }

    // Buscar el nodo a eliminar
    if (valor < raiz->valor)
        raiz->izq = eliminarNodo(raiz->izq, valor);
    else if (valor > raiz->valor)
        raiz->der = eliminarNodo(raiz->der, valor);
    else {
        // Nodo encontrado, manejar los casos de eliminación

        // Nodo con un solo hijo o sin hijos
        if (raiz->izq == nullptr || raiz->der == nullptr) {
            nodo* temp = (raiz->izq != nullptr) ? raiz->izq : raiz->der;
            free(raiz);
            return temp;
        }

        // Nodo con dos hijos: obtener el sucesor in-order (el más pequeño del subárbol derecho)
        nodo* temp = nodoMinimo(raiz->der);
        raiz->valor = temp->valor;
        raiz->der = eliminarNodo(raiz->der, temp->valor);
    }

    // Actualizar altura del nodo actual
    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));

    // Verificar el balance y reequilibrar si es necesario
    int balance = obtenerBalance(raiz);

    // Casos de desbalance

    // Izquierda-Izquierda
    if (balance > 1 && obtenerBalance(raiz->izq) >= 0)
        return rotacionDerecha(raiz);

    // Izquierda-Derecha
    if (balance > 1 && obtenerBalance(raiz->izq) < 0) {
        raiz->izq = rotacionIzquierda(raiz->izq);
        return rotacionDerecha(raiz);
    }

    // Derecha-Derecha
    if (balance < -1 && obtenerBalance(raiz->der) <= 0)
        return rotacionIzquierda(raiz);

    // Derecha-Izquierda
    if (balance < -1 && obtenerBalance(raiz->der) > 0) {
        raiz->der = rotacionDerecha(raiz->der);
        return rotacionIzquierda(raiz);
    }

    return raiz;
}

// Función para recorrer el árbol en orden y mostrar los nodos
void recorrer(nodo* a) {
    if (a == nullptr) return;

    recorrer(a->izq);
    cout << "Valor: " << a->valor << ", Altura: " << a->altura << endl;
    recorrer(a->der);
}

// Funciones para el menú
void insertar() {
    int valor;
    cout << "Digite el valor del nodo: ";
    cin >> valor;
    raiz = insertarNodo(raiz, valor);
}

void eliminar() {
    int valor;
    cout << "Digite el valor del nodo a eliminar: ";
    cin >> valor;
    raiz = eliminarNodo(raiz, valor);
}

void mostrar() {
    if (raiz)
        recorrer(raiz);
    else
        cout << "El árbol está vacío." << endl;
}

int main() {
    int opcion;
    do {
        cout << "\nImplementación de árbol AVL" << endl;
        cout << "1. Insertar" << endl;
        cout << "2. Mostrar" << endl;
        cout << "3. Eliminar" << endl;
        cout << "4. Salir" << endl;
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1: insertar(); break;
            case 2: mostrar(); break;
            case 3: eliminar(); break;
            case 4: cout << "Saliendo del programa." << endl; break;
            default: cout << "Opción no válida. Intente de nuevo." << endl;
        }
    } while (opcion != 4);

    return 0;
}