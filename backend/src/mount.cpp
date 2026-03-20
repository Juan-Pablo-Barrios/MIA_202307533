#include "mount.h"
#include "mbr.h"
#include "ebr.h"

#include <iostream>
#include <fstream>

using namespace std;

vector<ParticionMontada> Mount::montadas;

string generarID(int numero) {
    return "vd" + to_string(numero);
}

string Mount::montar(const string& ruta, const string& nombre) {

    // evitar duplicados
    for (auto& p : Mount::montadas) {
        if (p.ruta == ruta && p.nombre == nombre) {
            return "Error: la particion ya esta montada.";
        }
    }

    ifstream archivo(ruta, ios::binary);
    if (!archivo.is_open()) {
        return "Error: no se pudo abrir el disco.";
    }

    MBR mbr;
    archivo.read(reinterpret_cast<char*>(&mbr), sizeof(MBR));

    // buscar en primarias y extendida
    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tamano > 0) {
            if (string(mbr.particiones[i].nombre) == nombre) {

                ParticionMontada nueva;
                nueva.id = generarID(montadas.size() + 1);
                nueva.ruta = ruta;
                nueva.nombre = nombre;

                montadas.push_back(nueva);

                return "Particion montada con ID: " + nueva.id;
            }
        }
    }

    // buscar en logicas (EBR)
    int inicioExtendida = -1;

    for (int i = 0; i < 4; i++) {
        if (mbr.particiones[i].tipo == 'E') {
            inicioExtendida = mbr.particiones[i].inicio;
            break;
        }
    }

    if (inicioExtendida != -1) {
        int pos = inicioExtendida;

        while (true) {
            EBR ebr;
            archivo.seekg(pos);
            archivo.read(reinterpret_cast<char*>(&ebr), sizeof(EBR));

            if (ebr.tamano == 0) break;

            if (string(ebr.nombre) == nombre) {

                ParticionMontada nueva;
                nueva.id = generarID(montadas.size() + 1);
                nueva.ruta = ruta;
                nueva.nombre = nombre;

                montadas.push_back(nueva);

                return "Particion logica montada con ID: " + nueva.id;
            }

            if (ebr.siguiente == -1) break;

            pos = ebr.siguiente;
        }
    }

    return "Error: no se encontro la particion.";
}

void Mount::mostrar() {
    cout << "\n===== PARTICIONES MONTADAS =====\n";

    for (auto& p : montadas) {
        cout << "ID: " << p.id
             << " | Ruta: " << p.ruta
             << " | Nombre: " << p.nombre << endl;
    }
}