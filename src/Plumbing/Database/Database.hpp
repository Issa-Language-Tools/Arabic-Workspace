#pragma once

#include <sqlite3.h>

#include <vector>

#include "Word.hpp"

class Database {
public:
  Database();  // if no existing db exists, create a new db
  ~Database();

  void addNewWord(const Word& word);
  void updateWord(const Word& word);
  void setExported(int id, bool exported);
  std::vector<Word> getAllWords(int limit = 0);
  std::vector<Word> getUnexportedWords();
  std::vector<Word> searchWords(const std::string& query, int limit = 100);

private:
  sqlite3* db = nullptr;
  sqlite3* versions = nullptr;
};
