# Status do Projeto - EP2 OAC II
**Data de Entrega:** 08/12/2025  
**Data Atual:** 10/12/2025  
**Status:** ⚠️ ATRASADO (2 dias)

---

## 📊 Progresso Geral

**Implementações:** 2/4 (50%) ✅ SEQUENCIAL + PTHREAD COMPLETOS  
**Testes e Medições:** 66% concluído (Sequencial: 3/3 ✅ | Pthread: 15/15 ✅)  
**Relatório:** Não iniciado  
**Gráficos:** Não gerados  
**Biblioteca STB:** ✅ Submódulo configurado  
**Imagens de Teste:** ✅ 3 resoluções criadas (512, 1024, 4096)  

---

## ✅ O QUE JÁ FOI CONCLUÍDO

### 1. Versão Sequencial (`Sequencial.c`) - ✅ COMPLETA
- ✅ Carregamento de imagens PNG usando stb_image
- ✅ Salvamento de imagens PNG usando stb_image_write
- ✅ Aplicação do kernel 3×3 de blur (média 1/9)
- ✅ Função de convolução RGB implementada (`aplicar_kernel_rgb`)
- ✅ Tratamento de bordas (cópia sem filtrar)
- ✅ **Bug `saia` CORRIGIDO!** Agora usa `saida` corretamente
- ✅ **Medição de tempo implementada** (clock_gettime)
  - ✅ 10 repetições por teste
  - ✅ Cálculo de média e desvio padrão
  - ✅ Mede apenas tempo de convolução (sem I/O)
- ✅ **Aceita parâmetros CLI** (input, output, num_repetições)
- ✅ **Exporta 2 tipos de CSV:**
  - ✅ `tempos_sequencial_<resolução>.csv` (detalhado, 10 linhas)
  - ✅ `tempos_sequencial.csv` (resumo consolidado)
- ✅ **Código comentado em português**
- ✅ **TESTADO COMPLETAMENTE:**
  - ✅ 3 resoluções: 512×512, 1024×1024, 4096×4096
  - ✅ 30 execuções totais (3 resoluções × 10 repetições)
  - ✅ 3 imagens PNG geradas
  - ✅ CSVs organizados
- ✅ **Pasta `sequencial/` criada** com:
  - Script de testes automatizado (`executar_testes.sh`)
  - 3 imagens processadas em `resultados/`
  - Arquivos CSV consolidados
  - Análise comparativa vs pthread

### 2. Versão com Threads Explícitas (`Threads pthread.c`) ✅ CONCLUÍDA
- ✅ Estrutura completa com pthread
- ✅ Divisão de trabalho entre threads (por linhas)
- ✅ Função de convolução RGB implementada (`aplicar_kernel_rgb`)
- ✅ Kernel de blur 3×3 (média 1/9) configurado
- ✅ **Integrado com stb_image** para carregar PNG
- ✅ **Integrado com stb_image_write** para salvar PNG
- ✅ Trabalha com `unsigned char*` (RGB correto)
- ✅ **Medição de tempo implementada** (10 repetições + média + desvio padrão)
- ✅ Aceita parâmetros por linha de comando (input, output, threads)
- ✅ Tratamento de bordas (copia sem filtrar)
- ✅ Exporta dados para CSV (`tempos_threads.csv`)
- ✅ **TESTADO COMPLETAMENTE:**
  - ✅ 3 resoluções: 512×512, 1024×1024, 4096×4096
  - ✅ 5 configurações de threads: 1, 2, 4, 8, 16
  - ✅ Total: 15 testes executados com sucesso
  - ✅ 15 imagens PNG geradas
  - ✅ Dados em CSV organizados por threads
- ✅ **Código comentado em português** com explicações detalhadas
- ✅ **Pasta `pthreads/` criada** com:
  - Script de testes automatizado (`executar_testes.sh`)
  - 15 imagens processadas em `resultados/`
  - Arquivo CSV com todos os tempos
  - Resumo de resultados (RESUMO_RESULTADOS.md)

### 3. Biblioteca STB (Submódulo)
- ✅ Submódulo Git configurado e clonado
- ✅ Headers `stb_image.h` e `stb_image_write.h` disponíveis

### 4. Imagens de Teste Preparadas
- ✅ `img20x20.png` - imagem base (20×20 pixels) - 4.7 KB
- ✅ `entrada.png` - imagem para teste manual (96 KB)
- ✅ `output.png` - resultado de teste manual (114 KB)
- ✅ **3 resoluções obrigatórias no diretório raiz:**
  - ✅ `input_512.png` - 512×512 pixels (~125 KB)
  - ✅ `input_1024.png` - 1024×1024 pixels (~358 KB)
  - ✅ `input_4096.png` - 4096×4096 pixels (~2.5 MB)
  - ✅ Criadas com gradiente RGB usando Python PIL

