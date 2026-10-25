#include "Helper.hpp"

#include <sqlite3.h>

#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

#define MIGRATIONS_PATH "/home/issaaboudi/Dev/language-workspace/migrations/";

Status check_versions_db(sqlite3** versions) {
  Status res;
  if ((res = initialize_versions_db(versions)) != Status::OK) {
    return res;
  }

  return Status::OK;
}

Status initialize_versions_db(sqlite3** out) {
  sqlite3* versions = nullptr;
  if (sqlite3_open("versions.db", &versions) != SQLITE_OK) {
    fprintf(stderr, "Can't open db: %s\r\n", sqlite3_errmsg(versions));
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
    fprintf(stderr, "SQL error: %s\r\n", err);
    sqlite3_free(err);
    return Status::ERR_SQL;
  }
  *out = versions;
  return Status::OK;
}

std::string get_latest_migration(sqlite3* versions) {
  const char* sql = "SELECT version FROM versions ORDER BY version DESC LIMIT 1";
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(versions, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare query: %s\r\n", sqlite3_errmsg(versions));
    return "";
  }

  std::string result;
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    const unsigned char* t = sqlite3_column_text(stmt, 0);
    result = t ? (const char*)t : "";
  }

  sqlite3_finalize(stmt);
  return result;
}

Status apply_migration_versions(sqlite3* versions, sqlite3* db) {
  std::string latest = get_latest_migration(versions);

  fs::path path = MIGRATIONS_PATH;

  std::vector<fs::path> files;
  for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
    if (entry.is_regular_file() && entry.path().extension() == ".sql") {
      files.push_back(entry.path());
    }
  }

  std::sort(files.begin(), files.end());

  // figure out what migration we're on:
  size_t start = 0;
  if (!latest.empty()) {
    bool found = false;
    for (size_t i = 0; i < files.size(); i++) {
      if (files[i].filename().string() == latest) {
        start = i + 1;
        found = true;
        break;
      }
    }

    if (!found) {
      fprintf(stderr, "last saved migration: %s was not located\n", latest.c_str());
      return Status::ERR_IO;
    }
  }

  if (start < files.size()) {
    printf("applying migrations:\r\n");
  }

  for (size_t i = start; i < files.size(); i++) {
    fs::path& p = files[i];
    std::string version = p.filename().string();

    if (apply_migration_file(db, p) != Status::OK) {
      return Status::ERR_SQL;
    }

    // advance versions migration ledger
    const char* sql = "INSERT INTO versions (version) VALUES (?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(versions, sql, -1, &stmt, nullptr) != SQLITE_OK) {
      fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(versions));
      return Status::ERR_SQL;
    }

    sqlite3_bind_text(stmt, 1, version.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
      fprintf(stderr, "insert failed: %s\n", sqlite3_errmsg(versions));
      return Status::ERR_SQL;
    }

    sqlite3_finalize(stmt);
  }

  return Status::OK;
}

Status apply_migration_file(sqlite3* db, const fs::path& path) {
  // open a file
  std::ifstream in(path, std::ios::binary);
  std::ostringstream ss;

  // check that its valid
  if (!in) {
    fprintf(stderr, "cannot open %s\r\n", path.string().c_str());
    return Status::ERR_IO;
  }

  // read all its contents
  ss << in.rdbuf();
  // make a string with it's content
  std::string sql = ss.str();

  // apply that sql code to the db via sqlite3_exec
  char* err = nullptr;
  // begin trx
  if (sqlite3_exec(db, "BEGIN;", nullptr, nullptr, &err) != SQLITE_OK) {
    fprintf(stderr, "BEGIN failed: %s\r\n", err);
    sqlite3_free(err);
    return Status::ERR_SQL;
  }

  // apply migration
  if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    fprintf(stderr, "SQL ERROR in %s: %s\r\n", path.string().c_str(), err);
    sqlite3_free(err);
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return Status::ERR_SQL;
  }

  // commit trx
  if (sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &err) != SQLITE_OK) {
    fprintf(stderr, "COMMIT failed: %s\r\n", err);
    sqlite3_free(err);
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    return Status::ERR_SQL;
  }

  return Status::OK;
}

// SQLITE returns NULL as nullptr; std::string(nullptr) is UB; map NULL -> ""
std::string col_text(sqlite3_stmt* stmt, int col) {
  const unsigned char* t = sqlite3_column_text(stmt, col);
  return t ? (const char*)t : "";
}
