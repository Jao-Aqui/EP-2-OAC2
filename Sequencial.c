/*
 * EP2 - Computação de Alto Desempenho
 * Versão SEQUENCIAL - Convolução 2D com kernel 3×3
 * 
 * Uso: ./sequencial <arquivo_entrada> <arquivo_saida> [num_repeticoes]
 * Exemplo: ./sequencial input_512.png output_512.png 10
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // Para medição de tempo
#include <math.h>   // Para cálculo de desvio padrão

// Aplica kernel 3×3 em um único pixel RGB
// NÃO é chamada para pixels de borda (bordas são copiadas diretamente)
void aplicar_kernel_rgb(
    unsigned char *img,
    unsigned char *out,
    float *kernel,
    int larguraAplica, int alturaAplica,
    int x, int y
) {
    int half = 1; // Raio do kernel 3×3
    float soma_r = 0.0f;
    float soma_g = 0.0f;
    float soma_b = 0.0f;

    // Percorre vizinhança 3×3 ao redor do pixel (x, y)
    for (int ky = -half; ky <= half; ky++) {
        for (int kx = -half; kx <= half; kx++) {
            int px = x + kx;
            int py = y + ky;

            int idx = (py * larguraAplica + px) * 3;
            float kval = kernel[(ky + half) * 3 + (kx + half)];

            soma_r += img[idx]     * kval;
            soma_g += img[idx + 1] * kval;
            soma_b += img[idx + 2] * kval;
        }
    }

    // Escreve resultado no pixel de saída
    int out_idx = (y * larguraAplica + x) * 3;
    out[out_idx]     = (unsigned char)soma_r;
    out[out_idx + 1] = (unsigned char)soma_g;
    out[out_idx + 2] = (unsigned char)soma_b;
}

int main(int argc, char *argv[]) {
    // Verifica argumentos de linha de comando
    if (argc < 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida> [num_repeticoes]\n", argv[0]);
        printf("Exemplo: %s input_512.png output_512.png 10\n", argv[0]);
        return 1;
    }

    char *arquivo_entrada = argv[1];
    char *arquivo_saida = argv[2];
    int num_repeticoes = (argc >= 4) ? atoi(argv[3]) : 10;

    if (num_repeticoes < 1) {
        printf("Número de repetições deve ser >= 1\n");
        return 1;
    }

    int largura, altura, canais;

    // Carrega imagem PNG (força 3 canais RGB)
    unsigned char *entrada = stbi_load(arquivo_entrada, &largura, &altura, &canais, 3);
    if (!entrada) {
        printf("Erro ao carregar %s\n", arquivo_entrada);
        return 1;
    }

    // Aloca imagem de saída
    unsigned char *saida = malloc(largura * altura * 3);
    if (!saida) {
        printf("Erro ao alocar memória para imagem de saída\n");
        stbi_image_free(entrada);
        return 1;
    }

    // Kernel 3×3 de blur (média simples)
    float kernel[9] = {
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f
    };

    // Array para armazenar tempos de cada repetição
    double tempos[num_repeticoes];
    struct timespec inicio, fim;

    // ============================================
    // LOOP DE REPETIÇÕES PARA MEDIÇÃO DE TEMPO
    // ============================================
    for (int rep = 0; rep < num_repeticoes; rep++) {
        // Marca tempo de INÍCIO (apenas da convolução, não do I/O)
        clock_gettime(CLOCK_MONOTONIC, &inicio);

        for (int y = 0; y < altura; y++) {
            for (int x = 0; x < largura; x++) {
                int idx = (y * largura + x) * 3;

                // Pixels de borda: copia sem aplicar kernel
                if (x == 0 || y == 0 || x == largura - 1 || y == altura - 1) {
                    saida[idx]     = entrada[idx];
                    saida[idx + 1] = entrada[idx + 1];
                    saida[idx + 2] = entrada[idx + 2];
                    continue;
                }

                // Pixels internos: aplica convolução 3×3
                aplicar_kernel_rgb(entrada, saida, kernel, largura, altura, x, y);
            }
        }

        // Marca tempo de FIM
        clock_gettime(CLOCK_MONOTONIC, &fim);

        // Calcula tempo decorrido em segundos
        double tempo_decorrido = (fim.tv_sec - inicio.tv_sec) + 
                                 (fim.tv_nsec - inicio.tv_nsec) / 1e9;
        tempos[rep] = tempo_decorrido;

        printf("Repetição %2d/%d: %.6f s\n", rep + 1, num_repeticoes, tempo_decorrido);
    }

    // ============================================
    // CÁLCULO DE ESTATÍSTICAS
    // ============================================
    double soma = 0.0;
    for (int i = 0; i < num_repeticoes; i++) {
        soma += tempos[i];
    }
    double media = soma / num_repeticoes;

    double soma_quadrados = 0.0;
    for (int i = 0; i < num_repeticoes; i++) {
        double diff = tempos[i] - media;
        soma_quadrados += diff * diff;
    }
    double desvio_padrao = sqrt(soma_quadrados / num_repeticoes);

    // ============================================
    // SALVA IMAGEM DE SAÍDA
    // ============================================
    if (!stbi_write_png(arquivo_saida, largura, altura, 3, saida, largura * 3)) {
        printf("Erro ao salvar %s\n", arquivo_saida);
        stbi_image_free(entrada);
        free(saida);
        return 1;
    }
    printf("Imagem salva: %s\n", arquivo_saida);

    // ============================================
    // EXPORTA RESULTADOS PARA CSV
    // ============================================
    
    // CSV 1: Dados detalhados (todas as repetições)
    char arquivo_csv_consolidado[256];
    snprintf(arquivo_csv_consolidado, sizeof(arquivo_csv_consolidado), 
             "tempos_sequencial_%dx%d.csv", largura, altura);
    
    FILE *csv_consolidado = fopen(arquivo_csv_consolidado, "w");
    if (csv_consolidado) {
        fprintf(csv_consolidado, "Resolucao,Repeticao,Tempo_s\n");
        for (int i = 0; i < num_repeticoes; i++) {
            fprintf(csv_consolidado, "%dx%d,%d,%.6f\n", largura, altura, i + 1, tempos[i]);
        }
        fclose(csv_consolidado);
        printf("CSV consolidado: %s\n", arquivo_csv_consolidado);
    }
    
    // CSV 2: Resumo estatístico
    char arquivo_csv_resumo[256];
    snprintf(arquivo_csv_resumo, sizeof(arquivo_csv_resumo), 
             "tempos_sequencial.csv");
    
    // Calcula tempo total
    double tempo_total = soma;
    
    // Verifica se arquivo já existe para adicionar header
    FILE *csv_resumo = fopen(arquivo_csv_resumo, "r");
    int arquivo_existe = (csv_resumo != NULL);
    if (csv_resumo) fclose(csv_resumo);
    
    // Abre para append ou escrita
    csv_resumo = fopen(arquivo_csv_resumo, arquivo_existe ? "a" : "w");
    if (csv_resumo) {
        // Escreve header apenas se arquivo não existia
        if (!arquivo_existe) {
            fprintf(csv_resumo, "Versao,Largura,Altura,TempoTotal,TempoMedio,DesvioPadrao\n");
        }
        fprintf(csv_resumo, "Sequencial,%d,%d,%.6f,%.6f,%.6f\n", 
                largura, altura, tempo_total, media, desvio_padrao);
        fclose(csv_resumo);
        printf("CSV resumo: %s\n", arquivo_csv_resumo);
    }

    // Libera memória
    stbi_image_free(entrada);
    free(saida);

    return 0;
}

