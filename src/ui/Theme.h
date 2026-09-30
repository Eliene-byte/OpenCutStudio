#pragma once
#include <QString>
// Tema dark estilo DaVinci Resolve — leve (só QSS, sem imagens)
inline QString davinciThemeQss() {
    return R"(
    QMainWindow, QWidget { background:#181818; color:#e8e8e8; font-size:13px; }
    QToolBar { background:#232323; border:none; spacing:6px; padding:4px; }
    QToolButton, QPushButton { background:#333; border:1px solid #444; padding:6px 12px; border-radius:4px; }
    QToolButton:hover, QPushButton:hover { background:#4a4a4a; border-color:#888; }
    QPushButton:pressed { background:#555; }
    QTabBar::tab { background:#232323; padding:8px 18px; border-top-left-radius:6px; border-top-right-radius:6px; }
    QTabBar::tab:selected { background:#3a3a3a; color:#fff; font-weight:bold; }
    QListWidget, QTextEdit, QTableWidget { background:#1f1f1f; border:1px solid #333; border-radius:4px; }
    QSlider::groove:horizontal { height:4px; background:#444; }
    QSlider::handle:horizontal { width:14px; background:#aaa; border-radius:7px; margin:-5px 0; }
    QDockWidget::title { background:#232323; padding:4px; }
    QProgressBar { background:#222; border:1px solid #444; text-align:center; }
    QProgressBar::chunk { background:#4caf50; }
    QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit { background:#2a2a2a; border:1px solid #444; padding:4px; border-radius:4px; }
    QSplitter::handle { background:#333; }
    )";
}
