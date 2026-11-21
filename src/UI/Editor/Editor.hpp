#pragma once

#include <QWidget>
#include <vector>

#include "Word.hpp"

class Database;
class EntryDefaults;
class NewEntry;
class WordList;

class Editor : public QWidget {
public:
  explicit Editor(Database* db, const EntryDefaults* ed, QWidget* parent = nullptr);
  void addImportedWords(const std::vector<Word>& words);

private:
  Database* m_db;
  NewEntry* m_newEntry;
  WordList* m_wordList;
  const EntryDefaults* m_defaults;
};
