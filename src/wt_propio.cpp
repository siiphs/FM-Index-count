#include "../include/wtree-lib/wtree.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

class WT_Propio {
private:
    CDS_UDEC::WaveletTree wt;
    int* C;
    std::string alfabeto;

    int indiceEnAlfabeto(uint8_t c) {
        for (int i = 0; i < this->alfabeto.size(); i++) {
            if (this->alfabeto[i] == c) {
                return i;
            }
        }
        return -1;
    }

public:
    // Constructor el string del BWT y lo transforma a WT
    WT_Propio(std::string T_BWT, std::set<uint8_t> alphabet_set) {
        // Convierte el string del BWT a un vector de uint32_t para construir el wavelet tree
        std::vector<uint32_t> seq;
        for (auto e: T_BWT) {
            seq.push_back((uint32_t)e);
        }
        this->wt = CDS_UDEC::WaveletTree(seq);

        std::string alfabeto = "";
        for (uint8_t c : alphabet_set) {
            this->alfabeto.push_back(c);
        }
        generarC();
    }

    // Occ(v, k) = rankv(BWT, k)
    int occ(uint8_t v, int k){
        return this->wt.rank((uint32_t)v, k);
    }

    // C = arreglo tamaño sigma, donde C[v] almacena el número total de ocurrencias en T de los caracteres menores a v
    void generarC(){
        int* C = new int[this->alfabeto.size() + 1];
        // El primero siempre es 0 porque no hay caracteres menores al primero del alfabeto
        C[0] = 0;
        for (int i = 1; i <= this->alfabeto.size(); i++) {
            // Cuenta las ocurrencias del caracter anterior en el alfabeto y lo suma al conteo acumulado del caracter anterior
            C[i] = this->wt.rank((uint32_t) this->alfabeto[i-1], this->wt.getSize()) + C[i-1];
        }
        this->C = C;
    }

    int* obtenerC(){
        return this->C;
    }

    // Busca un patrón en el wavelet tree y devuelve el número de ocurrencias
    int count(std::string patron){
        int m = patron.length();
        int i = m;
        char v = patron[m - 1];
        int indice_v = indiceEnAlfabeto(v);

        int sp = this->C[indice_v] + 1;
        int ep = this->C[indice_v + 1];

        while (sp <= ep and i >= 2) {
            v = patron[i - 2];
            indice_v = indiceEnAlfabeto(v);
            if (indice_v == -1) {
                return 0;
            }

            sp = this->C[indice_v] + occ(v, sp - 1) + 1;
            ep = this->C[indice_v] + occ(v, ep);
            i--;
        }
        if (ep < sp) {
            return 0;
        }
        else {
            return (ep - sp + 1);
        }
    }

    int obtenerEspacio(){
        return (int)this->wt.size_in_bytes();
    }

    std::string obtenerAlfabeto(){
        return this->alfabeto;
    }
};