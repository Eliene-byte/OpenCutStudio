#pragma once
#include <QWidget>
#include "core/Project.h"
class QDoubleSpinBox; class QLineEdit; class QSpinBox; class QComboBox;
class InspectorWidget : public QWidget {
    Q_OBJECT
public:
    explicit InspectorWidget(QWidget *p=nullptr);
    void setProject(Project *proj);
    void edit(int track, int clip);
signals: void changed();
private:
    Project *m_proj=nullptr; int m_t=-1, m_c=-1;
    QDoubleSpinBox *sbBri, *sbCon, *sbSat, *sbBlur, *sbVol, *sbDur;
    QLineEdit *edText; QSpinBox *sbTSize; QComboBox *cbColor;
    void apply();
};
