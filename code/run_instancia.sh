#!/bin/bash

# Ruta a la instancia WCNF específica
INSTANCE="/home/user/TFM_Operadores_Cruce/mse24-anytime-unweighted/minimize-5gons-u5-cube-maxcdcl-44-tmp-15-2655746.wcnf"

# Ruta al ejecutable del solver
SOLVER="./tt-open-wbo-inc-Glucose4_1"

# Tiempo máximo por instancia (en segundos)
TIMEOUT=120

# Carpeta para guardar los resultados
OUTPUT_DIR="time_log"
mkdir -p "$OUTPUT_DIR"

# Nombre del archivo de salida
INSTANCE_NAME=$(basename "$INSTANCE")
OUTPUT_FILE="$OUTPUT_DIR/${INSTANCE_NAME%.wcnf}.out"

echo "Ejecutando: $INSTANCE"

# Ejecutar el solver con timeout usando /usr/bin/time para capturar métricas
/usr/bin/time -v timeout "$TIMEOUT" "$SOLVER" -verbosity=1 "$INSTANCE" > "$OUTPUT_FILE" 2> "$OUTPUT_FILE.time.log"

# Verificar si timeout ocurrió
if [ $? -eq 124 ]; then
    echo "Timeout en $INSTANCE_NAME" >> "$OUTPUT_FILE"
fi

# Extraer métricas desde el log de /usr/bin/time
WCTIME=$(grep "Elapsed (wall clock) time" "$OUTPUT_FILE.time.log" | awk '{print $8}')

USERTIME=$(grep "User time (seconds)" "$OUTPUT_FILE.time.log" | awk '{print $4}')
SYSTEMTIME=$(grep "System time (seconds)" "$OUTPUT_FILE.time.log" | awk '{print $4}')
CPUTIME=$(echo "$USERTIME + $SYSTEMTIME" | bc)
CPUUSAGE=$(echo "scale=2; $CPUTIME / $WCTIME * 100" | bc)
MAXVM=$(grep "Maximum resident set size" "$OUTPUT_FILE.time.log" | awk '{print $6}')

echo "WCTIME=$WCTIME s"
echo "CPUTIME=$CPUTIME s"
echo "USERTIME=$USERTIME s"
echo "SYSTEMTIME=$SYSTEMTIME s"
echo "CPUUSAGE=$CPUUSAGE %"
echo "MAXVM=$MAXVM KiB"
