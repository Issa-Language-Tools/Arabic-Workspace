#include "Database.hpp"

#include <cstdio>
#include <sstream>

#include "Helper.hpp"
#include "Status.hpp"

namespace {

// Remove Arabic diacritics (harakaat) from a UTF-8 string so search can
// match "كلمة" against "كَلِمَة".
std::string stripHarakaat(const std::string& in) {
  std::string out;
  out.reserve(in.size());

  for (size_t i = 0; i < in.size();) {
    const unsigned char cha = in[i];

    if (cha < 0x80) {
      out += (char)cha;
      ++i;
    } else if ((cha & 0xE0) == 0xC0 && i + 1 < in.size()) {
      // Two-byte UTF-8 sequence (covers Arabic U+0600..U+06FF).
      const unsigned char cha2 = in[i + 1];
      const bool haraka =
        (cha == 0xD9 && cha2 >= 0x8B && cha2 <= 0x9F) ||  // U+064B..U+065F
        (cha == 0xD9 && cha2 == 0xB0);                    // U+0670
      if (!haraka) {
        out += (char)cha;
        out += (char)cha2;
      }
      i += 2;
    } else {
      out += (char)cha;
      ++i;
    }
  }

  return out;
}

void noHarakaatFunc(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
  if (argc != 1) {
    sqlite3_result_null(ctx);
    return;
  }

  const unsigned char* text = sqlite3_value_text(argv[0]);
  if (!text) {
    sqlite3_result_null(ctx);
    return;
  }

  const std::string stripped = stripHarakaat(reinterpret_cast<const char*>(text));
  sqlite3_result_text(ctx, stripped.c_str(), -1, SQLITE_TRANSIENT);
}

}  // namespace

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

  sqlite3_create_function(db, "no_harakaat", 1, SQLITE_UTF8, nullptr, noHarakaatFunc,
                          nullptr, nullptr);
}

Database::~Database() {
  if (db) sqlite3_close(db);
  if (versions) sqlite3_close(versions);
}

void Database::addNewWord(const Word& word) {
  if (!db) return;
  const char* sql =
    "INSERT INTO words (english, arabic, source, dialect, wordform, category, tags, exported) "
    "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

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
  sqlite3_bind_text(stmt, 7, word.tags.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 8, word.exported ? 1 : 0);

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
    "SELECT id, english, arabic, source, dialect, wordform, category, tags, exported "
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
    w.tags = col_text(stmt, 7);
    w.exported = sqlite3_column_int(stmt, 8) != 0;
    result.push_back(w);
  }

  sqlite3_finalize(stmt);
  return result;
}

std::vector<Word> Database::getUnexportedWords() {
  std::vector<Word> result;
  if (!db) return result;

  const char* sql =
    "SELECT id, english, arabic, source, dialect, wordform, category, tags, exported "
    "FROM words WHERE exported = 0 ORDER BY id DESC;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
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
    w.tags = col_text(stmt, 7);
    w.exported = sqlite3_column_int(stmt, 8) != 0;
    result.push_back(w);
  }

  sqlite3_finalize(stmt);
  return result;
}

void Database::updateWord(const Word& word) {
  if (!db || word.id == 0) return;

  const char* sql =
    "UPDATE words SET english=?, arabic=?, source=?, dialect=?, wordform=?, "
    "category=?, tags=?, exported=? WHERE id=?;";

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
  sqlite3_bind_text(stmt, 6, word.category.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 7, word.tags.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 8, word.exported ? 1 : 0);
  sqlite3_bind_int(stmt, 9, word.id);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    fprintf(stderr, "update failed: %s\n", sqlite3_errmsg(db));
  }

  sqlite3_finalize(stmt);
}

std::vector<Word> Database::searchWords(const std::string& query, int limit) {
  std::vector<Word> result;
  if (!db || query.empty()) return result;

  const std::string pattern = "%" + query + "%";
  const char* sql =
    "SELECT id, english, arabic, source, dialect, wordform, category, tags, exported "
    "FROM words "
    "WHERE english LIKE ? OR no_harakaat(arabic) LIKE no_harakaat(?) "
    "ORDER BY id DESC LIMIT ?;";

  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    fprintf(stderr, "prepare failed: %s\n", sqlite3_errmsg(db));
    return result;
  }

  sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 3, limit);

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    Word w;
    w.id = sqlite3_column_int(stmt, 0);
    w.english = col_text(stmt, 1);
    w.arabic = col_text(stmt, 2);
    w.source = col_text(stmt, 3);
    w.dialect = col_text(stmt, 4);
    w.wordform = col_text(stmt, 5);
    w.category = col_text(stmt, 6);
    w.tags = col_text(stmt, 7);
    w.exported = sqlite3_column_int(stmt, 8) != 0;
    result.push_back(w);
  }

  sqlite3_finalize(stmt);
  return result;
}

