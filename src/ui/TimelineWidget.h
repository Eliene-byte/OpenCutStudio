#pragma once
#include <QWidget>
#include "core/Project.h"
class TimelineWidget : public QWidget {
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *p=nullptr);
    void setProject(Project *proj);
signals: void clipSelected(int track, int clip);
protected: void paintEvent(QPaintEvent*) override; void mousePressEvent(QMouseEvent*) override;
private: Project *m_proj=nullptr;
};
