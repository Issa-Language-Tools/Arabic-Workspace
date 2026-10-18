#pragma once

#include <QWidget>
#include <vector>

#include "Word.hpp"

class EntryDefaults;
class QTableWidget;

class WordList : public QWidget {
  Q_OBJECT
public:
  explicit WordList(const EntryDefaults* ed, QWidget* parent = nullptr);
  void populate();
public slots:
  void addWord(const Word& w);

private:
  void makeSampleData();  // TODO: remove
  void refreshRow(int r);

private:
  QTableWidget* m_table;
  const EntryDefaults* m_defaults;
  std::vector<Word> m_words;
};
