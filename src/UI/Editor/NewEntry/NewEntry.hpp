#pragma once

#include <QWidget>

#include "Word.hpp"

class QPlainTextEdit;
struct EntryDefaults;

class NewEntry : public QWidget {
  Q_OBJECT
public:
  explicit NewEntry(const EntryDefaults* ed, QWidget* parent = nullptr);
  void clear();  // clear fields
  void save();   // print current editors

private:
  QPlainTextEdit* m_arabic;
  QPlainTextEdit* m_english;
  const EntryDefaults* m_defaults;

signals:
  void wordSaved(const Word&);
};
