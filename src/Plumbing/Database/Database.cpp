#include "Database.hpp"
#include <sqlite3.h>
#include <cstdio>

Database::Database() {
  if (sqlite3_open("words.db", &db) != SQLITE_OK) {
    fprintf(stderr, "Can't open db: %s\n", sqlite3_errmsg(db));
    db = nullptr;
    return;
  }

  const char* sql =
    "CREATE TABLE IF NOT EXISTS words ("
    "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
    "  english    TEXT NOT NULL,"
    "  arabic     TEXT NOT NULL,"
    "  source     TEXT DEFAULT '',"
    "  dialect    TEXT DEFAULT '',"
    "  wordform   TEXT DEFAULT '',"
    "  exported   INTEGER DEFAULT 0,"
    "  created_at TEXT DEFAULT (datetime('now'))"
    ");";

  char* err = nullptr;
  if (sqlite3_exec(db, sql, nullptr, nullptr, &err) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err);
    sqlite3_free(err);
  }
}

Database::~Database() {
  if (db) sqlite3_close(db);
}

void Database::addNewWord(const Word& word) {
  if (!db) return;
  const char* sql =
    "INSERT INTO words (english, arabic, source, dialect, wordform, exported) "
    "VALUES (?, ?, ?, ?, ?, ?);";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(db));
    return;
  }

  sqlite3_bind_text(stmt, 1, word.english.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, word.arabic.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, word.source.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, word.dialect.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, word.wordform.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 6, word.exported ? 1 : 0);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "insert failed: %s\n", sqlite3_errmsg(db));
  }

  sqlite3_finalize(stmt);
}

std::vector<Word> Database::getAllWords() {
  std::vector<Word> result;
  if (!db) return result;

  const char* sql =
    "SELECT id, english, arabic, source, dialect, wordform, exported "
    "FROM words ORDER BY created_at ASC;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(db));
    return result;
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Word w;
    w.id = sqlite3_column_int(stmt, 0);
    w.english = (const char*)sqlite3_column_text(stmt, 1);
    w.arabic = (const char*)sqlite3_column_text(stmt, 2);
    w.source = (const char*)sqlite3_column_text(stmt, 3);
    w.dialect = (const char*)sqlite3_column_text(stmt, 4);
    w.wordform = (const char*)sqlite3_column_text(stmt, 5);
    w.exported = sqlite3_column_int(stmt, 6) != 0;
    result.push_back(w);
  }

  sqlite3_finalize(stmt);
  return result;
}
