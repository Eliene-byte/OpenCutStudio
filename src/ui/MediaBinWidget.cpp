#include "ui/MediaBinWidget.h"
#include <QMimeData>
#include <QUrl>
MediaBinWidget::MediaBinWidget(QWidget *p): QListWidget(p){
    setDragEnabled(true);               // CapCut: arrastar da galeria
    setSelectionMode(QAbstractItemView::SingleSelection);
    setToolTip("Arraste para a timeline");
}
void MediaBinWidget::addFiles(const QStringList &f){
    for (auto &s : f) {
        auto *it = new QListWidgetItem(s.split("/").last().split("\\").last());
        it->setData(Qt::UserRole, s);   // caminho real
        it->setToolTip(s);
        addItem(it);
    }
}
QStringList MediaBinWidget::files() const {
    QStringList r; for(int i=0;i<count();i++) r<<item(i)->data(Qt::UserRole).toString(); return r;
}
// Envia file:// URLs para a Timeline aceitar o drop (antes ia datalist e caía no vazio)
QStringList MediaBinWidget::mimeTypes() const { return {"text/uri-list", "text/plain"}; }
QMimeData *MediaBinWidget::mimeData(const QList<QListWidgetItem *> &items) const {
    auto *m = new QMimeData();
    QList<QUrl> urls; QStringList texts;
    for (auto *it : items) {
        QString f = it->data(Qt::UserRole).toString();
        if (!f.isEmpty()) { urls << QUrl::fromLocalFile(f); texts << f; }
    }
    m->setUrls(urls);
    m->setText(texts.join("\n"));
    return m;
}
