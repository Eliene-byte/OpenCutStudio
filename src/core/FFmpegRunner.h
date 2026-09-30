#pragma once
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QObject>

// Roda ffmpeg/ffprobe como subprocesso (leve, sem linkar libav).
// No GitHub Actions o ffmpeg.exe é baixado e colocado ao lado do .exe.
class FFmpegRunner : public QObject {
    Q_OBJECT
public:
    explicit FFmpegRunner(QObject *parent=nullptr);
    static QString ffmpegPath();
    static QString ffprobePath();
    static double probeDuration(const QString &file);
    QProcess *run(const QStringList &args, const QString &workDir = QString());
signals:
    void logLine(const QString &line);
};
