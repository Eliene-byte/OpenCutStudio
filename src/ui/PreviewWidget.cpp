#include "ui/PreviewWidget.h"
#include "core/FFmpegRunner.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QPixmap>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>

PreviewWidget::PreviewWidget(QWidget *p): QWidget(p) {
    auto *l = new QVBoxLayout(this);
    m_img = new QLabel("▶ Preview\nImporte um vídeo\ne clique para ver aqui", this);
    m_img->setAlignment(Qt::AlignCenter);
    m_img->setMinimumHeight(300);
    m_img->setStyleSheet("background:#000; color:#fff; font-size:16px; border:1px solid #444;");
    m_img->setScaledContents(false);
    m_info = new QLabel("", this);
    m_open = new QPushButton("▶ Abrir no player do Windows", this);
    connect(m_open, &QPushButton::clicked, this, [this](){
        if (!m_file.isEmpty()) QDesktopServices::openUrl(QUrl::fromLocalFile(m_file));
    });
    l->addWidget(m_img); l->addWidget(m_info); l->addWidget(m_open);
    setLayout(l);
}
void PreviewWidget::load(const QString &f) {
    m_file = f;
    m_info->setText(f);
    makeThumb();
}
void PreviewWidget::playPause() {
    if (!m_file.isEmpty()) QDesktopServices::openUrl(QUrl::fromLocalFile(m_file));
}
void PreviewWidget::makeThumb() {
    if (m_file.isEmpty()) return;
    QString l = m_file.toLower();
    if (l.endsWith(".mp3") || l.endsWith(".wav")) {
        m_img->setText("♪ Áudio:\n" + QFileInfo(m_file).fileName());
        return;
    }
    // Gera thumbnail 480px com ffmpeg (rápido, 1 frame)
    QString tmp = QDir::temp().absoluteFilePath("opencut_thumb.jpg");
    QProcess p;
    p.start(FFmpegRunner::ffmpegPath(), {"-y","-v","error","-ss","1","-i",m_file,"-frames:v","1","-vf","scale=640:-1",tmp});
    p.waitForFinished(8000);
    QPixmap px(tmp);
    if (!px.isNull()) {
        m_img->setPixmap(px.scaled(QSize(640,360), Qt::KeepAspectRatio, Qt::FastTransformation));
    } else {
        m_img->setText("🎬 " + QFileInfo(m_file).fileName() + "\n(thumbnail indisponível, export funciona)");
    }
}
