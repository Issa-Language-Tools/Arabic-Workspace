#pragma once

#include <sqlite3.h>

#include <filesystem>
#include <string>

#include "Status.hpp"

namespace fs = std::filesystem;

Status check_versions_db(sqlite3** versions);
Status initialize_versions_db(sqlite3** out);
std::string get_latest_migration(sqlite3* versions);

Status apply_migration_versions(sqlite3* versions, sqlite3* db);
Status apply_migration_file(sqlite3* db, const fs::path& path);

std::string col_text(sqlite3_stmt* stmt, int col);
