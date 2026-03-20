#ifndef MBR_H
#define MBR_H

#include <ctime>
#include "particion.h"

struct MBR {
    int tamano;                    // mbr_tamano
    time_t fecha_creacion;         // mbr_fecha_creacion
    int firma_disco;               // mbr_dsk_signature
    char ajuste;                   // dsk_fit
    Particion particiones[4];      // mbr_partitions
};

#endif