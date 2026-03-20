#include "fdisk.h"
#include "mbr.h"
#include "ebr.h"

#include <fstream>
#include <cstring>

using namespace std;

static int convertirBytes(int tamano, char unidad) {
    if (unidad == 'B' || unidad == 'b') return tamano;
    if (unidad == 'K' || unidad == 'k') return tamano * 1024;
    if (unidad == 'M' || unidad == 'm') return tamano * 1024 * 1024;
    return -1;
}

static bool nombreRepetido(const MBR& mbr, const string& nombre) {
    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano > 0) {
            if (string(mbr.particiones[i].nombre) == nombre) {
                return true;
            }
        }
    }
    return false;
}

static int buscarIndiceLibre(const MBR& mbr) {
    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano == 0) {
            return i;
        }
    }
    return -1;
}

static int calcularInicioNuevaParticion(const MBR& mbr) {
    int ultimoFin = sizeof(MBR);

    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano > 0) {
            int finActual = mbr.particiones[i].inicio + mbr.particiones[i].tamano;
            if (finActual > ultimoFin) {
                ultimoFin = finActual;
            }
        }
    }

    return ultimoFin;
}

static bool yaExisteExtendida(const MBR& mbr) {
    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano > 0 && mbr.particiones[i].tipo == 'E') {
            return true;
        }
    }
    return false;
}

string FDisk::crearParticionPrimaria(
    int tamano,
    char unidad,
    char ajuste,
    const string& ruta,
    const string& nombre
) {
    if (tamano <= 0) {
        return "Error: el tamano de la particion debe ser mayor a 0.";
    }

    int tamanoBytes = convertirBytes(tamano, unidad);
    if (tamanoBytes <= 0) {
        return "Error: unidad no valida. Use B, K o M.";
    }

    if (nombre.empty()) {
        return "Error: el nombre de la particion es obligatorio.";
    }

    if (nombre.size() > 15) {
        return "Error: el nombre de la particion no debe exceder 16 caracteres.";
    }

    ifstream archivoLectura(ruta, ios::binary);
    if (!archivoLectura.is_open()) {
        return "Error: no se pudo abrir el disco.";
    }

    MBR mbr;
    archivoLectura.seekg(0, ios::beg);
    archivoLectura.read(reinterpret_cast<char*>(&mbr), sizeof(MBR));
    archivoLectura.close();

    if (nombreRepetido(mbr, nombre)) {
        return "Error: ya existe una particion con ese nombre.";
    }

    int indiceLibre = buscarIndiceLibre(mbr);
    if (indiceLibre == -1) {
        return "Error: ya existen 4 particiones en el disco.";
    }

    int inicio = calcularInicioNuevaParticion(mbr);

    if (inicio + tamanoBytes > mbr.tamano) {
        return "Error: no hay espacio suficiente para la particion.";
    }

    mbr.particiones[indiceLibre].estado = '0';
    mbr.particiones[indiceLibre].tipo = 'P';
    mbr.particiones[indiceLibre].ajuste = ajuste;
    mbr.particiones[indiceLibre].inicio = inicio;
    mbr.particiones[indiceLibre].tamano = tamanoBytes;
    mbr.particiones[indiceLibre].correlativo = -1;

    memset(mbr.particiones[indiceLibre].nombre, '\0', sizeof(mbr.particiones[indiceLibre].nombre));
    memset(mbr.particiones[indiceLibre].id, '\0', sizeof(mbr.particiones[indiceLibre].id));
    strncpy(mbr.particiones[indiceLibre].nombre, nombre.c_str(), sizeof(mbr.particiones[indiceLibre].nombre) - 1);

    fstream archivoEscritura(ruta, ios::in | ios::out | ios::binary);
    if (!archivoEscritura.is_open()) {
        return "Error: no se pudo abrir el disco para escritura.";
    }

    archivoEscritura.seekp(0, ios::beg);
    archivoEscritura.write(reinterpret_cast<char*>(&mbr), sizeof(MBR));
    archivoEscritura.close();

    return "Particion primaria creada correctamente.";
}

