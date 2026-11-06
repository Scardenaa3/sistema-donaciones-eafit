#ifndef SISTEMA_H
#define SISTEMA_H

#include "entidades.h"
#include "avl.h"
#include "heap.h"
#include "grafo.h"

typedef struct {
    Donante donantes[100];
    int num_donantes;
    LoteAyuda lotes[500];
    int num_lotes;
    CentroAcopio centros[20];
    int num_centros;
    MunicipioAfectado municipios[50];
    int num_municipios;

    NodoAVL* avl_raiz;
    GrafoRedTransporte red_vial;
} SistemaGestion;

void sistema_inicializar(SistemaGestion* s);
void sistema_cargar_datos_prueba(SistemaGestion* s);
void sistema_registrar_lote(SistemaGestion* s, LoteAyuda l);
void sistema_procesar_triaje_y_despacho(SistemaGestion* s);
void sistema_mostrar_red_vial(SistemaGestion* s);
void sistema_mostrar_inventario(SistemaGestion* s);
void sistema_alertas_desabastecimiento(SistemaGestion* s);

#endif
// Declaraciones de funciones faltantes para completar la rúbrica EAFIT
void sistema_buscar_lotes_avl_rango(SistemaGestion* s);
void sistema_registrar_lote_interactivo(SistemaGestion* s);
void sistema_cargar_inventario_csv(SistemaGestion* s, const char* nombre_archivo);
void sistema_guardar_inventario_csv(SistemaGestion* s, const char* nombre_archivo);
void sistema_ejecutar_benchmark_rendimiento(SistemaGestion* s);
