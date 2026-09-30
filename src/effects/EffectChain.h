#pragma once
#include "core/Project.h"
#include <QString>
#include <QStringList>

// Gera filter_complex + inputs para ffmpeg.
// Leve: usa apenas filtros CPU baratos (eq, gblur, scale) — roda em Celeron/2GB.
class EffectChain {
public:
    struct Plan {
        QStringList inputArgs; // pares -ss/-t/-i já ordenados
        QString filterComplex;
        QStringList mapArgs;   // -map ... finais
        int nVideoInputs = 0;
    };
    static QString eqFilter(const Clip &c);
    static Plan build(const Project &p, const QString &outPath, const QString &preset);
};
