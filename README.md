# Sistema Integral de Gestión y Distribución de Donaciones Post-Desastre (Colombia)
**Universidad EAFIT — Escuela de Ciencias Aplicadas e Ingeniería**  
**Departamento de Informática y Analítica | Asignatura: Estructuras de Datos y Algoritmos**

---

## 1. Contexto y Descripción del Reto
Tras un evento sísmico de gran magnitud en el territorio colombiano, la logística humanitaria enfrenta una crisis severa por la descoordinación entre los puntos de entrada (aeropuertos, puertos y centros de acopio regionales) y los municipios aislados. Este software de alto rendimiento en **C puro (`.c` y `.h`)** garantiza la trazabilidad integral de los recursos, la priorización algorítmica de despachos y el modelado de rutas críticas mediante teoría de grafos.

---

## 2. Arquitectura Modular y Estructuras de Datos Implementadas

El proyecto está diseñado bajo un estricto paradigma modular, implementando las estructuras exigidas en la rúbrica institucional:

1. **Modelado de Entidades (`entidades.h`):** 
   - Estructuras (`struct`) fuertemente tipadas en memoria para `Donante`, `LoteAyuda`, `CentroAcopio` y `MunicipioAfectado`, con validación de rangos y valores no negativos.
2. **Árbol Binario de Búsqueda Balanceado - AVL (`avl.h` / `avl.c`):**
   - Garantiza operaciones de inserción y consultas por rango de fecha de expiración estrictamente acotadas en complejidad temporal **$O(\log n)$**.
3. **Cola de Prioridad - Max-Heap (`heap.h` / `heap.c`):**
   - Implementa el algoritmo de triaje humanitario dinámico, ordenando y extrayendo en **$O(\log n)$** las solicitudes de los municipios con mayor nivel de urgencia.
4. **Grafos Ponderados y Algoritmo de Dijkstra (`grafo.h` / `grafo.c`):**
   - Modela la red vial de transporte en Colombia. Las aristas integran distancias kilométricas reales y factores de transitabilidad afectados por derrumbes y cierres viales post-sismo. Calcula rutas óptimas en **$O((V + E) \log V)$**.
5. **Persistencia e I/O (`sistema.c`):**
   - Mecanismos de lectura, parsing con manejo de errores y serialización tabular interoperable en formato **CSV**.
6. **Análisis Algorítmico y Benchmark (`sistema.c`):**
   - Suite de pruebas de estrés empíricas que evalúan el rendimiento temporal contrastando estructuras eficientes frente a alternativas lineales ingenuas.

---

## 3. Instrucciones de Compilación y Ejecución en WSL (Linux)

### Prerrequisitos
Tener instalado un compilador de C (`gcc`) y la herramienta `make` o `build-essential` en tu entorno WSL (Ubuntu).

### Paso a Paso para Compilar

1. Clona o ubícate en el directorio del proyecto en tu terminal de WSL:
   ```bash
   cd sistema_donaciones
   ```

2. Compila todos los módulos ejecutando el siguiente comando con `gcc`:
   ```bash
   gcc main.c avl.c heap.c grafo.c sistema.c -o sistema_donaciones
   ```

3. Ejecuta el sistema interactivo de consola:
   ```bash
   ./sistema_donaciones
   ```

---

## 4. Catálogo de Operaciones del Menú en Consola

Al ejecutar el programa, se desplegará el menú interactivo con las siguientes opciones operativas:

1. **Ver Estado de la Red Vial y Topología:** Muestra la matriz de adyacencia del grafo ponderado con los costos efectivos considerando los factores de sismo.
2. **Ejecutar Triaje Humanitario y Despacho Óptimo:** Extrae la máxima prioridad del *Max-Heap*, asigna el lote disponible y calcula la ruta más corta mediante *Dijkstra*.
3. **Consultar Inventario General:** Despliega todos los lotes de ayuda y sus estados actuales (*DISPONIBLE* / *DESPACHADO*).
4. **Búsqueda Multinivel por Rango de Expiración:** Utiliza el *Árbol AVL* para filtrar eficientemente lotes por intervalos de fechas de caducidad.
5. **Registrar Nuevo Lote por Teclado:** Permite el ingreso interactivo aplicando validaciones estrictas de rangos no negativos.
6. **Ver Alertas Tempranas de Desabastecimiento:** Contrasta la demanda cuantitativa municipal frente al inventario actual en centros de acopio.
7. **Guardar / Exportar Datos a CSV:** Serializa la información actual en el archivo `inventario_donaciones.csv`.
8. **Ejecutar Benchmark de Rendimiento:** Genera 20,000 registros sintéticos y mide en milisegundos la ventaja teórica y práctica del *Árbol AVL* ($O(\log n)$) frente a una búsqueda lineal ($O(n)$).
9. **Salir:** Cierra el sistema de emergencia.
