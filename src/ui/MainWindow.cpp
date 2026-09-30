#include "ui/MainWindow.h"
#include "ui/TimelineWidget.h"
#include "ui/PreviewWidget.h"
#include "ui/InspectorWidget.h"
#include "ui/MediaBinWidget.h"
#include "effects/EffectChain.h"
#include <QSplitter> <QMenuBar> <QToolBar> <QFileDialog> <QTextEdit> <QComboBox>
#include <QProgressBar> <QDockWidget> <QMessageBox> <QJsonDocument> <QFile>
#include <QInputDialog> <QUuid> <QLabel>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    setWindowTitle("OpenCut Studio — editor leve open-source");
    resize(1280, 800);

    auto *tb = addToolBar("Main");
    tb->addAction("Importar", this, &MainWindow::importMedia);
    tb->addAction("+ Texto", this, &MainWindow::addTextClip);
    tb->addAction("Salvar", this, &MainWindow::saveProject);
    tb->addAction("Abrir", this, &MainWindow::openProject);
    tb->addAction("▶ Preview", this, &MainWindow::playPreview);
    m_preset = new QComboBox(this);
    m_preset->addItems({"Padrão 720p","Leve 720p","Leve 480p"});
    tb->addWidget(new QLabel("  Preset: ")); tb->addWidget(m_preset);
    tb->addAction("Exportar", this, &MainWindow::exportVideo);

    m_bin = new MediaBinWidget(this);
    m_preview = new PreviewWidget(this);
    m_inspector = new InspectorWidget(this);
    m_inspector->setProject(&m_proj);
    m_timeline = new TimelineWidget(this);
    m_timeline->setProject(&m_proj);

    auto *top = new QSplitter(Qt::Horizontal, this);
    top->addWidget(m_bin); top->addWidget(m_preview); top->addWidget(m_inspector);
    top->setSizes({220, 700, 260});

    m_log = new QTextEdit(this); m_log->setReadOnly(true); m_log->setMaximumHeight(120);
    m_prog = new QProgressBar(this); m_prog->setRange(0,0); m_prog->hide();

    auto *main = new QSplitter(Qt::Vertical, this);
    main->addWidget(top); main->addWidget(m_timeline); main->addWidget(m_log); main->addWidget(m_prog);
    main->setSizes({450, 200, 80, 20});
    setCentralWidget(main);

    connect(m_timeline, &TimelineWidget::clipSelected, m_inspector, &InspectorWidget::edit);
    connect(m_inspector, &InspectorWidget::changed, this, &MainWindow::refreshAll);
    connect(&m_ff, &FFmpegRunner::logLine, this, &MainWindow::log);
    log("OpenCut pronto. Importe vídeos/imagens e exporte. FFmpeg: " + FFmpegRunner::ffmpegPath());
}
void MainWindow::log(const QString &s){ m_log->append(s.mid(0, 2000)); }
void MainWindow::refreshAll(){ m_timeline->update(); }
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
        if (!isAudio) m_preview->load(f);
        cursor += c.duration;
    }
    refreshAll(); log(QString("Importado %1 arquivo(s).").arg(fs.size()));
}
void MainWindow::addTextClip() {
    QString t = QInputDialog::getText(this, "Texto", "Texto do título:");
    if (t.isEmpty()) return;
    Clip c; c.id = QUuid::createUuid().toString(); c.kind="video"; c.text=t; c.filePath="";
    // título = color preta + drawtext: criamos via filter? simplificação: usa primeiro vídeo ou fundo
    c.duration=3.0; c.startOnTrack=m_proj.duration();
    m_proj.addClip(1, c); refreshAll();
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
    m_inspector->setProject(&m_proj); m_timeline->setProject(&m_proj); refreshAll();
}
void MainWindow::playPreview() { m_preview->playPause(); }
void MainWindow::exportVideo() {
    if (m_proj.duration() <= 0) { QMessageBox::warning(this, "Nada", "Timeline vazia."); return; }
    QString out = QFileDialog::getSaveFileName(this, "Exportar", "saida.mp4", "*.mp4");
    if (out.isEmpty()) return;
    auto plan = EffectChain::build(m_proj, out, m_preset->currentText());
    QStringList args = plan.inputArgs
        << "-filter_complex" << plan.filterComplex
        << plan.mapArgs;
    log("ffmpeg " + args.join(" ").mid(0, 1500));
    m_prog->show();
    m_cur = m_ff.run(args);
    connect(m_cur, &QProcess::finished, this, &MainWindow::onExportFinished);
}
void MainWindow::onExportFinished(int code, QProcess::ExitStatus) {
    m_prog->hide();
    QMessageBox::information(this, "Export", code==0 ? "Vídeo exportado!" : "Falha — veja o log.");
}
