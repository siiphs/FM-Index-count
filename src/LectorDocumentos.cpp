#include "../include/LectorDocumentos.h"

std::vector<uint8_t> LectorDocumentos::cargarBinario(const std::string& ruta) {
    std::ifstream entrada(ruta, std::ios::binary);
    if (!entrada) return {};
    return std::vector<uint8_t>(std::istreambuf_iterator<char>(entrada), std::istreambuf_iterator<char>());
}

std::vector<std::string> LectorDocumentos::leerPorLineas(const std::string& nombreArchivo) {
    std::vector<std::string> lineas; 
    std::ifstream archivo(nombreArchivo);
    std::string linea;
    while (std::getline(archivo, linea)) {
        if (!linea.empty()) {
            lineas.push_back(linea);
        }
    }
    return lineas;
}

std::string LectorDocumentos::cargarTxt(std::string& ruta) {
    std::ifstream entrada(ruta);
    if (!entrada) return "";
    std::string contenido((std::istreambuf_iterator<char>(entrada)), std::istreambuf_iterator<char>());
    return contenido;
}

void LectorDocumentos::crearTxt( const std::string& ruta,const std::string& contenido) {
    std::ofstream salida(ruta);
    if (salida) {
        salida << contenido;
        salida.close();
    }
}

std::vector<std::string> LectorDocumentos::extraerPatrones(const std::string& texto, int cantidad, int long_patron) {
    std::vector<std::string> patrones;
    if (texto.size() < long_patron || cantidad <= 0) return patrones;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, texto.size() - long_patron);

    for (int i = 0; i < cantidad; ++i) {
        int inicio = dis(gen);
        patrones.push_back(texto.substr(inicio, long_patron));
    }
    return patrones;
}