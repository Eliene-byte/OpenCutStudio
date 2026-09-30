#pragma once
#include <QWidget>
#include "core/Project.h"
class QDoubleSpinBox; class QCheckBox; class QLineEdit; class QComboBox;
// Photoshop: crop/flip/HSL/highlights/clarity/grain + custom ffmpeg + velocidade + transição
class PhotoPanel : public QWidget {
    Q_OBJECT
public:
    explicit PhotoPanel(QWidget *p=nullptr);
    void setProject(Project *proj);
    void edit(int track, int clip);
signals: void changed();
private:
    Project *m_proj=nullptr; int m_t=-1, m_c=-1;
    QDoubleSpinBox *sbCrop, *sbHue, *sbHi, *sbSh, *sbCl, *sbGrain, *sbSpeed;
    QCheckBox *ckFH, *ckFV;
    QLineEdit *edCustom;
    QComboBox *cbTrans;
    void apply();
};
