#include "EffectsLibrary.h"
QList<EffectDef> EffectsLibrary::all() {
    return {
        {"none","⭕ Nenhum","Remove filtro",""},
        {"pb","⬛ P&B","Preto e branco","hue=s=0"},
        {"cinematic","🎬 Cinemático","Contraste + teal","eq=contrast=1.15:saturation=0.85,colorbalance=rs=.15:bs=.25"},
        {"vintage","📼 Vintage","Curvas + grão","curves=vintage,noise=alls=8:allf=t"},
        {"sharpen","🔪 Nitidez","Clarity","unsharp=5:5:0.8"},
        {"soft","🌸 Soft","Pele suave","gblur=sigma=1.2,eq=saturation=1.1"},
        {"glow","✨ Glow","Brilho neon","gblur=sigma=3,eq=brightness=0.08:saturation=1.4"},
        {"cold","❄ Frio","Azulado","colortemperature=temperature=8500,colorbalance=bs=.3"},
        {"warm","🔥 Quente","Alambrado","colortemperature=temperature=4500,colorbalance=rs=.3"},
        {"neon","💡 Neon","Saturação extrema","eq=saturation=2.2:contrast=1.2"},
        {"noir","🕶 Noir","P&B duro","hue=s=0,eq=contrast=1.5:brightness=-0.05"},
        {"sepia","🏜 Sépia","Antigo","colorsepia"},
        {"invert","🙃 Inverter","Negativo","negate"},
        {"mirror","🪞 Espelho","Flip horizontal","hflip"},
        {"flipv","🙂 Flip V","Flip vertical","vflip"},
        {"rotate90","↻ Girar 90°","Rotação","transpose=1"},
        {"zoom15","🔍 Zoom 1.5x","Crop central","crop=iw/1.5:ih/1.5,scale=iw*1.5:ih*1.5:flags=fast_bilinear"},
        {"fisheye","🐟 Fisheye","Lente","vignette=PI/2.5,eq=saturation=1.3"},
        {"grain","🎞 Grão filme","35mm","noise=alls=12:allf=t"},
        {"denoise","🧹 Denoise","Limpa ruído","hqdn3d=4:3:6:4.5"},
        {"edge","✏ Traço","Cartoon borda","edgedetect=low=0.1:high=0.4"},
        {"pixel","🧱 Pixel","Mosaico","scale=iw/12:ih/12:flags=neighbor,scale=iw*12:ih*12:flags=neighbor"},
        {"glitch","📺 Glitch","RGB shift","noise=alls=20:allf=t,eq=saturation=1.8,hue=h=5"},
        {"shake","📳 Shake","Tremido leve","crop=iw*0.95:ih*0.95,scale=iw/0.95:ih/0.95"},
        {"slowglow","🌙 Noturno","Escuro + sat","eq=brightness=-0.15:saturation=1.3:contrast=1.2"},
        {"sunny","☀ Ensolarado","Claro quente","eq=brightness=0.12:saturation=1.3,colortemperature=temperature=5500"},
        {"fadebw","🌫 Fade P&B","Suave","hue=s=0.3,eq=brightness=0.05"},
        {"vignette","⭘ Vinheta forte","Foco central","vignette=PI/3"},
        {"clarity","💎 Clarity+","Detalhe","unsharp=7:7:1.2,eq=contrast=1.1"},
        {"custom","⚙ Custom…","Digite ffmpeg",""},
    };
}
QString EffectsLibrary::filterFor(const QString &id, const QString &custom) {
    if (id=="custom") return custom.trimmed();
    for (auto &e : all()) if (e.id==id) return e.filter;
    return "";
}
