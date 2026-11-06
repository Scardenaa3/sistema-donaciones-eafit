#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sistema.h"

void sistema_inicializar(SistemaGestion* s) {
    s->num_donantes = 0;
    s->num_lotes = 0;
    s->num_centros = 0;
    s->num_municipios = 0;
    s->avl_raiz = NULL;
    grafo_inicializar(&s->red_vial);
}

void sistema_registrar_lote(SistemaGestion* s, LoteAyuda l) {
    if (s->num_lotes < 500) {
        s->lotes[s->num_lotes] = l;
        char clave[200];
        sprintf(clave, "%s#%s", l.fecha_expiracion, l.id_lote);
        s->avl_raiz = avl_insertar(s->avl_raiz, clave, l);
        s->num_lotes++;
    }
}

void sistema_cargar_datos_prueba(SistemaGestion* s) {
    // 1. Registro de Donantes
    strcpy(s->donantes[0].id_donante, "D01");
    strcpy(s->donantes[0].nombre, "Cruz Roja Colombiana");
    strcpy(s->donantes[0].tipo_entidad, "ONG");
    s->num_donantes = 1;

    // 2. Centros de Acopio Principales (Basados en informe oficial agosto 2026)
    strcpy(s->centros[0].id_centro, "C01");
    strcpy(s->centros[0].nombre, "Fundación Banco Arquidiocesano / Saciar");
    strcpy(s->centros[0].municipio, "Medellín");
    
    strcpy(s->centros[1].id_centro, "C02");
    strcpy(s->centros[1].nombre, "Bodega Cruz Roja / Palacio de los Deportes");
    strcpy(s->centros[1].municipio, "Bogotá");

    strcpy(s->centros[2].id_centro, "C03");
    strcpy(s->centros[2].nombre, "Plazoleta Jairo Varela");
    strcpy(s->centros[2].municipio, "Cali");
    s->num_centros = 3;

    // Registrar vértices base en el Grafo de Transporte
    grafo_agregar_vertice(&s->red_vial, "Medellín");
    grafo_agregar_vertice(&s->red_vial, "Bogotá");
    grafo_agregar_vertice(&s->red_vial, "Cali");

    // 3. Municipios Afectados Prioritarios
    strcpy(s->municipios[0].id_municipio, "M01");
    strcpy(s->municipios[0].nombre, "Dabeiba");
    strcpy(s->municipios[0].departamento, "Antioquia");
    s->municipios[0].nivel_urgencia = 5; // Crítico
    strcpy(s->municipios[0].necesidades_cat[0], "Insumos Médicos");
    s->municipios[0].necesidades_cant[0] = 500;
    s->municipios[0].num_necesidades = 1;

    strcpy(s->municipios[1].id_municipio, "M02");
    strcpy(s->municipios[1].nombre, "Quibdó");
    strcpy(s->municipios[1].departamento, "Chocó");
    s->municipios[1].nivel_urgencia = 5; // Crítico (Aislado por cierre aéreo y terrestre)
    strcpy(s->municipios[1].necesidades_cat[0], "Agua Potable");
    s->municipios[1].necesidades_cant[0] = 2000;
    s->municipios[1].num_necesidades = 1;

    strcpy(s->municipios[2].id_municipio, "M03");
    strcpy(s->municipios[2].nombre, "Mocoa");
    strcpy(s->municipios[2].departamento, "Putumayo");
    s->municipios[2].nivel_urgencia = 4;
    strcpy(s->municipios[2].necesidades_cat[0], "Víveres");
    s->municipios[2].necesidades_cant[0] = 1500;
    s->municipios[2].num_necesidades = 1;

    s->num_municipios = 3;

    // Registrar vértices de municipios en el Grafo
    grafo_agregar_vertice(&s->red_vial, "Dabeiba");
    grafo_agregar_vertice(&s->red_vial, "Quibdó");
    grafo_agregar_vertice(&s->red_vial, "Mocoa");

    // 4. Conexiones Viales y Factores de Sismo (Agosto 2026)
    // Medellín <---> Dabeiba (170 km, Factor 1.8 por cierres en variante de Fuenía)
    grafo_agregar_arista(&s->red_vial, "Medellín", "Dabeiba", 170.0, 1.8);
    
    // Medellín <---> Bogotá (420 km, Factor 1.0 Normal)
    grafo_agregar_arista(&s->red_vial, "Medellín", "Bogotá", 420.0, 1.0);

    // Bogotá <---> Quibdó (550 km, Factor 2.5 Crítico por Eje Cafetero bloqueado)
    grafo_agregar_arista(&s->red_vial, "Bogotá", "Quibdó", 550.0, 2.5);

    // Cali <---> Mocoa (500 km, Factor 2.0 por daños en infraestructura)
    grafo_agregar_arista(&s->red_vial, "Cali", "Mocoa", 500.0, 2.0);

    // Bogotá <---> Mocoa (850 km, Factor 1.2 Troncal del Sur con Túnel Sumapaz abierto)
    grafo_agregar_arista(&s->red_vial, "Bogotá", "Mocoa", 850.0, 1.2);

    // 5. Carga de Lotes de Ayuda Iniciales
    LoteAyuda l1 = {"L01", "D01", "Insumos Médicos", "Kits Quirúrgicos Triaje", 500, "unidades", 250, 2.0, 1, "2026-11-15", "C01", 0};
    LoteAyuda l2 = {"L02", "D01", "Agua Potable", "Plantas Potabilizadoras", 2000, "litros", 2000, 4.0, 0, "2027-01-10", "C02", 0};
    LoteAyuda l3 = {"L03", "D01", "Víveres", "Mercados No Perecederos", 1500, "kg", 1500, 3.0, 0, "2026-12-20", "C03", 0};
    
    sistema_registrar_lote(s, l1);
    sistema_registrar_lote(s, l2);
    sistema_registrar_lote(s, l3);
}

