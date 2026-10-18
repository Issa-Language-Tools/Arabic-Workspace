#pragma once

#include <QString>
class QLabel;
class QPushButton;
class QPlainTextEdit;
class QComboBox;
class QTableWidget;
class QTabWidget;

namespace Theme {
void editor(QPlainTextEdit* e, QString placeholder = "");
void label(QLabel* l);
void button(QPushButton* b);
void combo(QComboBox* c);
void table(QTableWidget* t);
void tabs(QTabWidget* t);
}  // namespace Theme
