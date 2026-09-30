#pragma once
#include <QWidget>
#include "core/Project.h"
class QSlider; class QLabel;
// Mixer Fairlight-lite: volume por track + presets de resolução
class AudioMixer : public QWidget {
    Q_OBJECT
public:
    explicit AudioMixer(QWidget *p=nullptr);
    void setProject(Project *proj);
signals: void changed();
private: Project *m_proj=nullptr;
};
