CREATE TABLE IF NOT EXISTS words (
  id         INTEGER PRIMARY KEY AUTOINCREMENT,
  english    TEXT NOT NULL,
  arabic     TEXT NOT NULL,
  source     TEXT DEFAULT '',
  dialect    TEXT DEFAULT '',
  wordform   TEXT DEFAULT '',
  category   TEXT DEFAULT '',
  exported   INTEGER DEFAULT 0,
  created_at TEXT DEFAULT (datetime('now'))
);

-- find unexported to send to anki
CREATE INDEX indx_words_exported ON words(exported);
