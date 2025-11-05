@echo off
REM Script de compilação automática para Windows
REM NeonGuitarFX Plugin VST3/CLAP

echo ========================================
echo NeonGuitarFX - Compilacao para Windows
echo ========================================
echo.

REM Verificar se o CMake está instalado
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERRO: CMake nao encontrado!
    echo Por favor, instale o CMake e adicione ao PATH.
    echo Download: https://cmake.org/download/
    pause
    exit /b 1
)

REM Verificar se o Visual Studio está instalado
where cl >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERRO: Visual Studio nao encontrado!
    echo Por favor, execute este script no "Developer Command Prompt for VS 2022"
    pause
    exit /b 1
)

REM Criar diretório de build
if not exist "build" mkdir build
cd build

echo.
echo [1/3] Configurando projeto com CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% NEQ 0 (
    echo ERRO: Falha na configuracao do CMake
    cd ..
    pause
    exit /b 1
)

echo.
echo [2/3] Compilando plugin (Release)...
cmake --build . --config Release --parallel
if %ERRORLEVEL% NEQ 0 (
    echo ERRO: Falha na compilacao
    cd ..
    pause
    exit /b 1
)

echo.
echo [3/3] Compilacao concluida com sucesso!
echo.
echo Plugin VST3 localizado em:
echo   build\NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3
echo.

REM Perguntar se deseja instalar
set /p INSTALL="Deseja instalar o plugin no sistema? (S/N): "
if /i "%INSTALL%"=="S" (
    echo.
    echo Instalando plugin...
    if not exist "%COMMONPROGRAMFILES%\VST3" mkdir "%COMMONPROGRAMFILES%\VST3"
    xcopy /E /I /Y "NeonGuitarFX_artefacts\Release\VST3\NeonGuitarFX.vst3" "%COMMONPROGRAMFILES%\VST3\NeonGuitarFX.vst3"
    if %ERRORLEVEL% EQU 0 (
        echo Plugin instalado com sucesso em:
        echo   %COMMONPROGRAMFILES%\VST3\NeonGuitarFX.vst3
    ) else (
        echo AVISO: Falha na instalacao. Execute como Administrador.
    )
)

cd ..
echo.
echo Processo concluido!
pause
