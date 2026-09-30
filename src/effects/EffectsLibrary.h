#pragma once
#include <QString>
#include <QList>
#include <QPair>
// Biblioteca "infinita": presets + filtro custom. Tudo vira ffmpeg — leve.
struct EffectDef { QString id; QString name; QString desc; QString filter; };
class EffectsLibrary {
public:
    static QList<EffectDef> all();
    static QString filterFor(const QString &id, const QString &custom = QString());
};
