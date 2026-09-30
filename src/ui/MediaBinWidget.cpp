#include "ui/MediaBinWidget.h"
#include <QDrag>
#include <QMimeData>
MediaBinWidget::MediaBinWidget(QWidget *p): QListWidget(p){
    setDragEnabled(true);               // CapCut: arrastar da galeria
    setSelectionMode(QAbstractItemView::SingleSelection);
    setToolTip("Arraste para a timeline ⬇");
}
void MediaBinWidget::addFiles(const QStringList &f){
    for (auto &s : f) {
        auto *it = new QListWidgetItem("🎬 " + s.split("/").last().split("\\").last());
        it->setData(Qt::UserRole, s);   // caminho real
        it->setToolTip(s);
        addItem(it);
    }
}
QStringList MediaBinWidget::files() const {
    QStringList r; for(int i=0;i<count();i++) r<<item(i)->data(Qt::UserRole).toString(); return r;
}
