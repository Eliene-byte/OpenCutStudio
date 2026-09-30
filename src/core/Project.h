#pragma once
#include <QString>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>

// Clip = um arquivo na timeline (Premiere-style)
// Suporta video, imagem, audio, texto
struct Clip {
    QString id;
    QString filePath;      // caminho do arquivo (vazio = texto/cor sólida)
    QString kind;          // "video" | "image" | "audio" | "text" | "color"
    double startOnTrack = 0.0;   // posição na track (segundos)
    double inPoint = 0.0;        // onde começa dentro do arquivo
    double duration = 5.0;       // duração na timeline
    double speed = 1.0;
    // Photoshop-lite / DaVinci-lite
    double brightness = 0.0; // -1..1
    double contrast = 1.0;   // 0..3
    double saturation = 1.0; // 0..3
    double blur = 0.0;       // 0..10
    // DaVinci Color: wheels lite
    double temperature = 0.0; // -1..1 (quente/frio)
    double tint = 0.0;        // -1..1 (verde/magenta)
    double vignette = 0.0;    // 0..1
    double opacity = 1.0;     // 0..1 (After)
    double scale = 1.0;       // 0.25..4 (After transform)
    double posX = 0.0, posY = 0.0; // -500..500
    double rotation = 0.0;    // -180..180
    double fadeIn = 0.0, fadeOut = 0.0; // segundos (Premiere transition)
    QString effect;           // id da EffectsLibrary + legacy pb/cinematic/...
    QString customFilter;     // ffmpeg livre ("infinito")
    // Photoshop completo
    double cropPct = 0.0;     // 0..40 (% crop central)
    bool flipH = false, flipV = false;
    double hue = 0.0;         // -180..180
    double highlights = 0.0;  // -1..1
    double shadows = 0.0;     // -1..1
    double clarity = 0.0;     // 0..2
    double grain = 0.0;       // 0..30
    QString text;            // se kind==text
    int textSize = 48;
    QString textColor = "white";
    double volume = 1.0;     // 0..2
    QString transition;      // "" | "fade" | "xfade"

    QJsonObject toJson() const;
    static Clip fromJson(const QJsonObject &o);
};

struct Track {
    QString kind; // "video" | "audio"
    QList<Clip> clips;
};

class Project {
public:
    Project();
    QList<Track> tracks;
    int width = 1280;
    int height = 720;
    int fps = 30;
    QString proxyMode = "quarter"; // proxy para notebooks modestos
    double duration() const;
    void addClip(int trackIndex, const Clip &c);
    void clear();
    QJsonObject toJson() const;
    static Project fromJson(const QJsonObject &o);
    static Project demo();
};
