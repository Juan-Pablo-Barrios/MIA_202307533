#ifndef PARTICION_H
#define PARTICION_H

struct Particion {
    char estado;          // part_status
    char tipo;            // part_type
    char ajuste;          // part_fit
    int inicio;           // part_start
    int tamano;           // part_s
    char nombre[16];      // part_name
    int correlativo;      // part_correlative
    char id[4];           // part_id
};

#endif