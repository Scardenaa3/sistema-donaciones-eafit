#include <stdio.h>
#include "heap.h"

void heap_inicializar(MaxHeap* h) {
    h->tamaño = 0;
}

static void intercambiar(SolicitudDespacho* a, SolicitudDespacho* b) {
    SolicitudDespacho temp = *a;
    *a = *b;
    *b = temp;
}

void heap_insertar(MaxHeap* h, MunicipioAfectado m, LoteAyuda l, float prioridad) {
    if (h->tamaño >= 1000) return;
    int i = h->tamaño;
    h->elementos[i].municipio = m;
    h->elementos[i].lote = l;
    h->elementos[i].prioridad_score = prioridad;
    h->tamaño++;

    while (i != 0 && h->elementos[(i - 1) / 2].prioridad_score < h->elementos[i].prioridad_score) {
        intercambiar(&h->elementos[(i - 1) / 2], &h->elementos[i]);
        i = (i - 1) / 2;
    }
}

static void heapify_down(MaxHeap* h, int i) {
    int mayor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if (izq < h->tamaño && h->elementos[izq].prioridad_score > h->elementos[mayor].prioridad_score)
        mayor = izq;
    if (der < h->tamaño && h->elementos[der].prioridad_score > h->elementos[mayor].prioridad_score)
        mayor = der;

    if (mayor != i) {
        intercambiar(&h->elementos[i], &h->elementos[mayor]);
        heapify_down(h, mayor);
    }
}

SolicitudDespacho heap_extraer_max(MaxHeap* h) {
    SolicitudDespacho vacio = {0};
    if (h->tamaño <= 0) return vacio;

    SolicitudDespacho raiz = h->elementos[0];
    h->elementos[0] = h->elementos[h->tamaño - 1];
    h->tamaño--;
    heapify_down(h, 0);

    return raiz;
}

int heap_esta_vacio(MaxHeap* h) {
    return h->tamaño == 0;
}
