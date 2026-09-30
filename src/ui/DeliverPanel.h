#pragma once
#include <QWidget>
class QComboBox; class QSpinBox;
class DeliverPanel : public QWidget {
    Q_OBJECT
public:
    explicit DeliverPanel(QWidget *p=nullptr);
    QString preset() const;
    int outWidth() const; int outHeight() const; int outFps() const;
private:
    QComboBox *cbPreset, *cbRes, *cbFps;
};
