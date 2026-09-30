#include "ui/MediaBinWidget.h"
MediaBinWidget::MediaBinWidget(QWidget *p): QListWidget(p){}
void MediaBinWidget::addFiles(const QStringList &f){ addItems(f); }
QStringList MediaBinWidget::files() const {
    QStringList r; for(int i=0;i<count();i++) r<<item(i)->text(); return r;
}
