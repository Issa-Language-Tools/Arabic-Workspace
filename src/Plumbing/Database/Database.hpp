#pragma once

#include <vector>
#include "Word.hpp"
#include <sqlite3.h>


class Database { 
  public:
    Database();
    ~Database();

    void addNewWord(const Word& word);
    std::vector<Word> getAllWords();

  private:
    sqlite3* db;
};
