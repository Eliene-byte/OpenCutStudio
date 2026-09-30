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
