#include "ui/GalleryPanel.h"
#include "effects/EffectsLibrary.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
GalleryPanel::GalleryPanel(QWidget *p): QWidget(p) {
    auto *l = new QVBoxLayout(this);
    l->addWidget(new QLabel("<b>✨ EFEITOS ∞ — clique p/ aplicar</b>"));
    m_search = new QLineEdit(this);
    m_search->setPlaceholderText("🔍 buscar efeito… (glow, vintage, p&b)");
    m_list = new QListWidget(this);
    l->addWidget(m_search); l->addWidget(m_list);
    setLayout(l);
    refill();
    connect(m_search, &QLineEdit::textChanged, this, [this](const QString &q){ refill(q); });
    connect(m_list, &QListWidget::itemClicked, this, [this](QListWidgetItem *it){
        emit pickEffect(it->data(Qt::UserRole).toString());
    });
}
void GalleryPanel::refill(const QString &q) {
    m_list->clear();
    for (auto &e : EffectsLibrary::all()) {
        if (!q.isEmpty() && !(e.name + e.desc + e.id).contains(q, Qt::CaseInsensitive)) continue;
        auto *it = new QListWidgetItem(e.name + " — " + e.desc);
        it->setData(Qt::UserRole, e.id);
        it->setToolTip(e.filter.isEmpty() ? e.desc : e.filter);
        m_list->addItem(it);
    }
}
