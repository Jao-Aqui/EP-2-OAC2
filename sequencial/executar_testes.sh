#!/bin/bash
#
# Script de testes automatizados para versão SEQUENCIAL
# Testa 3 resoluções: 512x512, 1024x1024, 4096x4096
# Executa 10 repetições para cada resolução
#

set -e  # Termina em caso de erro

echo "========================================"
echo "EP2 - TESTES VERSÃO SEQUENCIAL"
echo "========================================"
echo ""

# Cores para output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Diretório de trabalho
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
RESULTS_DIR="$SCRIPT_DIR/resultados"

echo "Diretório raiz: $ROOT_DIR"
echo "Diretório de resultados: $RESULTS_DIR"
echo ""

# Limpa resultados anteriores
rm -rf "$RESULTS_DIR"
mkdir -p "$RESULTS_DIR"

# Compila o código
echo "----------------------------------------"
echo "Compilando Sequencial.c..."
echo "----------------------------------------"
cd "$ROOT_DIR"
gcc -o sequencial_exe Sequencial.c -lm -I./stb

if [ ! -f "sequencial_exe" ]; then
    echo "ERRO: Falha na compilação!"
    exit 1
fi

echo -e "${GREEN}✓ Compilação bem-sucedida!${NC}"
echo ""

# Verifica se as imagens de teste existem
for size in 512 1024 4096; do
    if [ ! -f "$ROOT_DIR/input_${size}.png" ]; then
        echo "ERRO: Imagem input_${size}.png não encontrada!"
        echo "Execute: python3 gerar_imagens.py"
        exit 1
    fi
done

# Array com as resoluções
RESOLUCOES=(512 1024 4096)
NUM_REPETICOES=10

# Remove CSV resumo anterior para recriá-lo
rm -f "$ROOT_DIR/tempos_sequencial.csv"

# Loop pelas resoluções
for size in "${RESOLUCOES[@]}"; do
    echo "========================================"
    echo -e "${BLUE}Testando resolução: ${size}x${size}${NC}"
    echo "========================================"
    
    INPUT_FILE="$ROOT_DIR/input_${size}.png"
    OUTPUT_FILE="$RESULTS_DIR/output_sequencial_${size}.png"
    
    # Executa o programa
    "$ROOT_DIR/sequencial_exe" "$INPUT_FILE" "$OUTPUT_FILE" "$NUM_REPETICOES"
    
    echo ""
    echo -e "${GREEN}✓ Teste ${size}x${size} concluído!${NC}"
    echo ""
    
    # Move CSVs para pasta de resultados
    if [ -f "$ROOT_DIR/tempos_sequencial_${size}x${size}.csv" ]; then
        mv "$ROOT_DIR/tempos_sequencial_${size}x${size}.csv" "$RESULTS_DIR/"
    fi
done

# Move CSV resumo para pasta de resultados
if [ -f "$ROOT_DIR/tempos_sequencial.csv" ]; then
    mv "$ROOT_DIR/tempos_sequencial.csv" "$RESULTS_DIR/"
fi

echo "========================================"
echo -e "${GREEN}✓ TODOS OS TESTES CONCLUÍDOS!${NC}"
echo "========================================"
echo ""
echo "Resultados salvos em: $RESULTS_DIR"
echo ""
echo "Arquivos gerados:"
ls -lh "$RESULTS_DIR"
echo ""
echo "CSVs importantes:"
echo "  - Detalhado por resolução: tempos_sequencial_512x512.csv, etc."
echo "  - Resumo estatístico: tempos_sequencial.csv"
echo ""

# Limpa executável temporário
rm -f "$ROOT_DIR/sequencial_exe"

echo "Pronto para análise!"
