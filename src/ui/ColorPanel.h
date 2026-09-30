#pragma once
#include <QWidget>
#include "core/Project.h"
class QDoubleSpinBox; class QComboBox; class QSlider;
// Painel Cor estilo DaVinci: Temp/Tint/Vinheta + presets + wheels simplificados em sliders
class ColorPanel : public QWidget {
    Q_OBJECT
public:
    explicit ColorPanel(QWidget *p=nullptr);
    void setProject(Project *proj);
    void edit(int track, int clip);
signals: void changed();
private:
    Project *m_proj=nullptr; int m_t=-1, m_c=-1;
    QDoubleSpinBox *sbTemp, *sbTint, *sbVig, *sbSat, *sbCon, *sbBri;
    QComboBox *cbLut;
    void apply();
};