### 5. Resultados de Testes - Versão Sequencial
- ✅ **3 imagens processadas** salvas em `sequencial/resultados/`
- ✅ **CSVs gerados:**
  - `tempos_sequencial.csv` (resumo: 3 linhas)
  - `tempos_sequencial_512x512.csv` (10 repetições)
  - `tempos_sequencial_1024x1024.csv` (10 repetições)
  - `tempos_sequencial_4096x4096.csv` (10 repetições)
- ✅ **Tempos medidos (baseline para speedup):**
  - 512×512: 0.083s (média de 10 execuções)
  - 1024×1024: 0.420s (média de 10 execuções)
  - 4096×4096: 3.280s (média de 10 execuções)
- ✅ **Análise comparativa vs pthread** em `ANALISE_SEQUENCIAL_VS_PTHREAD.md`

### 6. Resultados de Testes - Threads pthread
- ✅ **15 imagens processadas** salvas em `pthreads/resultados/`
- ✅ **CSV com tempos** (`tempos_threads.csv`):
  - 15 linhas de dados (3 resoluções × 5 configs threads)
  - Média e desvio padrão calculados
- ✅ **Speedup medido (vs Sequencial):**
  - 512×512: até 1.14x (16 threads)
  - 1024×1024: até 1.52x (16 threads)
  - 4096×4096: até 1.70x (16 threads)

---

## ❌ O QUE AINDA FALTA

### 1. Novas Implementações Obrigatórias

#### Versão 3: OpenMP (NÃO INICIADA) - PRIORIDADE ALTA
- [ ] Criar arquivo `OpenMP.c`
- [ ] Implementar convolução com `#pragma omp parallel for`
- [ ] **Testar 3 estratégias de escalonamento (OBRIGATÓRIO pelo enunciado):**
  - [ ] `schedule(static)` - distribuição estática
  - [ ] `schedule(dynamic)` - distribuição dinâmica
  - [ ] `collapse(2)` - colapsar loops aninhados
- [ ] Integrar com stb_image para carregar/salvar PNG
- [ ] Adicionar medição de tempo (média de 10 execuções + desvio padrão)
- [ ] Aceitar número de threads como argumento (`OMP_NUM_THREADS` ou setenv)
- [ ] Testar com threads: 1, 2, 4, 8
- [ ] Testar com as 3 resoluções: 512×512, 1024×1024, 4096×4096
- [ ] **Comparar desempenho das 3 estratégias** (análise obrigatória)
- [ ] Gerar dados de tempo em formato CSV ou similar

#### Versão 4: GPU (NÃO INICIADA) - ESCOLHER UMA OPÇÃO

**Opção A - CUDA (Recomendado se tiver GPU NVIDIA):**
- [ ] Criar arquivo `GPU_CUDA.cu`
- [ ] Implementar kernel CUDA para convolução 2D
- [ ] **Usar grids 16×16 ou 32×32** (especificado no enunciado)
- [ ] **Implementar 2 versões (OBRIGATÓRIO):**
  - [ ] Versão 1: Sem shared memory (memória global apenas)
  - [ ] Versão 2: Com shared memory (otimizada)
- [ ] **Medir separadamente:**
  - [ ] Tempo de transferência Host → Device
  - [ ] Tempo de execução do kernel na GPU
  - [ ] Tempo de transferência Device → Host
  - [ ] Tempo total (incluindo transferências)
- [ ] Integrar com stb_image (carregar/salvar na CPU)
- [ ] Testar com as 3 resoluções obrigatórias
- [ ] Comparar versão com/sem shared memory
- [ ] Salvar imagem processada

**Opção B - OpenMP Target Offloading (Se não tiver CUDA):**
- [ ] Criar arquivo `GPU_OpenMP_Target.c`
- [ ] Implementar com `#pragma omp target teams distribute parallel for`
- [ ] Configurar mapeamento de memória:
  - [ ] `map(to: img[0:H*W], K[0:9])` - copiar para device
  - [ ] `map(from: out[0:H*W])` - copiar de volta
- [ ] Usar `collapse(2)` para paralelizar loops aninhados
- [ ] **Medir separadamente:**
  - [ ] Custo de transferência CPU → GPU
  - [ ] Tempo de execução no dispositivo
  - [ ] Custo de transferência GPU → CPU
