#include "ui/PreviewWidget.h"
PreviewWidget::PreviewWidget(QWidget *p): QWidget(p) {
    m_p = new QMediaPlayer(this); m_v = new QVideoWidget(this);
    m_p->setVideoOutput(m_v);
    auto *l = new QVBoxLayout(this); l->addWidget(m_v); setLayout(l);
}
void PreviewWidget::load(const QString &f){ m_p->setSource(QUrl::fromLocalFile(f)); m_p->play(); }
void PreviewWidget::playPause(){ m_p->isPlaying() ? m_p->pause() : m_p->play(); }
