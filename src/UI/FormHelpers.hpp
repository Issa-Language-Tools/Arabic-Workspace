#pragma once

#include <qboxlayout.h>

#include <QComboBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QString>
#include <QStringList>

#include "Theme/Theme.hpp"

inline QPlainTextEdit* addLabeledRow(QVBoxLayout* stack, const QString& label, const QString& value) {
  QHBoxLayout* row = new QHBoxLayout();

  QLabel* labelWidget = new QLabel(label);
  Theme::label(labelWidget);
  row->addWidget(labelWidget);

  QPlainTextEdit* input = new QPlainTextEdit(value);
  Theme::editor(input);
  row->addWidget(input);

  stack->addLayout(row);
  return input;
}

inline QComboBox* addLabeledComboBox(QVBoxLayout* stack, const QString& label, const QStringList& items) {
  QHBoxLayout* row = new QHBoxLayout();

  QLabel* labelWidget = new QLabel(label);
  Theme::label(labelWidget);
  row->addWidget(labelWidget);

  QComboBox* input = new QComboBox();
  Theme::combo(input);
  input->addItems(items);
  row->addWidget(input);

  stack->addLayout(row);
  return input;
}
