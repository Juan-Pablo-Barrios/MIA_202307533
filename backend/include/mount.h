#ifndef MOUNT_H
#define MOUNT_H

#include <string>
#include <vector>

struct ParticionMontada {
    std::string id;
    std::string ruta;
    std::string nombre;
};

class Mount {
public:
    static std::string montar(const std::string& ruta, const std::string& nombre);
    static void mostrar();
private:
    static std::vector<ParticionMontada> montadas;
};

#endif