#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define MAX_THREADS 16

// ========================================
// ESTRUTURA DE DADOS
// ========================================
// Esta estrutura armazena todos os dados que cada thread precisa
// para processar sua parte da imagem
typedef struct {
    int id;
    int total_threads;
    int largura;
    int altura;
    unsigned char *imagem_entrada;
    unsigned char *imagem_saida;
    float *kernel;
} DadosThread;

// ========================================
// FUNÇÃO DE CONVOLUÇÃO
// ========================================
// Aplica o kernel 3×3 em um único pixel RGB
// Esta é a operação matemática central do blur:
// - Multiplica cada vizinho pelo valor correspondente do kernel
// - Soma todos os resultados para cada canal (R, G, B)
void aplicar_kernel_rgb(
    unsigned char *img,
    unsigned char *saida,
    float *kernel,
    int largura,
    int altura,
    int x,
    int y
) {
    int metade = 1;
    
    // Acumuladores para cada canal de cor
    float soma_vermelho = 0.0f;
    float soma_verde = 0.0f;
    float soma_azul = 0.0f;

    // Percorre a vizinhança 3×3 ao redor do pixel (x, y)
    for (int ky = -metade; ky <= metade; ky++) {
        for (int kx = -metade; kx <= metade; kx++) {
            // Calcula coordenadas do pixel vizinho
            int px = x + kx;
            int py = y + ky;

            // Calcula índice no array da imagem (RGB tem 3 bytes por pixel)
            int indice = (py * largura + px) * 3;
            
            // Pega o valor correspondente do kernel
            float valor_kernel = kernel[(ky + metade) * 3 + (kx + metade)];

            // Multiplica cada canal pelo kernel e acumula
            soma_vermelho += img[indice]     * valor_kernel;
            soma_verde    += img[indice + 1] * valor_kernel;
            soma_azul     += img[indice + 2] * valor_kernel;
        }
    }

    // Grava o resultado na imagem de saída
    int indice_saida = (y * largura + x) * 3;
    saida[indice_saida] = (unsigned char)soma_vermelho;
    saida[indice_saida + 1] = (unsigned char)soma_verde;
    saida[indice_saida + 2] = (unsigned char)soma_azul;
}

// ========================================
// FUNÇÃO EXECUTADA POR CADA THREAD
// ========================================
// Cada thread processa um conjunto de linhas da imagem
// Exemplo: se há 4 threads e 100 linhas:
//   Thread 0: linhas 0-24
//   Thread 1: linhas 25-49
//   Thread 2: linhas 50-74
//   Thread 3: linhas 75-99
void *funcao_thread(void *arg) {
    // Converte o argumento genérico para nossa estrutura
    DadosThread *dados = (DadosThread *)arg;

    // Calcula qual faixa de linhas esta thread vai processar
    int linha_inicio = (dados->altura / dados->total_threads) * dados->id;
    int linha_fim = (dados->altura / dados->total_threads) * (dados->id + 1);

    // A última thread pega qualquer linha residual
    if(dados->id == dados->total_threads - 1)
        linha_fim = dados->altura;

    // Percorre as linhas atribuídas a esta thread
    for(int y = linha_inicio; y < linha_fim; y++) {
        // Percorre todas as colunas da linha
        for(int x = 0; x < dados->largura; x++) {
            int indice = (y * dados->largura + x) * 3;

            // TRATAMENTO DE BORDAS:
            // Pixels da borda não têm vizinhos completos para convolução
            // Solução: copiar sem modificar
            if (x == 0 || y == 0 || 
                x == dados->largura - 1 || y == dados->altura - 1) {
                dados->imagem_saida[indice]     = dados->imagem_entrada[indice];
                dados->imagem_saida[indice + 1] = dados->imagem_entrada[indice + 1];
                dados->imagem_saida[indice + 2] = dados->imagem_entrada[indice + 2];
                continue;
            }

            // PIXELS INTERNOS: aplica a convolução 3×3
            aplicar_kernel_rgb(
                dados->imagem_entrada,
                dados->imagem_saida,
                dados->kernel,
                dados->largura,
                dados->altura,
                x, y
            );
        }
    }

    return NULL;
}

