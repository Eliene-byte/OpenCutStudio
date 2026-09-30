#include "ui/MainWindow.h"
#include "ui/TimelineWidget.h"
#include "ui/PreviewWidget.h"
#include "ui/InspectorWidget.h"
#include "ui/MediaBinWidget.h"
#include "ui/ColorPanel.h"
#include "ui/EffectsPanel.h"
#include "ui/AudioMixer.h"
#include "ui/DeliverPanel.h"
#include "ui/Theme.h"
#include "effects/EffectChain.h"
#include <QSplitter>
#include <QToolBar>
#include <QFileDialog>
#include <QTextEdit>
#include <QTabWidget>
#include <QProgressBar>
#include <QMessageBox>
#include <QJsonDocument>
#include <QFile>
#include <QInputDialog>
#include <QUuid>
#include <QLabel>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    setWindowTitle("OpenCut Studio — Premiere + DaVinci + After + Photoshop (leve)");
    resize(1400, 900);
    setStyleSheet(davinciThemeQss());

    // Barra topo estilo Premiere/DaVinci
    auto *tb = addToolBar("Main");
    tb->addAction("📁 Importar", this, &MainWindow::importMedia);
    tb->addAction("＋ Texto", this, &MainWindow::addTextClip);
    tb->addAction("✂ Dividir", this, &MainWindow::splitClip);
    tb->addAction("🗑 Excluir", this, &MainWindow::deleteClip);
    tb->addSeparator();
    tb->addAction("💾 Salvar", this, &MainWindow::saveProject);
    tb->addAction("📂 Abrir", this, &MainWindow::openProject);
    tb->addSeparator();
    tb->addAction("▶ Preview", this, &MainWindow::playPreview);
    tb->addAction("⬇ Exportar", this, &MainWindow::exportVideo);

    m_bin = new MediaBinWidget(this);
    m_preview = new PreviewWidget(this);
    m_inspector = new InspectorWidget(this);
    m_inspector->setProject(&m_proj);
    m_color = new ColorPanel(this); m_color->setProject(&m_proj);
    m_fx = new EffectsPanel(this); m_fx->setProject(&m_proj);
    m_mixer = new AudioMixer(this); m_mixer->setProject(&m_proj);
    m_deliver = new DeliverPanel(this);

    // Abas direitas = páginas DaVinci: Editar | Fusão | Cor | Áudio | Entrega
    m_rightTabs = new QTabWidget(this);
    m_rightTabs->addTab(m_inspector, "Editar");
    m_rightTabs->addTab(m_fx, "Fusão");
    m_rightTabs->addTab(m_color, "Cor");
    m_rightTabs->addTab(m_mixer, "Áudio");
    m_rightTabs->addTab(m_deliver, "Entrega");
    m_rightTabs->setMinimumWidth(300);

    m_timeline = new TimelineWidget(this);
    m_timeline->setProject(&m_proj);

    auto *top = new QSplitter(Qt::Horizontal, this);
    m_bin->setMinimumWidth(200);
    top->addWidget(m_bin); top->addWidget(m_preview); top->addWidget(m_rightTabs);
    top->setSizes({240, 760, 320});

    m_log = new QTextEdit(this); m_log->setReadOnly(true); m_log->setMaximumHeight(110);
    m_prog = new QProgressBar(this); m_prog->setRange(0,0); m_prog->hide();

    auto *main = new QSplitter(Qt::Vertical, this);
    main->addWidget(top); main->addWidget(m_timeline); main->addWidget(m_log); main->addWidget(m_prog);
    main->setSizes({560, 220, 80, 20});
    setCentralWidget(main);
    statusBar()->showMessage("Mídia | Corte | Edição | Fusão | Cor | Áudio | Entrega — clique num bloco da timeline para editar");

    auto sel = [this](int t,int c){ onClipSelected(t,c); };
    connect(m_timeline, &TimelineWidget::clipSelected, this, sel);
    connect(m_inspector, &InspectorWidget::changed, this, &MainWindow::refreshAll);
    connect(m_color, &ColorPanel::changed, this, &MainWindow::refreshAll);
    connect(m_fx, &EffectsPanel::changed, this, &MainWindow::refreshAll);
    connect(m_mixer, &AudioMixer::changed, this, &MainWindow::refreshAll);
    connect(&m_ff, &FFmpegRunner::logLine, this, &MainWindow::log);
    log("OpenCut pronto. FFmpeg: " + FFmpegRunner::ffmpegPath());
}
void MainWindow::log(const QString &s){ m_log->append(s.mid(0, 2000)); }
void MainWindow::refreshAll(){ m_timeline->update(); }
void MainWindow::onClipSelected(int t,int c){
    m_selT=t; m_selC=c;
    m_inspector->edit(t,c); m_color->edit(t,c); m_fx->edit(t,c);
    statusBar()->showMessage(QString("Selecionado: trilha %1 bloco %2").arg(t).arg(c));
}
void MainWindow::importMedia() {
    auto fs = QFileDialog::getOpenFileNames(this, "Importar mídia", "", "Mídia (*.mp4 *.mkv *.mov *.mp3 *.wav *.png *.jpg *.jpeg *.webp)");
    if (fs.isEmpty()) return;
    m_bin->addFiles(fs);
    double cursor = m_proj.duration();
    for (auto &f : fs) {
        Clip c; c.id = QUuid::createUuid().toString();
        c.filePath = f;
        QString l = f.toLower();
        c.kind = (l.endsWith(".mp3")||l.endsWith(".wav")) ? "audio" : ((l.endsWith(".png")||l.endsWith(".jpg")||l.endsWith(".jpeg")||l.endsWith(".webp")) ? "image" : "video");
        if (c.kind == "image") c.duration = 5.0;
        else { double d = FFmpegRunner::probeDuration(f); c.duration = d > 0 ? d : 5.0; if (c.kind=="audio") c.duration = qMin(c.duration, 600.0); }
        c.startOnTrack = cursor;
        bool isAudio = (c.kind == "audio");
        m_proj.addClip(isAudio ? 2 : 0, c);
        if (!isAudio && !f.isEmpty()) m_preview->load(f);
        cursor += c.duration;
    }
    refreshAll(); log(QString("Importado %1 arquivo(s).").arg(fs.size()));
}
void MainWindow::addTextClip() {
    QString t = QInputDialog::getText(this, "Texto", "Texto do título:");
    if (t.isEmpty()) return;
    Clip c; c.id = QUuid::createUuid().toString(); c.kind="video"; c.text=t;
    c.duration=3.0; c.startOnTrack=m_proj.duration();
    m_proj.addClip(1, c); refreshAll();
}
void MainWindow::splitClip() {
    if (m_selT<0||m_selC<0) { QMessageBox::information(this,"Dividir","Selecione um bloco na timeline."); return; }
    auto &cl = m_proj.tracks[m_selT].clips[m_selC];
    if (cl.duration < 1.0) return;
    Clip b = cl; b.id = QUuid::createUuid().toString();
    double half = cl.duration/2;
    cl.duration = half; b.startOnTrack = cl.startOnTrack + half; b.duration = half; b.inPoint += half;
    m_proj.tracks[m_selT].clips.insert(m_selC+1, b);
    refreshAll();
}
void MainWindow::deleteClip() {
    if (m_selT<0||m_selC<0) return;
    m_proj.tracks[m_selT].clips.removeAt(m_selC);
    m_selT=m_selC=-1; refreshAll();
}
void MainWindow::saveProject() {
    auto f = QFileDialog::getSaveFileName(this, "Salvar", "projeto.opencut", "*.opencut");
    if (f.isEmpty()) return;
    QFile fh(f); fh.open(QIODevice::WriteOnly); fh.write(QJsonDocument(m_proj.toJson()).toJson()); fh.close();
}
void MainWindow::openProject() {
    auto f = QFileDialog::getOpenFileName(this, "Abrir", "", "*.opencut");
    if (f.isEmpty()) return;
    QFile fh(f); fh.open(QIODevice::ReadOnly);
    m_proj = Project::fromJson(QJsonDocument::fromJson(fh.readAll()).object()); fh.close();
    m_inspector->setProject(&m_proj); m_color->setProject(&m_proj);
    m_fx->setProject(&m_proj); m_mixer->setProject(&m_proj);
    m_timeline->setProject(&m_proj); refreshAll();
}
void MainWindow::playPreview() { m_preview->playPause(); }
void MainWindow::exportVideo() {
    if (m_proj.duration() <= 0) { QMessageBox::warning(this, "Nada", "Timeline vazia."); return; }
    m_proj.width = m_deliver->outWidth(); m_proj.height = m_deliver->outHeight(); m_proj.fps = m_deliver->outFps();
    QString out = QFileDialog::getSaveFileName(this, "Exportar", "saida.mp4", "*.mp4");
    if (out.isEmpty()) return;
    auto plan = EffectChain::build(m_proj, out, m_deliver->preset());
    QStringList args = plan.inputArgs << "-filter_complex" << plan.filterComplex << plan.mapArgs;
    log("ffmpeg " + args.join(" ").mid(0, 1500));
    m_prog->show();
    m_cur = m_ff.run(args);
    connect(m_cur, &QProcess::finished, this, &MainWindow::onExportFinished);
}
void MainWindow::onExportFinished(int code, QProcess::ExitStatus) {
    m_prog->hide();
    QMessageBox::information(this, "Export", code==0 ? "Vídeo exportado!" : "Falha — veja o log.");
}
