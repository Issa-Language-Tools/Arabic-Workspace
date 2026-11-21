#include "NewEntry.hpp"

#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>

#include "../../Theme/Theme.hpp"
#include "EntryDefaults.hpp"
#include "NewEntryFilter.hpp"
#include "Word.hpp"

NewEntry::NewEntry(const EntryDefaults* ed, QWidget* parent) : QWidget(parent), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);
  QHBoxLayout* row = new QHBoxLayout();

  m_arabic = new QPlainTextEdit();
  Theme::editor(m_arabic, "عربي");
  m_english = new QPlainTextEdit();
  Theme::editor(m_english, "English");

  m_filter = new NewEntryFilter(m_arabic, m_english, this);
  m_arabic->installEventFilter(m_filter);
  m_english->installEventFilter(m_filter);

  connect(m_filter, &NewEntryFilter::searchRequested, this, &NewEntry::searchRequested);
  connect(m_filter, &NewEntryFilter::searchCleared, this, &NewEntry::searchCleared);
  connect(m_filter, &NewEntryFilter::saveRequested, this, &NewEntry::save);

  QPushButton* saveButton = new QPushButton("Save");
  Theme::button(saveButton);
  connect(saveButton, &QPushButton::clicked, this, &NewEntry::save);
  QPushButton* editButton = new QPushButton("Edit");
  Theme::button(editButton);

  // constructing the widget
  row->addWidget(m_arabic);
  row->addWidget(m_english);
  row->addWidget(saveButton);
  row->addWidget(editButton);
  stack->addLayout(row);
};

void NewEntry::save() {
  Word w;
  w.arabic = m_arabic->toPlainText().toStdString();
  w.english = m_english->toPlainText().toStdString();
  w.source = m_defaults->source.toStdString();
  w.dialect = m_defaults->dialect.toStdString();

  emit wordSaved(w);
  this->clear();
}

void NewEntry::clear() {
  m_arabic->setPlainText("");
  m_english->setPlainText("");
  m_arabic->setFocus();
}
