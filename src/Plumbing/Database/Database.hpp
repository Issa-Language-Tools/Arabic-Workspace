#pragma once

#include <sqlite3.h>

#include <vector>

#include "Word.hpp"

class Database {
public:
  Database();  // if no existing db exists, create a new db
  ~Database();

  void addNewWord(const Word& word);
  std::vector<Word> getAllWords(int limit = 0);

private:
  sqlite3* db = nullptr;
  sqlite3* versions = nullptr;
};
