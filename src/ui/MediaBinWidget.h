#pragma once
#include <QListWidget>
class MediaBinWidget : public QListWidget {
    Q_OBJECT
public:
    explicit MediaBinWidget(QWidget *p=nullptr);
    void addFiles(const QStringList &f);
    QStringList files() const;
protected:
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;
};