// ========================================
// FUNÇÃO PRINCIPAL
// ========================================
int main(int argc, char *argv[]) {
    // ----------------------------------------
    // ETAPA 1: VALIDAÇÃO DE ARGUMENTOS
    // ----------------------------------------
    // O programa requer 3 argumentos:
    //   1. Arquivo de entrada (PNG)
    //   2. Arquivo de saída (PNG)
    //   3. Número de threads a usar
    
    // Se não houver 4 argumentos (programa + 3) nao roda o programa
    if (argc != 4) {
        printf("Uso: %s <input.png> <output.png> <num_threads>\n", argv[0]);
        printf("Exemplo: %s input_512.png output_threads_512.png 4\n", argv[0]);
        return 1;
    }

    char *arquivo_entrada = argv[1];
    char *arquivo_saida = argv[2];
    int num_threads = atoi(argv[3]);

    // Validar número de threads
    if (num_threads < 1 || num_threads > MAX_THREADS) {
        printf("Número de threads deve estar entre 1 e %d\n", MAX_THREADS);
        return 1;
    }

    // ----------------------------------------
    // ETAPA 2: CARREGAR IMAGEM PNG
    // ----------------------------------------
    // stbi_load carrega a imagem e retorna um array de bytes
    // O último parâmetro (3) força a conversão para RGB
    int largura, altura, canais;
    unsigned char *imagem_entrada = stbi_load(arquivo_entrada, &largura, &altura, &canais, 3);
    if (!imagem_entrada) {
        printf("Erro ao carregar %s\n", arquivo_entrada);
        return 1;
    }

    printf("Imagem carregada: %dx%d pixels, usando %d thread(s)\n", largura, altura, num_threads);

    // ----------------------------------------
    // ETAPA 3: ALOCAR MEMÓRIA PARA SAÍDA
    // ----------------------------------------
    // Precisamos de largura × altura × 3 bytes
    // Isso pois cada pixel RGB tem 3 canais (R, G, B)
    unsigned char *imagem_saida = malloc(largura * altura * 3);
    if (!imagem_saida) {
        printf("Erro ao alocar memória para saída\n");
        stbi_image_free(imagem_entrada);
        return 1;
    }

    // ----------------------------------------
    // ETAPA 4: DEFINIR KERNEL DE CONVOLUÇÃO
    // ----------------------------------------
    // Kernel 3×3 para blur (média dos vizinhos)
    // Cada elemento vale 1/9
    // Matriz:
    //   [1/9  1/9  1/9]
    //   [1/9  1/9  1/9]
    //   [1/9  1/9  1/9]
    float kernel[9] = {
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f,
        1/9.f, 1/9.f, 1/9.f
    };

    // ----------------------------------------
    // ETAPA 5: PREPARAR MEDIÇÃO DE TEMPO
    // ----------------------------------------
    // Vamos executar 10 vezes e calcular média/desvio padrão
    // para ter resultados mais confiáveis
    double tempos[10];
    struct timespec tempo_inicio, tempo_fim;

    printf("Executando 10 repetições para medição...\n");

    // ----------------------------------------
    // ETAPA 6: LOOP DE MEDIÇÃO (10 repetições)
    // ----------------------------------------
    for (int repeticao = 0; repeticao < 10; repeticao++) {
        // Limpar buffer de saída antes de cada execução
        memset(imagem_saida, 0, largura * altura * 3);

        // Marca o tempo de INÍCIO
        clock_gettime(CLOCK_MONOTONIC, &tempo_inicio);

        // ----------------------------------------
        // CRIAR E INICIALIZAR THREADS
        // ----------------------------------------
        pthread_t threads[MAX_THREADS];
        DadosThread dados[MAX_THREADS];

        // Para cada thread, configurar seus dados e criar
        for(int i = 0; i < num_threads; i++) {
            dados[i].id = i;
            dados[i].total_threads = num_threads;
            dados[i].largura = largura;
            dados[i].altura = altura;
            dados[i].imagem_entrada = imagem_entrada;
            dados[i].imagem_saida = imagem_saida;
            dados[i].kernel = kernel;

            // Criar a thread e passar os dados
            pthread_create(&threads[i], NULL, funcao_thread, &dados[i]);
        }

        // ----------------------------------------
        // AGUARDAR CONCLUSÃO DE TODAS AS THREADS
        // ----------------------------------------
        // pthread_join bloqueia até a thread terminar
        for(int i = 0; i < num_threads; i++)
            pthread_join(threads[i], NULL);

        // Marca o tempo de FIM
        clock_gettime(CLOCK_MONOTONIC, &tempo_fim);

        // Calcula o tempo decorrido em segundos
        tempos[repeticao] = (tempo_fim.tv_sec - tempo_inicio.tv_sec) + 
                            (tempo_fim.tv_nsec - tempo_inicio.tv_nsec) / 1e9;
    }

    // ----------------------------------------
    // ETAPA 7: CALCULAR ESTATÍSTICAS
    // ----------------------------------------
    // Média aritmética dos 10 tempos
    double soma_tempos = 0.0;
    for (int i = 0; i < 10; i++) {
        soma_tempos += tempos[i];
    }
    double tempo_medio = soma_tempos / 10.0;

    // Desvio padrão (mede a variabilidade das medições)
    // Fórmula: sqrt(Σ(xi - média)² / N)
    double soma_quadrados = 0.0;
    for (int i = 0; i < 10; i++) {
        double diferenca = tempos[i] - tempo_medio;
        soma_quadrados += diferenca * diferenca;
    }
    double desvio_padrao = sqrt(soma_quadrados / 10.0);

    // ----------------------------------------
    // ETAPA 8: EXIBIR RESULTADOS
    // ----------------------------------------
    printf("\n=== RESULTADOS ===\n");
    printf("Tempo médio: %.6f s (%.2f ms)\n", tempo_medio, tempo_medio * 1000);
    printf("Desvio padrão: %.6f s (%.2f ms)\n", desvio_padrao, desvio_padrao * 1000);
    printf("Threads: %d\n", num_threads);
    printf("Resolução: %dx%d\n", largura, altura);

    // ----------------------------------------
    // ETAPA 9: SALVAR IMAGEM PROCESSADA
    // ----------------------------------------
    // stbi_write_png grava a imagem no formato PNG
    if (!stbi_write_png(arquivo_saida, largura, altura, 3, imagem_saida, largura * 3)) {
        printf("Erro ao salvar %s\n", arquivo_saida);
    } else {
        printf("\n%s gerado com sucesso!\n", arquivo_saida);
    }

    // ----------------------------------------
    // ETAPA 10: EXPORTAR DADOS PARA CSV
    // ----------------------------------------
    // Salva os resultados em arquivo CSV para análise posterior
    // Formato: Versao,Largura,Altura,Threads,TempoMedio,DesvioPadrao
    FILE *arquivo_csv = fopen("tempos_threads.csv", "a");
    if (arquivo_csv) {
        fprintf(arquivo_csv, "Threads,%d,%d,%d,%.6f,%.6f\n", 
                largura, altura, num_threads, tempo_medio, desvio_padrao);
        fclose(arquivo_csv);
    }

    // ----------------------------------------
    // ETAPA 11: LIBERAR MEMÓRIA
    // ----------------------------------------
    stbi_image_free(imagem_entrada);
    free(imagem_saida);
    
    return 0;
}
