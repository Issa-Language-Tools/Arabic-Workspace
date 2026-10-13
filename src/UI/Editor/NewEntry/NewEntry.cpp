#include "NewEntry.hpp"

#include <qboxlayout.h>

#include <QPlainTextEdit>
#include <QPushButton>

#include "../../Theme/Theme.hpp"

NewEntry::NewEntry(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* stack = new QVBoxLayout(this);
  QHBoxLayout* row = new QHBoxLayout();

  _arabic = new QPlainTextEdit();
  Theme::editor(_arabic, "عربي");
  _english = new QPlainTextEdit();
  Theme::editor(_english, "English");

  QPushButton* saveButton = new QPushButton("Save");
  Theme::button(saveButton);
  connect(saveButton, &QPushButton::clicked, this, &NewEntry::save);
  QPushButton* editButton = new QPushButton("Edit");
  Theme::button(editButton);

  // constructing the widget
  row->addWidget(_arabic);
  row->addWidget(_english);
  row->addWidget(saveButton);
  row->addWidget(editButton);
  stack->addLayout(row);
};

void NewEntry::save() {
  // insert new row to db
  this->clear();
}

void NewEntry::clear() {
  _arabic->setPlainText("");
  _english->setPlainText("");
  _arabic->setFocus();
}
