#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include "ui/MainWindow.h"

static void logToFile(const QString &msg) {
    QString path = QCoreApplication::applicationDirPath() + "/opencut_log.txt";
    // fallback: pasta temp se sem permissão
    QFile f(path);
    if (!f.open(QIODevice::Append | QIODevice::Text)) {
        f.setFileName(QDir::temp().absoluteFilePath("opencut_log.txt"));
        f.open(QIODevice::Append | QIODevice::Text);
    }
    QTextStream s(&f);
    s << QDateTime::currentDateTime().toString("[hh:mm:ss] ") << msg << "\n";
    f.flush(); f.close();
}

int main(int argc, char **argv) {
    // Log o mais cedo possível — antes de criar widgets
    QApplication a(argc, argv);
    logToFile("=== OpenCut iniciando === v0.2");
    logToFile("Dir: " + QCoreApplication::applicationDirPath());
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    try {
        MainWindow w;
        w.show();
        logToFile("Janela criada OK — event loop");
        int r = a.exec();
        logToFile(QString("Saída normal (%1)").arg(r));
        return r;
    } catch (const std::exception &e) {
        logToFile(QString("EXCEÇÃO std: ") + e.what());
        QMessageBox::critical(nullptr, "OpenCut", QString("Falha: ") + e.what());
        return 1;
    } catch (...) {
        logToFile("EXCEÇÃO desconhecida na inicialização");
        QMessageBox::critical(nullptr, "OpenCut", "Falha desconhecida ao abrir. Veja opencut_log.txt");
        return 1;
    }
}
