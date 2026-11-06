#ifndef ENTIDADES_H
#define ENTIDADES_H

#define MAX_STR 100

typedef struct {
    char id_donante[MAX_STR];
    char nombre[MAX_STR];
    char tipo_entidad[MAX_STR];
    char contacto[MAX_STR];
} Donante;

typedef struct {
    char id_lote[MAX_STR];
    char id_donante[MAX_STR];
    char categoria[MAX_STR]; // Insumos Médicos, Víveres, Agua Potable, Kits Refugio
    char descripcion[MAX_STR];
    float cantidad;
    char unidad[MAX_STR];
    float peso_kg;
    float volumen_m3;
    int requiere_frio;
    char fecha_expiracion[11]; // "YYYY-MM-DD"
    char id_centro_acopio[MAX_STR];
    int asignado;
} LoteAyuda;

typedef struct {
    char id_centro[MAX_STR];
    char nombre[MAX_STR];
    char municipio[MAX_STR];
    float capacidad_m3;
    int tiene_cadena_frio;
} CentroAcopio;

typedef struct {
    char id_municipio[MAX_STR];
    char nombre[MAX_STR];
    char departamento[MAX_STR];
    int nivel_urgencia; // 1 a 5
    char necesidades_cat[5][MAX_STR];
    float necesidades_cant[5];
    int num_necesidades;
} MunicipioAfectado;

#endif