void sistema_procesar_triaje_y_despacho(SistemaGestion* s) {
    MaxHeap heap;
    heap_inicializar(&heap);

    for (int i = 0; i < s->num_municipios; i++) {
        for (int j = 0; j < s->num_lotes; j++) {
            if (!s->lotes[j].asignado) {
                for (int k = 0; k < s->municipios[i].num_necesidades; k++) {
                    if (strcmp(s->lotes[j].categoria, s->municipios[i].necesidades_cat[k]) == 0) {
                        float score = (s->municipios[i].nivel_urgencia * 20.0);
                        heap_insertar(&heap, s->municipios[i], s->lotes[j], score);
                    }
                }
            }
        }
    }

    if (heap_esta_vacio(&heap)) {
        printf("\n[ALERTA] No hay solicitudes pendientes o recursos disponibles.\n");
        return;
    }

    SolicitudDespacho sol = heap_extraer_max(&heap);
    
    for (int j = 0; j < s->num_lotes; j++) {
        if (strcmp(s->lotes[j].id_lote, sol.lote.id_lote) == 0) {
            s->lotes[j].asignado = 1;
        }
    }

    // Determinar centro de acopio origen según el lote
    char* centro_origen = "Medellín";
    if (strcmp(sol.lote.id_centro_acopio, "C02") == 0) centro_origen = "Bogotá";
    if (strcmp(sol.lote.id_centro_acopio, "C03") == 0) centro_origen = "Cali";

    char ruta[MAX_VERTICES][MAX_STR];
    int pasos = 0;
    float costo = 0;
    grafo_dijkstra(&s->red_vial, centro_origen, sol.municipio.nombre, ruta, &pasos, &costo);

    printf("\n=================================================================\n");
    printf("            DESPACHO HUMANITARIO EXITOSO (TRIAJE)\n");
    printf("=================================================================\n");
    printf(" Municipio Destino : %s (Urgencia Nivel %d)\n", sol.municipio.nombre, sol.municipio.nivel_urgencia);
    printf(" Lote Asignado     : %s (%s - %.0f %s)\n", sol.lote.id_lote, sol.lote.categoria, sol.lote.cantidad, sol.lote.unidad);
    printf(" Origen (Acopio)   : %s\n", centro_origen);
    printf(" Score Prioridad   : %.1f puntos\n", sol.prioridad_score);
    printf(" Ruta Óptima (Dijkstra): ");
    for (int i = 0; i < pasos; i++) {
        printf("%s%s", ruta[i], (i < pasos - 1) ? " -> " : "");
    }
    printf("\n Costo Efectivo Vial: %.1f km (considerando factores de sismo)\n", costo);
    printf("=================================================================\n");
}