- [ ] Integrar com stb_image
- [ ] Testar com as 3 resoluções
- [ ] Salvar imagem processada

**⚠️ IMPORTANTE:** Verificar disponibilidade de hardware antes de escolher:
- CUDA requer GPU NVIDIA + CUDA Toolkit instalado
- OpenMP Target requer GCC 9+ ou Clang com suporte a offloading

---

### 3. Preparação de Dados e Imagens

#### Imagens de Entrada (OBRIGATÓRIAS):
- [ ] **Obter/criar imagem base 20×20** (mencionada no enunciado)
- [ ] **Redimensionar para 512 × 512** (resolução obrigatória 1)
- [ ] **Redimensionar para 1024 × 1024** (resolução obrigatória 2)
- [ ] **Redimensionar para 4096 × 4096** (resolução obrigatória 3)
- [ ] Salvar todas as versões em formato PNG
- [ ] Nomear adequadamente (ex: `input_512.png`, `input_1024.png`, `input_4096.png`)
- [ ] Usar ImageMagick ou Python/PIL para redimensionamento

**Comando sugerido (ImageMagick):**
```bash
convert input_20x20.png -resize 512x512 input_512.png
convert input_20x20.png -resize 1024x1024 input_1024.png
convert input_20x20.png -resize 4096x4096 input_4096.png
```

**Kernel Obrigatório (3×3 Blur) - TODOS devem usar o mesmo:**
```
K = 1/9 × | 1  1  1 |
          | 1  1  1 |
          | 1  1  1 |
```
- ✅ Kernel já implementado corretamente em ambos os códigos existentes

---

### 4. Medições e Testes - PROTOCOLO OBRIGATÓRIO

#### Sistema de Medição de Tempo:
- ✅ Timer preciso usando `clock_gettime(CLOCK_MONOTONIC, ...)`
- ✅ **Mede APENAS a convolução** (exclui I/O de leitura/escrita de arquivos)
- ✅ **10 repetições** por configuração (implementado em Threads pthread)
- ✅ **Média aritmética** dos tempos calculada
- ✅ **Desvio padrão** dos tempos calculado
- ✅ Resultados salvos em formato CSV

**Estrutura CSV implementada:**
```
Versao,Largura,Altura,Threads,TempoMedio,DesvioPadrao
Threads,512,512,1,0.172704,0.012218
Threads,512,512,2,0.112811,0.027474
Threads,1024,1024,4,0.274320,0.044108
...
```

#### Configurações de Teste:

**1. Sequencial:** ✅ **COMPLETO!**
- ✅ 512×512 - 10 execuções (média: 0.083s)
- ✅ 1024×1024 - 10 execuções (média: 0.420s)
- ✅ 4096×4096 - 10 execuções (média: 3.280s)
- ✅ **Total: 30 execuções concluídas**

**2. Threads (pthread):** ✅ **COMPLETO!**
- ✅ 512×512 com 1, 2, 4, 8, 16 threads - 10 execuções cada
- ✅ 1024×1024 com 1, 2, 4, 8, 16 threads - 10 execuções cada
- ✅ 4096×4096 com 1, 2, 4, 8, 16 threads - 10 execuções cada
- ✅ **Total: 15 configurações testadas (150 execuções)**

**3. OpenMP:**
- [ ] Para cada tamanho (512, 1024, 4096):
  - [ ] schedule(static) com 1, 2, 4, 8 threads
  - [ ] schedule(dynamic) com 1, 2, 4, 8 threads
  - [ ] collapse(2) com 1, 2, 4, 8 threads
- [ ] Total: 3 tamanhos × 3 estratégias × 4 números de threads × 10 repetições = **360 execuções**

**4. GPU:**
- [ ] 512×512 - versão sem shared memory - 10 execuções
- [ ] 512×512 - versão com shared memory - 10 execuções
- [ ] 1024×1024 - ambas versões - 10 execuções cada
- [ ] 4096×4096 - ambas versões - 10 execuções cada
- [ ] **Registrar separadamente:**
  - Tempo de transferência Host→Device
  - Tempo de execução do kernel
  - Tempo de transferência Device→Host

#### Métricas a Calcular (OBRIGATÓRIAS pelo enunciado):

**Speedup:**
```
S = T_sequencial / T_paralelo
```
- Calcular para cada configuração paralela
- Speedup ideal = número de threads

**Eficiência (apenas CPU):**
```
E = S / N_threads
```
- E = 1.0 (100%) é ideal
- E < 1.0 indica overhead ou contenção

