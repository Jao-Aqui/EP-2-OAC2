# EP-2-OAC2
Diferentes implementações de paralelização de convolução 2D (blur) em imagens

Este projeto implementa 4 versões de um filtro blur 3×3 usando diferentes técnicas de paralelização:
1. **Sequencial** - Sem paralelismo (baseline)
2. **Pthread** - Threads explícitas (manual)
3. **OpenMP** - Paralelismo implícito (3 estratégias)
4. **GPU** - CUDA ou OpenMP Target *(planejado)*

---

## 📋 Requisitos

- **GCC** com suporte a OpenMP (`gcc >= 4.2`)
- **pthread** (geralmente incluído no sistema)
- **make** (opcional, para compilação simplificada)
- **Bibliotecas**: stb_image.h e stb_image_write.h (incluídas em `stb/`)

### Instalar dependências (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install build-essential
```

---

## 🔧 Compilação

### Compilar todas as versões:
```bash
# Versão Sequencial
gcc -O3 -o sequencial Sequencial.c -lm -I./stb

# Versão Pthread
gcc -pthread -O3 -o pthread "Threads pthread.c" -lm -I./stb

# Versão OpenMP (3 estratégias em um único executável)
gcc -fopenmp -O3 -o openmp_exec OpenMP.c -lm -I./stb
```

---

## 🚀 Execução

### 1. Versão Sequencial

**Sintaxe:**
```bash
./sequencial <entrada.png> <saida.png> [num_repeticoes]
```

**Exemplos:**
```bash
# Processar imagem 512x512 com 10 repetições (padrão)
./sequencial input_512.png output_sequencial.png

# Processar com 5 repetições
./sequencial input_1024.png output_sequencial.png 5
```

**Saídas:**
- Imagem processada: `output_sequencial.png`
- CSV consolidado: `sequencial/resultados/tempos_sequencial.csv`
- CSV detalhado: `sequencial/resultados/tempos_sequencial_<resolução>.csv`

---

### 2. Versão Pthread (Threads Explícitas)

**Sintaxe:**
```bash
./pthread <entrada.png> <saida.png> <num_threads>
```

**Exemplos:**
```bash
# Processar com 4 threads
./pthread input_512.png output_pthread.png 4

# Processar com 8 threads
./pthread input_1024.png output_pthread.png 8
```

**Saídas:**
- Imagem processada: `output_pthread.png`
- CSV consolidado: `pthreads/resultados/tempos_threads.csv`
- CSV detalhado por configuração

---

### 3. Versão OpenMP (3 Estratégias)

**Sintaxe:**
```bash
./openmp_exec <entrada.png> <saida.png> <threads> <estrategia> [repeticoes]
```

**Estratégias disponíveis:**
- `static` - Divisão estática em blocos fixos
- `dynamic` - Distribuição dinâmica em tempo de execução
- `collapse` - Colapsa 2 loops aninhados

**Exemplos:**
```bash
# Estratégia static com 4 threads
./openmp_exec input_512.png output_openmp.png 4 static

# Estratégia dynamic com 8 threads e 5 repetições
./openmp_exec input_1024.png output_openmp.png 8 dynamic 5

# Estratégia collapse com 16 threads
./openmp_exec input_4096.png output_openmp.png 16 collapse
```

**Saídas:**
- Imagem processada: `output_openmp.png`
- CSV consolidado: `openmp/resultados/tempos_openmp_consolidado.csv`
- CSV detalhado: `openmp/resultados/tempos_openmp_<estrategia>_<res>_t<threads>.csv`

---

## 🧪 Executar Baterias de Testes Completas

### Sequencial (3 testes):
```bash
cd sequencial
./executar_testes.sh
```
Testa 3 resoluções (512, 1024, 4096) com 10 repetições cada.

### Pthread (15 testes):
```bash
cd pthreads
./executar_testes.sh
```
Testa 3 resoluções × 5 configurações de threads (1, 2, 4, 8, 16).

### OpenMP (45 testes):
```bash
cd openmp
./executar_testes.sh
```
Testa 3 resoluções × 3 estratégias × 5 configurações de threads = **45 combinações**.

---

## 📊 Análise de Resultados

### Analisar resultados OpenMP:
```bash
cd openmp

# Extrair dados do log de testes
./extrair_csv_do_log.sh

# Executar análise (programa em C)
./analisar

# Ver análise completa
cat ANALISE_RESULTADOS.txt

