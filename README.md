# NeonGuitarFX - Plugin VST3/CLAP de Guitarra com Neural Amp Modeler

## Visão Geral

O **NeonGuitarFX** é um plugin de áudio VST3/CLAP completo para processamento de guitarra, desenvolvido em C++ usando o framework JUCE. O plugin integra múltiplas tecnologias de ponta para oferecer uma experiência de processamento de áudio profissional com uma interface neon moderna e responsiva.

## Funcionalidades Principais

### 1. Neural Amp Modeler (NAM)
- **2 slots independentes** para carregamento de arquivos `.nam`:
  - **Slot 1**: Simulação de amplificador
  - **Slot 2**: Simulação de pedal/distorção/overdrive
- Processamento em **stereo**
- Controles de ganho de entrada e saída para cada slot

### 2. Impulse Response (IR) Loader
- **2 slots independentes** para carregamento de arquivos de IR
- Suporte para processamento stereo (L/R independentes)
- Controle de mix (dry/wet)

### 3. Delay Stereo Avançado
- **Dois canais de delay independentes** (L/R)
- **Tipos de delay**:
  - Digital
  - Analógico
  - Tape delay
- **Modos de sincronização**:
  - Manual (ms)
  - Colcheia pontuada (sincronizado com BPM do host)
- **Controles**:
  - Feedback
  - Mix (dry/wet)
  - Tempo de delay para L e R
  - Ping-pong
  - Mono/Stereo

### 4. Reverb
- Processamento em **mono ou stereo**
- **Controles**:
  - Room Size
  - Damping
  - Wet Level
  - Dry Level
  - Width
  - Freeze Mode

### 5. Pitch Shifter/Harmonist Avançado
- Baseado na biblioteca **Signalsmith Stretch**
- **Modos de operação**:
  - **Harmonia Não Inteligente**: Intervalo fixo (-24 a +24 semitons)
  - **Harmonia Inteligente**: Segue escalas musicais
- **Escalas suportadas**:
  - Maior
  - Menor
  - Frígio
  - Jônico
  - Dórico
  - Lócrio
  - Árabe
