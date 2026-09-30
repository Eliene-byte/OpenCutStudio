#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include "ui/MainWindow.h"

static void logToFile(const QString &msg) {
    QFile f(QCoreApplication::applicationDirPath() + "/opencut_log.txt");
    f.open(QIODevice::Append | QIODevice::Text);
    QTextStream s(&f);
    s << QDateTime::currentDateTime().toString("[hh:mm:ss] ") << msg << "\n";
}

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    logToFile("=== OpenCut iniciando ===");
    try {
        MainWindow w;
        w.show();
        logToFile("Janela criada OK");
        return a.exec();
    } catch (const std::exception &e) {
        logToFile(QString("EXCEÇÃO: ") + e.what());
        QMessageBox::critical(nullptr, "OpenCut", QString("Falha: ") + e.what());
        return 1;
    }
}
