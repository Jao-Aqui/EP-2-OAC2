#!/bin/bash

# ========================================
# Script de Teste Completo - Threads pthread
# ========================================
# Este script executa todos os testes necessários conforme o enunciado:
# - 3 resoluções (512, 1024, 4096)
# - Número variável de threads (1, 2, 4, 8, 16)
# - 10 repetições por configuração (feito internamente pelo programa)

echo "========================================"
echo "TESTES - Threads pthread"
echo "========================================"
echo ""

# Compilar o programa
echo "Compilando Threads pthread.c..."
gcc -o threads "../Threads pthread.c" -lpthread -lm
if [ $? -ne 0 ]; then
    echo "Erro na compilação!"
    exit 1
fi
echo "✅ Compilação concluída"
echo ""

# Limpar CSV anterior
rm -f tempos_threads.csv
echo "Versao,Largura,Altura,Threads,TempoMedio,DesvioPadrao" > tempos_threads.csv

# Array de resoluções
resolucoes=("512" "1024" "4096")

# Array de números de threads (1, 2, 4, 8, 16)
threads_configs=(1 2 4 8 16)

# Contador de testes
total_testes=$((${#resolucoes[@]} * ${#threads_configs[@]}))
teste_atual=0

# Loop pelas resoluções
for res in "${resolucoes[@]}"; do
    echo "----------------------------------------"
    echo "Testando resolução ${res}×${res}"
    echo "----------------------------------------"
    
    # Loop pelos números de threads
    for num_threads in "${threads_configs[@]}"; do
        teste_atual=$((teste_atual + 1))
        echo ""
        echo "[$teste_atual/$total_testes] Executando: ${res}×${res} com $num_threads thread(s)..."
        
        # Executar o programa
        ./threads "/home/kaua/faculdade/EP-2-OAC2/input_${res}.png" "resultados/output_${res}_threads${num_threads}.png" $num_threads
        
        if [ $? -eq 0 ]; then
            echo "✅ Teste concluído"
        else
            echo "❌ Erro no teste"
        fi
    done
    echo ""
done

echo ""
echo "========================================"
echo "TODOS OS TESTES CONCLUÍDOS!"
echo "========================================"
echo ""
echo "📊 Resultados salvos em:"
echo "   - tempos_threads.csv (dados brutos)"
echo "   - resultados/ (imagens processadas)"
echo ""

# Mover CSV para pasta de resultados
mv tempos_threads.csv resultados/

# Exibir resumo dos dados
echo "📈 Resumo dos tempos (por threads):"
echo "-----------------------------------"
cat resultados/tempos_threads.csv
echo ""

echo "✅ Script concluído com sucesso!"
