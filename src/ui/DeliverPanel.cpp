#include "ui/DeliverPanel.h"
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
DeliverPanel::DeliverPanel(QWidget *p): QWidget(p) {
    auto *f = new QFormLayout(this);
    f->addRow(new QLabel("<b>ENTREGA — Deliver</b>"));
    cbPreset=new QComboBox(this); cbPreset->addItems({"Padrão 720p","Leve 720p","Leve 480p","Full 1080p"});
    cbRes=new QComboBox(this); cbRes->addItems({"1280x720","854x480","1920x1080","640x360"});
    cbFps=new QComboBox(this); cbFps->addItems({"24","30","60"});
    cbFps->setCurrentText("30");
    f->addRow("Preset",cbPreset); f->addRow("Resolução",cbRes); f->addRow("FPS",cbFps);
    setLayout(f);
}
QString DeliverPanel::preset() const { return cbPreset->currentText(); }
int DeliverPanel::outWidth() const { return cbRes->currentText().split("x")[0].toInt(); }
int DeliverPanel::outHeight() const { return cbRes->currentText().split("x")[1].toInt(); }
int DeliverPanel::outFps() const { return cbFps->currentText().toInt(); }