**Análises Obrigatórias:**
- [ ] Speedup × Tamanho da imagem (todas as 4 versões)
- [ ] Speedup × Número de threads (CPU apenas)
- [ ] Eficiência × Número de threads
- [ ] Overhead de paralelização (diferença entre ideal e real)
- [ ] Custo de transferência CPU-GPU (em % do tempo total)
- [ ] Impacto de shared memory na GPU

---

### 5. Análise e Visualização - GRÁFICOS OBRIGATÓRIOS

#### Gráficos Exigidos pelo Enunciado:

**Gráfico 1: Speedup × Tamanho da imagem**
- [ ] Eixo X: Tamanho (512, 1024, 4096)
- [ ] Eixo Y: Speedup
- [ ] **4 curvas:** Sequencial (baseline=1), Threads, OpenMP, GPU
- [ ] Mostrar como cada técnica escala com o tamanho
- [ ] Incluir linha de referência Speedup=1 (sequencial)

**Gráfico 2: Speedup × Número de threads**
- [ ] Eixo X: Número de threads (1, 2, 4, 8)
- [ ] Eixo Y: Speedup
- [ ] **Comparar:** Threads pthread vs OpenMP (diferentes estratégias)
- [ ] Gerar um gráfico para cada tamanho de imagem
- [ ] Incluir linha de speedup ideal (linear)
- [ ] **Para OpenMP:** linhas separadas para static, dynamic, collapse(2)

**Gráfico 3: Tempo total × Técnica**
- [ ] Eixo X: Técnica (Sequencial, Threads, OpenMP-static, OpenMP-dynamic, OpenMP-collapse, GPU, GPU-shared)
- [ ] Eixo Y: Tempo (ms ou segundos)
- [ ] Gráfico de barras agrupadas
- [ ] Um grupo para cada tamanho (512, 1024, 4096)
- [ ] Incluir barras de erro (desvio padrão)

**Gráfico Extra Sugerido: Breakdown GPU**
- [ ] Tempo de transferência Host→Device
- [ ] Tempo de execução (kernel)
- [ ] Tempo de transferência Device→Host
- [ ] Comparar versão com/sem shared memory

#### Script de Geração:
- [ ] Criar script Python usando matplotlib/seaborn
- [ ] Ler dados do arquivo CSV gerado nos testes
- [ ] Gerar todos os gráficos automaticamente
- [ ] Exportar em formato PNG (alta resolução, 300 DPI)
- [ ] Exportar também em PDF (para inclusão no relatório LaTeX)

**Exemplo de estrutura:**
```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('resultados.csv')
# ... gerar gráficos ...
plt.savefig('speedup_vs_tamanho.png', dpi=300)
```

---

### 6. Relatório Final - ESTRUTURA OBRIGATÓRIA

#### Conteúdo Mínimo Exigido pelo Enunciado:

- [ ] **1. Introdução**
  - Objetivo do trabalho
  - Descrição do problema (convolução 2D para blur de imagens)
  - Importância do paralelismo em processamento de imagens
  - Visão geral das 4 abordagens implementadas

- [ ] **2. Metodologia**
  - **2.1 Descrição das Implementações:**
    - Versão Sequencial (algoritmo base)
    - Threads explícitas (pthread) - divisão de trabalho
    - OpenMP - 3 estratégias de escalonamento
    - GPU (CUDA ou OpenMP Target) - com/sem shared memory
  - **2.2 Hardware Utilizado:**
    - CPU (modelo, núcleos, frequência, cache)
    - GPU (se aplicável: modelo, CUDA cores, memória)
    - RAM total
  - **2.3 Software e Compiladores:**
    - SO e versão do kernel
    - GCC/Clang versão e flags de compilação
    - CUDA Toolkit versão (se aplicável)
    - Bibliotecas utilizadas (stb_image, pthread, OpenMP)
  - **2.4 Procedimento de Medição:**
    - Ferramenta de timing (clock_gettime)
    - 10 repetições por configuração
    - Cálculo de média e desvio padrão
    - Exclusão de I/O das medições

- [ ] **3. Resultados Experimentais**
  - **3.1 Tabelas de Tempo de Execução:**
    - Tempo médio ± desvio padrão para cada configuração
    - Organizar por versão e tamanho de imagem
  - **3.2 Tabelas de Speedup e Eficiência:**
    - Speedup relativo à versão sequencial
    - Eficiência para versões com threads
  - **3.3 Gráficos (os 3 obrigatórios):**
    - Speedup × Tamanho da imagem
    - Speedup × Número de threads
    - Tempo total × Técnica
  - **3.4 Análise GPU (se aplicável):**
    - Breakdown de tempos (transferência vs. computação)
    - Impacto de shared memory

