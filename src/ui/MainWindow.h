#pragma once
#include <QMainWindow>
#include <QProcess>
#include "core/Project.h"
#include "core/FFmpegRunner.h"
class TimelineWidget; class PreviewWidget; class InspectorWidget; class MediaBinWidget;
class ColorPanel; class EffectsPanel; class AudioMixer; class DeliverPanel;
class PhotoPanel; class GalleryPanel;
class QTextEdit; class QProgressBar; class QTabWidget; class QTabBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr);
private slots:
    void importMedia(); void exportVideo(); void onExportFinished(int, QProcess::ExitStatus);
    void saveProject(); void openProject(); void addTextClip(); void playPreview();
    void splitClip(); void deleteClip(); void onClipSelected(int track, int clip);
    void onFilesDropped(QStringList files, double t, int track);
    void onClipMoved(int t, int c, double nt);
    void quickFilter(const QString &fx);
    void setVertical(); void setHorizontal(); void autoCaption();
    void addFilesAt(const QStringList &files, double timeSec, int track);
private:
    Project m_proj;
    int m_selT=-1, m_selC=-1;
    TimelineWidget *m_timeline; PreviewWidget *m_preview;
    InspectorWidget *m_inspector; MediaBinWidget *m_bin;
    ColorPanel *m_color; EffectsPanel *m_fx; AudioMixer *m_mixer; DeliverPanel *m_deliver;
    PhotoPanel *m_photo; GalleryPanel *m_gallery;
    QTabWidget *m_rightTabs;
    QTextEdit *m_log; QProgressBar *m_prog;
    FFmpegRunner m_ff; QProcess *m_cur=nullptr;
    void refreshAll(); void log(const QString &s);
};
