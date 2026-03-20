#include "crear_disco.h"
#include "leer_mbr.h"
#include "leer_ebr.h"
#include "fdisk.h"
#include "mount.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
    srand(time(nullptr));

    std::string ruta = "/home/jp/Documents/MIA_P1/discos/Disco1.mia";

    std::cout << CrearDisco::crear(10, 'F', 'M', ruta) << std::endl;
    std::cout << FDisk::crearParticionPrimaria(300, 'K', 'W', ruta, "Part1") << std::endl;
    std::cout << FDisk::crearParticionExtendida(500, 'K', 'F', ruta, "Extend1") << std::endl;
    std::cout << FDisk::crearParticionLogica(100, 'K', 'F', ruta, "Log1") << std::endl;
    std::cout << FDisk::crearParticionLogica(100, 'K', 'F', ruta, "Log2") << std::endl;


    std::cout << Mount::montar(ruta, "Part1") << std::endl;
    std::cout << Mount::montar(ruta, "Log1") << std::endl;
    std::cout << Mount::montar(ruta, "Log2") << std::endl;

    Mount::mostrar();
    LeerMBR::leer(ruta);
    LeerEBR::leerInicial(ruta);
    LeerEBR::leerCadena(ruta);

    return 0;
}