#include "TitleBar.hpp"

TitleBar::TitleBar(const QString& text, QWidget* parent)
  : QLabel(text, parent) {
  setStyleSheet(
    "background-color: rgb(55, 55, 55);"
    "color: rgb(160, 160, 160);"
    "font-size: 12px;"
    "padding-left: 8px;");
  setFixedHeight(30);
  setAlignment(Qt::AlignCenter);
}
