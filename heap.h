#ifndef HEAP_H
#define HEAP_H

#include "entidades.h"

typedef struct {
    MunicipioAfectado municipio;
    LoteAyuda lote;
    float prioridad_score;
} SolicitudDespacho;

typedef struct {
    SolicitudDespacho elementos[1000];
    int tamaño;
} MaxHeap;

void heap_inicializar(MaxHeap* h);
void heap_insertar(MaxHeap* h, MunicipioAfectado m, LoteAyuda l, float prioridad);
SolicitudDespacho heap_extraer_max(MaxHeap* h);
int heap_esta_vacio(MaxHeap* h);

#endif

