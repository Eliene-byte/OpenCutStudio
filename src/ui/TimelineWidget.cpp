#include "ui/TimelineWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
TimelineWidget::TimelineWidget(QWidget *p): QWidget(p){
    setMinimumHeight(190);
    setAcceptDrops(true);   // CapCut: soltar aqui
    setMouseTracking(true);
}
void TimelineWidget::setProject(Project *proj){ m_proj=proj; update(); }
double TimelineWidget::xToTime(int x) const {
    double total = qMax(10.0, m_proj ? m_proj->duration() : 10.0);
    double px = (width()-70) / total;
    return qMax(0.0, (x - 60) / px);
}
int TimelineWidget::yToTrack(int y) const {
    for (int t=0; t<3; t++) { int ty=18+t*52; if (y>=ty && y<=ty+44) return t; }
    return 0;
}
void TimelineWidget::paintEvent(QPaintEvent*) {
    QPainter pt(this); pt.fillRect(rect(), QColor("#141414"));
    pt.setPen(QColor("#888"));
    for (int s=0; s<=60; s+=5) {
        int x = 60 + s * (width()-70) / 60;
        pt.drawLine(x, 2, x, 12); pt.drawText(x+2, 11, QString("%1s").arg(s));
    }
    pt.setPen(QColor("#666"));
    pt.drawText(5, height()-8, "⬇ Arraste vídeos aqui (estilo CapCut)");
    if (!m_proj) return;
    double total = qMax(10.0, m_proj->duration());
    double px = (width()-70) / total;
    QStringList names = {"V2", "V1", "A1"};
    QStringList cols = {"#8d6e00", "#2d6a4f", "#1d3557"};
    for (int t=0; t<m_proj->tracks.size(); t++) {
        int y = 18 + t*52, h = 44;
        pt.fillRect(0, y, 60, h, QColor("#232323"));
        pt.setPen(Qt::white); pt.drawText(8, y+26, names.value(t, QString("T%1").arg(t)));
        for (int i=0;i<m_proj->tracks[t].clips.size();i++) {
            auto &c = m_proj->tracks[t].clips[i];
            QRect r(int(60 + c.startOnTrack*px), y, int(qMax(8.0, c.duration*px)), h);
            if (t==m_dragT && i==m_dragC) pt.fillRect(r, QColor("#ffffff88"));
            pt.fillRect(r, QColor(cols[t % 3]));
            if (!c.effect.isEmpty() || c.temperature != 0) pt.fillRect(r.adjusted(0,0,0,-h+8), QColor("#ffffff55"));
            pt.setPen((t==m_dragT&&i==m_dragC)?Qt::yellow:Qt::white); pt.drawRect(r);
            QString label = !c.text.isEmpty() ? QString("T: %1").arg(c.text)
                : c.filePath.split("/").last().split("\\").last();
            if (!c.effect.isEmpty()) label += QString(" [%1]").arg(c.effect);
            pt.drawText(r.adjusted(4,4,-4,-4), Qt::AlignLeft|Qt::AlignTop, label);
            if (c.fadeIn > 0 || c.fadeOut > 0) {
                pt.setPen(Qt::yellow);
                pt.drawLine(r.topLeft(), r.bottomLeft()); pt.drawLine(r.topRight(), r.bottomRight());
            }
        }
    }
}
void TimelineWidget::mousePressEvent(QMouseEvent *e) {
    if (!m_proj) return;
    double total = qMax(10.0, m_proj->duration()); double px = (width()-70)/total;
    m_dragT=m_dragC=-1; m_dragging=false;
    for (int t=0;t<m_proj->tracks.size();t++){
        int y=18+t*52;
        if (e->pos().y()>=y && e->pos().y()<=y+44)
            for (int i=0;i<m_proj->tracks[t].clips.size();i++){
                auto &c=m_proj->tracks[t].clips[i];
                if (e->pos().x()>=60+c.startOnTrack*px && e->pos().x()<=60+(c.startOnTrack+c.duration)*px) {
                    m_dragT=t; m_dragC=i; m_dragging=true;
                    emit clipSelected(t,i);
                }
            }
    }
    update();
}
void TimelineWidget::mouseMoveEvent(QMouseEvent *e) {
    if (m_dragging && m_dragT>=0 && e->buttons() & Qt::LeftButton) {
        double nt = xToTime(e->pos().x());
        emit clipMoved(m_dragT, m_dragC, nt);
    }
}
void TimelineWidget::mouseReleaseEvent(QMouseEvent *) { m_dragging=false; m_dragT=m_dragC=-1; update(); }
void TimelineWidget::dragEnterEvent(QDragEnterEvent *e) {
    if (e->mimeData()->hasUrls() || e->mimeData()->hasText()) e->acceptProposedAction();
}
void TimelineWidget::dropEvent(QDropEvent *e) {
    QStringList files;
    for (auto &u : e->mimeData()->urls()) files << u.toLocalFile();
    if (files.isEmpty() && e->mimeData()->hasText())
        files << e->mimeData()->text().split("\n", Qt::SkipEmptyParts);
    // item da MediaBin usa UserRole mas chega como texto do item; tenta urls primeiro
    files.removeAll("");
    if (!files.isEmpty()) {
        emit filesDropped(files, xToTime(e->position().toPoint().x()), yToTrack(e->position().toPoint().y()));
        e->acceptProposedAction();
    }
}
