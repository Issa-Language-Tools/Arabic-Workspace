#pragma once

#include <QString>
#include <QWidget>

#include "Word.hpp"

class NewEntryFilter;
class QPlainTextEdit;
struct EntryDefaults;

class NewEntry : public QWidget {
  Q_OBJECT
public:
  explicit NewEntry(const EntryDefaults* ed, QWidget* parent = nullptr);
  void clear();  // clear fields
  void save();   // save the current entry

private:
  QPlainTextEdit* m_arabic;
  QPlainTextEdit* m_english;
  NewEntryFilter* m_filter;
  const EntryDefaults* m_defaults;

signals:
  void wordSaved(const Word&);
  void searchRequested(const QString& query);
  void searchCleared();
};
