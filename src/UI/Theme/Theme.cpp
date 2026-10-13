#include "Theme.hpp"

#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
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

}
