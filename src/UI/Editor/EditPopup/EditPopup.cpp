#include "EditPopup.hpp"

#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>

#include "../../FormHelpers.hpp"
#include "../../Theme/Theme.hpp"
#include "Word.hpp"

EditPopup::EditPopup(Word* word, const QStringList& wordforms, QWidget* parent) : QDialog(parent) {
  setStyleSheet("background-color: rgb(40, 40, 40);");

  QVBoxLayout* stack = new QVBoxLayout(this);
  this->w = word;

  m_inEnglish = addLabeledRow(stack, "English", QString::fromStdString(w->english));
  m_inArabic = addLabeledRow(stack, "Arabic", QString::fromStdString(w->arabic));
  m_inSource = addLabeledRow(stack, "Source", QString::fromStdString(w->source));
  m_inDialect = addLabeledRow(stack, "Dialect", QString::fromStdString(w->dialect));
  m_inWordForm = addLabeledMultiSelect(stack, "Wordform", wordforms);
  QStringList selected;
  const QStringList existing = QString::fromStdString(w->wordform).split(',', Qt::SkipEmptyParts);
  for (QString part : existing) {
    part = part.trimmed();
    if (!part.isEmpty()) {
      selected << part;
    }
  }
  m_inWordForm->setCheckedItems(selected);

  QPushButton* saveButton = new QPushButton("Save");
  Theme::button(saveButton);
  QPushButton* cancelButton = new QPushButton("Cancel");
  Theme::button(cancelButton);

  QHBoxLayout* row = new QHBoxLayout();
  connect(saveButton, &QPushButton::clicked, this, [this]() {
    w->english = m_inEnglish->toPlainText().toStdString();
    w->arabic = m_inArabic->toPlainText().toStdString();
    w->source = m_inSource->toPlainText().toStdString();
    w->dialect = m_inDialect->toPlainText().toStdString();
    w->wordform = m_inWordForm->checkedItems().join(", ").toStdString();
    accept();
  });
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  row->addWidget(saveButton);
  row->addWidget(cancelButton);
  stack->addLayout(row);
}
