#include "Editor.hpp"

#include <qboxlayout.h>

#include "NewEntry/NewEntry.hpp"
#include "WordList/WordList.hpp"

Editor::Editor(const EntryDefaults* ed, QWidget* parent) : QWidget(parent), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_wordList = new WordList(m_defaults);
  m_wordList->populate();
  m_newEntry = new NewEntry(m_defaults);

  connect(m_newEntry, &NewEntry::wordSaved, m_wordList, &WordList::addWord);

  stack->addWidget(m_wordList);
  stack->addStretch();
  stack->addWidget(m_newEntry);
}
