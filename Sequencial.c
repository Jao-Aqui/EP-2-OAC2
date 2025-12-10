#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION



#include <stdio.h>
#include <stdlib.h>

// Calcula padding necessário para tornar o valor múltiplo de 3
int calcular_padding(int tamanho) {
    return (3 - (tamanho % 3)) % 3;
}

// Aplica padding (com zeros) em imagem RGB
unsigned char* aplicar_padding_rgb(
    unsigned char *img, int w, int h, int canais,
    int *novo_w, int *novo_h,
    int *pad_w, int *pad_h
) {
    *pad_w = calcular_padding(w);
    *pad_h = calcular_padding(h);

    *novo_w = w + *pad_w;
    *novo_h = h + *pad_h;

    unsigned char *nova_img =
        calloc((*novo_w) * (*novo_h) * canais, sizeof(unsigned char));

    for(int y = 0; y < h; y++) {
        for(int x = 0; x < w; x++) {
            for(int c = 0; c < canais; c++) {
                nova_img[(y * (*novo_w) + x) * canais + c] =
                    img[(y * w + x) * canais + c];
            }
        }
    }

    return nova_img;
}

// Remove padding previamente aplicado
unsigned char* remover_padding_rgb(
    unsigned char *img, int w, int h, int canais,
    int pad_w, int pad_h,
    int *orig_w, int *orig_h
) {
    *orig_w = w - pad_w;
    *orig_h = h - pad_h;

    unsigned char *recortada =
        malloc((*orig_w) * (*orig_h) * canais);

    for(int y = 0; y < *orig_h; y++) {
        for(int x = 0; x < *orig_w; x++) {
            for(int c = 0; c < canais; c++) {
                recortada[(y * (*orig_w) + x) * canais + c] =
                    img[(y * w + x) * canais + c];
            }
        }
    }

    return recortada;
}

int main() {
    char nome_entrada[] = "entrada.png";
    char nome_padded[]  = "com_padding.png";
    char nome_saida[]   = "sem_padding.png";

    int w, h, canais;

    printf("Carregando imagem PNG...\n");

    unsigned char *img = stbi_load(nome_entrada, &w, &h, &canais, 3);
    canais = 3; // força RGB

    if(!img) {
        printf("Erro ao carregar %s\n", nome_entrada);
        return 1;
    }

    printf("Imagem carregada: %dx%d (%d canais)\n", w, h, canais);

    int novo_w, novo_h;
    int pad_w, pad_h;

    unsigned char *img_padded =
        aplicar_padding_rgb(img, w, h, canais, &novo_w, &novo_h, &pad_w, &pad_h);

    printf("Padding aplicado: +%d largura, +%d altura => %dx%d\n",
           pad_w, pad_h, novo_w, novo_h);

    stbi_write_png(nome_padded,
                   novo_w, novo_h,
                   canais,
                   img_padded,
                   novo_w * canais);

    printf("Imagem com padding salva como %s\n", nome_padded);

    // -------- REMOVER PADDING --------
    int rec_w, rec_h;

    unsigned char *img_recortada =
        remover_padding_rgb(img_padded, novo_w, novo_h, canais,
                            pad_w, pad_h, &rec_w, &rec_h);

    printf("Padding removido: imagem volta para %dx%d\n", rec_w, rec_h);

    stbi_write_png(nome_saida,
                   rec_w, rec_h,
                   canais,
                   img_recortada,
                   rec_w * canais);

    printf("Imagem recortada (original) salva como %s\n", nome_saida);

    // libera memória
    stbi_image_free(img);
    free(img_padded);
    free(img_recortada);

    return 0;
}
