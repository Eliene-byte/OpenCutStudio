#pragma once
#include <QWidget>
#include "core/Project.h"
class QDoubleSpinBox; class QComboBox; class QLineEdit; class QSpinBox;
// Painel Fusão/After: transform + opacidade + fade + texto animado
class EffectsPanel : public QWidget {
    Q_OBJECT
public:
    explicit EffectsPanel(QWidget *p=nullptr);
    void setProject(Project *proj);
    void edit(int track, int clip);
signals: void changed();
private:
    Project *m_proj=nullptr; int m_t=-1, m_c=-1;
    QDoubleSpinBox *sbScale, *sbX, *sbY, *sbRot, *sbOp, *sbBlur, *sbFadeIn, *sbFadeOut;
    QLineEdit *edText; QSpinBox *sbTSize; QComboBox *cbColor;
    void apply();
};
