# Status do Projeto - EP2 OAC II
**Data de Entrega:** 08/12/2025  
**Data Atual:** 10/12/2025  
**Status:** ⚠️ ATRASADO (2 dias)

---

## 📊 Progresso Geral

**Implementações:** 2/4 (50%)  
**Testes e Medições:** 0% concluído  
**Relatório:** Não iniciado  
**Gráficos:** Não gerados  

---

## ✅ O QUE JÁ FOI CONCLUÍDO

### 1. Versão Sequencial (`Sequencial.c`)
- ✅ Carregamento de imagens PNG usando stb_image
- ✅ Salvamento de imagens PNG usando stb_image_write
- ✅ Aplicação de padding (múltiplo de 3)
- ✅ Remoção de padding
- ⚠️ **PROBLEMA:** Não implementa a convolução com kernel 3×3 de blur!
  - Código atual apenas manipula padding, não aplica o filtro

### 2. Versão com Threads Explícitas (`Threads pthread.c`)
- ✅ Estrutura básica com pthread
- ✅ Divisão de trabalho entre threads (por linhas)
- ✅ Função de aplicação de kernel 3×3 implementada
- ✅ Kernel de blur (média 1/9) configurado
- ✅ Número de threads configurável (MAX_THREADS = 8)
- ⚠️ **PROBLEMAS:**
  - Usa dados sintéticos (random), não carrega imagem real
  - Não integrado com stb_image
  - Não mede tempo de execução
  - Não salva resultado em arquivo de imagem
  - Trabalha com float, deveria usar unsigned char para RGB

---

## ❌ O QUE AINDA FALTA

### 1. Corrigir e Completar Implementações Existentes

#### Sequencial.c - Correções Necessárias:
- [ ] Implementar função de convolução 2D
- [ ] Aplicar kernel 3×3 de blur na imagem
- [ ] Adicionar medição de tempo (apenas da convolução)
- [ ] Testar com as 3 resoluções obrigatórias
- [ ] Salvar imagem processada

#### Threads pthread.c - Correções Necessárias:
- [ ] Integrar com stb_image para carregar/salvar imagens reais
- [ ] Converter de float para unsigned char (RGB)
- [ ] Adicionar medição de tempo
- [ ] Salvar imagem processada
- [ ] Testar com número variável de threads (1, 2, 4, 8)
- [ ] Garantir sincronização adequada (se necessário)

---

### 2. Novas Implementações Obrigatórias

#### Versão 3: OpenMP (NÃO INICIADA)
- [ ] Criar arquivo `OpenMP.c`
- [ ] Implementar convolução com `#pragma omp parallel for`
- [ ] Testar 3 estratégias de escalonamento:
  - [ ] `schedule(static)`
  - [ ] `schedule(dynamic)`
  - [ ] `collapse(2)`
- [ ] Integrar com stb_image
- [ ] Adicionar medição de tempo
- [ ] Testar com número variável de threads
- [ ] Comparar desempenho das estratégias

#### Versão 4: GPU (NÃO INICIADA)
**Opção A - CUDA:**
- [ ] Criar arquivo `GPU_CUDA.cu`
- [ ] Implementar kernel CUDA para convolução
- [ ] Usar grids 16×16 ou 32×32
- [ ] Versão 1: Sem shared memory
- [ ] Versão 2: Com shared memory (otimizada)
- [ ] Medir tempo de:
  - [ ] Transferência Host → Device
  - [ ] Execução do kernel
  - [ ] Transferência Device → Host
- [ ] Salvar imagem processada

**Opção B - OpenMP Target:**
- [ ] Criar arquivo `GPU_OpenMP_Target.c`
- [ ] Implementar com `#pragma omp target teams distribute parallel for`
- [ ] Configurar mapeamento de memória (map clauses)
- [ ] Medir custos de transferência
- [ ] Salvar imagem processada

---

### 3. Preparação de Dados

#### Imagens de Entrada:
- [ ] Obter/criar imagem base 20×20
- [ ] Redimensionar para **512 × 512**
- [ ] Redimensionar para **1024 × 1024**
- [ ] Redimensionar para **4096 × 4096**
- [ ] Salvar todas as versões em formato PNG

**Kernel Obrigatório (3×3 Blur):**
```
K = 1/9 * | 1  1  1 |
          | 1  1  1 |
          | 1  1  1 |
```

---

### 4. Medições e Testes

#### Sistema de Medição de Tempo:
- [ ] Implementar timer preciso (clock_gettime ou similar)
- [ ] Medir APENAS a convolução (excluir I/O)
- [ ] Executar 10 repetições para cada configuração
- [ ] Calcular média dos tempos
- [ ] Calcular desvio padrão

#### Configurações de Teste:
- [ ] **Sequencial:** 3 tamanhos de imagem
- [ ] **Threads:** 3 tamanhos × N threads (1, 2, 4, 8)
- [ ] **OpenMP:** 3 tamanhos × N threads × 3 estratégias
- [ ] **GPU:** 3 tamanhos (comparar versões com/sem shared memory)

