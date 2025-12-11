#!/bin/bash

# Script para executar testes de TODAS as 3 estratégias OpenMP
# Testa: static, dynamic, collapse
# 3 resoluções × 3 estratégias × 5 configurações de threads = 45 testes

echo "========================================"
echo "TESTES COMPLETOS - OpenMP (3 Estratégias)"
echo "========================================"
echo ""

# Verifica se o executável existe
if [ ! -f "../openmp_exec" ]; then
    echo "ERRO: Executável '../openmp_exec' não encontrado!"
    echo "Compile com: gcc -fopenmp -O3 -o openmp_exec OpenMP.c -lm -I./stb"
    exit 1
fi

# Configurações de teste
RESOLUCOES=("512" "1024" "4096")
THREADS=(1 2 4 8 16)
ESTRATEGIAS=("static" "dynamic" "collapse")
REPETICOES=10

# Limpa arquivo CSV consolidado
rm -f resultados/tempos_openmp_consolidado.csv

echo "Iniciando bateria de testes..."
echo "Resoluções: ${RESOLUCOES[@]}"
echo "Threads: ${THREADS[@]}"
echo "Estratégias: ${ESTRATEGIAS[@]}"
echo "Repetições por teste: $REPETICOES"
echo ""

TESTE_ATUAL=0
TOTAL_TESTES=$((${#RESOLUCOES[@]} * ${#THREADS[@]} * ${#ESTRATEGIAS[@]}))

# Loop por cada estratégia
for ESTRATEGIA in "${ESTRATEGIAS[@]}"; do
    echo ""
    echo "###################################"
    echo "# ESTRATÉGIA: $ESTRATEGIA"
    echo "###################################"
    echo ""
    
    # Loop por cada resolução
    for RES in "${RESOLUCOES[@]}"; do
        INPUT="../input_${RES}.png"
        
        if [ ! -f "$INPUT" ]; then
            echo "AVISO: Imagem $INPUT não encontrada, pulando..."
            continue
        fi
        
        # Loop por cada configuração de threads
        for THR in "${THREADS[@]}"; do
            TESTE_ATUAL=$((TESTE_ATUAL + 1))
            OUTPUT="resultados/output_openmp_${ESTRATEGIA}_${RES}_t${THR}.png"
            
            echo "----------------------------------------"
            echo "Teste $TESTE_ATUAL/$TOTAL_TESTES: ${RES}x${RES} | $THR thread(s) | $ESTRATEGIA"
            echo "----------------------------------------"
            
            # Executa o teste
            ../openmp_exec "$INPUT" "$OUTPUT" $THR "$ESTRATEGIA" $REPETICOES
            
            if [ $? -eq 0 ]; then
                echo "✓ Teste concluído com sucesso"
            else
                echo "✗ Erro no teste"
            fi
            
            echo ""
        done
    done
done

echo ""
echo "========================================"
echo "TODOS OS TESTES CONCLUÍDOS!"
echo "========================================"
echo ""
echo "Total de testes executados: $TESTE_ATUAL"
echo "Total de execuções do algoritmo: $((TESTE_ATUAL * REPETICOES))"
echo ""
echo "Resultados salvos em:"
echo "  - Imagens: openmp/resultados/output_*.png"
echo "  - CSV consolidado: openmp/resultados/tempos_openmp_consolidado.csv"
echo "  - CSVs detalhados: openmp/resultados/tempos_openmp_*_*.csv"
echo ""
