#ifndef LECTORDOCUMENTOS_H
#define LECTORDOCUMENTOS_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <random>
#include <cstdint>

class LectorDocumentos {
public:
    std::vector<std::string> leerPorLineas(const std::string& nombreArchivo); 
    std::string cargarTxt(std::string& ruta);
    std::vector<uint8_t> cargarBinario(const std::string& ruta);
    void crearTxt(const std::string& ruta, const std::string& contenido);
    std::vector<std::string> extraerPatrones(const std::string& texto, int cantidad, int long_patron);
};

#endif