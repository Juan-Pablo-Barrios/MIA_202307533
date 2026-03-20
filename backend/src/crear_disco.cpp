#include "crear_disco.h"
#include "mbr.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <cstdlib>
#include <ctime>

using namespace std;
namespace fs = std::filesystem;

MBR inicializarMBR(int tamanoBytes, char ajuste) {
    MBR mbr;
    mbr.tamano = tamanoBytes;
    mbr.fecha_creacion = time(nullptr);
    mbr.firma_disco = rand();
    mbr.ajuste = ajuste;

    for (int i = 0; i < 4; i++) {
        mbr.particiones[i].estado = '0';
        mbr.particiones[i].tipo = '0';
        mbr.particiones[i].ajuste = '0';
        mbr.particiones[i].inicio = -1;
        mbr.particiones[i].tamano = 0;
        mbr.particiones[i].correlativo = -1;

        memset(mbr.particiones[i].nombre, '\0', sizeof(mbr.particiones[i].nombre));
        memset(mbr.particiones[i].id, '\0', sizeof(mbr.particiones[i].id));
    }

    return mbr;
}

string CrearDisco::crear(int tamano, char ajuste, char unidad, const string& ruta) {
    if (tamano <= 0) {
        return "Error: el tamano debe ser mayor a 0.";
    }

    int tamanoBytes = 0;

    if (unidad == 'K' || unidad == 'k') {
        tamanoBytes = tamano * 1024;
    } else if (unidad == 'M' || unidad == 'm') {
        tamanoBytes = tamano * 1024 * 1024;
    } else {
        return "Error: unidad no valida. Use K o M.";
    }

    fs::path rutaArchivo(ruta);
    fs::create_directories(rutaArchivo.parent_path());

    ofstream archivo(ruta, ios::binary);
    if (!archivo.is_open()) {
        return "Error: no se pudo crear el disco.";
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bloquesCompletos = tamanoBytes / 1024;
    int resto = tamanoBytes % 1024;

    for (int i = 0; i < bloquesCompletos; i++) {
        archivo.write(buffer, sizeof(buffer));
    }

    if (resto > 0) {
        archivo.write(buffer, resto);
    }

    MBR mbr = inicializarMBR(tamanoBytes, ajuste);

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<char*>(&mbr), sizeof(MBR));
    archivo.close();

    return "Disco creado correctamente en: " + ruta;
}