#include "Theme.hpp"

#include <QComboBox>
#include <QLabel>
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
    "background-color: rgb(50, 50, 50);"
    "color: rgb(245, 245, 245);"
    "font-size: 20px;"
    "border: 1px solid rgb(200, 200, 200);");
  c->setFixedHeight(55);
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