- [ ] **4. Discussão - COMPARAÇÕES OBRIGATÓRIAS**
  - [ ] **4.1 Threads explícitas vs OpenMP**
    - Facilidade de implementação
    - Desempenho comparativo
    - Overhead de cada abordagem
  - [ ] **4.2 CPU vs GPU**
    - Em quais tamanhos a GPU superou a CPU?
    - Custo de transferência de dados
    - Quando vale a pena usar GPU?
  - [ ] **4.3 Efeito do escalonamento no OpenMP**
    - static vs dynamic vs collapse(2)
    - Qual foi mais eficiente? Por quê?
  - [ ] **4.4 Impacto do tamanho da imagem na escalabilidade**
    - Como o speedup varia com o tamanho?
    - Lei de Amdahl aplicada
  - [ ] **4.5 Overhead de criação de threads**
    - Quando 1 thread é mais lento que sequencial?
  - [ ] **4.6 Afinidade de cache**
    - Efeitos de localidade espacial/temporal
  - [ ] **4.7 Custo de transferência CPU–GPU**
    - % do tempo total gasto em transferências
    - Como minimizar esse custo?

- [ ] **5. Conclusões - RESPONDER PERGUNTAS DO ENUNCIADO**
  - [ ] **Qual técnica foi mais eficiente?**
  - [ ] **A GPU superou a CPU? Em quais tamanhos?**
  - [ ] **OpenMP apresenta desempenho similar a threads manuais?**
  - [ ] **Onde o paralelismo não trouxe ganho?**
  - [ ] **Qual abordagem oferece melhor custo-benefício?**
  - Lições aprendidas
  - Trabalhos futuros

- [ ] **6. Referências**
  - Documentação do OpenMP
  - CUDA Programming Guide (se aplicável)
  - Artigos sobre convolução paralela
  - Manuais das bibliotecas utilizadas

---

## 🛠️ Ferramentas Necessárias

### Instaladas/Disponíveis:
- ✅ GCC (para C/C++)
- ✅ pthread (biblioteca de threads POSIX)
- ✅ stb_image (submódulo Git configurado e clonado)

### A Verificar/Instalar URGENTEMENTE:

- [ ] **OpenMP** (CRÍTICO para Versão 3)
  ```bash
  gcc -fopenmp --version  # Verificar suporte
  # Se não tiver: sudo apt install libomp-dev (Ubuntu/Debian)
  ```

- [ ] **CUDA Toolkit** (Se escolher GPU com CUDA)
  ```bash
  nvcc --version  # Verificar se está instalado
  nvidia-smi     # Verificar se detecta GPU
  # Baixar de: https://developer.nvidia.com/cuda-downloads
  ```

- [ ] **OpenMP 5.0+ com offloading** (Se escolher GPU com OpenMP Target)
  ```bash
  gcc -fopenmp -foffload=nvptx-none --version  # Para NVIDIA
  # ou
  gcc -fopenmp -foffload=amdgcn-amdhsa --version  # Para AMD
  ```

- [ ] **Python 3 + bibliotecas** (Para gráficos e processamento de imagens)
  ```bash
  python3 --version
  pip3 install matplotlib numpy pandas pillow
  ```

- [ ] **ImageMagick** (Para redimensionar imagens)
  ```bash
  convert --version
  # Se não tiver: sudo apt install imagemagick
  ```

- [ ] **pdflatex** (Para gerar relatório em PDF, opcional)
  ```bash
  pdflatex --version
  # Se não tiver: sudo apt install texlive-full
  ```

---

## 📅 Cronograma Sugerido (Recuperação de Atraso)

### 🔥 Dia 1 (Hoje - 10/12) - URGENTE:
**Foco: Corrigir bugs e preparar infraestrutura**
- [ ] **09:00-10:00:** Corrigir bug `saia→saida` em Sequencial.c
- [ ] **10:00-12:00:** Adicionar medição de tempo em Sequencial.c
  - Implementar função `medir_tempo()`
  - Executar 10 repetições
  - Calcular média e desvio padrão
- [ ] **12:00-13:00:** Almoço
- [ ] **13:00-15:00:** Preparar imagens de teste
  - Obter/criar imagem base 20×20
  - Redimensionar para 512, 1024, 4096 usando ImageMagick
  - Testar Sequencial.c com as 3 resoluções
