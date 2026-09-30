#include "ui/ColorPanel.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
ColorPanel::ColorPanel(QWidget *p): QWidget(p) {
    auto *f = new QFormLayout(this);
    f->addRow(new QLabel("<b>COR — DaVinci lite</b>"));
    sbBri=new QDoubleSpinBox(this); sbBri->setRange(-1,1); sbBri->setSingleStep(0.05);
    sbCon=new QDoubleSpinBox(this); sbCon->setRange(0,3); sbCon->setValue(1);
    sbSat=new QDoubleSpinBox(this); sbSat->setRange(0,3); sbSat->setValue(1);
    sbTemp=new QDoubleSpinBox(this); sbTemp->setRange(-1,1); sbTemp->setSingleStep(0.05);
    sbTint=new QDoubleSpinBox(this); sbTint->setRange(-1,1); sbTint->setSingleStep(0.05);
    sbVig=new QDoubleSpinBox(this); sbVig->setRange(0,1); sbVig->setSingleStep(0.05);
    cbLut=new QComboBox(this); cbLut->addItems({"Nenhum","P&B","Cinematic","Vintage","Sharpen"});
    f->addRow("Brilho",sbBri); f->addRow("Contraste",sbCon); f->addRow("Saturação",sbSat);
    f->addRow("Temperatura",sbTemp); f->addRow("Tint",sbTint); f->addRow("Vinheta",sbVig);
    f->addRow("Efeito",cbLut);
    for(auto w: {sbBri,sbCon,sbSat,sbTemp,sbTint,sbVig}) connect(w,&QDoubleSpinBox::valueChanged,this,[this](double){apply();});
    connect(cbLut,&QComboBox::currentTextChanged,this,[this](const QString&){apply();});
    setLayout(f);
}
void ColorPanel::setProject(Project *p){ m_proj=p; }
void ColorPanel::edit(int t,int c){
    m_t=t; m_c=c; if(!m_proj||t<0||t>=m_proj->tracks.size()||c<0||c>=m_proj->tracks[t].clips.size()) return;
    auto &cl=m_proj->tracks[t].clips[c];
    sbBri->setValue(cl.brightness); sbCon->setValue(cl.contrast); sbSat->setValue(cl.saturation);
    sbTemp->setValue(cl.temperature); sbTint->setValue(cl.tint); sbVig->setValue(cl.vignette);
    QString e = cl.effect;
    cbLut->setCurrentText(e=="pb"?"P&B":e=="cinematic"?"Cinematic":e=="vintage"?"Vintage":e=="sharpen"?"Sharpen":"Nenhum");
}
void ColorPanel::apply(){
    if(!m_proj||m_t<0||m_c<0) return;
    auto &cl=m_proj->tracks[m_t].clips[m_c];
    cl.brightness=sbBri->value(); cl.contrast=sbCon->value(); cl.saturation=sbSat->value();
    cl.temperature=sbTemp->value(); cl.tint=sbTint->value(); cl.vignette=sbVig->value();
    QString t=cbLut->currentText();
    cl.effect = t=="P&B"?"pb":t=="Cinematic"?"cinematic":t=="Vintage"?"vintage":t=="Sharpen"?"sharpen":"";
    emit changed();
}
