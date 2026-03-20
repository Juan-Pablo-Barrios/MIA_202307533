#include "leer_ebr.h"
#include "mbr.h"
#include "ebr.h"

#include <iostream>
#include <fstream>

using namespace std;

bool LeerEBR::leerInicial(const string& ruta) {
    ifstream archivo(ruta, ios::binary);

    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el disco.\n";
        return false;
    }

    MBR mbr;
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&mbr), sizeof(MBR));

    if (!archivo) {
        cout << "Error: no se pudo leer el MBR.\n";
        archivo.close();
        return false;
    }

    int inicioExtendida = -1;

    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano > 0 && mbr.particiones[i].tipo == 'E') {
            inicioExtendida = mbr.particiones[i].inicio;
            break;
        }
    }

    if (inicioExtendida == -1) {
        cout << "No existe una particion extendida en el disco.\n";
        archivo.close();
        return false;
    }

    EBR ebr;
    archivo.seekg(inicioExtendida, ios::beg);
    archivo.read(reinterpret_cast<char*>(&ebr), sizeof(EBR));

    if (!archivo) {
        cout << "Error: no se pudo leer el EBR inicial.\n";
        archivo.close();
        return false;
    }

    cout << "\n===== EBR INICIAL =====\n";
    cout << "Estado: " << ebr.estado << "\n";
    cout << "Ajuste: " << ebr.ajuste << "\n";
    cout << "Inicio: " << ebr.inicio << "\n";
    cout << "Tamano: " << ebr.tamano << "\n";
    cout << "Siguiente: " << ebr.siguiente << "\n";
    cout << "Nombre: " << ebr.nombre << "\n";

    archivo.close();
    return true;
}

bool LeerEBR::leerCadena(const string& ruta) {
    ifstream archivo(ruta, ios::binary);

    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el disco.\n";
        return false;
    }

    MBR mbr;
    archivo.seekg(0, ios::beg);
    archivo.read(reinterpret_cast<char*>(&mbr), sizeof(MBR));

    if (!archivo) {
        cout << "Error: no se pudo leer el MBR.\n";
        archivo.close();
        return false;
    }

    int inicioExtendida = -1;
    int finExtendida = -1;

    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano > 0 && mbr.particiones[i].tipo == 'E') {
            inicioExtendida = mbr.particiones[i].inicio;
            finExtendida = inicioExtendida + mbr.particiones[i].tamano;
            break;
        }
    }

    if (inicioExtendida == -1) {
        cout << "No existe una particion extendida en el disco.\n";
        archivo.close();
        return false;
    }

    int posicionActual = inicioExtendida;
    int contador = 1;

    cout << "\n===== CADENA DE EBRs =====\n";

    while (posicionActual != -1 && posicionActual < finExtendida) {
        EBR ebr;

        archivo.seekg(posicionActual, ios::beg);
        archivo.read(reinterpret_cast<char*>(&ebr), sizeof(EBR));

        if (!archivo) {
            cout << "Error: no se pudo leer un EBR en la posicion " << posicionActual << ".\n";
            archivo.close();
            return false;
        }

        if (ebr.tamano == 0) {
            cout << "EBR vacio encontrado en posicion " << posicionActual << ".\n";
            break;
        }

        cout << "\nEBR " << contador << ":\n";
        cout << "  Estado: " << ebr.estado << "\n";
        cout << "  Ajuste: " << ebr.ajuste << "\n";
        cout << "  Inicio: " << ebr.inicio << "\n";
        cout << "  Tamano: " << ebr.tamano << "\n";
        cout << "  Siguiente: " << ebr.siguiente << "\n";
        cout << "  Nombre: " << ebr.nombre << "\n";

        if (ebr.siguiente == -1) {
            break;
        }

        posicionActual = ebr.siguiente;
        contador++;
    }

    archivo.close();
    return true;
}