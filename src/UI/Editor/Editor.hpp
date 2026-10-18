#pragma once

#include <QWidget>

class NewEntry;
class WordList;
class EntryDefaults;

class Editor : public QWidget {
public:
  explicit Editor(const EntryDefaults* ed, QWidget* parent = nullptr);

private:
  NewEntry* m_newEntry;
  WordList* m_wordList;
  const EntryDefaults* m_defaults;
};
