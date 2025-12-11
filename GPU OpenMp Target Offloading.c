#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//  Função GPU: aplica kernel em toda a imagem (exceto bordas) 
void aplicar_kernel_gpu(
    unsigned char *img,
    unsigned char *saida,
    float *kernel,
    int largura,
    int altura
) {
    int total = largura * altura;

    // Copiar bordas antes (CPU)
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

    //  EXECUÇÃO NA GPU VIA OPENMP TARGET
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

//  MAIN 
int main() {
    int largura, altura, canais;

    unsigned char *entrada =
        stbi_load("entrada.png", &largura, &altura, &canais, 3);

    if (!entrada) {
        printf("Erro ao carregar entrada.png\n");
        return 1;
    }

    printf("Imagem carregada: %dx%d (%d canais)\n",
           largura, altura, canais);

    unsigned char *saida = malloc(largura * altura * 3);

    // Kernel blur 3x3
    float kernel[9] = {
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f
    };

    double tempoInicio = omp_get_wtime();
    aplicar_kernel_gpu(entrada, saida, kernel, largura, altura);
    double tempoFinal = omp_get_wtime();

    printf("Tempo GPU (OpenMP target): %.4f ms\n", (tempoFinal - tempoInicio) * 1000);

    // Salvar PNG de saída
    if (!stbi_write_png("saida.png", largura, altura, 3, saida, largura * 3)) {
        printf("Erro ao salvar saida.png\n");
        return 1;
    }

    printf("Arquivo salvo: saida.png\n");

    stbi_image_free(entrada);
    free(saida);
    return 0;
}
