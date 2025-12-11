#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

// Aplica kernel 3×3 em 1 pixel RGB
// NÃO é chamada para pixels de borda!
void aplicar_kernel_rgb(
    unsigned char *img,
    unsigned char *out,
    float *kernel,
    int larguraAplica, int alturaAplica,
    int x, int y
) {
    int half = 1; // kernel 3x3
    float soma_r = 0.0f;
    float soma_g = 0.0f;
    float soma_b = 0.0f;

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

    int out_idx = (y * larguraAplica + x) * 3;
    out[out_idx]     = (unsigned char)soma_r;
    out[out_idx + 1] = (unsigned char)soma_g;
    out[out_idx + 2] = (unsigned char)soma_b;
}

int main() {
    int largura, altura, canais;

    // Carrega PNG RGB, canal 3= RGB
    unsigned char *entrada = stbi_load("entrada.png", &largura, &altura, &canais, 3);
    if (!entrada) {
        printf("Erro ao carregar entrada.png\n");
        return 1;
    }

    unsigned char *saida = malloc(largura * altura * 3);
    if (!saida) {
        printf("Erro ao alocar imagem de saída.\n");
        stbi_image_free(entrada);
        return 1;
    }

    // Kernel 3×3 oferecido (blur)
    float kernel[9] = {
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f
    };

    // Percorre a imagem
 for (int y = 0; y < altura; y++) {
    for (int x = 0; x < largura; x++) {

        int idx = (y * largura + x) * 3;

        // Se é pixel de borda  para a imagem resultado sem aplicar o kernel
        if (x == 0 || y == 0 || x == largura - 1 || y == altura - 1) {
            saida[idx]     = entrada[idx];
            saida[idx + 1] = entrada[idx + 1];
            saida[idx + 2] = entrada[idx + 2];
            continue;
        }

        // Aplica kernel nos pixels internos
        aplicar_kernel_rgb(
            entrada,
            saida,
            kernel,
            largura,
            altura,
            x, y
        );
    }
}


    // Salva PNG de saída
    if (!stbi_write_png("output.png", largura, altura, 3, saida, largura * 3)) {
        printf("Erro ao salvar output.png\n");
    } else {
        printf("output.png gerado com sucesso.\n");
    }

    stbi_image_free(entrada);
    free(saida);
    return 0;
}

