// compilar con g++ -o main main.cpp wt_balanced.cpp wt_huffman.cpp brute_force.cpp wt_propio.cpp bitvector_constructors.cpp bitvector_rank.cpp bitvector_others.cpp wtree_constructors.cpp wtree_operations.cpp LectorDocumentos.cpp -L/mnt/c/Users/chips/lib -I/mnt/c/Users/chips/include -lsdsl

#include <iostream>
#include <chrono>
#include "../include/LectorDocumentos.h"
#include "wt_balanced.cpp"
#include "wt_huffman.cpp"
#include "brute_force.cpp"
#include "wt_propio.cpp"

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Uso: " << argv[0] << " <implementación> <ruta_de_texto> <archivo_patrones>\n";
        return 1;
    }
    std::string implementacion = argv[1];
    std::string carpetaPath = argv[2];
    std::string archivoPatrones = argv[3];

    LectorDocumentos lector;

    std::set<char> alphabet_set_char;
    std::set<uint8_t> alphabet_set_byte;
    std::string T_str;
    std::vector<uint8_t> T_bin;

    // Para las implementaciones BruteForce, Propia y Huff, se carga el texto como binario y se construye el alfabeto de bytes.
    if (implementacion == "BruteForce" || implementacion == "Propia" || implementacion == "Huff") {
        T_bin = lector.cargarBinario(carpetaPath);
        alphabet_set_byte = std::set<uint8_t>(T_bin.begin(), T_bin.end());
    } else {
        T_str = lector.cargarTxt(carpetaPath);
        alphabet_set_char = std::set<char>(T_str.begin(), T_str.end());
    }
    std::vector<std::string> patrones = lector.leerPorLineas(archivoPatrones);

    // Toma el tiempo que se demora en construir la estructura de datos y el tiempo que se demora en buscar cada patrón,
    // además de contar las coincidencias y el espacio utilizado por la clase.
    // Imprime los datos con formato específico para facilitar la lectura por medio de un bash que analice los resultados
    if (implementacion == "Huff") {
        auto start1 = std::chrono::high_resolution_clock::now();
        WT_Huff wt(carpetaPath, alphabet_set_byte);
        auto end1 = std::chrono::high_resolution_clock::now();
        double tiempoConstruccion = std::chrono::duration<double>(end1 - start1).count();
        double tiempoConstruccion_us = std::chrono::duration<double, std::micro>(end1 - start1).count();
        int espacio = wt.obtenerEspacio();
        
        std::cout << "METADATA;" << tiempoConstruccion_us << ";" << espacio << std::endl;
        std::cout << "patron_index;tiempo_microsegundos;coincidencias" << std::endl;
        int coincidencias = 0;
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < (int)patrones.size(); i++) {
            auto t_start = std::chrono::high_resolution_clock::now();
            int coinc = wt.count(patrones[i]);
            auto t_end = std::chrono::high_resolution_clock::now();
            double t_us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
            coincidencias += coinc;
            std::cout << i << ";" << t_us << ";" << coinc << std::endl;
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        double tiempo = std::chrono::duration<double>(end2 - start2).count();

        std::cout << "Implementación;TiempoConstrucción;CantidadPatrones;TiempoBúsqueda;Coincidencias;Espacio" << std::endl;
        std::cout << implementacion << ";" << tiempoConstruccion << ";" << patrones.size() << ";" << tiempo << ";" << coincidencias << ";" << espacio << std::endl;
    } else if (implementacion == "Balanced") {
        auto start1 = std::chrono::high_resolution_clock::now();
        WT_Balanced wt(carpetaPath, alphabet_set_char);
        auto end1 = std::chrono::high_resolution_clock::now();
        double tiempoConstruccion = std::chrono::duration<double>(end1 - start1).count();
        double tiempoConstruccion_us = std::chrono::duration<double, std::micro>(end1 - start1).count();
        int espacio = wt.obtenerEspacio();

        std::cout << "METADATA;" << tiempoConstruccion_us << ";" << espacio << std::endl;
        std::cout << "patron_index;tiempo_microsegundos;coincidencias" << std::endl;
        int coincidencias = 0;
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < (int)patrones.size(); i++) {
            auto t_start = std::chrono::high_resolution_clock::now();
            int coinc = wt.count(patrones[i]);
            auto t_end = std::chrono::high_resolution_clock::now();
            double t_us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
            coincidencias += coinc;
            std::cout << i << ";" << t_us << ";" << coinc << std::endl;
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        double tiempo = std::chrono::duration<double>(end2 - start2).count();

        std::cout << "Implementación;TiempoConstrucción;CantidadPatrones;TiempoBúsqueda;Coincidencias;Espacio" << std::endl;
        std::cout << implementacion << ";" << tiempoConstruccion << ";" << patrones.size() << ";" << tiempo << ";" << coincidencias << ";" << espacio << std::endl;
    } else if (implementacion == "BruteForce") {
        auto start1 = std::chrono::high_resolution_clock::now();
        BruteForce wt(carpetaPath, alphabet_set_byte);
        auto end1 = std::chrono::high_resolution_clock::now();
        double tiempoConstruccion = std::chrono::duration<double>(end1 - start1).count();
        double tiempoConstruccion_us = std::chrono::duration<double, std::micro>(end1 - start1).count();
        int espacio = wt.obtenerEspacio();

        std::cout << "METADATA;" << tiempoConstruccion_us << ";" << espacio << std::endl;
        std::cout << "patron_index;tiempo_microsegundos;coincidencias" << std::endl;
        int coincidencias = 0;
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < (int)patrones.size(); i++) {
            auto t_start = std::chrono::high_resolution_clock::now();
            int coinc = wt.count(patrones[i]);
            auto t_end = std::chrono::high_resolution_clock::now();
            double t_us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
            coincidencias += coinc;
            std::cout << i << ";" << t_us << ";" << coinc << std::endl;
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        double tiempo = std::chrono::duration<double>(end2 - start2).count();
        
        std::cout << "Implementación;TiempoConstrucción;CantidadPatrones;TiempoBúsqueda;Coincidencias;Espacio" << std::endl;
        std::cout << implementacion << ";" << tiempoConstruccion << ";" << patrones.size() << ";" << tiempo << ";" << coincidencias << ";" << espacio << std::endl;
    } else if (implementacion == "Propia") {
        auto start1 = std::chrono::high_resolution_clock::now();
        std::string T_prop(T_bin.begin(), T_bin.end());
        WT_Propio wt(T_prop, alphabet_set_byte);
        auto end1 = std::chrono::high_resolution_clock::now();
        double tiempoConstruccion = std::chrono::duration<double>(end1 - start1).count();
        double tiempoConstruccion_us = std::chrono::duration<double, std::micro>(end1 - start1).count();
        int espacio = wt.obtenerEspacio();

        std::cout << "METADATA;" << tiempoConstruccion_us << ";" << espacio << std::endl;
        std::cout << "patron_index;tiempo_microsegundos;coincidencias" << std::endl;
        int coincidencias = 0;
        auto start2 = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < (int)patrones.size(); i++) {
            auto t_start = std::chrono::high_resolution_clock::now();
            int coinc = wt.count(patrones[i]);
            auto t_end = std::chrono::high_resolution_clock::now();
            double t_us = std::chrono::duration<double, std::micro>(t_end - t_start).count();
            coincidencias += coinc;
            std::cout << i << ";" << t_us << ";" << coinc << std::endl;
        }
        auto end2 = std::chrono::high_resolution_clock::now();
        double tiempo = std::chrono::duration<double>(end2 - start2).count();

        std::cout << "Implementación;TiempoConstrucción;CantidadPatrones;TiempoBúsqueda;Coincidencias;Espacio" << std::endl;
        std::cout << implementacion << ";" << tiempoConstruccion << ";" << patrones.size() << ";" << tiempo << ";" << coincidencias << ";" << espacio << std::endl;
    } else {
        std::cerr << "Implementación no reconocida: " << implementacion << std::endl;
        return 1;
    }

    return 0;
}