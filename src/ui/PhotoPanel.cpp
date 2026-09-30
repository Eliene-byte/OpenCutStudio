#include "ui/PhotoPanel.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
PhotoPanel::PhotoPanel(QWidget *p): QWidget(p) {
    auto *f = new QFormLayout(this);
    f->addRow(new QLabel("<b>PHOTOSHOP + PREMIERE</b>"));
    sbCrop=new QDoubleSpinBox(this); sbCrop->setRange(0,40);
    sbHue=new QDoubleSpinBox(this); sbHue->setRange(-180,180);
    sbHi=new QDoubleSpinBox(this); sbHi->setRange(-1,1); sbHi->setSingleStep(0.05);
    sbSh=new QDoubleSpinBox(this); sbSh->setRange(-1,1); sbSh->setSingleStep(0.05);
    sbCl=new QDoubleSpinBox(this); sbCl->setRange(0,2); sbCl->setSingleStep(0.1);
    sbGrain=new QDoubleSpinBox(this); sbGrain->setRange(0,30);
    sbSpeed=new QDoubleSpinBox(this); sbSpeed->setRange(0.25,4); sbSpeed->setValue(1); sbSpeed->setSingleStep(0.05);
    ckFH=new QCheckBox("Espelhar H",this); ckFV=new QCheckBox("Espelhar V",this);
    edCustom=new QLineEdit(this); edCustom->setPlaceholderText("⚙ filtro ffmpeg livre ex: hue=s=0, vignette=PI/4");
    cbTrans=new QComboBox(this); cbTrans->addItems({"", "fade", "xfade"});
    f->addRow("Crop %",sbCrop); f->addRow("Matiz",sbHue);
    f->addRow("Realces",sbHi); f->addRow("Sombras",sbSh);
    f->addRow("Clarity",sbCl); f->addRow("Grão",sbGrain); f->addRow("Velocidade",sbSpeed);
    f->addRow(ckFH); f->addRow(ckFV);
    f->addRow("Custom ∞",edCustom); f->addRow("Transição",cbTrans);
    for(auto w:{sbCrop,sbHue,sbHi,sbSh,sbCl,sbGrain,sbSpeed}) connect(w,&QDoubleSpinBox::valueChanged,this,[this](double){apply();});
    connect(ckFH,&QCheckBox::toggled,this,[this](bool){apply();});
    connect(ckFV,&QCheckBox::toggled,this,[this](bool){apply();});
    connect(edCustom,&QLineEdit::textChanged,this,[this](const QString&){apply();});
    connect(cbTrans,&QComboBox::currentTextChanged,this,[this](const QString&){apply();});
    setLayout(f);
}
void PhotoPanel::setProject(Project *p){ m_proj=p; }
void PhotoPanel::edit(int t,int c){
    m_t=t; m_c=c; if(!m_proj||t<0||t>=m_proj->tracks.size()||c<0||c>=m_proj->tracks[t].clips.size()) return;
    auto &cl=m_proj->tracks[t].clips[c];
    sbCrop->setValue(cl.cropPct); sbHue->setValue(cl.hue); sbHi->setValue(cl.highlights);
    sbSh->setValue(cl.shadows); sbCl->setValue(cl.clarity); sbGrain->setValue(cl.grain);
    sbSpeed->setValue(cl.speed); ckFH->setChecked(cl.flipH); ckFV->setChecked(cl.flipV);
    edCustom->setText(cl.customFilter); cbTrans->setCurrentText(cl.transition);
}
void PhotoPanel::apply(){
    if(!m_proj||m_t<0||m_c<0) return;
    auto &cl=m_proj->tracks[m_t].clips[m_c];
    cl.cropPct=sbCrop->value(); cl.hue=sbHue->value(); cl.highlights=sbHi->value();
    cl.shadows=sbSh->value(); cl.clarity=sbCl->value(); cl.grain=sbGrain->value();
    cl.speed=sbSpeed->value(); cl.flipH=ckFH->isChecked(); cl.flipV=ckFV->isChecked();
    cl.customFilter=edCustom->text();
    if (!cl.customFilter.isEmpty()) cl.effect="custom";
    cl.transition=cbTrans->currentText();
    emit changed();
}
