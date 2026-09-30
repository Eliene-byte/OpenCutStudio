#include "EffectChain.h"

QString EffectChain::eqFilter(const Clip &c) {
    QString f = QString("eq=brightness=%1:contrast=%2:saturation=%3")
        .arg(c.brightness).arg(c.contrast).arg(c.saturation);
    // Cor quente/fria aproximada via colorbalance + colortemperature
    if (qAbs(c.temperature) > 0.01)
        f += QString(",colortemperature=temperature=%1").arg(int(6500 + c.temperature*3000));
    if (qAbs(c.tint) > 0.01)
        f += QString(",colorbalance=gm=%1:bm=%2").arg(-c.tint*0.5).arg(c.tint*0.5);
    if (c.effect == "pb") f += ",hue=s=0";
    else if (c.effect == "cinematic") f += ",eq=contrast=1.15:saturation=0.85,colorbalance=rs=.15:bs=.25";
    else if (c.effect == "vintage") f += ",curves=vintage,noise=alls=8:allf=t";
    else if (c.effect == "sharpen") f += ",unsharp=5:5:0.8";
    if (c.blur > 0.01) f += QString(",gblur=sigma=%1").arg(c.blur);
    // Transform After-lite: escala + opacidade
    if (qAbs(c.scale - 1.0) > 0.01) f += QString(",scale=iw*%1:ih*%1").arg(c.scale);
    if (c.opacity < 0.99) f += QString(",format=rgba,colorchannelmixer=aa=%1").arg(c.opacity);
    if (c.vignette > 0.01) f += QString(",vignette=PI/%1").arg(5.0 - c.vignette*3.0);
    if (c.fadeIn > 0) f += QString(",fade=t=in:st=0:d=%1").arg(c.fadeIn);
    if (c.fadeOut > 0) f += QString(",fade=t=out:st=%1:d=%2").arg(qMax(0.0, c.duration - c.fadeOut)).arg(c.fadeOut);
    return f;
}

EffectChain::Plan EffectChain::build(const Project &p, const QString &outPath, const QString &preset) {
    Plan plan;
    QStringList vc; // cadeias de vídeo
    QStringList ac; // cadeias de áudio
    int idx = 0;
    int vCount = 0, aCount = 0;

    // Coleta clips de todas as tracks em ordem de tempo
    struct Item { Clip c; };
    QList<Clip> videos, audios;
    for (auto &t : p.tracks)
        for (auto &c : t.clips)
            (t.kind == "audio" || c.kind == "audio" ? audios : videos).append(c);

    auto esc = [](QString s){ s.replace("'","\\'"); s.replace(":","\\:"); return s; };

    for (auto &c : videos) {
        if (c.kind == "text" || c.kind == "color") continue; // overlay via drawtext depois
        plan.inputArgs << "-ss" << QString::number(c.inPoint)
                       << "-t" << QString::number(c.duration)
                       << "-i" << c.filePath;
        QString chain = QString("[%1:v]setpts=PTS-STARTPTS,scale=%2:%3:flags=fast_bilinear,%4,settb=AVTB,fps=%5")
            .arg(idx).arg(p.width).arg(p.height).arg(eqFilter(c)).arg(p.fps);
        // texto simples sobreposto (After-effects lite)
        if (!c.text.isEmpty())
            chain += QString(",drawtext=text='%1':fontsize=%2:fontcolor=%3:x=(w-text_w)/2:y=h-80")
                .arg(esc(c.text)).arg(c.textSize).arg(c.textColor);
        chain += QString("[v%1]").arg(vCount);
        vc << chain;
        idx++; vCount++;
    }
    for (auto &c : audios) {
        if (c.filePath.isEmpty()) continue;
        plan.inputArgs << "-ss" << QString::number(c.inPoint)
                       << "-t" << QString::number(c.duration)
                       << "-i" << c.filePath;
        ac << QString("[%1:a]aresample=44100,volume=%2[a%3]").arg(idx).arg(c.volume).arg(aCount);
        idx++; aCount++;
    }
    // Concatena
    QString filt;
    if (vCount > 1) {
        QString ins; for (int i=0;i<vCount;i++) ins += QString("[v%1]").arg(i);
        filt += ins + QString("concat=n=%1:v=1:a=0[vout];").arg(vCount);
    } else if (vCount == 1) filt += "[v0]copy[vout];";
    else filt += QString("color=c=black:s=%1x%2:r=%3:d=%4[vout];").arg(p.width).arg(p.height).arg(p.fps).arg(qMax(1.0,p.duration()));
    if (aCount > 1) {
        QString ins; for (int i=0;i<aCount;i++) ins += QString("[a%1]").arg(i);
        filt += ins + QString("amix=inputs=%1:normalize=0[aout]").arg(aCount);
    } else if (aCount == 1) filt += "[a0]copy[aout]";
    else filt += "anullsrc=r=44100:cl=stereo[aout]";

    plan.filterComplex = vc.join(";") + (vc.isEmpty()?"":";") + ac.join(";") + (ac.isEmpty()?"":";") + filt;
    plan.mapArgs << "-map" << "[vout]" << "-map" << "[aout]";

    // Presets leves para PC modesto
    QString vcodec = "libx264", extra = "";
    if (preset == "Leve 720p") extra = "-preset veryfast -crf 26";
    else if (preset == "Leve 480p") extra = "-preset veryfast -crf 28 -vf scale=854:480";
    else extra = "-preset fast -crf 23"; // Padrão 1080p
    plan.mapArgs << extra.split(" ", Qt::SkipEmptyParts)
                 << "-c:a" << "aac" << "-b:a" << "128k"
                 << "-movflags" << "+faststart" << "-y" << outPath;
    plan.nVideoInputs = vCount;
    return plan;
}
