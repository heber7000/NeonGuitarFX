# NeonGuitarFX - Compilação para Windows

## 🚀 Início Rápido

### Método Automático (Recomendado)

1. **Instale os pré-requisitos:**
   - Visual Studio 2022 Community (com "Desktop development with C++")
   - CMake 3.22+
   - Git for Windows

2. **Clone o JUCE** no diretório pai do projeto:
   ```bash
   cd ..
   git clone --branch 7.0.9 --depth 1 https://github.com/juce-framework/JUCE.git
   ```

3. **Execute o script de compilação:**
   - Abra o **Developer Command Prompt for VS 2022**
   - Navegue até a pasta do projeto
   - Execute: `build_windows.bat`

4. **Pronto!** O plugin estará em `build\NeonGuitarFX_artefacts\Release\VST3\`

### Método Manual

Consulte o arquivo `WINDOWS_BUILD_GUIDE.md` para instruções detalhadas.

## 📁 Estrutura de Diretórios

```
C:\Dev\
├── JUCE\                    # Clone do JUCE Framework
└── NeonGuitarFX\            # Este projeto
    ├── Source\
    ├── Libs\
    ├── build\               # Criado durante compilação
    └── build_windows.bat    # Script de compilação
```

## 🎯 Instalação do Plugin

Após a compilação, copie a pasta `NeonGuitarFX.vst3` para:
```
C:\Program Files\Common Files\VST3\
```

Ou execute o script `build_windows.bat` e escolha instalar automaticamente.

## ⚠️ Problemas Comuns

### "CMake not found"
- Instale o CMake: https://cmake.org/download/
- Marque "Add CMake to system PATH" durante a instalação

### "Visual Studio not found"
- Execute o script no **Developer Command Prompt for VS 2022**
- Não use o Command Prompt normal

### "JUCE not found"
- Clone o JUCE no diretório pai: `cd .. && git clone https://github.com/juce-framework/JUCE.git`

## 📖 Documentação Completa

- `WINDOWS_BUILD_GUIDE.md` - Guia completo de compilação
- `README.md` - Visão geral do projeto
- `NEXT_STEPS.md` - Próximas etapas de desenvolvimento

## 🆘 Suporte

Para mais informações, consulte:
- Documentação do JUCE: https://docs.juce.com/
- Fórum do JUCE: https://forum.juce.com/

---

**Desenvolvido com JUCE Framework** 🎸
