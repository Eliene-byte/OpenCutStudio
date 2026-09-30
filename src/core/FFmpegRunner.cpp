#include "FFmpegRunner.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>

FFmpegRunner::FFmpegRunner(QObject *parent): QObject(parent) {}

QString FFmpegRunner::ffmpegPath() {
    QString app = QCoreApplication::applicationDirPath();
    for (auto p : {app + "/ffmpeg.exe", app + "/bin/ffmpeg.exe", QString("ffmpeg.exe"), QString("ffmpeg")}) {
        if (QFileInfo::exists(p) || p == QString("ffmpeg.exe") || p == QString("ffmpeg")) {
            if (QFileInfo::exists(p)) return QDir(p).absolutePath();
        }
    }
    return "ffmpeg"; // cai no PATH
}
QString FFmpegRunner::ffprobePath() {
    QString app = QCoreApplication::applicationDirPath();
    QString c = app + "/ffprobe.exe";
    if (QFileInfo::exists(c)) return QDir(c).absolutePath();
    return "ffprobe";
}
double FFmpegRunner::probeDuration(const QString &file) {
    QProcess p;
    p.start(ffprobePath(), {"-v","quiet","-print_format","json","-show_format", file});
    p.waitForFinished(5000);
    auto doc = QJsonDocument::fromJson(p.readAllStandardOutput());
    return doc.object()["format"].toObject()["duration"].toString("0").toDouble();
}
QProcess *FFmpegRunner::run(const QStringList &args, const QString &workDir) {
    auto *p = new QProcess(this);
    if (!workDir.isEmpty()) p->setWorkingDirectory(workDir);
    connect(p, &QProcess::readyReadStandardError, this, [this,p](){ emit logLine(QString::fromUtf8(p->readAllStandardError())); });
    connect(p, &QProcess::readyReadStandardOutput, this, [this,p](){ emit logLine(QString::fromUtf8(p->readAllStandardOutput())); });
    p->start(ffmpegPath(), args);
    return p;
}
