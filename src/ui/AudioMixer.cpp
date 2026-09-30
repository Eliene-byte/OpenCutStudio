#include "ui/AudioMixer.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
AudioMixer::AudioMixer(QWidget *p): QWidget(p) {
    auto *h = new QHBoxLayout(this);
    h->addWidget(new QLabel("<b>MIXER</b>"));
    for (int i=0;i<3;i++) {
        auto *v = new QVBoxLayout();
        auto *l = new QLabel(QString("Trilha %1").arg(i+1));
        auto *s = new QSlider(Qt::Vertical, this);
        s->setRange(0,200); s->setValue(100);
        connect(s,&QSlider::valueChanged,this,[this,i](int val){
            if(!m_proj||i>=m_proj->tracks.size()) return;
            for(auto &c: m_proj->tracks[i].clips) c.volume = val/100.0;
            emit changed();
        });
        v->addWidget(l); v->addWidget(s);
        h->addLayout(v);
    }
    setLayout(h);
}
void AudioMixer::setProject(Project *p){ m_proj=p; }
