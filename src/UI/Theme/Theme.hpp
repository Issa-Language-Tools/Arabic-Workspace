#pragma once

#include <QString>
class QLabel;
class QPushButton;
class QPlainTextEdit;
class QTableWidget;

namespace Theme {
void editor(QPlainTextEdit* e, QString placeholder = "");
void label(QLabel* l);
void button(QPushButton* b);
void table(QTableWidget* t);
}  // namespace Theme