- Controle de mix (dry/wet)
- Seleção de tom raiz (C, C#, D, etc.)

### 6. Panorama
- Controle de panorama para o sinal processado

## Estrutura do Projeto

```
NeonGuitarFX/
├── Source/
│   ├── PluginProcessor.h         # Processador principal do plugin
│   ├── PluginProcessor.cpp
│   ├── PluginEditor.h            # Interface gráfica do plugin
│   ├── PluginEditor.cpp
│   ├── NAMProcessor.h            # Wrapper para Neural Amp Modeler
│   └── NAMProcessor.cpp
├── Libs/
│   ├── NeuralAmpModelerCore/     # Biblioteca NAM
│   └── SignalsmithStretch/       # Biblioteca de pitch shifting
├── build/                        # Diretório de compilação
├── CMakeLists.txt                # Configuração do CMake
├── README.md                     # Este arquivo
├── PROGRESS_PHASE2.md            # Progresso da Fase 2
└── plugin_research_summary.md    # Resumo da pesquisa inicial
```

## Compilação

### Pré-requisitos

- **Sistema Operacional**: Linux (Ubuntu 22.04 ou superior)
- **Compilador**: GCC 11 ou superior
- **CMake**: 3.22 ou superior
- **Bibliotecas**:
  - JUCE Framework (7.0.9)
  - libasound2-dev
  - libx11-dev, libxext-dev, libxrandr-dev, libxinerama-dev, libxcursor-dev
  - mesa-common-dev

### Passos para Compilar

```bash
# 1. Clonar o repositório (se ainda não foi feito)
cd /home/ubuntu/NeonGuitarFX

# 2. Inicializar os submódulos do NAM
cd Libs/NeuralAmpModelerCore
git submodule update --init --recursive
cd ../..

# 3. Criar o diretório de build
mkdir -p build
cd build

# 4. Configurar com CMake
cmake ..

# 5. Compilar
make -j$(nproc)

# 6. O plugin VST3 estará em:
# build/NeonGuitarFX_artefacts/VST3/NeonGuitarFX.vst3
```

## Status de Implementação

### ✅ Concluído

1. **Fase 1**: Pesquisa e análise de tecnologias
2. **Fase 2**: Configuração do ambiente e estrutura do projeto
3. Estrutura básica do plugin JUCE
4. Sistema de parâmetros (APVTS)
5. Wrapper básico para NAMProcessor
6. Compilação bem-sucedida do VST3

### 🚧 Em Progresso (Fase 3)

1. Integração completa do Neural Amp Modeler
2. Implementação do IR Loader
3. Integração do Signalsmith Stretch para pitch shifting

### 📋 Próximas Etapas

#### Fase 3: Processamento de Áudio Core
- [ ] Implementar carregamento de arquivos `.nam` no NAMProcessor
- [ ] Integrar a biblioteca NeuralAmpModelerCore
- [ ] Implementar o IR Loader com suporte a arquivos WAV/AIFF
- [ ] Integrar o Signalsmith Stretch
- [ ] Implementar detecção de pitch para harmonização inteligente
- [ ] Criar lógica de escalas musicais para harmonização

#### Fase 4: Efeitos de Delay e Reverb
- [ ] Implementar delay stereo com tipos (digital, analógico, tape)
- [ ] Implementar sincronização com BPM do host
- [ ] Implementar colcheia pontuada
- [ ] Implementar ping-pong
- [ ] Configurar reverb com todos os parâmetros

#### Fase 5: Interface Gráfica Neon
- [ ] Criar componentes personalizados com estilo neon
- [ ] Implementar cores frias (azul, ciano, roxo)
- [ ] Adicionar animações modernas
- [ ] Implementar responsividade e escalabilidade
- [ ] Criar file choosers para NAM e IR
- [ ] Adicionar visualizadores de forma de onda

#### Fase 6: Integração e Testes
- [ ] Conectar GUI com processamento de áudio
- [ ] Testar todos os efeitos
- [ ] Otimizar performance
- [ ] Corrigir bugs

#### Fase 7: Compilação Final
- [ ] Compilar VST3
- [ ] Compilar CLAP
- [ ] Testar em DAWs (Reaper, Bitwig, etc.)
- [ ] Corrigir erros finais

#### Fase 8: Entrega
- [ ] Documentação de usuário
- [ ] Guia de instalação
- [ ] Exemplos de uso

## Parâmetros do Plugin

| Categoria | Parâmetro | Tipo | Faixa | Padrão |
|-----------|-----------|------|-------|--------|
| **NAM Amp** | Gain | Float | 0.0 - 2.0 | 1.0 |
| **NAM Pedal** | Gain | Float | 0.0 - 2.0 | 1.0 |
| **IR** | Mix | Float | 0.0 - 1.0 | 1.0 |
| **Delay** | Feedback | Float | 0.0 - 1.0 | 0.5 |
| **Delay** | Mix | Float | 0.0 - 1.0 | 0.5 |
| **Delay** | Time Left (ms) | Float | 1.0 - 2000.0 | 500.0 |
| **Delay** | Time Right (ms) | Float | 1.0 - 2000.0 | 500.0 |
| **Delay** | Ping Pong | Bool | - | false |
| **Delay** | Dotted 8th | Bool | - | false |
| **Delay** | Type | Choice | Digital/Analog/Tape | Digital |
| **Delay** | Mono | Bool | - | false |
| **Reverb** | Room Size | Float | 0.0 - 1.0 | 0.5 |
| **Reverb** | Damping | Float | 0.0 - 1.0 | 0.5 |
| **Reverb** | Wet Level | Float | 0.0 - 1.0 | 0.33 |
| **Reverb** | Dry Level | Float | 0.0 - 1.0 | 0.4 |
| **Reverb** | Width | Float | 0.0 - 1.0 | 1.0 |
| **Reverb** | Freeze | Float | 0.0 - 1.0 | 0.0 |
| **Reverb** | Stereo | Bool | - | true |
| **Pitch** | Mix | Float | 0.0 - 1.0 | 0.5 |
| **Pitch** | Mode | Choice | Fixed/Intelligent | Fixed |
| **Pitch** | Interval | Int | -24 - 24 | 0 |
| **Pitch** | Scale Root | Choice | C, C#, D, ... | C |
| **Pitch** | Scale Type | Choice | Major, Minor, ... | Major |
| **Pan** | Panorama | Float | -1.0 - 1.0 | 0.0 |

## Tecnologias Utilizadas

- **JUCE Framework 7.0.9**: Framework C++ para desenvolvimento de plugins de áudio
- **NeuralAmpModelerCore**: Biblioteca C++ para simulação de amplificadores usando deep learning
- **Signalsmith Stretch**: Biblioteca C++ para pitch e time stretching polifônico
- **CMake**: Sistema de build multiplataforma
- **Eigen**: Biblioteca de álgebra linear (dependência do NAM)

## Licença

Este projeto é desenvolvido como parte de um estudo de caso. Consulte as licenças individuais das bibliotecas utilizadas:
- JUCE: GPL v3 ou licença comercial
- NeuralAmpModelerCore: MIT
- Signalsmith Stretch: MIT

## Contato e Suporte

Para dúvidas ou sugestões, consulte a documentação do JUCE e das bibliotecas utilizadas.

---

**Desenvolvido com ❤️ usando JUCE, NAM e Signalsmith Stretch**
