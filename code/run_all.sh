#!/bin/bash

# Carpeta donde están las instancias WCNF
INSTANCES_DIR="/home/user/TFM_Operadores_Cruce/mse24-anytime-unweighted/"

# Ruta al ejecutable del solver
SOLVER="./tt-open-wbo-inc-Glucose4_1"

# Tiempo máximo por instancia (en segundos)
TIMEOUT=600

# Carpeta para guardar los resultados (opcional)
OUTPUT_DIR="time_log"
mkdir -p "$OUTPUT_DIR"

# Recorrer todos los archivos .wcnf en la carpeta
for INSTANCE in "$INSTANCES_DIR"/*.wcnf; do
    INSTANCE_NAME=$(basename "$INSTANCE")
    OUTPUT_FILE="$OUTPUT_DIR/${INSTANCE_NAME%.wcnf}.out"

    echo "Ejecutando: $INSTANCE"
    
    # Ejecutar el solver con timeout y guardar salida
    timeout "$TIMEOUT" "$SOLVER" -verbosity=1 "$INSTANCE" > "$OUTPUT_FILE"

    # Verificar si timeout ocurrió
    if [ $? -eq 124 ]; then
        echo "Timeout en $INSTANCE_NAME" >> "$OUTPUT_FILE"
    fi
done
