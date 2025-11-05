# Guia de Compilação para Windows - NeonGuitarFX

Este guia detalha como compilar o plugin NeonGuitarFX para Windows.

## Pré-requisitos

### Software Necessário

1. **Visual Studio 2022 Community Edition** (gratuito)
   - Download: https://visualstudio.microsoft.com/downloads/
   - Durante a instalação, selecione:
     - "Desktop development with C++"
     - "Windows 10 SDK" (ou superior)

2. **CMake** (versão 3.22 ou superior)
   - Download: https://cmake.org/download/
   - Durante a instalação, marque "Add CMake to system PATH"

3. **Git for Windows**
   - Download: https://git-scm.com/download/win
   - Necessário para clonar o repositório e submódulos

## Passo a Passo

### 1. Clonar o Repositório

Abra o **Git Bash** ou **PowerShell** e execute:

```bash
# Clonar o projeto
git clone <URL_DO_REPOSITORIO> NeonGuitarFX
cd NeonGuitarFX

# Inicializar submódulos do NAM
cd Libs/NeuralAmpModelerCore
git submodule update --init --recursive
cd ../..
```

### 2. Baixar o JUCE

```bash
# Baixar JUCE 7.0.9
cd ..
git clone --branch 7.0.9 --depth 1 https://github.com/juce-framework/JUCE.git
```

### 3. Configurar com CMake

Abra o **Developer Command Prompt for VS 2022** e execute:

```bash
cd NeonGuitarFX
mkdir build
cd build

# Configurar o projeto
cmake .. -G "Visual Studio 17 2022" -A x64
```

### 4. Compilar o Projeto

**Opção 1: Via Command Line**

```bash
cmake --build . --config Release
```

**Opção 2: Via Visual Studio**

1. Abra o arquivo `NeonGuitarFX.sln` na pasta `build`
2. Selecione a configuração **Release** no topo
3. Clique com o botão direito em **NeonGuitarFX_VST3** no Solution Explorer
4. Selecione **Build**

### 5. Localizar o Plugin Compilado

Após a compilação bem-sucedida, o plugin estará em:

```
build\NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3
```

### 6. Instalar o Plugin

**Copiar para o diretório de plugins do sistema:**

```bash
# Diretório padrão do VST3 no Windows
xcopy /E /I "build\NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3" "%COMMONPROGRAMFILES%\VST3\NeonGuitarFX.vst3"
```

Ou manualmente copie a pasta `NeonGuitarFX.vst3` para:
- `C:\Program Files\Common Files\VST3\`

## Solução de Problemas

### Erro: "JUCE not found"

**Solução**: Certifique-se de que o JUCE está no diretório correto. O CMakeLists.txt espera encontrar o JUCE em `/home/ubuntu/JUCE` (Linux) ou você precisa ajustar o caminho.

**Ajustar o CMakeLists.txt para Windows:**

Edite o arquivo `CMakeLists.txt` e altere a linha:

```cmake
# De:
add_subdirectory(/home/ubuntu/JUCE /home/ubuntu/NeonGuitarFX/build/JUCE)

# Para:
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JUCE ${CMAKE_CURRENT_BINARY_DIR}/JUCE)
```

### Erro: "Cannot find ALSA"

**Solução**: ALSA é específico do Linux. No Windows, o JUCE usa WASAPI/ASIO automaticamente. Você pode ignorar este aviso.

### Erro de Compilação com NAM

Se houver erros relacionados ao NeuralAmpModelerCore:

1. Verifique se os submódulos foram inicializados:
   ```bash
   cd Libs/NeuralAmpModelerCore
   git submodule update --init --recursive
   ```

2. Se o erro persistir, comente temporariamente a integração do NAM no CMakeLists.txt

### Erro: "MSBuild not found"

**Solução**: Certifique-se de estar usando o **Developer Command Prompt for VS 2022**, não o Command Prompt normal.

## Compilação para Formato CLAP

Para compilar também o formato CLAP, certifique-se de que `FORMATS VST3 CLAP` está presente no CMakeLists.txt (já está configurado).

O plugin CLAP estará em:
```
build\NeonGuitarFX_artefacts\Release\CLAP\NeonGuitarFX.clap
```

Instale copiando para:
- `C:\Program Files\Common Files\CLAP\`

## Otimizações de Compilação

### Compilação Otimizada para Performance

Para uma compilação otimizada:

```bash
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel
```

### Compilação com Símbolos de Debug

Para debugging:

```bash
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

## Estrutura de Diretórios Esperada

```
C:\Dev\
├── JUCE\                          # Framework JUCE
│   ├── modules\
│   ├── extras\
│   └── ...
└── NeonGuitarFX\                  # Projeto do plugin
    ├── Source\
    ├── Libs\
    │   ├── NeuralAmpModelerCore\
    │   └── SignalsmithStretch\
    ├── build\                     # Diretório de compilação
    └── CMakeLists.txt
```

## Alternativa: Usar o Projucer

Se preferir usar o Projucer (GUI do JUCE):

1. Abra o Projucer (em `JUCE/extras/Projucer/Builds/VisualStudio2022/`)
2. Compile o Projucer primeiro
3. Abra o Projucer e crie um novo projeto de plugin de áudio
4. Configure os módulos JUCE necessários
5. Adicione os arquivos fonte do NeonGuitarFX
6. Exporte para Visual Studio 2022
7. Compile no Visual Studio

## Testando o Plugin

### DAWs Recomendadas para Teste

- **Reaper** (trial gratuito): https://www.reaper.fm/
- **Cakewalk by BandLab** (gratuito): https://www.bandlab.com/products/cakewalk
- **Tracktion Waveform Free**: https://www.tracktion.com/products/waveform-free

### Plugin Host para Testes

- **JUCE AudioPluginHost**: Incluído no JUCE em `extras/AudioPluginHost`
- **Kushview Element**: https://kushview.net/element/

## Criando um Instalador (Opcional)

Para criar um instalador profissional:

1. **Inno Setup** (gratuito): https://jrsoftware.org/isinfo.php
2. **NSIS**: https://nsis.sourceforge.io/

Exemplo de script Inno Setup:

```inno
[Setup]
AppName=NeonGuitarFX
AppVersion=1.0
DefaultDirName={commonpf}\VST3
OutputBaseFilename=NeonGuitarFX_Setup

[Files]
Source: "build\NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3\*"; DestDir: "{commonpf}\VST3\NeonGuitarFX.vst3"; Flags: recursesubdirs
```

## Suporte e Recursos

- **Documentação do JUCE**: https://docs.juce.com/
- **Fórum do JUCE**: https://forum.juce.com/
- **CMake Documentation**: https://cmake.org/documentation/

## Notas Importantes

1. **Licença do JUCE**: Para uso comercial, é necessária uma licença do JUCE. Para uso pessoal/open-source, a GPL v3 é suficiente.

2. **Assinatura de Código**: Para distribuição profissional, considere assinar o plugin com um certificado de código.

3. **Testes**: Sempre teste o plugin em múltiplas DAWs antes de distribuir.

---

**Boa sorte com a compilação!** 🎸🔊
