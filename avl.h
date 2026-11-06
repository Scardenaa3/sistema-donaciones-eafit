#ifndef AVL_H
#define AVL_H

#include "entidades.h"

typedef struct NodoAVL {
    char clave[MAX_STR];
    LoteAyuda lote;
    struct NodoAVL *izq;
    struct NodoAVL *der;
    int altura;
} NodoAVL;

NodoAVL* avl_insertar(NodoAVL* raiz, char* clave, LoteAyuda lote);
void avl_buscar_rango(NodoAVL* raiz, const char* min_clave, const char* max_clave, LoteAyuda* resultados, int* contador, int max_resultados);

#endif
