#!/bin/bash

LOG_FILE="testes_output.log"
CSV_FILE="resultados/tempos_openmp_consolidado.csv"

echo "Versao,Largura,Altura,Threads,Estrategia,TempoMedio,DesvioPadrao" > "$CSV_FILE"

LARGURA=""
THREADS=""
ESTRATEGIA=""
TEMPO_MEDIO=""
DESVIO=""

while IFS= read -r linha; do
    if [[ $linha =~ Imagem\ carregada:\ ([0-9]+)x([0-9]+) ]]; then
        LARGURA="${BASH_REMATCH[1]}"
    fi
    
    if [[ $linha =~ Threads:\ ([0-9]+).*Estratégia:\ ([a-z]+) ]]; then
        THREADS="${BASH_REMATCH[1]}"
        ESTRATEGIA="${BASH_REMATCH[2]}"
    fi
    
    if [[ $linha =~ Tempo\ médio:\ ([0-9.]+) ]]; then
        TEMPO_MEDIO="${BASH_REMATCH[1]}"
    fi
    
    if [[ $linha =~ Desvio\ padrão:\ ([0-9.]+) ]]; then
        DESVIO="${BASH_REMATCH[1]}"
        
        if [ -n "$LARGURA" ] && [ -n "$THREADS" ] && [ -n "$ESTRATEGIA" ] && [ -n "$TEMPO_MEDIO" ] && [ -n "$DESVIO" ]; then
            echo "OpenMP,$LARGURA,$LARGURA,$THREADS,$ESTRATEGIA,$TEMPO_MEDIO,$DESVIO" >> "$CSV_FILE"
        fi
    fi
done < "$LOG_FILE"

NUM_RESULTADOS=$(($(wc -l < "$CSV_FILE") - 1))

echo "✓ CSV consolidado criado: $CSV_FILE"
echo "✓ Total de resultados extraídos: $NUM_RESULTADOS"
