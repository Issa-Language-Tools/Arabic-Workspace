#include "Database.hpp"

#include <cstdio>
#include <sstream>

#include "Helper.hpp"
#include "Status.hpp"

Database::Database() {
  if (sqlite3_open("words.db", &db) != SQLITE_OK) {
    fprintf(stderr, "Can't open db: %s\n", sqlite3_errmsg(db));
    sqlite3_close_v2(db);
    db = nullptr;
    return;
  }

  Status version_check;
  if ((version_check = check_versions_db(&versions)) != Status::OK) {
    return;
  }

  // apply migrations
  apply_migration_versions(versions, db);
}

Database::~Database() {
  if (db) sqlite3_close(db);
  if (versions) sqlite3_close(versions);
}

void Database::addNewWord(const Word& word) {
  if (!db) return;
  const char* sql =
    "INSERT INTO words (english, arabic, source, dialect, wordform, category, exported) "
    "VALUES (?, ?, ?, ?, ?, ?, ?);";

  // turn sql statement into bytecode program
  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(db));
    return;
  }

  // fill in each of the parameters (1-indexed)
  sqlite3_bind_text(stmt, 1, word.english.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, word.arabic.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, word.source.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, word.dialect.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, word.wordform.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 6, word.category.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 7, word.exported ? 1 : 0);

  // run the insert
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "insert failed: %s\n", sqlite3_errmsg(db));
  }

  sqlite3_finalize(stmt);
}

std::vector<Word> Database::getAllWords(int limit) {
  std::vector<Word> result;
  if (!db) return result;

  std::stringstream final_sql;

  const char* sql =
    "SELECT id, english, arabic, source, dialect, wordform, category, exported "
    "FROM words ORDER BY id DESC";

  if (limit > 0) {
    final_sql << sql << " LIMIT " << limit << ";";
  } else {
    final_sql << sql << ";";
  }

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, final_sql.str().c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(db));
    return result;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Word w;
    w.id = sqlite3_column_int(stmt, 0);
    w.english = col_text(stmt, 1);
    w.arabic = col_text(stmt, 2);
    w.source = col_text(stmt, 3);
    w.dialect = col_text(stmt, 4);
    w.wordform = col_text(stmt, 5);
    w.category = col_text(stmt, 6);
    w.exported = sqlite3_column_int(stmt, 7) != 0;
    result.push_back(w);
  }

  sqlite3_finalize(stmt);
  return result;
}
