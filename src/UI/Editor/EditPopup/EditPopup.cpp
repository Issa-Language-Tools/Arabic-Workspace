#include "EditPopup.hpp"

#include <qboxlayout.h>

#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>

#include "Word.hpp"

#include "../../Theme/Theme.hpp"

QPlainTextEdit* EditPopup::addRow(const QString& label, const QString& value, QVBoxLayout* stack) {
  QHBoxLayout* row = new QHBoxLayout();

  auto* labelWidget = new QLabel(label);
  Theme::label(labelWidget);
  row->addWidget(labelWidget);

  QPlainTextEdit* input = new QPlainTextEdit(value);
  Theme::editor(input);
  row->addWidget(input);
  stack->addLayout(row);
  return input;
}

EditPopup::EditPopup(Word* word, QWidget* parent) : QDialog(parent) {
  setStyleSheet("background-color: rgb(40, 40, 40);");

  QVBoxLayout* stack = new QVBoxLayout(this);
  this->w = word;

  _inEnglish = addRow("English", QString::fromStdString(w->english), stack);
  _inArabic = addRow("Arabic", QString::fromStdString(w->arabic), stack);
  _inSource = addRow("Source", QString::fromStdString(w->source), stack);
  _inDialect = addRow("Dialect", QString::fromStdString(w->dialect), stack);
  _inWordform = addRow("WordForm", QString::fromStdString(w->wordform), stack);

  QPushButton* saveButton = new QPushButton("Save");
  Theme::button(saveButton);
  QPushButton* cancelButton = new QPushButton("Cancel");
  Theme::button(cancelButton);

  QHBoxLayout* row = new QHBoxLayout();
  connect(saveButton, &QPushButton::clicked, this, [this]() {
      w->english  = _inEnglish->toPlainText().toStdString();
      w->arabic   = _inArabic->toPlainText().toStdString();
      w->source   = _inSource->toPlainText().toStdString();
      w->dialect  = _inDialect->toPlainText().toStdString();
      w->wordform = _inWordform->toPlainText().toStdString();
      accept();
      });
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  row->addWidget(saveButton);
  row->addWidget(cancelButton);
  stack->addLayout(row);
}
