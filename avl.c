#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

static int max(int a, int b) { return (a > b) ? a : b; }

static int altura(NodoAVL* n) {
    return n ? n->altura : 0;
}

static int obtener_balance(NodoAVL* n) {
    return n ? altura(n->izq) - altura(n->der) : 0;
}

static NodoAVL* nuevo_nodo(char* clave, LoteAyuda lote) {
    NodoAVL* nodo = (NodoAVL*)malloc(sizeof(NodoAVL));
    strcpy(nodo->clave, clave);
    nodo->lote = lote;
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->altura = 1;
    return nodo;
}

static NodoAVL* rotar_derecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    return x;
}

static NodoAVL* rotar_izquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    return y;
}

NodoAVL* avl_insertar(NodoAVL* raiz, char* clave, LoteAyuda lote) {
    if (!raiz) return nuevo_nodo(clave, lote);

    if (strcmp(clave, raiz->clave) < 0)
        raiz->izq = avl_insertar(raiz->izq, clave, lote);
    else
        raiz->der = avl_insertar(raiz->der, clave, lote);

    raiz->altura = 1 + max(altura(raiz->izq), altura(raiz->der));
    int balance = obtener_balance(raiz);

    if (balance > 1 && strcmp(clave, raiz->izq->clave) < 0)
        return rotar_derecha(raiz);
    if (balance < -1 && strcmp(clave, raiz->der->clave) > 0)
        return rotar_izquierda(raiz);
    if (balance > 1 && strcmp(clave, raiz->izq->clave) > 0) {
        raiz->izq = rotar_izquierda(raiz->izq);
        return rotar_derecha(raiz);
    }
    if (balance < -1 && strcmp(clave, raiz->der->clave) < 0) {
        raiz->der = rotar_derecha(raiz->der);
        return rotar_izquierda(raiz);
    }
    return raiz;
}

void avl_buscar_rango(NodoAVL* raiz, const char* min_clave, const char* max_clave, LoteAyuda* resultados, int* contador, int max_resultados) {
    if (!raiz || *contador >= max_resultados) return;

    if (strcmp(min_clave, raiz->clave) < 0)
        avl_buscar_rango(raiz->izq, min_clave, max_clave, resultados, contador, max_resultados);

    if (strcmp(min_clave, raiz->clave) <= 0 && strcmp(raiz->clave, max_clave) <= 0) {
        if (*contador < max_resultados) {
            resultados[*contador] = raiz->lote;
            (*contador)++;
        }
    }

    if (strcmp(max_clave, raiz->clave) > 0)
        avl_buscar_rango(raiz->der, min_clave, max_clave, resultados, contador, max_resultados);
}
