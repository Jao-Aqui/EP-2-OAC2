#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

void aplicar_kernel_gpu(
    unsigned char *img,
    unsigned char *saida,
    float *kernel,
    int largura,
    int altura
) {
    int total = largura * altura;

    // Copiar bordas primeiro (na CPU)
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            if (x == 0 || y == 0 || x == largura - 1 || y == altura - 1) {
                int idx = (y * largura + x) * 3;
                saida[idx]     = img[idx];
                saida[idx + 1] = img[idx + 1];
                saida[idx + 2] = img[idx + 2];
            }
        }
    }

    #pragma omp target teams distribute parallel for \
            map(to: img[0:total*3], kernel[0:9]) \
            map(from: saida[0:total*3])
    for (int y = 1; y < altura - 1; y++) {
        for (int x = 1; x < largura - 1; x++) {

            float somaR = 0, somaG = 0, somaB = 0;

            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {

                    int px = x + kx;
                    int py = y + ky;

                    int idx = (py * largura + px) * 3;
                    float k = kernel[(ky + 1) * 3 + (kx + 1)];

                    somaR += img[idx]     * k;
                    somaG += img[idx + 1] * k;
                    somaB += img[idx + 2] * k;
                }
            }

            int out = (y * largura + x) * 3;
            saida[out]     = (unsigned char) somaR;
            saida[out + 1] = (unsigned char) somaG;
            saida[out + 2] = (unsigned char) somaB;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <entrada> <saida> [repeticoes]\n", argv[0]);
        printf("Exemplo: %s input_512.png output.png 10\n", argv[0]);
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

    // Kernel blur 3×3
    float kernel[9] = {
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f
    };

    // Array para armazenar tempos de cada repetição
    double tempos[num_repeticoes];

    // Loop para medição de tempo
    for (int rep = 0; rep < num_repeticoes; rep++) {
        // Marca tempo de INÍCIO
        double inicio = omp_get_wtime();

        // Executa convolução na GPU
        aplicar_kernel_gpu(entrada, saida, kernel, largura, altura);

        // Marca tempo de FIM
        double fim = omp_get_wtime();

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
             "gpu/resultados/tempos_gpu_%dx%d.csv", 
             largura, altura);
    snprintf(csv_consolidado, sizeof(csv_consolidado), 
             "gpu/resultados/tempos_gpu_consolidado.csv");

    // CSV DETALHADO: todas as repetições
    FILE *f_det = fopen(csv_detalhado, "w");
    if (f_det) {
        fprintf(f_det, "Repeticao,Largura,Altura,Tempo\n");
        for (int i = 0; i < num_repeticoes; i++) {
            fprintf(f_det, "%d,%d,%d,%.6f\n", 
                    i + 1, largura, altura, tempos[i]);
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
            fprintf(f_cons, "Versao,Largura,Altura,TempoMedio,DesvioPadrao\n");
        }
        fprintf(f_cons, "GPU,%d,%d,%.6f,%.6f\n", 
                largura, altura, media, desvio_padrao);
        fclose(f_cons);
        printf("Resumo adicionado em: %s\n", csv_consolidado);
    }

    // Libera memória
    free(saida);
    stbi_image_free(entrada);

    return 0;
}
