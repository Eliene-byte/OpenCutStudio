#include "Project.h"
#include <QUuid>

QJsonObject Clip::toJson() const {
    QJsonObject o;
    o["id"]=id; o["filePath"]=filePath; o["kind"]=kind;
    o["startOnTrack"]=startOnTrack; o["inPoint"]=inPoint;
    o["duration"]=duration; o["speed"]=speed;
    o["brightness"]=brightness; o["contrast"]=contrast;
    o["saturation"]=saturation; o["blur"]=blur;
    o["text"]=text; o["textSize"]=textSize; o["textColor"]=textColor;
    o["volume"]=volume; o["transition"]=transition;
    o["temperature"]=temperature; o["tint"]=tint; o["vignette"]=vignette;
    o["opacity"]=opacity; o["scale"]=scale; o["posX"]=posX; o["posY"]=posY;
    o["rotation"]=rotation; o["fadeIn"]=fadeIn; o["fadeOut"]=fadeOut; o["effect"]=effect;
    return o;
}
Clip Clip::fromJson(const QJsonObject &o) {
    Clip c;
    c.id=o["id"].toString(QUuid::createUuid().toString());
    c.filePath=o["filePath"].toString(); c.kind=o["kind"].toString("video");
    c.startOnTrack=o["startOnTrack"].toDouble();
    c.inPoint=o["inPoint"].toDouble(); c.duration=o["duration"].toDouble(5.0);
    c.speed=o["speed"].toDouble(1.0);
    c.brightness=o["brightness"].toDouble(); c.contrast=o["contrast"].toDouble(1.0);
    c.saturation=o["saturation"].toDouble(1.0); c.blur=o["blur"].toDouble();
    c.text=o["text"].toString(); c.textSize=o["textSize"].toInt(48);
    c.textColor=o["textColor"].toString("white");
    c.volume=o["volume"].toDouble(1.0); c.transition=o["transition"].toString();
    c.temperature=o["temperature"].toDouble(); c.tint=o["tint"].toDouble();
    c.vignette=o["vignette"].toDouble(); c.opacity=o["opacity"].toDouble(1.0);
    c.scale=o["scale"].toDouble(1.0); c.posX=o["posX"].toDouble(); c.posY=o["posY"].toDouble();
    c.rotation=o["rotation"].toDouble(); c.fadeIn=o["fadeIn"].toDouble(); c.fadeOut=o["fadeOut"].toDouble();
    c.effect=o["effect"].toString();
    return c;
}

Project::Project() {
    tracks = { Track{"video", {}}, Track{"video", {}}, Track{"audio", {}} };
}
double Project::duration() const {
    double d=0;
    for(auto &t: tracks) for(auto &c: t.clips)
        d = qMax(d, c.startOnTrack + c.duration);
    return d;
}
void Project::addClip(int ti, const Clip &c){ if(ti>=0&&ti<tracks.size()) tracks[ti].clips.append(c); }
void Project::clear(){ for(auto &t: tracks) t.clips.clear(); }
QJsonObject Project::toJson() const {
    QJsonObject o; o["width"]=width; o["height"]=height; o["fps"]=fps; o["proxyMode"]=proxyMode;
    QJsonArray ta;
    for(auto &t: tracks){ QJsonObject to; to["kind"]=t.kind; QJsonArray ca;
        for(auto &c: t.clips) ca.append(c.toJson()); to["clips"]=ca; ta.append(to); }
    o["tracks"]=ta; return o;
}
Project Project::fromJson(const QJsonObject &o) {
    Project p;
    p.width=o["width"].toInt(1280); p.height=o["height"].toInt(720);
    p.fps=o["fps"].toInt(30); p.proxyMode=o["proxyMode"].toString("quarter");
    p.tracks.clear();
    for(auto tv: o["tracks"].toArray()){ Track t; t.kind=tv.toObject()["kind"].toString("video");
        for(auto cv: tv.toObject()["clips"].toArray()) t.clips.append(Clip::fromJson(cv.toObject()));
        p.tracks.append(t); }
    return p;
}
Project Project::demo() { return Project(); }
