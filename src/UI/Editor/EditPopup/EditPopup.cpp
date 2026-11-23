#include "EditPopup.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>

#include "../../FormHelpers.hpp"
#include "../../Theme/Theme.hpp"
#include "Word.hpp"

EditPopup::EditPopup(Word* word, const QStringList& wordforms, const QStringList& tags, QWidget* parent) : QDialog(parent) {
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

  // Tags: multi-select from known tags, plus an inline "new tag" input.
  QHBoxLayout* tagsRow = new QHBoxLayout();
  QLabel* tagsLabel = new QLabel("Tags");
  Theme::label(tagsLabel);

  QStringList existingTags;
  const QStringList rawTags = QString::fromStdString(w->tags).split(',', Qt::SkipEmptyParts);
  for (QString part : rawTags) {
    part = part.trimmed();
    if (!part.isEmpty()) {
      existingTags << part;
    }
  }

  // Base the list on the configured tags, but keep any existing tags on the
  // word so editing never silently drops them.
  QStringList tagItems = tags;
  for (const QString& tag : existingTags) {
    if (!tagItems.contains(tag, Qt::CaseInsensitive)) {
      tagItems << tag;
    }
  }

  m_inTags = new MultiSelectComboBox();
  m_inTags->setItems(tagItems);
  m_inTags->setCheckedItems(existingTags);

  m_newTagInput = new QLineEdit();
  Theme::lineEdit(m_newTagInput, "New tag");

  QPushButton* addTagButton = new QPushButton("Add");
  Theme::button(addTagButton);
  connect(addTagButton, &QPushButton::clicked, this, [this]() {
    const QString tag = m_newTagInput->text().trimmed();
    if (!tag.isEmpty()) {
      m_inTags->addCheckedItem(tag);
      m_newTagInput->clear();
    }
  });

  tagsRow->addWidget(tagsLabel);
  tagsRow->addWidget(m_inTags, 1);
  tagsRow->addWidget(m_newTagInput, 1);
  tagsRow->addWidget(addTagButton);
  stack->addLayout(tagsRow);

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
    w->tags = m_inTags->checkedItems().join(", ").toStdString();
    accept();
  });
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  row->addWidget(saveButton);
  row->addWidget(cancelButton);
  stack->addLayout(row);
}
