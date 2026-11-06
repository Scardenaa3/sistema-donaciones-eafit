#include <stdio.h>
#include <string.h>
#include "grafo.h"

void grafo_inicializar(GrafoRedTransporte* g) {
    g->num_vertices = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            g->matriz_adyacencia[i][j] = (i == j) ? 0.0 : INF;
        }
    }
}

int grafo_agregar_vertice(GrafoRedTransporte* g, const char* nombre) {
    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->nombres[i], nombre) == 0) return i;
    }
    if (g->num_vertices < MAX_VERTICES) {
        strcpy(g->nombres[g->num_vertices], nombre);
        g->num_vertices++;
        return g->num_vertices - 1;
    }
    return -1;
}

void grafo_agregar_arista(GrafoRedTransporte* g, const char* origen, const char* destino, float distancia_km, float factor_transitabilidad) {
    int u = grafo_agregar_vertice(g, origen);
    int v = grafo_agregar_vertice(g, destino);
    if (u != -1 && v != -1) {
        float peso_efectivo = distancia_km * factor_transitabilidad;
        g->matriz_adyacencia[u][v] = peso_efectivo;
        g->matriz_adyacencia[v][u] = peso_efectivo;
    }
}

void grafo_dijkstra(GrafoRedTransporte* g, const char* origen, const char* destino, char ruta[MAX_VERTICES][MAX_STR], int* pasos_ruta, float* costo_total) {
    int src = -1, dest = -1;
    for (int i = 0; i < g->num_vertices; i++) {
        if (strcmp(g->nombres[i], origen) == 0) src = i;
        if (strcmp(g->nombres[i], destino) == 0) dest = i;
    }

    if (src == -1 || dest == -1) {
        *costo_total = INF;
        *pasos_ruta = 0;
        return;
    }

    float dist[MAX_VERTICES];
    int visitado[MAX_VERTICES] = {0};
    int previo[MAX_VERTICES];

    for (int i = 0; i < g->num_vertices; i++) {
        dist[i] = INF;
        previo[i] = -1;
    }
    dist[src] = 0;

    for (int count = 0; count < g->num_vertices - 1; count++) {
        float min_val = INF;
        int u = -1;
        for (int i = 0; i < g->num_vertices; i++) {
            if (!visitado[i] && dist[i] < min_val) {
                min_val = dist[i];
                u = i;
            }
        }

        if (u == -1 || u == dest) break;
        visitado[u] = 1;

        for (int v = 0; v < g->num_vertices; v++) {
            if (!visitado[v] && g->matriz_adyacencia[u][v] != INF && dist[u] + g->matriz_adyacencia[u][v] < dist[v]) {
                dist[v] = dist[u] + g->matriz_adyacencia[u][v];
                previo[v] = u;
            }
        }
    }

    *costo_total = dist[dest];
    if (*costo_total >= INF) {
        *pasos_ruta = 0;
        return;
    }

    int curr = dest;
    int temp_idx = 0;
    int temp_camino[MAX_VERTICES];

    while (curr != -1) {
        temp_camino[temp_idx++] = curr;
        curr = previo[curr];
    }

    *pasos_ruta = 0;
    for (int i = temp_idx - 1; i >= 0; i--) {
        strcpy(ruta[(*pasos_ruta)++], g->nombres[temp_camino[i]]);
    }
}
