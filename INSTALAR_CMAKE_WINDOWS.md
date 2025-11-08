# 🔧 Como Instalar o CMake no Windows (Passo a Passo)

## Método 1: Instalação Manual (Recomendado)

### Passo 1: Baixar o CMake

1. Acesse: **https://cmake.org/download/**
2. Procure por "Latest Release" (versão mais recente)
3. Baixe o arquivo: **`cmake-X.XX.X-windows-x86_64.msi`**
   - Exemplo: `cmake-3.28.1-windows-x86_64.msi`
   - Escolha o arquivo `.msi` (instalador)

### Passo 2: Instalar o CMake

1. Execute o arquivo `.msi` baixado
2. Clique em **"Next"**
3. Aceite os termos de licença
4. **IMPORTANTE**: Na tela "Install Options", selecione:
   - ✅ **"Add CMake to the system PATH for all users"**
   - OU
   - ✅ **"Add CMake to the system PATH for current user"**
5. Clique em **"Next"** → **"Install"**
6. Aguarde a instalação
7. Clique em **"Finish"**

### Passo 3: Verificar a Instalação

1. Abra um **novo** Command Prompt (cmd)
   - Pressione `Win + R`
   - Digite `cmd`
   - Pressione Enter

2. Digite:
   ```
   cmake --version
   ```

3. Se aparecer algo como:
   ```
   cmake version 3.28.1
   ```
   ✅ **CMake instalado com sucesso!**

4. Se aparecer:
   ```
   'cmake' is not recognized...
   ```
   ❌ Veja "Solução de Problemas" abaixo

---

## Método 2: Instalação via Chocolatey (Automático)

### Passo 1: Instalar o Chocolatey

1. Abra o **PowerShell como Administrador**:
   - Pressione `Win + X`
   - Selecione "Windows PowerShell (Admin)"

2. Cole este comando:
   ```powershell
   Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
   ```

3. Pressione Enter e aguarde

### Passo 2: Instalar o CMake

1. No mesmo PowerShell, digite:
   ```
   choco install cmake -y
   ```

2. Aguarde a instalação

3. Feche e abra um novo Command Prompt

4. Verifique:
   ```
   cmake --version
   ```

---

## Método 3: Usar o Developer Command Prompt

Se você já tem o **Visual Studio 2022** instalado:

1. Pressione `Win + S`
2. Digite: **"Developer Command Prompt for VS 2022"**
3. Abra o prompt que aparecer
4. Execute o `build_windows.bat` **dentro deste prompt**

O Visual Studio já inclui uma versão do CMake!

---

## Solução de Problemas

### "cmake não é reconhecido como comando"

**Causa**: O CMake não foi adicionado ao PATH durante a instalação.

**Solução 1: Reinstalar com PATH**
1. Desinstale o CMake pelo Painel de Controle
2. Reinstale marcando a opção "Add to PATH"

**Solução 2: Adicionar ao PATH Manualmente**

1. Pressione `Win + Pause/Break` (ou clique com direito em "Este Computador" → "Propriedades")
2. Clique em "Configurações avançadas do sistema"
3. Clique em "Variáveis de Ambiente"
4. Em "Variáveis do sistema", encontre "Path"
5. Clique em "Editar"
6. Clique em "Novo"
7. Adicione: `C:\Program Files\CMake\bin`
8. Clique em "OK" em todas as janelas
9. **Feche e abra um novo Command Prompt**
10. Teste: `cmake --version`

### "Acesso negado" ao instalar

**Solução**: Execute o instalador como Administrador
- Clique com botão direito no arquivo `.msi`
- Selecione "Executar como administrador"

### CMake instalado mas build_windows.bat não funciona

**Solução**: Feche e abra um novo Command Prompt
- O PATH só é atualizado em novos terminais

---

## Instalação Completa (CMake + Visual Studio + Git)

Se você quer instalar tudo de uma vez:

### Opção A: Script Automático

1. Execute o arquivo **`setup_and_build.bat`** como Administrador
   - Clique com direito → "Executar como administrador"
   - Ele instalará tudo automaticamente

### Opção B: Manual

1. **Visual Studio 2022 Community**
   - Download: https://visualstudio.microsoft.com/downloads/
   - Durante instalação, marque: "Desktop development with C++"

2. **CMake**
   - Download: https://cmake.org/download/
   - Marque "Add to PATH"

3. **Git for Windows**
   - Download: https://git-scm.com/download/win
   - Deixe as opções padrão

---

## Verificação Final

Após instalar tudo, abra um **novo** Command Prompt e teste:

```batch
cmake --version
git --version
cl
```

Se todos os comandos funcionarem, você está pronto para compilar!

---

## Próximos Passos

Após instalar o CMake:

1. Abra um **novo** Command Prompt
2. Navegue até a pasta do NeonGuitarFX
3. Execute: `build_windows.bat`

Ou use o script automático: `setup_and_build.bat`

---

## Links Úteis

- **CMake Download**: https://cmake.org/download/
- **Visual Studio**: https://visualstudio.microsoft.com/downloads/
- **Git for Windows**: https://git-scm.com/download/win
- **Chocolatey**: https://chocolatey.org/install

---

**Tempo de Instalação**: 5-10 minutos  
**Espaço em Disco**: ~500 MB (CMake) + ~7 GB (Visual Studio)
