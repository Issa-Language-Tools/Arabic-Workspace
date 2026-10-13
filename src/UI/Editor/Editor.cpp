#include "Editor.hpp"

#include <qboxlayout.h>

#include "NewEntry/NewEntry.hpp"
#include "WordList/WordList.hpp"

Editor::Editor(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_wordList = new WordList();
  m_wordList->populate();
  m_newEntry = new NewEntry();

  stack->addWidget(m_wordList, 1);
  stack->addWidget(m_newEntry);
}
