// g++ -o wt_balanced wt_balanced.cpp LectorDocumentos.cpp -L/mnt/c/Users/chips/lib -I/mnt/c/Users/chips/include -lsdsl

#include <iostream>
#include <string>
#include <sdsl/wavelet_trees.hpp>
#include "../include/LectorDocumentos.h"
using namespace std;
using namespace sdsl;

class WT_Balanced {
private:
  wt_int<> wt;
  int* C;
  std::string alfabeto;

  int indiceEnAlfabeto(char c) {
    for (int i = 0; i < this->alfabeto.size(); i++) {
      if (this->alfabeto[i] == c) {
        return i;
      }
    }
    return -1;
  }

public:
  // Constructor recibe la ruta del BWT y lo transforma a WT
  WT_Balanced(std::string ruta, std::set<char> alphabet_set) {
    construct(this->wt, ruta, 1);
    std::string alfabeto = "";
    for (char c : alphabet_set) {
      this->alfabeto.push_back(c);
    }
    generarC();
  }

  // Occ(v, k) = rankv(BWT, k)
  int occ(char v, int k){
    return this->wt.rank(k, v);
  }

  // C = arreglo tamaño sigma, donde C[v] almacena el número total de ocurrencias en T de los caracteres menores a v
  void generarC(){
    int* C = new int[this->alfabeto.size() + 1];
    // El primero siempre es 0 porque no hay caracteres menores al primero del alfabeto
    C[0] = 0;
    for (int i = 1; i <= this->alfabeto.size(); i++) {
      // Cuenta las ocurrencias del caracter anterior en el alfabeto y lo suma al conteo acumulado del caracter anterior
      C[i] = this->wt.rank(this->wt.size(), this->alfabeto[i-1]) + C[i-1];
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
    return size_in_bytes(this->wt);
  }

  std::string obtenerAlfabeto(){
    return this->alfabeto;
  }
};
