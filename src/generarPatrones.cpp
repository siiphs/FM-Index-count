#include "..\include\LectorDocumentos.h"
#include <iostream>
#include <fstream>
#include <set>

std::string limpiarSaltos(const std::string& s) {
    std::string limpio;
    for (char c : s) {
        if (c != '\n' && c != '\r' && c != '\t') limpio += c;
    }
    return limpio;
}

// Programa para generar patrones únicos a partir de un archivo de texto
int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " <ruta_txt> <cantidad_patrones> <longitud_patron> <ruta_salida>\n";
        return 1;
    }

    std::string ruta = argv[1];
    int cantidad = std::stoi(argv[2]);
    int longitud = std::stoi(argv[3]);
    std::string ruta_salida = argv[4];

    LectorDocumentos lector;
    std::string texto = lector.cargarTxt(ruta);

    if (texto.empty()) {
        std::cerr << "No se pudo cargar el archivo o esta vacio.\n";
        return 1;
    }

    std::set<std::string> patrones_unicos;
    int intentos = 0, max_intentos = cantidad * 10;

    while ((int)patrones_unicos.size() < cantidad && intentos < max_intentos) {
        auto nuevos = lector.extraerPatrones(texto, 1, longitud);
        if (!nuevos.empty()) {
            std::string patron = limpiarSaltos(nuevos[0]);
            if (patron.size() == longitud) { // Solo patrones del tamaño correcto
                patrones_unicos.insert(nuevos[0]);
            }
        }
        ++intentos;
    }

    std::ofstream salida(ruta_salida);
    if (!salida) {
        std::cerr << "No se pudo abrir el archivo de salida.\n";
        return 1;
    }
    for (const auto& patron : patrones_unicos) {
        salida << patron << std::endl;
    }
    salida.close();

    std::cout << "Patrones unicos guardados en " << ruta_salida << ": " << patrones_unicos.size() << std::endl;
    return 0;
}