#ifndef EBR_H
#define EBR_H

struct EBR {
    char estado;
    char ajuste;
    int inicio;
    int tamano;
    int siguiente;
    char nombre[16];
};

#endif