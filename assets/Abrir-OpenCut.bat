@echo off
REM OpenCut Studio - launcher portatil (duplo clique aqui)
REM Nao precisa instalar nada: Qt + ffmpeg + fonte ja vao junto.
setlocal
cd /d "%~dp0"
if not exist "OpenCutStudio.exe" echo ERRO: OpenCutStudio.exe nao encontrado.& pause& exit /b 1
if not exist "ffmpeg.exe" echo AVISO: ffmpeg.exe ausente - export e thumbnail nao vao funcionar.
if not exist "platforms\qwindows.dll" echo AVISO: platforms\qwindows.dll ausente - reextraia o ZIP inteiro.
start "" "OpenCutStudio.exe"
