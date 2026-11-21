#include "Editor.hpp"

#include <qboxlayout.h>

#include <QString>

#include "../../Plumbing/Database/Database.hpp"
#include "NewEntry/NewEntry.hpp"
#include "WordList/WordList.hpp"

Editor::Editor(Database* db, const EntryDefaults* ed, QWidget* parent)
  : QWidget(parent), m_db(db), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_wordList = new WordList(m_defaults);
  m_wordList->setWords(m_db->getUnexportedWords());
  m_newEntry = new NewEntry(m_defaults);

  connect(m_newEntry, &NewEntry::wordSaved, this, [this](const Word& w) {
    m_db->addNewWord(w);
    m_wordList->setWords(m_db->getUnexportedWords());
  });

  connect(m_newEntry, &NewEntry::searchRequested, this, [this](const QString& query) {
    m_wordList->setWords(m_db->searchWords(query.toStdString()));
  });

  connect(m_newEntry, &NewEntry::searchCleared, this, [this]() {
    m_wordList->setWords(m_db->getUnexportedWords());
  });

  connect(m_wordList, &WordList::wordEdited, this, [this](const Word& w) {
    m_db->updateWord(w);
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
