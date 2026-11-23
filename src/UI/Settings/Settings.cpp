#include "Settings.hpp"

#include <QPushButton>
#include <QSettings>
#include <QShowEvent>
#include <QVBoxLayout>

#include "../FormHelpers.hpp"
#include "../Theme/Theme.hpp"
#include "EntryDefaults.hpp"

Settings::Settings(EntryDefaults* ed, QWidget* parent) : QWidget(parent), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_inSource = addLabeledRow(stack, "Source", m_defaults->source);
  m_inDialect = addLabeledRow(stack, "Dialect", m_defaults->dialect);
  m_inWordform = addLabeledRow(stack, "Wordforms (comma separated)", m_defaults->wordforms.join(", "));
  m_inTags = addLabeledRow(stack, "Tags (comma separated)", m_defaults->tags.join(", "));

  QPushButton* saveButton = new QPushButton("Save");
  Theme::button(saveButton);

  connect(saveButton, &QPushButton::clicked, this, &Settings::save);

  stack->addWidget(saveButton);
  stack->addStretch();
}

void Settings::showEvent(QShowEvent* event) {
  QWidget::showEvent(event);

  // Re-sync in case defaults changed elsewhere (e.g. tags added from the
  // edit popup).
  m_inSource->setPlainText(m_defaults->source);
  m_inDialect->setPlainText(m_defaults->dialect);
  m_inWordform->setPlainText(m_defaults->wordforms.join(", "));
  m_inTags->setPlainText(m_defaults->tags.join(", "));
}

void Settings::save() {
  m_defaults->source = m_inSource->toPlainText().trimmed();
  m_defaults->dialect = m_inDialect->toPlainText().trimmed();

  const auto splitList = [](const QString& text) {
    QStringList out;
    const QStringList raw = text.split(',', Qt::SkipEmptyParts);
    for (QString part : raw) {
      part = part.trimmed();
      if (!part.isEmpty()) out.append(part);
    }
    return out;
  };

  m_defaults->wordforms = splitList(m_inWordform->toPlainText());
  m_defaults->tags = splitList(m_inTags->toPlainText());

  QSettings settings("config.ini", QSettings::IniFormat);
  settings.setValue("defaults/source", m_defaults->source);
  settings.setValue("defaults/dialect", m_defaults->dialect);
  settings.setValue("defaults/wordforms", m_defaults->wordforms);
  settings.setValue("defaults/tags", m_defaults->tags);
  emit defaultsChanged();

}
