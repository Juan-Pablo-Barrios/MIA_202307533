#ifndef FDISK_H
#define FDISK_H

#include <string>

class FDisk {
public:
    static std::string crearParticionPrimaria(
        int tamano,
        char unidad,
        char ajuste,
        const std::string& ruta,
        const std::string& nombre
    );

    static std::string crearParticionExtendida(
        int tamano,
        char unidad,
        char ajuste,
        const std::string& ruta,
        const std::string& nombre
    );

    static std::string crearParticionLogica(
    int tamano,
    char unidad,
    char ajuste,
    const std::string& ruta,
    const std::string& nombre
);
};

#endif