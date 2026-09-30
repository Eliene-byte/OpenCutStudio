#pragma once
#include <QWidget>
#include "core/Project.h"
class TimelineWidget : public QWidget {
    Q_OBJECT
public:
    explicit TimelineWidget(QWidget *p=nullptr);
    void setProject(Project *proj);
signals:
    void clipSelected(int track, int clip);
    void filesDropped(QStringList files, double timeSec, int track);
    void clipMoved(int track, int clip, double newStart);
protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void dragEnterEvent(QDragEnterEvent*) override;
    void dropEvent(QDropEvent*) override;
private:
    Project *m_proj=nullptr;
    int m_dragT=-1, m_dragC=-1; bool m_dragging=false;
    double xToTime(int x) const;
    int yToTrack(int y) const;
};
