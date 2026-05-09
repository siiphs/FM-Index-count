// g++ -o brute_force brute_force.cpp LectorDocumentos.cpp -L/mnt/c/Users/chips/lib -I/mnt/c/Users/chips/include -lsdsl
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <cstdint>
#include "../include/LectorDocumentos.h"

using namespace std;

class BruteForce {
private:
   std::vector<uint8_t> T;
   std::vector<uint8_t> alfabeto;
   int* C;

   int indiceEnAlfabeto(uint8_t c) {
      for (int i = 0; i < this->alfabeto.size(); i++) {
         if (this->alfabeto[i] == c) {
            return i;
         }
      }
      return -1;
   }

public:
   BruteForce(std::string ruta, std::set<uint8_t> alphabet_set) {
      LectorDocumentos lector;
      this->T = lector.cargarBinario(ruta);
      
      for (uint8_t c : alphabet_set) {
         this->alfabeto.push_back(c);
      }
      this->generarC();
   }
   
   // Cuenta las ocurrencias de un byte v en [0, k)
   int rank(int k, uint8_t v){
      int count = 0;
      for (int i = 0; i < k; i++) {
         if (this->T[i] == v) {
            count++;
         }
      }
      return count;
   }

   // Occ usa rank
   int occ(uint8_t v, int k){
      return this->rank(k, v);
   }
   
   void generarC(){
      int* C = new int[this->alfabeto.size() + 1];
      // El primero siempre es 0 porque no hay caracteres menores al primero del alfabeto
      C[0] = 0;
      for (int i = 1; i <= this->alfabeto.size(); i++) {
         // Cuenta las ocurrencias del caracter anterior en el alfabeto y lo suma al conteo acumulado del caracter anterior
         C[i] = this->rank(this->T.size(), this->alfabeto[i-1]) + C[i-1];
      }
      this->C = C;
   }

   int count(std::string patron){
      int m = patron.length();
      int i = m;
      char v = patron[m - 1];
      int indice_v = indiceEnAlfabeto(static_cast<uint8_t>(v));

      int sp = this->C[indice_v] + 1;
      int ep = this->C[indice_v + 1];

      while (sp <= ep and i >= 2) {
         v = patron[i - 2];
         indice_v = indiceEnAlfabeto(static_cast<uint8_t>(v));
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

   int* obtenerC(){
      return this->C;
   }

   std::vector<uint8_t> obtenerAlfabeto(){
      return this->alfabeto;
   }

   int obtenerEspacio(){
      return sizeof(this->T) + sizeof(this->alfabeto) + sizeof(this->C);
   }

};

// int main() {
//   std::string carpetaPath = "../docs/sources-bwt";

//    LectorDocumentos lector;
//    std::vector<uint8_t> T = lector.cargarBinario(carpetaPath);
//    std::set<uint8_t> alphabet_set(T.begin(), T.end());

//   BruteForce bruteforce("../docs/sources-bwt", alphabet_set);

//   int occurrences = bruteforce.count("asdf");
//   cout << "Número de ocurrencias: " << occurrences << endl;
  
//    cout << "Largo del alfabeto: " << bruteforce.obtenerAlfabeto().size() << endl;

// //   for (int i = 0; i < bruteforce.obtenerAlfabeto().size(); i++) {
// //     cout << bruteforce.obtenerAlfabeto()[i] << endl;
// //   }

// //   for (int i = 0; i < bruteforce.obtenerAlfabeto().size(); i++) {
// //     cout << bruteforce.obtenerC()[i] << endl;
// //   }

//   cout << "Tamaño en bytes: " << bruteforce.obtenerEspacio() << " B" <<endl;

//   return 0;
// }