- [ ] **15:00-18:00:** Refatorar Threads pthread.c
  - Integrar stb_image (carregar PNG real)
  - Converter para unsigned char (RGB)
  - Adicionar medição de tempo
  - Salvar resultado com stb_image_write
- [ ] **18:00-19:00:** Testar pthread com diferentes números de threads
- [ ] **19:00-20:00:** Gerar dados iniciais (CSV) - Sequencial vs Threads

**Entregas do Dia 1:**
✅ Sequencial.c funcional e testado  
✅ Threads pthread.c funcional e testado  
✅ 3 imagens de entrada prontas  
✅ Sistema de medição de tempo funcionando  

---

### Dia 2 (11/12) - OpenMP e Testes Completos:
**Foco: Implementar OpenMP e coletar todos os dados de CPU**
- [ ] **09:00-12:00:** Implementar OpenMP.c
  - Versão com schedule(static)
  - Versão com schedule(dynamic)
  - Versão com collapse(2)
  - Integrar stb_image + medição de tempo
- [ ] **12:00-13:00:** Almoço
- [ ] **13:00-16:00:** Executar bateria completa de testes
  - Sequencial: 3 tamanhos × 10 repetições
  - Threads: 3 tamanhos × 4 configs threads × 10 reps
  - OpenMP: 3 tamanhos × 3 estratégias × 4 configs threads × 10 reps
- [ ] **16:00-18:00:** Consolidar dados em CSV
  - Calcular speedups
  - Calcular eficiências
  - Preparar tabelas para o relatório
- [ ] **18:00-20:00:** Análise preliminar
  - Qual estratégia OpenMP foi melhor?
  - Threads vs OpenMP: quem venceu?

**Entregas do Dia 2:**
✅ OpenMP.c implementado e testado  
✅ Dados completos de CPU (360+ medições)  
✅ Arquivo CSV com todos os resultados  
✅ Análise preliminar documentada  

---

### Dia 3 (12/12) - GPU e Visualização:
**Foco: Implementar GPU e gerar gráficos**
- [ ] **09:00-10:00:** Escolher entre CUDA ou OpenMP Target
  - Verificar hardware disponível
  - Instalar ferramentas necessárias
- [ ] **10:00-13:00:** Implementar versão GPU
  - Versão sem shared memory
  - Versão com shared memory (se CUDA)
  - Medições separadas (transfer + kernel)
- [ ] **13:00-14:00:** Almoço
- [ ] **14:00-16:00:** Executar testes GPU
  - 3 tamanhos × 2 versões × 10 repetições
  - Registrar breakdown de tempos
- [ ] **16:00-18:00:** Criar script Python de visualização
  - Gráfico 1: Speedup vs Tamanho
  - Gráfico 2: Speedup vs Threads
  - Gráfico 3: Tempo vs Técnica
  - Gráfico extra: Breakdown GPU
- [ ] **18:00-20:00:** Gerar todos os gráficos
  - Exportar PNG (300 DPI)
  - Exportar PDF
  - Revisar qualidade visual

**Entregas do Dia 3:**
✅ GPU implementada e testada  
✅ Dados completos de GPU  
✅ 4 gráficos gerados em alta qualidade  
✅ Todos os dados consolidados  

---

### Dia 4 (13/12) - Relatório e Entrega:
**Foco: Escrever relatório completo**
- [ ] **09:00-11:00:** Escrever Introdução e Metodologia
- [ ] **11:00-13:00:** Documentar Resultados Experimentais
  - Inserir tabelas
  - Inserir gráficos
  - Descrever resultados observados
- [ ] **13:00-14:00:** Almoço
- [ ] **14:00-16:00:** Escrever Discussão
  - Responder todas as perguntas obrigatórias
  - Analisar comparações
  - Justificar resultados
- [ ] **16:00-17:00:** Escrever Conclusões
- [ ] **17:00-18:00:** Revisar código
  - Adicionar comentários
  - Criar Makefile
  - Documentar README.md
- [ ] **18:00-19:00:** Preparar estrutura de entrega
  - Organizar pastas
  - Testar compilação limpa
  - Verificar checklist completo
- [ ] **19:00-20:00:** Revisão final e submissão

**Entregas do Dia 4:**
✅ Relatório completo em PDF  
✅ Código revisado e documentado  
✅ README.md com instruções  
✅ Estrutura de entrega organizada  
✅ **TRABALHO ENVIADO**  

---

## 📦 Estrutura de Entrega Final

