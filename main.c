#include <stdio.h>
#include "sistema.h"

int main() {
    SistemaGestion sistema;
    sistema_inicializar(&sistema);
    
    // Opcional: Intentar cargar datos desde CSV al arrancar
    sistema_cargar_inventario_csv(&sistema, "inventario_donaciones.csv");
    if (sistema.num_lotes == 0) {
        sistema_cargar_datos_prueba(&sistema); // Datos base si el CSV está vacío
    }

    int opcion = 0;
    do {
        printf("\n=================================================================\n");
        printf("   SISTEMA DE GESTION Y DISTRIBUCION POST-DESASTRE (EAFIT)\n");
        printf("=================================================================\n");
        printf(" 1. Ver Estado de la Red Vial y Topología (Grafo + Dijkstra)\n");
        printf(" 2. Ejecutar Triaje Humanitario y Despacho Óptimo (Max-Heap)\n");
        printf(" 3. Consultar Inventario General\n");
        printf(" 4. Búsqueda Multinivel por Rango de Expiración (Árbol AVL O(log n))\n");
        printf(" 5. Registrar Nuevo Lote por Teclado (Validación de Rangos)\n");
        printf(" 6. Ver Alertas Tempranas de Desabastecimiento\n");
        printf(" 7. Guardar / Exportar Datos a CSV (Persistencia I/O)\n");
        printf(" 8. Ejecutar Benchmark de Rendimiento (Árbol AVL vs Lineal)\n");
        printf(" 9. Salir\n");
        printf(" Seleccione una opción: ");
        if (scanf("%d", &opcion) != 1) break;

        switch (opcion) {
            case 1:
                sistema_mostrar_red_vial(&sistema);
                break;
            case 2:
                sistema_procesar_triaje_y_despacho(&sistema);
                break;
            case 3:
                sistema_mostrar_inventario(&sistema);
                break;
            case 4:
                sistema_buscar_lotes_avl_rango(&sistema);
                break;
            case 5:
                sistema_registrar_lote_interactivo(&sistema);
                break;
            case 6:
                sistema_alertas_desabastecimiento(&sistema);
                break;
            case 7:
                sistema_guardar_inventario_csv(&sistema, "inventario_donaciones.csv");
                break;
            case 8:
                sistema_ejecutar_benchmark_rendimiento(&sistema);
                break;
            case 9:
                printf("\nSaliendo del sistema de emergencia...\n");
                break;
            default:
                printf("\nOpción inválida. Intente de nuevo.\n");
        }
    } while (opcion != 9);

    return 0;
}
