#pragma once

#include <QString>
class QCheckBox;
class QComboBox;
class QFileDialog;
class QLabel;
class QLineEdit;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class QTableWidget;
class QTabWidget;

namespace Theme {
void editor(QPlainTextEdit* e, QString placeholder = "");
void lineEdit(QLineEdit* e, QString placeholder = "");
void label(QLabel* l);
void button(QPushButton* b);
void combo(QComboBox* c);
void comboButton(QPushButton* b);
void checkbox(QCheckBox* b);
void fileDialog(QFileDialog* d);
void checkList(QListWidget* l);
void table(QTableWidget* t);
void tabs(QTabWidget* t);
}  // namespace Theme