```
EP-2-OAC2/
├── codigo/
│   ├── Sequencial.c
│   ├── Threads_pthread.c
│   ├── OpenMP.c
│   ├── GPU_CUDA.cu (ou GPU_OpenMP_Target.c)
│   ├── Makefile                    # Compilar todas as versões
│   ├── run_all_tests.sh            # Script para executar todos os testes
│   └── stb/
│       ├── stb_image.h
│       └── stb_image_write.h
├── imagens/
│   ├── entrada/
│   │   ├── input_20x20.png         # Imagem base fornecida
│   │   ├── input_512.png           # 512×512
│   │   ├── input_1024.png          # 1024×1024
│   │   └── input_4096.png          # 4096×4096
│   └── resultados/
│       ├── output_seq_512.png
│       ├── output_threads_512.png
│       ├── output_openmp_512.png
│       └── output_gpu_512.png
├── dados/
│   ├── tempos_brutos.csv           # Todas as medições individuais
│   ├── tempos_medios.csv           # Médias e desvios padrão
│   └── metricas.csv                # Speedups e eficiências calculadas
├── graficos/
│   ├── speedup_vs_tamanho.png
│   ├── speedup_vs_threads.png
│   ├── tempo_vs_tecnica.png
│   ├── breakdown_gpu.png           # (opcional)
│   └── gerar_graficos.py           # Script Python
├── relatorio/
│   ├── relatorio.pdf               # Relatório final
│   └── relatorio.tex               # Fonte LaTeX (opcional)
└── README.md                        # Instruções de compilação e execução
```

**Conteúdo obrigatório do README.md:**
- Instruções de compilação (make all)
- Como executar cada versão
- Requisitos de software
- Estrutura do projeto
- Membros do grupo (4 alunos)

---

## ⚠️ Observações Importantes

1. **Prazo vencido há 2 dias:** Foco total em implementações funcionais, não em otimizações avançadas
2. **Grupo de 4 pessoas:** Dividir tarefas de forma inteligente
   - ✅ Pessoa 1: Preparar imagens **FEITO**
   - ✅ Pessoa 2: Refatorar Threads pthread.c **FEITO**
   - Pessoa 3: Implementar OpenMP.c **PENDENTE**
   - Pessoa 4: Implementar GPU + gráficos **PENDENTE**
   - Todos: Colaborar nos testes e relatório
3. **Medições rigorosas:** ✅ 10 repetições implementadas em Threads pthread
4. **Kernel padronizado:** ✅ Kernel 3×3 blur implementado corretamente
5. **Comparações obrigatórias:** O relatório DEVE responder às 5 perguntas do enunciado
6. **GPU:** Se CUDA não estiver disponível, usar OpenMP Target
7. **Ser honesto:** Se algo não funcionou, documentar no relatório as limitações
8. **Código comentado:** ✅ Threads pthread.c totalmente comentado em português
9. **Makefile:** Pendente
10. **Testes automáticos:** ✅ Script bash criado (`pthreads/executar_testes.sh`)

## 🚨 BUGS CRÍTICOS IDENTIFICADOS

### ~~Sequencial.c - Linha 80~~ ✅ **CORRIGIDO!**
```c
// ❌ ERRO (ANTES):
saia[idx]     = entrada[idx];
saia[idx + 1] = entrada[idx + 1];
saia[idx + 2] = entrada[idx + 2];

// ✅ CORRETO (AGORA):
saida[idx]     = entrada[idx];
saida[idx + 1] = entrada[idx + 1];
saida[idx + 2] = entrada[idx + 2];
```
**Status:** ✅ BUG CORRIGIDO - Código compila e executa normalmente
**Impacto:** Código não compila! Bug trivial mas bloqueante.

### ~~Threads pthread.c~~ ✅ **TODOS OS PROBLEMAS CORRIGIDOS!**
1. ✅ Trabalha com `unsigned char*` em vez de `float*` (RGB correto)
2. ✅ Carrega imagens PNG reais usando stb_image
3. ✅ Salva resultado em arquivo PNG usando stb_image_write
4. ✅ Mede tempo corretamente (10 repetições + estatísticas)
5. ✅ Função `aplicar_kernel_rgb` processa 3 canais RGB corretamente

**Status:** ✅ TOTALMENTE FUNCIONAL E TESTADO

---

## 🎯 Prioridades Críticas (TOP 10)

