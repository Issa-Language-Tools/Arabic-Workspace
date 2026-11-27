#pragma once

#include <QWidget>
#include <vector>

#include "Word.hpp"

class GatewayClient;
class Database;
class EntryDefaults;
class NewEntry;
class WordList;

class Editor : public QWidget {
public:
  explicit Editor(Database* db, EntryDefaults* ed, QWidget* parent = nullptr);
  void addImportedWords(const std::vector<Word>& words);
  void refreshWords();

private:
  void syncTagsFromWord(const Word& w);

private:
  Database* m_db;
  NewEntry* m_newEntry;
  WordList* m_wordList;
  EntryDefaults* m_defaults;
  GatewayClient* m_gatewayClient;
};
