@echo off
REM ========================================
REM NeonGuitarFX - Setup e Build Automático
REM ========================================

echo.
echo ========================================
echo NeonGuitarFX - Instalacao Automatica
echo ========================================
echo.

REM Verificar se está rodando como Administrador
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ERRO: Este script precisa ser executado como Administrador!
    echo.
    echo Por favor:
    echo 1. Clique com botao direito neste arquivo
    echo 2. Selecione "Executar como administrador"
    echo.
    pause
    exit /b 1
)

echo [1/5] Verificando Chocolatey...
where choco >nul 2>nul
if %errorLevel% neq 0 (
    echo Instalando Chocolatey...
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))"
    if %errorLevel% neq 0 (
        echo ERRO: Falha ao instalar Chocolatey
        pause
        exit /b 1
    )
    echo Chocolatey instalado com sucesso!
    REM Recarregar PATH
    call refreshenv
) else (
    echo Chocolatey ja instalado!
)

echo.
echo [2/5] Verificando CMake...
where cmake >nul 2>nul
if %errorLevel% neq 0 (
    echo Instalando CMake...
    choco install cmake -y
    if %errorLevel% neq 0 (
        echo ERRO: Falha ao instalar CMake
        pause
        exit /b 1
    )
    echo CMake instalado com sucesso!
    call refreshenv
) else (
    echo CMake ja instalado!
)

echo.
echo [3/5] Verificando Git...
where git >nul 2>nul
if %errorLevel% neq 0 (
    echo Instalando Git...
    choco install git -y
    if %errorLevel% neq 0 (
        echo ERRO: Falha ao instalar Git
        pause
        exit /b 1
    )
    echo Git instalado com sucesso!
    call refreshenv
) else (
    echo Git ja instalado!
)

echo.
echo [4/5] Verificando Visual Studio Build Tools...
where cl >nul 2>nul
if %errorLevel% neq 0 (
    echo.
    echo ========================================
    echo ATENCAO: Visual Studio nao encontrado!
    echo ========================================
    echo.
    echo Voce precisa instalar o Visual Studio 2022 Community manualmente:
    echo.
    echo 1. Acesse: https://visualstudio.microsoft.com/downloads/
    echo 2. Baixe "Visual Studio 2022 Community"
    echo 3. Durante a instalacao, marque:
    echo    - "Desktop development with C++"
    echo    - "Windows 10 SDK"
    echo 4. Apos instalar, execute este script novamente
    echo.
    echo Ou execute o script no "Developer Command Prompt for VS 2022"
    echo.
    pause
    exit /b 1
) else (
    echo Visual Studio Build Tools encontrado!
)

echo.
echo [5/5] Clonando JUCE Framework...
if not exist "..\JUCE" (
    echo Clonando JUCE 7.0.9...
    cd ..
    git clone --branch 7.0.9 --depth 1 https://github.com/juce-framework/JUCE.git
    if %errorLevel% neq 0 (
        echo ERRO: Falha ao clonar JUCE
        cd NeonGuitarFX
        pause
        exit /b 1
    )
    cd NeonGuitarFX
    echo JUCE clonado com sucesso!
) else (
    echo JUCE ja existe!
)

echo.
echo ========================================
echo Inicializando submodulos...
echo ========================================
cd Libs\NeuralAmpModelerCore
git submodule update --init --recursive
cd ..\..

echo.
echo ========================================
echo Compilando NeonGuitarFX...
echo ========================================

if not exist "build" mkdir build
cd build

echo.
echo Configurando com CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if %errorLevel% neq 0 (
    echo ERRO: Falha na configuracao do CMake
    cd ..
    pause
    exit /b 1
)

echo.
echo Compilando (Release)...
cmake --build . --config Release --parallel
if %errorLevel% neq 0 (
    echo ERRO: Falha na compilacao
    cd ..
    pause
    exit /b 1
)

cd ..

echo.
echo ========================================
echo SUCESSO!
echo ========================================
echo.
echo Plugin compilado em:
echo   build\NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3
echo.

set /p INSTALL="Deseja instalar o plugin no sistema? (S/N): "
if /i "%INSTALL%"=="S" (
    echo.
    echo Instalando plugin...
    if not exist "%COMMONPROGRAMFILES%\VST3" mkdir "%COMMONPROGRAMFILES%\VST3"
    xcopy /E /I /Y "build\NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3" "%COMMONPROGRAMFILES%\VST3\NeonGuitarFX.vst3"
    if %errorLevel% equ 0 (
        echo.
        echo ========================================
        echo Plugin instalado com sucesso!
        echo ========================================
        echo.
        echo Localizacao: %COMMONPROGRAMFILES%\VST3\NeonGuitarFX.vst3
        echo.
        echo Abra o Reaper e escaneie os plugins VST3!
    ) else (
        echo ERRO: Falha na instalacao
    )
)

echo.
echo Processo concluido!
pause