#### Métricas a Calcular:
- [ ] Speedup: `S = T_sequencial / T_paralelo`
- [ ] Eficiência: `E = S / N_threads`
- [ ] Overhead de paralelização
- [ ] Custo de transferência CPU-GPU

---

### 5. Análise e Visualização

#### Gráficos Obrigatórios:
- [ ] **Gráfico 1:** Speedup × Tamanho da imagem
  - Comparar todas as 4 versões
- [ ] **Gráfico 2:** Speedup × Número de threads (CPU)
  - Comparar Threads vs OpenMP
- [ ] **Gráfico 3:** Tempo total × Técnica
  - Para cada tamanho de imagem

#### Script de Geração:
- [ ] Criar script Python/gnuplot para plotar gráficos
- [ ] Exportar gráficos em formato PNG/PDF

---

### 6. Relatório Final

#### Estrutura do Relatório:
- [ ] **1. Introdução**
  - Objetivo do trabalho
  - Descrição do problema (convolução 2D)

- [ ] **2. Metodologia**
  - Descrição das 4 implementações
  - Hardware utilizado (specs da máquina)
  - Compiladores e flags
  - Procedimento de medição

- [ ] **3. Resultados Experimentais**
  - Tabelas com tempos médios
  - Tabelas com speedup e eficiência
  - Apresentação dos gráficos

- [ ] **4. Discussão**
  - Threads explícitas vs OpenMP
  - CPU vs GPU
  - Efeito do escalonamento no OpenMP
  - Impacto do tamanho da imagem
  - Overhead de criação de threads
  - Afinidade de cache
  - Custo de transferência CPU-GPU

- [ ] **5. Conclusões**
  - Qual técnica foi mais eficiente?
  - GPU superou CPU? Em quais tamanhos?
  - OpenMP vs threads manuais?
  - Onde paralelismo não trouxe ganho?
  - Custo-benefício de cada abordagem

- [ ] **6. Referências**

---

## 🛠️ Ferramentas Necessárias

### Instaladas/Disponíveis:
- ✅ GCC (para C/C++)
- ✅ pthread (biblioteca de threads)
- ✅ stb_image (headers incluídos no projeto)

### A Verificar/Instalar:
- [ ] OpenMP (verificar suporte no GCC: `gcc -fopenmp --version`)
- [ ] CUDA Toolkit (se escolher CUDA para GPU)
- [ ] OpenMP 5.0+ com suporte a offloading (se escolher OpenMP Target)
- [ ] Python + matplotlib/numpy (para gráficos)
- [ ] ImageMagick ou similar (para redimensionar imagens)

---

## 📅 Cronograma Sugerido (Recuperação de Atraso)

### Dia 1 (Hoje - 10/12):
- [ ] Corrigir `Sequencial.c` com convolução real
- [ ] Corrigir `Threads pthread.c` com imagens reais
- [ ] Preparar as 3 imagens de teste
- [ ] Implementar sistema de medição de tempo

### Dia 2 (11/12):
- [ ] Implementar versão OpenMP
- [ ] Executar todos os testes (Sequencial, Threads, OpenMP)
- [ ] Coletar dados de desempenho

### Dia 3 (12/12):
- [ ] Implementar versão GPU (CUDA ou OpenMP Target)
- [ ] Executar testes da GPU
- [ ] Gerar todos os gráficos

### Dia 4 (13/12):
- [ ] Escrever relatório completo
- [ ] Revisar código e documentação
- [ ] Preparar entrega final

---

## 📦 Estrutura de Entrega Final

```
EP-2-OAC2/
├── codigo/
│   ├── Sequencial.c
│   ├── Threads_pthread.c
│   ├── OpenMP.c
│   ├── GPU_CUDA.cu (ou GPU_OpenMP_Target.c)
│   ├── stb_image.h
│   ├── stb_image_write.h
│   └── Makefile
├── imagens/
│   ├── entrada_512.png
│   ├── entrada_1024.png
│   ├── entrada_4096.png
│   └── resultados/
├── dados/
│   ├── tempos_medicao.csv
│   └── metricas_calculadas.csv
├── graficos/
│   ├── speedup_vs_tamanho.png
│   ├── speedup_vs_threads.png
│   └── tempo_vs_tecnica.png
├── relatorio.pdf
└── README.md
```

---

## ⚠️ Observações Importantes

1. **Prazo vencido:** Priorizar implementações funcionais sobre otimizações
2. **Foco:** Garantir que as 4 versões funcionem corretamente
3. **Medições:** Ser consistente e rigoroso nas medições de tempo
4. **GPU:** Se CUDA não estiver disponível, usar OpenMP Target
5. **Relatório:** Ser honesto sobre limitações e problemas encontrados

---

## 🎯 Prioridades Críticas (TOP 5)

1. **URGENTE:** Corrigir Sequencial.c para aplicar convolução real
2. **URGENTE:** Corrigir Threads pthread.c para usar imagens reais
3. **ALTA:** Implementar versão OpenMP
4. **ALTA:** Implementar versão GPU
5. **MÉDIA:** Gerar gráficos e relatório

---

**Última atualização:** 10/12/2025
