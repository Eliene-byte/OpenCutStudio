#pragma once
#include <QMainWindow>
#include <QProcess>
#include "core/Project.h"
#include "core/FFmpegRunner.h"
class TimelineWidget; class PreviewWidget; class InspectorWidget; class MediaBinWidget;
class QTextEdit; class QComboBox; class QProgressBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr);
private slots:
    void importMedia(); void exportVideo(); void onExportFinished(int, QProcess::ExitStatus);
    void saveProject(); void openProject(); void addTextClip(); void playPreview();
private:
    Project m_proj;
    TimelineWidget *m_timeline; PreviewWidget *m_preview;
    InspectorWidget *m_inspector; MediaBinWidget *m_bin;
    QTextEdit *m_log; QComboBox *m_preset; QProgressBar *m_prog;
    FFmpegRunner m_ff; QProcess *m_cur=nullptr;
    void refreshAll(); void log(const QString &s);
};
