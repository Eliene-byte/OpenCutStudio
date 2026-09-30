#pragma once
#include <QWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QVBoxLayout>
class PreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *p=nullptr);
    void load(const QString &file);
    void playPause();
private: QMediaPlayer *m_p; QVideoWidget *m_v;
};
