#pragma once

#include <QWidget>

class NewEntry;
class WordList;

class Editor : public QWidget {
public:
  explicit Editor(QWidget* parent = nullptr);

private:
  NewEntry* m_newEntry;
  WordList* m_wordList;
};
