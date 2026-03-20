#include "leer_mbr.h"
#include "mbr.h"

#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

bool LeerMBR::leer(const string& ruta) {
    ifstream archivo(ruta, ios::binary);

    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el archivo del disco.\n";
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

    cout << "\n===== MBR DEL DISCO =====\n";
    cout << "Tamano: " << mbr.tamano << " bytes\n";
    cout << "Firma del disco: " << mbr.firma_disco << "\n";
    cout << "Ajuste: " << mbr.ajuste << "\n";
    cout << "Fecha de creacion: " << ctime(&mbr.fecha_creacion);

    cout << "\n===== PARTICIONES =====\n";
    for (int i = 0; i < 4; i++) {
        cout << "\nParticion " << i + 1 << ":\n";
        cout << "  Estado: " << mbr.particiones[i].estado << "\n";
        cout << "  Tipo: " << mbr.particiones[i].tipo << "\n";
        cout << "  Ajuste: " << mbr.particiones[i].ajuste << "\n";
        cout << "  Inicio: " << mbr.particiones[i].inicio << "\n";
        cout << "  Tamano: " << mbr.particiones[i].tamano << "\n";
        cout << "  Nombre: " << mbr.particiones[i].nombre << "\n";
        cout << "  Correlativo: " << mbr.particiones[i].correlativo << "\n";
        cout << "  ID: " << mbr.particiones[i].id << "\n";
    }

    archivo.close();
    return true;
}