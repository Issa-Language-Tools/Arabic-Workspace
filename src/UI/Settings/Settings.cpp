#include "Settings.hpp"

#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include "../FormHelpers.hpp"
#include "../Theme/Theme.hpp"
#include "EntryDefaults.hpp"

Settings::Settings(EntryDefaults* ed, QWidget* parent) : QWidget(parent), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_inSource = addLabeledRow(stack, "Source", m_defaults->source);
  m_inDialect = addLabeledRow(stack, "Dialect", m_defaults->dialect);
  m_inWordform = addLabeledRow(stack, "Wordforms (comma separated)", m_defaults->wordforms.join(", "));

  QPushButton* saveButton = new QPushButton("Save");
  Theme::button(saveButton);

  connect(saveButton, &QPushButton::clicked, this, &Settings::save);

  stack->addWidget(saveButton);
  stack->addStretch();
}

void Settings::save() {
  m_defaults->source = m_inSource->toPlainText().trimmed();
  m_defaults->dialect = m_inDialect->toPlainText().trimmed();

  QStringList forms;
  const QStringList raw = m_inWordform->toPlainText().split(',', Qt::SkipEmptyParts);
  for (QString part : raw) {
    part = part.trimmed();
    if (!part.isEmpty()) {
      forms.append(part);
    }
  }

  m_defaults->wordforms = forms;

  QSettings settings("config.ini", QSettings::IniFormat);
  settings.setValue("defaults/source", m_defaults->source);
  settings.setValue("defaults/dialect", m_defaults->dialect);
  settings.setValue("defaults/wordforms", m_defaults->wordforms);
  emit defaultsChanged();

}
