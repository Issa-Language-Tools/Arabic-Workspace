#pragma once

#include <QWidget>
#include <vector>

#include "Word.hpp"

class NewEntry;
class WordList;
class EntryDefaults;

class Editor : public QWidget {
public:
  explicit Editor(const EntryDefaults* ed, QWidget* parent = nullptr);
  void addImportedWords(const std::vector<Word>& words);

private:
  NewEntry* m_newEntry;
  WordList* m_wordList;
  const EntryDefaults* m_defaults;
};
