#pragma once

#include <QString>
class QLabel;
class QPushButton;
class QPlainTextEdit;
class QComboBox;
class QTableWidget;
class QTabWidget;
class QListWidget;

namespace Theme {
void editor(QPlainTextEdit* e, QString placeholder = "");
void label(QLabel* l);
void button(QPushButton* b);
void combo(QComboBox* c);
void comboButton(QPushButton* b);
void checkList(QListWidget* l);
void table(QTableWidget* t);
void tabs(QTabWidget* t);
}  // namespace Theme
