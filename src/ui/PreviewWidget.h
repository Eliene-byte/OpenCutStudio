#pragma once
#include <QWidget>
#include <QString>
class QLabel; class QPushButton;
// Preview sem QtMultimedia: thumbnail via ffmpeg + abrir no player do sistema.
// Isso elimina o crash silencioso por falta de DLL/codec/GPU.
class PreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *p=nullptr);
    void load(const QString &file);
    void playPause();
private:
    QLabel *m_img; QLabel *m_info; QPushButton *m_open;
    QString m_file;
    void makeThumb();
};