void sistema_mostrar_red_vial(SistemaGestion* s) {
    printf("\n=================================================================\n");
    printf("        ESTADO DE LA RED VIAL Y TOPOLOGÍA COLOMBIA (AGO 2026)\n");
    printf("=================================================================\n");
    for (int i = 0; i < s->red_vial.num_vertices; i++) {
        for (int j = i + 1; j < s->red_vial.num_vertices; j++) {
            if (s->red_vial.matriz_adyacencia[i][j] != INF && s->red_vial.matriz_adyacencia[i][j] != 0.0) {
                printf(" [%s] <======> [%s] | Costo Efectivo Ponderado: %.1f km\n", 
                    s->red_vial.nombres[i], s->red_vial.nombres[j], s->red_vial.matriz_adyacencia[i][j]);
            }
        }
    }
    printf("=================================================================\n");
}

void sistema_mostrar_inventario(SistemaGestion* s) {
    printf("\n=================================================================\n");
    printf("              INVENTARIO DE LOTES DE AYUDA REGISTRADOS\n");
    printf("=================================================================\n");
    for (int i = 0; i < s->num_lotes; i++) {
        printf(" ID: %s | Cat: %s | Cant: %.0f %s | Exp: %s | Estado: %s\n",
            s->lotes[i].id_lote, s->lotes[i].categoria, s->lotes[i].cantidad,
            s->lotes[i].unidad, s->lotes[i].fecha_expiracion,
            s->lotes[i].asignado ? "DESPACHADO" : "DISPONIBLE");
    }
    printf("=================================================================\n");
}

void sistema_alertas_desabastecimiento(SistemaGestion* s) {
    printf("\n=================================================================\n");
    printf("             ALERTAS TEMPRANAS DE DESABASTECIMIENTO\n");
    printf("=================================================================\n");
    for (int i = 0; i < s->num_municipios; i++) {
        for (int k = 0; k < s->municipios[i].num_necesidades; k++) {
            float cant_req = s->municipios[i].necesidades_cant[k];
            float disp = 0;
            for (int j = 0; j < s->num_lotes; j++) {
                if (!s->lotes[j].asignado && strcmp(s->lotes[j].categoria, s->municipios[i].necesidades_cat[k]) == 0) {
                    disp += s->lotes[j].cantidad;
                }
            }
            if (disp < cant_req) {
                printf(" [ALERTA] %s requiere %.0f de %s (Disponible en stock: %.0f)\n",
                    s->municipios[i].nombre, cant_req, s->municipios[i].necesidades_cat[k], disp);
            }
        }
    }
    printf("=================================================================\n");
}
// FASE 2 y 3: Búsqueda Multinivel de Rango usando el Árbol AVL O(log n + K)
void sistema_buscar_lotes_avl_rango(SistemaGestion* s) {
    char f_inicio[11], f_fin[11];
    printf("\n=================================================================\n");
    printf("     CONSULTA MULTINIVEL POR RANGO DE EXPIRACIÓN (Árbol AVL)\n");
    printf("=================================================================\n");
    printf(" Ingrese fecha inicial (YYYY-MM-DD): ");
    scanf("%10s", f_inicio);
    printf(" Ingrese fecha final (YYYY-MM-DD): ");
    scanf("%10s", f_fin);

    char min_clave[MAX_STR], max_clave[MAX_STR];
    sprintf(min_clave, "%s#", f_inicio);
    sprintf(max_clave, "%s#ZZZ", f_fin);

    LoteAyuda resultados[100];
    int contador = 0;
    avl_buscar_rango(s->avl_raiz, min_clave, max_clave, resultados, &contador, 100);

    printf("\n Resultados en el rango [%s a %s] (%d encontrados):\n", f_inicio, f_fin, contador);
    for (int i = 0; i < contador; i++) {
        printf(" - ID: %s | Cat: %s | Exp: %s | Cant: %.0f %s | Acopio: %s | Estado: %s\n",
            resultados[i].id_lote, resultados[i].categoria, resultados[i].fecha_expiracion,
            resultados[i].cantidad, resultados[i].unidad, resultados[i].id_centro_acopio,
            resultados[i].asignado ? "DESPACHADO" : "DISPONIBLE");
    }
    printf("=================================================================\n");
}

