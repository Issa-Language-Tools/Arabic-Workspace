#pragma once

#include <QString>
class QCheckBox;
class QComboBox;
class QFileDialog;
class QFrame;
class QLabel;
class QLineEdit;
class QListWidget;
class QPlainTextEdit;
class QPushButton;
class QScrollArea;
class QTableWidget;
class QTabWidget;
class QWidget;

namespace Theme {
void editor(QPlainTextEdit* e, QString placeholder = "");
void lineEdit(QLineEdit* e, QString placeholder = "");
void searchBox(QLineEdit* e, QString placeholder = "");
void label(QLabel* l);
void cardFrame(QFrame* f);
void cardWord(QLabel* l);
void cardMeaning(QLabel* l);
void cardSource(QLabel* l);
void button(QPushButton* b);
void combo(QComboBox* c);
void comboButton(QPushButton* b);
void checkbox(QCheckBox* b);
void fileDialog(QFileDialog* d);
void checkList(QListWidget* l);
void scrollArea(QScrollArea* area);
void transparent(QWidget* w);
void table(QTableWidget* t);
void tabs(QTabWidget* t);
}  // namespace Theme
