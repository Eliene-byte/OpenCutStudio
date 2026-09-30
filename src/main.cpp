#include <QApplication>
#include "ui/MainWindow.h"
int main(int argc, char **argv) {
    QApplication a(argc, argv);
    // Leve: desliga animação pesada em PCs modestos
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);
    MainWindow w; w.show();
    return a.exec();
}