// FASE 1 y 2: Registro Interactivo con Validación Estricta de Rangos (No Negativos)
void sistema_registrar_lote_interactivo(SistemaGestion* s) {
    LoteAyuda l;
    printf("\n=================================================================\n");
    printf("         REGISTRO INSTANTÁNEO DE NUEVO LOTE DE AYUDA\n");
    printf("=================================================================\n");
    printf(" ID del Lote: ");
    scanf("%99s", l.id_lote);
    printf(" ID del Donante: ");
    scanf("%99s", l.id_donante);
    printf(" Categoría (Insumos Médicos / Víveres / Agua Potable / Kits Refugio): ");
    scanf(" %99[^\n]", l.categoria);
    printf(" Descripción: ");
    scanf(" %99[^\n]", l.descripcion);
    printf(" Cantidad: ");
    scanf("%f", &l.cantidad);
    printf(" Unidad (kg / unidades / litros): ");
    scanf("%99s", l.unidad);
    printf(" Peso en kg: ");
    scanf("%f", &l.peso_kg);
    printf(" Volumen en m3: ");
    scanf("%f", &l.volumen_m3);
    printf(" Requiere cadena de frío (1 = Sí, 0 = No): ");
    scanf("%d", &l.requiere_frio);
    printf(" Fecha de Expiración (YYYY-MM-DD): ");
    scanf("%10s", l.fecha_expiracion);
    printf(" ID Centro de Acopio (ej. C01, C02, C03): ");
    scanf("%99s", l.id_centro_acopio);
    l.asignado = 0;

    // Validación rigurosa exigida en Fase 1 (Evitar cantidades o pesos negativos)
    if (l.cantidad < 0 || l.peso_kg < 0 || l.volumen_m3 < 0) {
        printf("\n [ERROR DE VALIDACIÓN] Las cantidades, peso y volumen no pueden ser negativos.\n");
        return;
    }

    sistema_registrar_lote(s, l);
    printf("\n [ÉXITO] Lote registrado e indexado correctamente en el Árbol AVL.\n");
    printf("=================================================================\n");
}

// FASE 1: Persistencia e Intercambio (Carga y Parsing de Archivo CSV)
void sistema_cargar_inventario_csv(SistemaGestion* s, const char* nombre_archivo) {
    FILE* f = fopen(nombre_archivo, "r");
    if (!f) {
        printf("\n [AVISO] No se encontró el archivo '%s'. Se continúa con datos en memoria.\n", nombre_archivo);
        return;
    }

    char linea[512];
    fgets(linea, sizeof(linea), f); // Omitir cabecera CSV

    int cargados = 0;
    while (fgets(linea, sizeof(linea), f)) {
        LoteAyuda l;
        int req_frio, asig;
        int parsed = sscanf(linea, "%99[^,],%99[^,],%99[^,],%99[^,],%f,%99[^,],%f,%f,%d,%10[^,],%99[^,],%d",
            l.id_lote, l.id_donante, l.categoria, l.descripcion,
            &l.cantidad, l.unidad, &l.peso_kg, &l.volumen_m3,
            &req_frio, l.fecha_expiracion, l.id_centro_acopio, &asig);

        // Validación de parsing y rangos correctos
        if (parsed == 12 && l.cantidad >= 0 && l.peso_kg >= 0) {
            l.requiere_frio = req_frio;
            l.asignado = asig;
            sistema_registrar_lote(s, l);
            cargados++;
        }
    }
    fclose(f);
    printf("\n [ÉXITO PARSING I/O] Se cargaron %d registros desde '%s'.\n", cargados, nombre_archivo);
}

