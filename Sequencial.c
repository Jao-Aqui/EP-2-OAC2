#include <stdio.h>
#include <stdlib.h>

// Aplica o kernel a um pixel específico
float aplicar_kernel(float *imagem, float *kernel, int largura, int altura,
                     int tam_kernel, int x, int y) {
    int metade = tam_kernel / 2;
    float soma = 0.0f;

    for(int ky = -metade; ky <= metade; ky++) {
        for(int kx = -metade; kx <= metade; kx++) {
            int px = x + kx;
            int py = y + ky;

            // Ignorar pixels fora dos limites da imagem
            if(px < 0 || px >= largura || py < 0 || py >= altura)
                continue;

            float valor_pixel = imagem[py * largura + px];
            float valor_kernel = kernel[(ky + metade) * tam_kernel +
                                       (kx + metade)];

            soma += valor_pixel * valor_kernel;
        }
    }

    return soma;
}

int main() {
    int largura = 640;
    int altura = 480;
    int tam_kernel = 3;

    float *imagem_entrada = calloc(largura * altura, sizeof(float));
    float *imagem_saida   = calloc(largura * altura, sizeof(float));

    // Kernel simples de desfoque 3×3
    float kernel[9] = {
        1/9.0f, 1/9.0f, 1/9.0f,
        1/9.0f, 1/9.0f, 1/9.0f,
        1/9.0f, 1/9.0f, 1/9.0f
    };

    // Inicialização da imagem com valores aleatórios
    for(int i = 0; i < largura * altura; i++)
        imagem_entrada[i] = (float)(rand() % 255);

    // Convolução sequencial
    for(int y = 0; y < altura; y++) {
        for(int x = 0; x < largura; x++) {
            imagem_saida[y * largura + x] =
                aplicar_kernel(imagem_entrada, kernel, largura, altura,
                               tam_kernel, x, y);
        }
    }

    printf("Convolução sequencial concluída.\n");

    free(imagem_entrada);
    free(imagem_saida);
    return 0;
}
