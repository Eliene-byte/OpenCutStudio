#include "FFmpegRunner.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

FFmpegRunner::FFmpegRunner(QObject *parent): QObject(parent) {}

QString FFmpegRunner::ffmpegPath() {
    QString app = QCoreApplication::applicationDirPath();
    QStringList cand = {app + "/ffmpeg.exe", app + "/bin/ffmpeg.exe"};
    for (auto &p : cand) {
        if (QFileInfo::exists(p)) return QDir::toNativeSeparators(QFileInfo(p).absoluteFilePath());
    }
    return "ffmpeg"; // cai no PATH
}
QString FFmpegRunner::ffprobePath() {
    QString app = QCoreApplication::applicationDirPath();
    QString c = app + "/ffprobe.exe";
    if (QFileInfo::exists(c)) return QDir::toNativeSeparators(QFileInfo(c).absoluteFilePath());
    return "ffprobe";
}
double FFmpegRunner::probeDuration(const QString &file) {
    if (file.isEmpty() || !QFileInfo::exists(file)) return 0;
    QProcess p;
    p.start(ffprobePath(), {"-v","quiet","-print_format","json","-show_format", file});
    if (!p.waitForFinished(8000)) { p.kill(); return 0; }
    auto doc = QJsonDocument::fromJson(p.readAllStandardOutput());
    QJsonValue d = doc.object()["format"].toObject()["duration"];
    if (d.isString()) return d.toString("0").toDouble();
    return d.toDouble(0);
}
QProcess *FFmpegRunner::run(const QStringList &args, const QString &workDir) {
    auto *p = new QProcess(this);
    if (!workDir.isEmpty()) p->setWorkingDirectory(workDir);
    connect(p, &QProcess::readyReadStandardError, this, [this,p](){ emit logLine(QString::fromUtf8(p->readAllStandardError())); });
    connect(p, &QProcess::readyReadStandardOutput, this, [this,p](){ emit logLine(QString::fromUtf8(p->readAllStandardOutput())); });
    p->start(ffmpegPath(), args);
    return p;
}
