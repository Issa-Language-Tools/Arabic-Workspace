#include "Editor.hpp"

#include <qboxlayout.h>

#include <QSettings>
#include <QString>
#include <QStringList>

#include "../../Plumbing/Database/Database.hpp"
#include "../GatewayClient/GatewayClient.hpp"
#include "EntryDefaults.hpp"
#include "NewEntry/NewEntry.hpp"
#include "WordList/WordList.hpp"

Editor::Editor(Database* db, EntryDefaults* ed, QWidget* parent)
  : QWidget(parent), m_db(db), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_gatewayClient = new GatewayClient(this);

  m_wordList = new WordList(m_defaults);
  m_wordList->setWords(m_db->getUnexportedWords());
  m_newEntry = new NewEntry(m_defaults);

  connect(m_newEntry, &NewEntry::wordSaved, this, [this](const Word& w) {
    m_db->addNewWord(w);
    m_wordList->setWords(m_db->getUnexportedWords());
  });

  connect(m_newEntry, &NewEntry::searchRequested, this, [this](const QString& query) {
    m_wordList->setWords(m_db->searchWords(query.toStdString()));
    m_gatewayClient->lookup(query.toStdString());
  });

  connect(m_newEntry, &NewEntry::searchCleared, this, [this]() {
    m_wordList->setWords(m_db->getUnexportedWords());
  });

  connect(m_wordList, &WordList::wordEdited, this, [this](const Word& w) {
    m_db->updateWord(w);
    syncTagsFromWord(w);
  });

  stack->addWidget(m_wordList);
  stack->addStretch();
  stack->addWidget(m_newEntry);
}

void Editor::addImportedWords(const std::vector<Word>& words) {
  for (const Word& w : words) {
    m_db->addNewWord(w);
  }
  m_wordList->setWords(m_db->getUnexportedWords());
}

void Editor::refreshWords() {
  m_wordList->setWords(m_db->getUnexportedWords());
}

void Editor::syncTagsFromWord(const Word& w) {
  bool changed = false;

  const QStringList wordTags = QString::fromStdString(w.tags).split(',', Qt::SkipEmptyParts);
  for (QString tag : wordTags) {
    tag = tag.trimmed();
    if (tag.isEmpty()) continue;

    if (!m_defaults->tags.contains(tag, Qt::CaseInsensitive)) {
      m_defaults->tags << tag;
      changed = true;
    }
  }

  if (changed) {
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("defaults/tags", m_defaults->tags);
  }
}