string FDisk::crearParticionExtendida(
    int tamano,
    char unidad,
    char ajuste,
    const string& ruta,
    const string& nombre
) {
    if (tamano <= 0) {
        return "Error: el tamano de la particion debe ser mayor a 0.";
    }

    int tamanoBytes = convertirBytes(tamano, unidad);
    if (tamanoBytes <= 0) {
        return "Error: unidad no valida. Use B, K o M.";
    }

    if (nombre.empty()) {
        return "Error: el nombre de la particion es obligatorio.";
    }

    if (nombre.size() > 15) {
        return "Error: el nombre de la particion no debe exceder 16 caracteres.";
    }

    fstream archivo(ruta, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) {
        return "Error: no se pudo abrir el disco.";
    }

    MBR mbr;
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&mbr), sizeof(MBR));

    if (nombreRepetido(mbr, nombre)) {
        archivo.close();
        return "Error: ya existe una particion con ese nombre.";
    }

    if (yaExisteExtendida(mbr)) {
        archivo.close();
        return "Error: ya existe una particion extendida en el disco.";
    }

    int indiceLibre = buscarIndiceLibre(mbr);
    if (indiceLibre == -1) {
        archivo.close();
        return "Error: ya existen 4 particiones en el disco.";
    }

    int inicio = calcularInicioNuevaParticion(mbr);

    if (inicio + tamanoBytes > mbr.tamano) {
        archivo.close();
        return "Error: no hay espacio suficiente para la particion extendida.";
    }

    mbr.particiones[indiceLibre].estado = '0';
    mbr.particiones[indiceLibre].tipo = 'E';
    mbr.particiones[indiceLibre].ajuste = ajuste;
    mbr.particiones[indiceLibre].inicio = inicio;
    mbr.particiones[indiceLibre].tamano = tamanoBytes;
    mbr.particiones[indiceLibre].correlativo = -1;

    memset(mbr.particiones[indiceLibre].nombre, '\0', sizeof(mbr.particiones[indiceLibre].nombre));
    memset(mbr.particiones[indiceLibre].id, '\0', sizeof(mbr.particiones[indiceLibre].id));
    strncpy(mbr.particiones[indiceLibre].nombre, nombre.c_str(), sizeof(mbr.particiones[indiceLibre].nombre) - 1);

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<char*>(&mbr), sizeof(MBR));

    EBR ebrInicial;
    ebrInicial.estado = '0';
    ebrInicial.ajuste = ajuste;
    ebrInicial.inicio = inicio;
    ebrInicial.tamano = 0;
    ebrInicial.siguiente = -1;
    memset(ebrInicial.nombre, '\0', sizeof(ebrInicial.nombre));

    archivo.seekp(inicio, ios::beg);
    archivo.write(reinterpret_cast<char*>(&ebrInicial), sizeof(EBR));

    archivo.close();

    return "Particion extendida creada correctamente.";
}

string FDisk::crearParticionLogica(
    int tamano,
    char unidad,
    char ajuste,
    const string& ruta,
    const string& nombre
) {
    if (tamano <= 0) {
        return "Error: tamaño inválido.";
    }

    int tamanoBytes = convertirBytes(tamano, unidad);
    if (tamanoBytes <= 0) {
        return "Error: unidad inválida.";
    }

    fstream archivo(ruta, ios::in | ios::out | ios::binary);
    if (!archivo.is_open()) {
        return "Error: no se pudo abrir el disco.";
    }

    MBR mbr;
    archivo.seekg(0);
    archivo.read(reinterpret_cast<char*>(&mbr), sizeof(MBR));

    // buscar extendida
    int inicioExtendida = -1;
    int finExtendida = -1;

    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tipo == 'E') {
            inicioExtendida = mbr.particiones[i].inicio;
            finExtendida = inicioExtendida + mbr.particiones[i].tamano;
            break;
        }
    }

    if (inicioExtendida == -1) {
        archivo.close();
        return "Error: no existe particion extendida.";
    }

    // leer primer EBR
    EBR ebr;
    archivo.seekg(inicioExtendida);
    archivo.read(reinterpret_cast<char*>(&ebr), sizeof(EBR));

    // 👉 CASO 1: primera lógica
    if (ebr.tamano == 0) {

        if (inicioExtendida + sizeof(EBR) + tamanoBytes > finExtendida) {
            archivo.close();
            return "Error: no hay espacio en la extendida.";
        }

        ebr.estado = '0';
        ebr.ajuste = ajuste;
        ebr.inicio = inicioExtendida;
        ebr.tamano = tamanoBytes;
        ebr.siguiente = -1;

        memset(ebr.nombre, '\0', sizeof(ebr.nombre));
        strncpy(ebr.nombre, nombre.c_str(), sizeof(ebr.nombre) - 1);

        archivo.seekp(inicioExtendida);
        archivo.write(reinterpret_cast<char*>(&ebr), sizeof(EBR));

        archivo.close();
        return "Primera particion logica creada.";
    }

    // 👉 CASO 2: recorrer lista
    int posActual = inicioExtendida;

    while (true) {
        archivo.seekg(posActual);
        archivo.read(reinterpret_cast<char*>(&ebr), sizeof(EBR));

        if (ebr.siguiente == -1) {
            break;
        }

        posActual = ebr.siguiente;
    }

    int nuevoEBRPos = posActual + sizeof(EBR) + ebr.tamano;

    if (nuevoEBRPos + sizeof(EBR) + tamanoBytes > finExtendida) {
        archivo.close();
        return "Error: no hay espacio para otra lógica.";
    }

    // actualizar anterior
    ebr.siguiente = nuevoEBRPos;

    archivo.seekp(posActual);
    archivo.write(reinterpret_cast<char*>(&ebr), sizeof(EBR));

    // crear nuevo EBR
    EBR nuevo;
    nuevo.estado = '0';
    nuevo.ajuste = ajuste;
    nuevo.inicio = nuevoEBRPos;
    nuevo.tamano = tamanoBytes;
    nuevo.siguiente = -1;

    memset(nuevo.nombre, '\0', sizeof(nuevo.nombre));
    strncpy(nuevo.nombre, nombre.c_str(), sizeof(nuevo.nombre) - 1);

    archivo.seekp(nuevoEBRPos);
    archivo.write(reinterpret_cast<char*>(&nuevo), sizeof(EBR));

    archivo.close();
    return "Particion logica creada correctamente.";
}