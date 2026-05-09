#!/usr/bin/env bash
set -uo pipefail

# Auto-detect root directory
if [[ -d ./src && -d ./docs ]]; then
  cd "$(pwd)"
elif [[ -d "Numero 1/src" && -d "Numero 1/docs" ]]; then
  cd "Numero 1"
else
  cd "$(dirname "${BASH_SOURCE[0]}")"
fi

ROOT_DIR="."
SRC_DIR="./src"
DOCS_DIR="./docs"
BUILD_DIR="./build"
RESULTS_DIR="./results/experimentos"
MAIN_BIN="./build/main"

echo "DEBUG: ROOT_DIR=$ROOT_DIR"
echo "DEBUG: SRC_DIR=$SRC_DIR"
echo "DEBUG: DOCS_DIR=$DOCS_DIR"

IMPLS=(Balanced Huff BruteForce Propia)
TEXTS=(dna english sources)
FIXED_PATTERN_LEN=64
PATTERN_LENGTHS=(32 64 128 256 512)
PATTERN_COUNT=100

mkdir -p "$BUILD_DIR" "$RESULTS_DIR"

compile_main() {
  g++ -std=c++17 -O2 \
    -o "$MAIN_BIN" \
    "$SRC_DIR/main.cpp" \
    "$SRC_DIR/LectorDocumentos.cpp" \
    "$SRC_DIR/bitvector_constructors.cpp" \
    "$SRC_DIR/bitvector_others.cpp" \
    "$SRC_DIR/bitvector_rank.cpp" \
    "$SRC_DIR/bitvector_select_1.cpp" \
    "$SRC_DIR/bitvector_select_2.cpp" \
    "$SRC_DIR/wtree_constructors.cpp" \
    "$SRC_DIR/wtree_operations.cpp" \
    -I"$ROOT_DIR/include" \
    -I/mnt/c/Users/chips/include \
    -L/mnt/c/Users/chips/lib \
    -lsdsl
}

make_patterns() {
  local text_file="$1"
  local count="$2"
  local length="$3"
  local output_file="$4"

  python3 - "$text_file" "$count" "$length" "$output_file" <<'PY'
import pathlib
import sys

text_path = pathlib.Path(sys.argv[1])
count = int(sys.argv[2])
length = int(sys.argv[3])
out_path = pathlib.Path(sys.argv[4])

# Leer archivo en binario para preservar bytes exactos
data = text_path.read_bytes()

if len(data) < length:
    raise SystemExit(f'El texto {text_path} ({len(data)} bytes) no alcanza para patrones de largo {length}')

# Extraer patrones secuenciales del archivo (garantiza que existen)
patterns = []
step = max(1, len(data) // count)
for i in range(0, len(data) - length, step):
    if len(patterns) >= count:
        break
    patterns.append(data[i:i + length])

# Convertir a strings decodificando lo mejor posible
pattern_strs = []
for p in patterns:
    try:
        s = p.decode('utf-8')
    except:
        s = p.decode('latin-1')
    pattern_strs.append(s)

out_path.write_text('\n'.join(pattern_strs) + ('\n' if pattern_strs else ''), encoding='utf-8')
print(f"Generados {len(pattern_strs)} patrones de largo {length} de {text_path}", file=sys.stderr)
PY
}

run_case() {
  local scenario="$1"
  local source_name="$2"
  local bwt_name="$3"
  local length="$4"
  local patterns_file="$5"
  local csv_file="$6"

  make_patterns "$DOCS_DIR/$source_name" "$PATTERN_COUNT" "$length" "$patterns_file"

  # Asegurar que no haya más de PATTERN_COUNT líneas en el archivo de patrones
  if [[ -f "$patterns_file" ]]; then
    total_lines=$(wc -l < "$patterns_file" || echo 0)
    if (( total_lines > PATTERN_COUNT )); then
      echo "DEBUG: Recortando $patterns_file de $total_lines a $PATTERN_COUNT patrones"
      head -n "$PATTERN_COUNT" "$patterns_file" > "${patterns_file}.tmp" && mv "${patterns_file}.tmp" "$patterns_file"
    fi
  fi

  if [[ ! -f "$csv_file" ]]; then
    printf 'experiment_type;implementation;dataset;pattern_length;construction_time_us;space_bytes;patron_index;tiempo_microsegundos;coincidencias\n' > "$csv_file"
  fi

  for impl in "${IMPLS[@]}"; do
    local bwt_path="$DOCS_DIR/$bwt_name"
    if [[ ! -f "$bwt_path" ]]; then
      echo "ERROR: Archivo BWT no encontrado: $bwt_path" >&2
      continue
    fi
    echo "DEBUG: Ejecutando $impl con BWT=$bwt_path, patrones=$patterns_file"
    
    # Ejecutar main y procesar salida
    local first_header=true
    local construction_time_us=""
    local space_bytes=""
    while IFS= read -r line; do
      # Capturar línea METADATA
      if [[ $line =~ ^METADATA\; ]]; then
        IFS=';' read -r _ construction_time_us space_bytes <<< "$line"
        continue
      fi
      # Si es línea de encabezado, saltarla la primera vez
      if [[ $line =~ ^patron_index\; ]]; then
        if [[ $first_header == true ]]; then
          first_header=false
          continue
        fi
      fi
      # Si es línea de tiempo individual (número;tiempo;coincidencias)
      if [[ $line =~ ^[0-9]+\; ]]; then
        printf '%s;%s;%s;%s;%s;%s;%s\n' "$scenario" "$impl" "$source_name" "$length" "$construction_time_us" "$space_bytes" "$line" >> "$csv_file"
      fi
    done < <($MAIN_BIN "$impl" "$bwt_path" "$patterns_file")
  done
}

compile_main

echo 'Escenario 1: largo fijo de patrón y textos distintos'
FIXED_CSV="$RESULTS_DIR/largo_fijo_patron.csv"
: > "$FIXED_CSV"
for text_name in "${TEXTS[@]}"; do
  run_case \
    "largo_fijo_patron" \
    "$text_name" \
    "${text_name}-bwt" \
    "$FIXED_PATTERN_LEN" \
    "$RESULTS_DIR/patrones_${text_name}_${FIXED_PATTERN_LEN}.txt" \
    "$FIXED_CSV"
done

echo 'Escenario 2: largo fijo de texto y patrones de distintos tamaños'
FIXED_TEXT="english"
FIXED_TEXT_CSV="$RESULTS_DIR/largo_fijo_texto.csv"
: > "$FIXED_TEXT_CSV"
for pattern_length in "${PATTERN_LENGTHS[@]}"; do
  run_case \
    "largo_fijo_texto" \
    "$FIXED_TEXT" \
    "${FIXED_TEXT}-bwt" \
    "$pattern_length" \
    "$RESULTS_DIR/patrones_${FIXED_TEXT}_${pattern_length}.txt" \
    "$FIXED_TEXT_CSV"
done

echo "Listo. Resultados en: $RESULTS_DIR"
