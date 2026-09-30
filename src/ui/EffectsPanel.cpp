#include "ui/EffectsPanel.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
EffectsPanel::EffectsPanel(QWidget *p): QWidget(p) {
    auto *f = new QFormLayout(this);
    f->addRow(new QLabel("<b>FUSÃO — After lite (Transform + Texto)</b>"));
    sbScale=new QDoubleSpinBox(this); sbScale->setRange(0.25,4); sbScale->setValue(1); sbScale->setSingleStep(0.05);
    sbX=new QDoubleSpinBox(this); sbX->setRange(-800,800);
    sbY=new QDoubleSpinBox(this); sbY->setRange(-800,800);
    sbRot=new QDoubleSpinBox(this); sbRot->setRange(-180,180);
    sbOp=new QDoubleSpinBox(this); sbOp->setRange(0,1); sbOp->setValue(1); sbOp->setSingleStep(0.05);
    sbBlur=new QDoubleSpinBox(this); sbBlur->setRange(0,10);
    sbFadeIn=new QDoubleSpinBox(this); sbFadeIn->setRange(0,5);
    sbFadeOut=new QDoubleSpinBox(this); sbFadeOut->setRange(0,5);
    edText=new QLineEdit(this); sbTSize=new QSpinBox(this); sbTSize->setRange(8,200); sbTSize->setValue(48);
    cbColor=new QComboBox(this); cbColor->addItems({"white","black","yellow","red","cyan"});
    f->addRow("Escala",sbScale); f->addRow("Pos X",sbX); f->addRow("Pos Y",sbY);
    f->addRow("Rotação",sbRot); f->addRow("Opacidade",sbOp); f->addRow("Blur",sbBlur);
    f->addRow("Fade In (s)",sbFadeIn); f->addRow("Fade Out (s)",sbFadeOut);
    f->addRow("Texto",edText); f->addRow("Tam.fonte",sbTSize); f->addRow("Cor texto",cbColor);
    for(auto w: {sbScale,sbX,sbY,sbRot,sbOp,sbBlur,sbFadeIn,sbFadeOut}) connect(w,&QDoubleSpinBox::valueChanged,this,[this](double){apply();});
    connect(edText,&QLineEdit::textChanged,this,[this](const QString&){apply();});
    connect(sbTSize,&QSpinBox::valueChanged,this,[this](int){apply();});
    connect(cbColor,&QComboBox::currentTextChanged,this,[this](const QString&){apply();});
    setLayout(f);
}
void EffectsPanel::setProject(Project *p){ m_proj=p; }
void EffectsPanel::edit(int t,int c){
    m_t=t; m_c=c; if(!m_proj||t<0||t>=m_proj->tracks.size()||c<0||c>=m_proj->tracks[t].clips.size()) return;
    auto &cl=m_proj->tracks[t].clips[c];
    sbScale->setValue(cl.scale); sbX->setValue(cl.posX); sbY->setValue(cl.posY);
    sbRot->setValue(cl.rotation); sbOp->setValue(cl.opacity); sbBlur->setValue(cl.blur);
    sbFadeIn->setValue(cl.fadeIn); sbFadeOut->setValue(cl.fadeOut);
    edText->setText(cl.text); sbTSize->setValue(cl.textSize); cbColor->setCurrentText(cl.textColor);
}
void EffectsPanel::apply(){
    if(!m_proj||m_t<0||m_c<0) return;
    auto &cl=m_proj->tracks[m_t].clips[m_c];
    cl.scale=sbScale->value(); cl.posX=sbX->value(); cl.posY=sbY->value();
    cl.rotation=sbRot->value(); cl.opacity=sbOp->value(); cl.blur=sbBlur->value();
    cl.fadeIn=sbFadeIn->value(); cl.fadeOut=sbFadeOut->value();
    cl.text=edText->text(); cl.textSize=sbTSize->value(); cl.textColor=cbColor->currentText();
    emit changed();
}
