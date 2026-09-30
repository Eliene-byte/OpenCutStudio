#include "ui/TimelineWidget.h"
#include <QPainter> <QMouseEvent>
TimelineWidget::TimelineWidget(QWidget *p): QWidget(p){ setMinimumHeight(160); }
void TimelineWidget::setProject(Project *proj){ m_proj=proj; update(); }
void TimelineWidget::paintEvent(QPaintEvent*) {
    QPainter pt(this); pt.fillRect(rect(), QColor("#1e1e1e"));
    if (!m_proj) return;
    double total = qMax(10.0, m_proj->duration());
    double px = width() / total;
    QStringList cols = {"#2d6a4f", "#1d3557", "#6a4c93"};
    for (int t=0; t<m_proj->tracks.size(); t++) {
        int y = 10 + t*48, h = 40;
        pt.setPen(Qt::gray); pt.drawText(5, y+15, m_proj->tracks[t].kind);
        for (auto &c : m_proj->tracks[t].clips) {
            QRect r(int(c.startOnTrack*px), y, int(c.duration*px), h);
            pt.fillRect(r, QColor(cols[t % 3]));
            pt.setPen(Qt::white); pt.drawRect(r);
            pt.drawText(r.adjusted(4,4,-4,-4), Qt::AlignLeft|Qt::AlignTop,
                c.text.isEmpty() ? c.filePath.split("/").last().split("\\").last() : c.text);
        }
    }
}
void TimelineWidget::mousePressEvent(QMouseEvent *e) {
    if (!m_proj) return;
    double total = qMax(10.0, m_proj->duration()); double px = width()/total;
    for (int t=0;t<m_proj->tracks.size();t++){
        int y=10+t*48;
        if (e->pos().y()>=y && e->pos().y()<=y+40)
            for (int i=0;i<m_proj->tracks[t].clips.size();i++){
                auto &c=m_proj->tracks[t].clips[i];
                if (e->pos().x()>=c.startOnTrack*px && e->pos().x()<=(c.startOnTrack+c.duration)*px)
                    emit clipSelected(t,i);
            }
    }
}
