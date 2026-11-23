#include "Theme.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>

namespace Theme {

void editor(QPlainTextEdit* e, QString placeholder) {
  e->setStyleSheet(
    "background-color: rgb(50, 50, 50);"
    "color: rgb(245, 245, 245);"
    "font-size: 20px;"
    "border: 1px solid rgb(200, 200, 200);");
  e->setFixedHeight(55);
  e->setPlaceholderText(placeholder);
  e->setTabChangesFocus(true);
}

void lineEdit(QLineEdit* e, QString placeholder) {
  e->setStyleSheet(
    "background-color: rgb(50, 50, 50);"
    "color: rgb(245, 245, 245);"
    "font-size: 20px;"
    "border: 1px solid rgb(200, 200, 200);");
  e->setFixedHeight(55);
  e->setPlaceholderText(placeholder);
}

void searchBox(QLineEdit* e, QString placeholder) {
  e->setStyleSheet(
    "background-color: rgb(50, 50, 50);"
    "color: rgb(245, 245, 245);"
    "font-size: 16px;"
    "border: 1px solid rgb(200, 200, 200);");
  e->setFixedHeight(32);
  e->setPlaceholderText(placeholder);
}

void label(QLabel* l) {
  l->setStyleSheet(
    "color: rgb(245, 245, 245);"
    "font-weight: bold;"
    "font-size: 20px;");
}

void button(QPushButton* b) {
  b->setStyleSheet(
    "background-color: rgb(55, 55, 55);"
    "color: rgb(245, 245, 245);"
    "font-size: 20px;"
    "font-weight: bold;");
  b->setMaximumWidth(80);
}

void combo(QComboBox* c) {
  c->setStyleSheet(
    "QComboBox {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "  border: 1px solid rgb(200, 200, 200);"
    "}"
    "QComboBox QAbstractItemView {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "  selection-background-color: rgb(70, 70, 70);"
    "  border: 1px solid rgb(200, 200, 200);"
    "}");
  c->setFixedHeight(55);
}

void checkbox(QCheckBox* b) {
  b->setStyleSheet(
    "QCheckBox {"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "}");
}

void fileDialog(QFileDialog* d) {
  d->setStyleSheet(
    "QFileDialog { background-color: rgb(40, 40, 40); }"
    "QLabel { color: rgb(245, 245, 245); font-size: 20px; }"
    "QLineEdit {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "  border: 1px solid rgb(200, 200, 200);"
    "}"
    "QPushButton {"
    "  background-color: rgb(55, 55, 55);"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "  font-weight: bold;"
    "}"
    "QToolButton {"
    "  background-color: rgb(55, 55, 55);"
    "  color: rgb(245, 245, 245);"
    "}"
    "QListView, QTreeView, QTableView {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "}"
    "QHeaderView::section {"
    "  background-color: rgb(55, 55, 55);"
    "  color: rgb(245, 245, 245);"
    "  border: none;"
    "}"
    "QComboBox {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "}");
}

void comboButton(QPushButton* b) {
  b->setStyleSheet(
    "QPushButton {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "  border: 1px solid rgb(200, 200, 200);"
    "  text-align: left;"
    "  padding-left: 8px;"
    "}"
    "QPushButton:hover {"
    "  background-color: rgb(60, 60, 60);"
    "}");
  b->setFixedHeight(55);
}

void checkList(QListWidget* l) {
  l->setStyleSheet(
    "QListWidget {"
    "  background-color: rgb(50, 50, 50);"
    "  color: rgb(245, 245, 245);"
    "  font-size: 20px;"
    "  border: 1px solid rgb(200, 200, 200);"
    "}"
    "QListWidget::item {"
    "  padding: 4px;"
    "}"
    "QListWidget::item:hover {"
    "  background-color: rgb(70, 70, 70);"
    "}");
}

void table(QTableWidget* t) {
  t->setStyleSheet(
    "QTableWidget {"
    "  background-color: rgb(40, 40, 40);"
    "  color: rgb(245, 245, 245);"
    "  gridline-color: rgb(55, 55, 55);"
    "  border: none;"
    "}"
    "QTableWidget::item {"
    "  padding: 4px;"
    "}"
    "QHeaderView::section {"
    "  background-color: rgb(55, 55, 55);"
    "  color: rgb(245, 245, 245);"
    "  font-weight: bold;"
    "  padding: 4px;"
    "  border: none;"
    "}"
    "QTableWidget QTableCornerButton::section {"
    "  background-color: rgb(55, 55, 55);"
    "  border: none;"
    "}");
}

void tabs(QTabWidget* t) {
  t->setStyleSheet(
    "QTabWidget::pane {"
    "  border: none;"
    "  background-color: rgb(40, 40, 40);"
    "}"
    "QTabBar::tab {"
    "  background-color: rgb(55, 55, 55);"
    "  color: rgb(245, 245, 245);"
    "  padding: 6px 12px;"
    "  border: none;"
    "  border-radius: 6px;"
    "}"
    "QTabBar::tab:selected {"
    "  background-color: rgb(40, 40, 40);"
    "}"
    "QTabBar::tab:hover:!selected {"
    "  background-color: rgb(70, 70, 70);"
    "}");
}

}  // namespace Theme
