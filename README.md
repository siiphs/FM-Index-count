# FM-index

## Requisitos

- **Compilador C++** (recomendado: g++ de MinGW o similar)
- Estructura de carpetas como en el repositorio (`src`, `include`, etc.)

## Compilación manual con g++

Abre una terminal en la raíz del proyecto y ejecuta:

### Compilar el ejecutable principal (`main`):

```sh
cd src
g++ -o main main.cpp wt_balanced.cpp wt_huffman.cpp brute_force.cpp wt_propio.cpp bitvector_constructors.cpp bitvector_rank.cpp bitvector_others.cpp wtree_constructors.cpp wtree_operations.cpp LectorDocumentos.cpp -L~/lib -I~/include -lsdsl
```

### Compilar el ejecutable para transformar texto a BWT (`bwt`):

```sh
cd src
g++ -O3 -o bwt bwt.cpp -L~/lib -I~/include -lsdsl -lsdsl -ldivsufsort -ldivsufsort64
```

## Ejecución

### Ejecutar el programa principal

```sh
./main <implementación> <ruta_de_texto> <archivo_patrones>
```
- `<implementación>`: Huff, Balanced, BruteForce o Propia
- `<ruta_de_texto>`: Ruta al archivo con la transformada de Burrows-Wheeler del dataset a usar
- `<archivo_patrones>`: Archivo con los patrones a buscar

### Ejecutar BWT

```sh
./bwt <ruta_texto>
```
- `<ruta_texto>`: Ruta al archivo con el dataset que se quiere transformar a BWT

Esto creará un archivo con el mismo nombre que el original pero con un -bwt al final, en la misma carpeta de origen.
