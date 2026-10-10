#pragma once


#include "Status.hpp"
#include "Word.hpp"

#include <cstdio>
#include <sqlite3.h>
#include <sstream>
#include <vector>


class Database { 
  public:
    Database(); // if no existing db exists, create a new db
    ~Database();

    void addNewWord(const Word& word);
    std::vector<Word> getAllWords(int limit);

  private:
    sqlite3* db;
};
