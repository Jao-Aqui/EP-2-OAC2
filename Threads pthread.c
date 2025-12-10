#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 8

// Estrutura para enviar dados às threads
typedef struct {
    int id;
    int total_threads;
    int largura;
    int altura;
    int tam_kernel;
    float *imagem_entrada;
    float *imagem_saida;
    float *kernel;
} DadosThread;

// Função que aplica o kernel
float aplicar_kernel(float *imagem, float *kernel, int largura, int altura,
                     int tam_kernel, int x, int y) {
    int metade = tam_kernel / 2;
    float soma = 0.0f;

    for(int ky = -metade; ky <= metade; ky++) {
        for(int kx = -metade; kx <= metade; kx++) {
            int px = x + kx;
            int py = y + ky;

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

// Função executada por cada thread
void *funcao_thread(void *arg) {
    DadosThread *dados = (DadosThread *)arg;

    int inicio = (dados->altura / dados->total_threads) * dados->id;
    int fim = (dados->altura / dados->total_threads) * (dados->id + 1);

    if(dados->id == dados->total_threads - 1)
        fim = dados->altura;

    for(int y = inicio; y < fim; y++) {
        for(int x = 0; x < dados->largura; x++) {
            float valor = aplicar_kernel(
                dados->imagem_entrada,
                dados->kernel,
                dados->largura,
                dados->altura,
                dados->tam_kernel,
                x, y
            );

            dados->imagem_saida[y * dados->largura + x] = valor;
        }
    }

    return NULL;
}

int main() {
    int largura = 640;
    int altura = 480;
    int tam_kernel = 3;
    int num_threads = 4;

    pthread_t threads[MAX_THREADS];
    DadosThread dados[MAX_THREADS];

    float *imagem_entrada = calloc(largura * altura, sizeof(float));
    float *imagem_saida   = calloc(largura * altura, sizeof(float));

    // Kernel 3×3 simples
    float kernel[9] = {
        1/9.0f, 1/9.0f, 1/9.0f,
        1/9.0f, 1/9.0f, 1/9.0f,
        1/9.0f, 1/9.0f, 1/9.0f
    };

    for(int i = 0; i < largura * altura; i++)
        imagem_entrada[i] = (float)(rand() % 255);

    for(int i = 0; i < num_threads; i++) {
        dados[i].id = i;
        dados[i].total_threads = num_threads;
        dados[i].largura = largura;
        dados[i].altura = altura;
        dados[i].tam_kernel = tam_kernel;
        dados[i].imagem_entrada = imagem_entrada;
        dados[i].imagem_saida = imagem_saida;
        dados[i].kernel = kernel;

        pthread_create(&threads[i], NULL, funcao_thread, &dados[i]);
    }

    for(int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    printf("Convolução paralela concluída.\n");

    free(imagem_entrada);
    free(imagem_saida);
    return 0;
}