# Ver conclusões
cat CONCLUSOES.md
```

---

## 📁 Estrutura do Projeto

```
EP-2-OAC2/
├── README.md                    # Este arquivo
├── Sequencial.c                 # Versão sequencial
├── Threads pthread.c            # Versão com pthread
├── OpenMP.c                     # Versão OpenMP (3 estratégias)
├── input_512.png                # Imagem teste 512×512
├── input_1024.png               # Imagem teste 1024×1024
├── input_4096.png               # Imagem teste 4096×4096
├── img20x20.png                 # Imagem original 20×20
├── pokebola.jpg                 # Imagem alternativa
├── stb/                         # Biblioteca stb_image
│   ├── stb_image.h
│   └── stb_image_write.h
├── sequencial/
│   ├── executar_testes.sh
│   └── resultados/
│       ├── tempos_sequencial.csv
│       └── output_*.png
├── pthreads/
│   ├── executar_testes.sh
│   └── resultados/
│       ├── tempos_threads.csv
│       └── output_*.png
└── openmp/
    ├── executar_testes.sh
    ├── analisar_resultados.c    # Analisador em C
    ├── analisar                 # Executável do analisador
    ├── ANALISE_RESULTADOS.txt
    ├── CONCLUSOES.md
    ├── COMO_USAR.md
    └── resultados/
        ├── tempos_openmp_consolidado.csv
        └── output_*.png (45 imagens)
```

---

## 🎯 Exemplos de Uso Completo

### Processar uma imagem com todas as versões:

```bash
# 1. Sequencial
./sequencial input_512.png seq_output.png

# 2. Pthread com 4 threads
./pthread input_512.png pthread_output.png 4

# 3. OpenMP static com 4 threads
./openmp_exec input_512.png openmp_static_output.png 4 static

# 4. OpenMP dynamic com 4 threads
./openmp_exec input_512.png openmp_dynamic_output.png 4 dynamic

# 5. OpenMP collapse com 4 threads
./openmp_exec input_512.png openmp_collapse_output.png 4 collapse
```

### Comparar desempenho:

```bash
# Executar todos os testes
cd sequencial && ./executar_testes.sh && cd ..
cd pthreads && ./executar_testes.sh && cd ..
cd openmp && ./executar_testes.sh && cd ..

# Ver CSVs consolidados
cat sequencial/resultados/tempos_sequencial.csv
cat pthreads/resultados/tempos_threads.csv
cat openmp/resultados/tempos_openmp_consolidado.csv

# Analisar OpenMP
cd openmp && ./analisar
```

---

## 📈 Resultados Esperados

### Speedup típico (4096×4096 com 4 threads):
- **Sequencial:** 1.00x (baseline)
- **Pthread:** ~1.70x
- **OpenMP static:** ~2.39x ⭐
- **OpenMP dynamic:** ~1.11x
- **OpenMP collapse:** ~1.99x

### Melhor configuração por resolução:
- **512×512:** 8 threads + OpenMP static (speedup 5.39x)
- **1024×1024:** 16 threads + OpenMP static (speedup 3.30x)
- **4096×4096:** 4 threads + OpenMP static (speedup 2.39x)

---

## 🔍 Verificar se Está Funcionando

### Teste rápido:
```bash
# Compilar
gcc -O3 -o sequencial Sequencial.c -lm -I./stb

# Executar
./sequencial input_512.png teste.png 3

# Verificar se gerou a imagem
ls -lh teste.png

# Limpar
rm teste.png
```

### Comparar saídas (devem ser idênticas):
```bash
./sequencial input_512.png seq.png 1
./pthread input_512.png pthread.png 4
./openmp_exec input_512.png openmp.png 4 static 1

# Verificar hashes (devem ser iguais)
md5sum seq.png pthread.png openmp.png

# Limpar
rm seq.png pthread.png openmp.png
```

---

## ⚠️ Troubleshooting

### Erro: "stb_image.h: No such file or directory"
```bash
# Certifique-se de usar -I./stb
gcc -O3 -o sequencial Sequencial.c -lm -I./stb
```

### Erro: "undefined reference to sqrt"
```bash
# Adicione -lm no final
gcc -O3 -o sequencial Sequencial.c -I./stb -lm
```

### Erro: OpenMP não suportado
```bash
# Verifique versão do GCC
gcc --version

# Teste suporte OpenMP
gcc -fopenmp -o test_omp -xc - <<< 'int main(){return 0;}'
```

### Executável não roda
```bash
# Dê permissão de execução
chmod +x sequencial pthread openmp_exec

# Ou rode com bash
bash sequencial/executar_testes.sh
```

---

## 👥 Integrantes

*(Adicionar nomes dos 4 integrantes do grupo)*

---

## 📄 Licença

Trabalho acadêmico - Organização e Arquitetura de Computadores II
