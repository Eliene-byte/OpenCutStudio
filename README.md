# OpenCut Studio — Editor de vídeo open-source leve

[![Build Windows](https://github.com/Eliene-byte/OpenCutStudio/actions/workflows/build-windows.yml/badge.svg)](https://github.com/Eliene-byte/OpenCutStudio/actions)
![Qt6](https://img.shields.io/badge/Qt-6.6-green) ![Windows](https://img.shields.io/badge/Windows-x64-blue) ![MIT](https://img.shields.io/badge/license-MIT-lightgrey)

Repo: https://github.com/Eliene-byte/OpenCutStudio

Premiere (corte) + DaVinci (cor) + After Effects (texto/blur) + Photoshop-lite (imagem), feito do zero em **C++ Qt6**, rodando em notebooks modestos (2GB RAM, Celeron).

## Por que roda em PC fraco?
- Sem linkar libav: usa `ffmpeg.exe` como subprocesso (menos RAM).
- Filtros só-CPU baratos: `eq, gblur, scale fast_bilinear, fps`.
- Proxy/presets `Leve 480p/720p` (`veryfast`, CRF alto).
- Preview via QtMultimedia (sem decodificar 2x), timeline com QPainter simples.

## Compilar local (Windows)
```pwsh
# Qt 6.6 + MinGW + CMake
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Compilar no GitHub (download pronto)
1. Este código já está ligado a `https://github.com/Eliene-byte/OpenCutStudio`.
2. A cada `push` a Action **Build Windows** compila sozinha.
3. Baixe em **Actions → último run → Artifacts → OpenCutStudio-windows-x64.zip** (já vem com `ffmpeg.exe` + Qt DLLs).
4. Para release pública: crie tag `v0.1.0` — gera Release com zip.

## Uso
Importar → arraste na timeline (clique seleciona) → ajuste Brilho/Contraste/Saturação/Blur/Volume no Inspetor → Exportar.

## Estrutura
`src/core/` projeto + ffmpeg runner · `src/effects/` filter_complex · `src/ui/` MainWindow/Timeline/Preview/Inspetor/Bin
