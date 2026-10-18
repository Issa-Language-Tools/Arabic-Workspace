#include "Database.hpp"

Status initialize_versions_db(){
  sqlite3* versions;

  if (sqlite3_open("versions.db", &versions) != SQLITE_OK) {
    fprintf(stderr, "Can't open db: %s\n", sqlite3_errmsg(versions));
    sqlite3_close_v2(versions);
    versions = nullptr;
    return Status::ERR_IO;
  }

 const char* sql =
    "CREATE TABLE IF NOT EXISTS versions ("
    "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
    "  version   TEXT NOT NULL,"
    "  created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
    ");";

 char* err = nullptr;
  if (sqlite3_exec(versions, sql, nullptr, nullptr, &err) != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err);
    sqlite3_free(err);
    return Status::ERR_SQL;
  } 

  return Status::OK;
}

const char* get_latest_migration(sqlite3* versions) {
  const char* sql = "SELECT * FROM versions ORDER BY version DESC LIMIT 1";
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(versions, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare query: %s\n", sqlite3_errmsg(versions)); 
    return "";
  }

  if (sqlite3_step(stmt) != SQLITE_ROW) {
    fprintf(stderr, "No row found in versions table\n"); 
    return "";
  } else {
    return (const char*)sqlite3_column_text(stmt, 1);
  }
}

Status apply_migration_versions(sqlite3* versions) {
  const char* latest = get_latest_migration(versions);
  
  // TODO: go through all migration files and apply in order 

  return Status::OK;
}

Status check_versions_db() {
  Status res;
  if ((res = initialize_versions_db()) != Status::OK) {
    return res;
  }



  
  return Status::OK;
}

Database::Database() {
  
  if (sqlite3_open("words.db", &db) != SQLITE_OK) {
    fprintf(stderr, "Can't open db: %s\n", sqlite3_errmsg(db));
    sqlite3_close_v2(db);
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
  sqlite3_bind_int(stmt, 6, word.exported ? 1 : 0);

  // run the insert
  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "insert failed: %s\n", sqlite3_errmsg(db));
  }

  sqlite3_finalize(stmt);
}

// SQLITE returns NULL as nullptr; std::string(nullptr) is UB; map NULL -> ""
static std::string col_text(sqlite3_stmt* stmt, int col) {
    const unsigned char* t = sqlite3_column_text(stmt, col);
    return t ? (const char*)t : "";
}

std::vector<Word> Database::getAllWords(int limit) {
  std::vector<Word> result;
  if (!db) return result;

  std::stringstream final_sql;

  const char* sql =
    "SELECT id, english, arabic, source, dialect, wordform, exported "
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
    w.exported = sqlite3_column_int(stmt, 6) != 0;
    result.push_back(w);
  }

  sqlite3_finalize(stmt);
  return result;
}
