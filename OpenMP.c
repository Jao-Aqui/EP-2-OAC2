/*
 * EP2 - Computação de Alto Desempenho
 * Versão OPENMP - Convolução 2D com kernel 3×3
 * Suporta 3 tipos
 * 
 * 1. SCHEDULE(STATIC):
 *    - Divide as iterações em blocos de tamanho fixo ANTES da execução
 *    - Cada thread recebe um bloco contíguo de linhas
 * 
 * 2. SCHEDULE(DYNAMIC):
 *    - Distribui iterações dinamicamente em TEMPO DE EXECUÇÃO
 *    - Threads pegam novas linhas conforme terminam seu trabalho
 * 
 * 3. COLLAPSE(2):
 *    - Colapsa 2 loops aninhados em um único loop paralelo
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>

// Aplica kernel 3×3 em um único pixel RGB
// Esta função é thread-safe pois cada thread trabalha em pixels diferentes
void aplicar_kernel_rgb(
    unsigned char *img,
    unsigned char *out,
    float *kernel,
    int largura, int altura,
    int x, int y
) {
    int half = 1;
    float soma_r = 0.0f;
    float soma_g = 0.0f;
    float soma_b = 0.0f;

    for (int ky = -half; ky <= half; ky++) {
        for (int kx = -half; kx <= half; kx++) {
            int px = x + kx;
            int py = y + ky;

            int idx = (py * largura + px) * 3;
            float kval = kernel[(ky + half) * 3 + (kx + half)];

            soma_r += img[idx]     * kval;
            soma_g += img[idx + 1] * kval;
            soma_b += img[idx + 2] * kval;
        }
    }

    int out_idx = (y * largura + x) * 3;
    out[out_idx]     = (unsigned char)soma_r;
    out[out_idx + 1] = (unsigned char)soma_g;
    out[out_idx + 2] = (unsigned char)soma_b;
}

int main(int argc, char *argv[]) {
    // Verifica argumentos de linha de comando
    if (argc < 5) {
        printf("Uso: %s <entrada> <saida> <threads> <estrategia> [repeticoes]\n", argv[0]);
        printf("Estratégias: static, dynamic, collapse\n");
        printf("Exemplo: %s input_512.png output.png 4 static 10\n", argv[0]);
        return 1;
    }

    char *arquivo_entrada = argv[1];
    char *arquivo_saida = argv[2];
    int num_threads = atoi(argv[3]);
    char *estrategia = argv[4];
    // Atoi transforma string em inteiro
    int num_repeticoes = (argc >= 6) ? atoi(argv[5]) : 10;

    if (num_threads < 1) {
        printf("Número de threads deve ser >= 1\n");
        return 1;
    }

    if (num_repeticoes < 1) {
        printf("Número de repetições deve ser >= 1\n");
        return 1;
    }

    // Valida estratégia
    if (strcmp(estrategia, "static") != 0 && 
        strcmp(estrategia, "dynamic") != 0 && 
        strcmp(estrategia, "collapse") != 0) {
        printf("Estratégia inválida! Use: static, dynamic ou collapse\n");
        return 1;
    }

    // Define o número de threads para OpenMP
    omp_set_num_threads(num_threads);

    int largura, altura, canais;

    // Carrega imagem PNG
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

    // Kernel 3×3 de blur
    float kernel[9] = {
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f
    };

    // Array para armazenar tempos de cada repetição
    double tempos[num_repeticoes];
    double inicio, fim;

    // Loop para medição de tempo
    for (int rep = 0; rep < num_repeticoes; rep++) {
        // Marca tempo de INÍCIO
        inicio = omp_get_wtime();

        // Paralelização com OpenMP - estratégia selecionada
        
        if (strcmp(estrategia, "static") == 0) {
            // Schedule(static)
            #pragma omp parallel for schedule(static)
            for (int y = 0; y < altura; y++) {
                for (int x = 0; x < largura; x++) {
                    int idx = (y * largura + x) * 3;
                    if (x == 0 || y == 0 || x == largura - 1 || y == altura - 1) {
                        saida[idx] = entrada[idx];
                        saida[idx + 1] = entrada[idx + 1];
                        saida[idx + 2] = entrada[idx + 2];
                        continue;
                    }
                    aplicar_kernel_rgb(entrada, saida, kernel, largura, altura, x, y);
                }
            }
        } 
        else if (strcmp(estrategia, "dynamic") == 0) {
            // Schedule(dynamic)
            #pragma omp parallel for schedule(dynamic)
            for (int y = 0; y < altura; y++) {
                for (int x = 0; x < largura; x++) {
                    int idx = (y * largura + x) * 3;
                    if (x == 0 || y == 0 || x == largura - 1 || y == altura - 1) {
                        saida[idx] = entrada[idx];
                        saida[idx + 1] = entrada[idx + 1];
                        saida[idx + 2] = entrada[idx + 2];
                        continue;
                    }
                    aplicar_kernel_rgb(entrada, saida, kernel, largura, altura, x, y);
                }
            }
        }
        else if (strcmp(estrategia, "collapse") == 0) {
            // Collapse(2)
            #pragma omp parallel for collapse(2)
            for (int y = 0; y < altura; y++) {
                for (int x = 0; x < largura; x++) {
                    int idx = (y * largura + x) * 3;
                    if (x == 0 || y == 0 || x == largura - 1 || y == altura - 1) {
                        saida[idx] = entrada[idx];
                        saida[idx + 1] = entrada[idx + 1];
                        saida[idx + 2] = entrada[idx + 2];
                        continue;
                    }
                    aplicar_kernel_rgb(entrada, saida, kernel, largura, altura, x, y);
                }
            }
        }

        // Marca tempo de FIM
        fim = omp_get_wtime();

        // Calcula tempo decorrido em segundos
        double tempo_decorrido = fim - inicio;
        tempos[rep] = tempo_decorrido;

        printf("Repetição %2d/%d: %.6f s\n", rep + 1, num_repeticoes, tempo_decorrido);
    }

    // Calcula média e desvio padrão dos tempos
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

    // Salva imagem de saída
    if (!stbi_write_png(arquivo_saida, largura, altura, 3, saida, largura * 3)) {
        printf("Erro ao salvar imagem de saída %s\n", arquivo_saida);
        free(saida);
        stbi_image_free(entrada);
        return 1;
    }

    printf("Imagem salva em: %s\n", arquivo_saida);

    // Exportar para CSV
    char csv_detalhado[256];
    char csv_consolidado[256];
    snprintf(csv_detalhado, sizeof(csv_detalhado), 
             "openmp/resultados/tempos_openmp_%s_%dx%d_t%d.csv", 
             estrategia, largura, altura, num_threads);
    snprintf(csv_consolidado, sizeof(csv_consolidado), 
             "openmp/resultados/tempos_openmp_consolidado.csv");

    // CSV DETALHADO: todas as repetições
    FILE *f_det = fopen(csv_detalhado, "w");
    if (f_det) {
        fprintf(f_det, "Repeticao,Largura,Altura,Threads,Estrategia,Tempo\n");
        for (int i = 0; i < num_repeticoes; i++) {
            fprintf(f_det, "%d,%d,%d,%d,%s,%.6f\n", 
                    i + 1, largura, altura, num_threads, estrategia, tempos[i]);
        }
        fclose(f_det);
        printf("Dados detalhados salvos em: %s\n", csv_detalhado);
    }

    // CSV CONSOLIDADO
    FILE *f_cons = fopen(csv_consolidado, "a");
    if (f_cons) {
        // Se arquivo está vazio, escreve cabeçalho
        fseek(f_cons, 0, SEEK_END);
        if (ftell(f_cons) == 0) {
            fprintf(f_cons, "Versao,Largura,Altura,Threads,Estrategia,TempoMedio,DesvioPadrao\n");
        }
        fprintf(f_cons, "OpenMP,%d,%d,%d,%s,%.6f,%.6f\n", 
                largura, altura, num_threads, estrategia, media, desvio_padrao);
        fclose(f_cons);
        printf("Resumo adicionado em: %s\n", csv_consolidado);
    }

    // Libera memória
    free(saida);
    stbi_image_free(entrada);

    return 0;
}