### 🔴 ~~URGÊNCIA MÁXIMA~~ ✅ **CONCLUÍDO! (10/12 Noite):**
1. **~~Corrigir bug `saia→saida` em Sequencial.c~~** ✅ FEITO
2. **~~Adicionar medição de tempo em Sequencial.c~~** ✅ FEITO
3. **~~Recriar imagens de teste (512, 1024, 4096)~~** ✅ FEITO
4. **~~Adicionar parâmetros CLI em Sequencial.c~~** ✅ FEITO
5. **~~Testar Sequencial.c com as 3 resoluções~~** ✅ FEITO
6. **~~Criar pasta sequencial/ com infraestrutura~~** ✅ FEITO

### 🟠 ALTA PRIORIDADE (Dia 2 - 11/12):
7. **Implementar OpenMP.c** com as 3 estratégias obrigatórias
8. **Executar bateria completa de testes** (OpenMP: 45 configurações)

### 🟡 MÉDIA PRIORIDADE (Dia 3 - 12/12):
9. **Implementar versão GPU** (CUDA ou OpenMP Target)
10. **Gerar os 3 gráficos obrigatórios** usando Python/matplotlib

### 🟢 BAIXA PRIORIDADE (Dia 4 - 13/12):
11. **Escrever relatório completo** respondendo às 5 perguntas obrigatórias

---

## 📋 Checklist Final de Entrega

### Código (4 versões):
- [ ] Sequencial.c - compila e executa
- ✅ Threads_pthread.c - **compila e executa** (testado 15 configs)
- [ ] OpenMP.c - compila e executa
- [ ] GPU (CUDA ou OpenMP Target) - compila e executa
- [ ] Makefile funcional
- ✅ Código comentado (Threads pthread em português)

### Imagens:
- ✅ input_512.png
- ✅ input_1024.png
- ✅ input_4096.png
- ✅ **3 imagens de resultado Sequencial** salvas (sequencial/resultados/)
- ✅ **15 imagens de resultado Pthread** salvas (pthreads/resultados/)

### Dados:
- ✅ CSV Sequencial (sequencial/resultados/tempos_sequencial.csv) - 3 linhas
- ✅ CSV Pthread (pthreads/resultados/tempos_threads.csv) - 15 linhas
- ✅ CSVs detalhados por resolução (ambas versões)
- [ ] CSV consolidado com todas as versões (Sequencial + Threads + OpenMP + GPU)
- [ ] CSV com speedups e eficiências calculadas

### Gráficos:
- [ ] Speedup vs Tamanho da imagem
- [ ] Speedup vs Número de threads
- [ ] Tempo vs Técnica

### Relatório:
- [ ] Introdução
- [ ] Metodologia (hardware, software, procedimento)
- [ ] Resultados (tabelas + gráficos)
- [ ] Discussão (7 tópicos obrigatórios)
- [ ] Conclusões (5 perguntas respondidas)
- [ ] Referências
- [ ] PDF gerado

### Organização:
- [ ] README.md com instruções
- ✅ Estrutura de pastas organizada (`pthreads/` e `sequencial/` criadas)
- ✅ Scripts automatizados de teste (ambas versões)
- [ ] Nomes dos 4 integrantes documentados

---

## 🎉 CONQUISTAS RECENTES

### ✅ 10/12/2025 23:17 - Sequencial.c TOTALMENTE COMPLETO!
- **30 execuções concluídas** (3 resoluções × 10 repetições)
- **Medição de tempo implementada** (clock_gettime)
- **Parâmetros CLI funcionando** (input, output, num_repetições)
- **2 tipos de CSV gerados:**
  - Detalhado por resolução (10 linhas cada)
  - Resumo consolidado (3 linhas)
- **Pasta sequencial/ criada** com infraestrutura completa
- **Análise comparativa vs pthread** em ANALISE_SEQUENCIAL_VS_PTHREAD.md
- **Speedup pthread medido:** 1.14x (512²), 1.52x (1024²), 1.70x (4096²)
- **Baseline estabelecido** para comparações futuras

### ✅ 10/12/2025 22:50 - Bug Sequencial.c Corrigido!
- **Bug `saia` → `saida` CORRIGIDO**
- Código **compila sem erros** (com `-I./stb`)
- Código **executa com sucesso**
- Gera `output.png` corretamente (114 KB)

### ✅ 10/12/2025 22:24 - Threads pthread COMPLETO!
- **15 testes executados** (3 resoluções × 5 threads)
- **150 execuções** do algoritmo (10 repetições cada)
- **Código refatorado** e comentado em português
- **Script automatizado** para testes
- **Dados exportados** para CSV consolidado

---

**Última atualização:** 10/12/2025 23:17 - ✅ SEQUENCIAL + PTHREAD COMPLETOS (50% do projeto)
