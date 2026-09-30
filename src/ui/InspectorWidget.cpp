#include "ui/InspectorWidget.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
InspectorWidget::InspectorWidget(QWidget *p): QWidget(p) {
    auto *f = new QFormLayout(this);
    f->addRow(new QLabel("<b>Inspetor (Cor/Efeito)</b>"));
    sbBri=new QDoubleSpinBox(this); sbBri->setRange(-1,1); sbBri->setSingleStep(0.05);
    sbCon=new QDoubleSpinBox(this); sbCon->setRange(0,3); sbCon->setValue(1);
    sbSat=new QDoubleSpinBox(this); sbSat->setRange(0,3); sbSat->setValue(1);
    sbBlur=new QDoubleSpinBox(this); sbBlur->setRange(0,10);
    sbVol=new QDoubleSpinBox(this); sbVol->setRange(0,2); sbVol->setValue(1);
    sbDur=new QDoubleSpinBox(this); sbDur->setRange(0.5,600);
    edText=new QLineEdit(this); sbTSize=new QSpinBox(this); sbTSize->setRange(8,200); sbTSize->setValue(48);
    cbColor=new QComboBox(this); cbColor->addItems({"white","black","yellow","red","cyan"});
    f->addRow("Brilho",sbBri); f->addRow("Contraste",sbCon); f->addRow("Saturação",sbSat);
    f->addRow("Blur",sbBlur); f->addRow("Volume",sbVol); f->addRow("Duração",sbDur);
    f->addRow("Texto",edText); f->addRow("Tam.fonte",sbTSize); f->addRow("Cor texto",cbColor);
    for(auto w: {sbBri,sbCon,sbSat,sbBlur,sbVol,sbDur}) connect(w,&QDoubleSpinBox::valueChanged,this,[this](double){apply();});
    connect(edText,&QLineEdit::textChanged,this,[this](const QString&){apply();});
    connect(sbTSize,&QSpinBox::valueChanged,this,[this](int){apply();});
    connect(cbColor,&QComboBox::currentTextChanged,this,[this](const QString&){apply();});
    setLayout(f);
}
void InspectorWidget::setProject(Project *p){ m_proj=p; }
void InspectorWidget::edit(int t,int c){
    m_t=t; m_c=c; if(!m_proj||t<0||t>=m_proj->tracks.size()||c<0||c>=m_proj->tracks[t].clips.size()) return;
    auto &cl=m_proj->tracks[t].clips[c];
    sbBri->setValue(cl.brightness); sbCon->setValue(cl.contrast); sbSat->setValue(cl.saturation);
    sbBlur->setValue(cl.blur); sbVol->setValue(cl.volume); sbDur->setValue(cl.duration);
    edText->setText(cl.text); sbTSize->setValue(cl.textSize); cbColor->setCurrentText(cl.textColor);
}
void InspectorWidget::apply(){
    if(!m_proj||m_t<0||m_c<0) return;
    auto &cl=m_proj->tracks[m_t].clips[m_c];
    cl.brightness=sbBri->value(); cl.contrast=sbCon->value(); cl.saturation=sbSat->value();
    cl.blur=sbBlur->value(); cl.volume=sbVol->value(); cl.duration=sbDur->value();
    cl.text=edText->text(); cl.textSize=sbTSize->value(); cl.textColor=cbColor->currentText();
    emit changed();
}
