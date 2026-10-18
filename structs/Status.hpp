#pragma once

enum class Status {
  OK = 0,
  ERR_IO,        // can't open / read / write the db file
  ERR_SQL,       // prepare/bind/step failed (bug or constraint)
  ERR_NOT_FOUND  // only if you want "missing" to be an error

};
