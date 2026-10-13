#pragma once

#include <QWidget>
#include <vector>

#include "Word.hpp"

class QTableWidget;

class WordList : public QWidget {
public:
  explicit WordList(QWidget* parent = nullptr);
  void populate();

private:
  void makeSampleData();  // TODO: remove
  void refreshRow(int r);

  QTableWidget* m_table;
  std::vector<Word> m_words;
};
