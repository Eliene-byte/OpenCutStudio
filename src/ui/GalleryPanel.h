#pragma once
#include <QWidget>
class QListWidget; class QLineEdit;
// Galeria CapCut: busca + 1 clique aplica no bloco selecionado (efeitos "infinitos")
class GalleryPanel : public QWidget {
    Q_OBJECT
public:
    explicit GalleryPanel(QWidget *p=nullptr);
signals: void pickEffect(const QString &id);
private:
    QListWidget *m_list; QLineEdit *m_search;
    void refill(const QString &q = QString());
};
