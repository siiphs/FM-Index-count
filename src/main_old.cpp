#include <iostream>
#include "..\include\LectorDocumentos.h"
#include "..\include\KMP.h"
#include "..\include\BM.h"
#include "..\include\RK.h"
#include "..\include\memoria.h"
#include <chrono>


int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Uso: " << argv[0] << " <implementacion> <ruta_de_texto> <archivo_patrones>\n";
        return 1;
    }
    std::string algoritmo = argv[1];
    std::string carpetaPath = argv[2];
    std::string archivoPatrones = argv[3];
    int num_documentos = std::stoi(argv[4]);

    std::string archivoTxt = "../data/textoT.txt";
    std::string T;

    LectorDocumentos lector;

    // Si el archivo existe, cargarlo, si no, crearlo con lector y guardar
    if (std::filesystem::exists(archivoTxt)) {
        T = lector.cargarTxt(archivoTxt);
    } else {
        T = lector.concatenarDocumentosConSeparador(carpetaPath, num_documentos);
        lector.crearTxt(archivoTxt, T);
    }

    std::vector<std::string> nombres_documentos = lector.leerPorLineas("data/orden_documentos.txt");
    std::vector<int> coincidencias_doc(nombres_documentos.size(), 0);

    // Leer todos los patrones
    std::vector<std::string> patrones = lector.leerPorLineas(archivoPatrones);
    // for (auto& patrom : patrones) {
    //     std::cout << patrom << std::endl;
    // }
    // std::cout << patrones.size() << std::endl;
   // size_t total_espacio_bytesA = getMemoryUsage();
    std::vector<int> coincidencias;
    auto start = std::chrono::high_resolution_clock::now();
    for (auto& patron : patrones) {
        int veces = 0;
        if (algoritmo == "KMP") {
            size_t total_espacio_bytesAT = getMemoryUsage();
            veces = KMP(patron, T, coincidencias_doc);
            size_t total_espacio_bytesDT = getMemoryUsage();
            std::cout << "Espacio usado por KMP: " << total_espacio_bytesDT - total_espacio_bytesAT << " bytes\n";

        } else if (algoritmo == "BM") {
            veces = boyerMoore(patron, T, coincidencias_doc);
        } else if (algoritmo == "RK") {
            veces = rabinKarp(patron, T, coincidencias_doc);
        } else {
            std::cerr << "Algoritmo no reconocido.\n";
            return 1;
        }
        coincidencias.push_back(veces);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double>(end - start).count();
    int total_coincidencias = std::accumulate(coincidencias.begin(), coincidencias.end(), 0);
    // size_t total_espacio_bytesD = getMemoryUsage();
    // size_t total_espacio_bytes = total_espacio_bytesD - total_espacio_bytesA;

    std::cout << algoritmo << ";" << patrones.size()
              << ";" << tiempo
              << ";" << total_coincidencias
              << ";" << num_documentos
              // << ";" << total_espacio_bytes
              << std::endl;

    return 0;
}