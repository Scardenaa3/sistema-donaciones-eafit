#ifndef GRAFO_H
#define GRAFO_H

#define MAX_VERTICES 50
#define MAX_STR 100
#define INF 1e9

typedef struct {
    char nombres[MAX_VERTICES][MAX_STR];
    float matriz_adyacencia[MAX_VERTICES][MAX_VERTICES];
    int num_vertices;
} GrafoRedTransporte;

void grafo_inicializar(GrafoRedTransporte* g);
int grafo_agregar_vertice(GrafoRedTransporte* g, const char* nombre);
void grafo_agregar_arista(GrafoRedTransporte* g, const char* origen, const char* destino, float distancia_km, float factor_transitabilidad);
void grafo_dijkstra(GrafoRedTransporte* g, const char* origen, const char* destino, char ruta[MAX_VERTICES][MAX_STR], int* pasos_ruta, float* costo_total);

#endif