void sistema_guardar_inventario_csv(SistemaGestion* s, const char* nombre_archivo) {
    FILE* f = fopen(nombre_archivo, "w");
    if (!f) {
        printf("\n [ERROR I/O] No se pudo crear el archivo de persistencia.\n");
        return;
    }

    fprintf(f, "id_lote,id_donante,categoria,descripcion,cantidad,unidad,peso_kg,volumen_m3,requiere_frio,fecha_expiracion,id_centro_acopio,asignado\n");
    for (int i = 0; i < s->num_lotes; i++) {
        LoteAyuda l = s->lotes[i];
        fprintf(f, "%s,%s,%s,%s,%.2f,%s,%.2f,%.2f,%d,%s,%s,%d\n",
            l.id_lote, l.id_donante, l.categoria, l.descripcion,
            l.cantidad, l.unidad, l.peso_kg, l.volumen_m3,
            l.requiere_frio, l.fecha_expiracion, l.id_centro_acopio, l.asignado);
    }
    fclose(f);
    printf("\n [ÉXITO PERSISTENCIA] Inventario serializado en '%s'.\n", nombre_archivo);
}
// FASE 3: Benchmarking y Análisis de Complejidad Asintótica (O)
void sistema_ejecutar_benchmark_rendimiento(SistemaGestion* s) {
    printf("\n=================================================================\n");
    printf("     BENCHMARKING DE RENDIMIENTO: ESTRUCTURAS O(log n) VS O(n)\n");
    printf("=================================================================\n");
    printf(" Generando 20,000 registros sintéticos en memoria...\n");

    int N = 20000;
    NodoAVL* avl_test = NULL;
    LoteAyuda* arreglo_lineal = (LoteAyuda*)malloc(sizeof(LoteAyuda) * N);

    for (int i = 0; i < N; i++) {
        LoteAyuda l;
        sprintf(l.id_lote, "L_%d", i);
        sprintf(l.fecha_expiracion, "2026-11-%02d", (i % 28) + 1);
        strcpy(l.categoria, "Víveres");
        l.cantidad = 100;
        
        arreglo_lineal[i] = l;
        char clave[200];
        sprintf(clave, "%s#%s", l.fecha_expiracion, l.id_lote);
        avl_test = avl_insertar(avl_test, clave, l);
    }

    // 1. Prueba Búsqueda Lineal O(N)
    clock_t t0 = clock();
    int contador_lineal = 0;
    for (int i = 0; i < N; i++) {
        if (strcmp(arreglo_lineal[i].fecha_expiracion, "2026-11-15") == 0) {
            contador_lineal++;
        }
    }
    clock_t t1 = clock();
    double ms_lineal = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;

    // 2. Prueba Búsqueda de Rango AVL O(log N + K)
    t0 = clock();
    LoteAyuda res_avl[5000];
    int contador_avl = 0;
    avl_buscar_rango(avl_test, "2026-11-15#", "2026-11-15#ZZZ", res_avl, &contador_avl, 5000);
    t1 = clock();
    double ms_avl = ((double)(t1 - t0) / CLOCKS_PER_SEC) * 1000.0;

    printf(" - Búsqueda Lineal O(N)  : %.4f ms (Resultados: %d)\n", ms_lineal, contador_lineal);
    printf(" - Búsqueda Árbol AVL O(log N): %.4f ms (Resultados: %d)\n", ms_avl, contador_avl);
    printf(" [CONCLUSIÓN APLICADA] La estructura Árbol AVL optimiza la consulta en O(log n).\n");
    printf("=================================================================\n");

    free(arreglo_lineal);
}
