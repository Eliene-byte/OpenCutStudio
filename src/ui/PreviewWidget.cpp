#include "ui/PreviewWidget.h"
#include <QLabel>
PreviewWidget::PreviewWidget(QWidget *p): QWidget(p), m_p(nullptr), m_v(nullptr) {
    auto *l = new QVBoxLayout(this);
    // Vídeo criado de forma preguiçosa: se QtMultimedia falhar (PC sem codec/GPU),
    // o app continua abrindo com um placeholder em vez de crashar.
    try {
        m_p = new QMediaPlayer(this);
        m_v = new QVideoWidget(this);
        m_p->setVideoOutput(m_v);
        l->addWidget(m_v);
    } catch (...) {
        l->addWidget(new QLabel("Preview indisponível neste PC — exportação funciona normal."));
    }
    if (!m_v) l->addWidget(new QLabel("▶ Preview (importe um vídeo)"));
    setLayout(l);
}
void PreviewWidget::load(const QString &f){
    if (!m_p) return;
    try { m_p->setSource(QUrl::fromLocalFile(f)); m_p->play(); } catch (...) {}
}
void PreviewWidget::playPause(){
    if (!m_p) return;
    try { m_p->playbackState() == QMediaPlayer::PlayingState ? m_p->pause() : m_p->play(); } catch (...) {}
}
