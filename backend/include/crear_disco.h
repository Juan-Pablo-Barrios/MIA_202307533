#ifndef CREAR_DISCO_H
#define CREAR_DISCO_H

#include <string>

class CrearDisco {
public:
    static std::string crear(int tamano, char ajuste, char unidad, const std::string& ruta);
};

#endif