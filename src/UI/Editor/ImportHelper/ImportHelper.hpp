#pragma once

#include <QDialog>
#include <QStringList>
#include <filesystem>
#include <vector>

#include "Word.hpp"

class QCheckBox;
class QComboBox;
class QGridLayout;
class QLineEdit;
class QTableWidget;
struct EntryDefaults;

namespace fs = std::filesystem;

class ImportHelper : public QDialog {
  Q_OBJECT
public:
  explicit ImportHelper(const EntryDefaults* defaults, QWidget* parent = nullptr);

signals:
  // Emitted after the user confirms the import. For now this only feeds the
  // word list; later it can also be persisted to the database.
  void imported(const std::vector<Word>& words);

private:
  void rebuild(bool autoDetect);
  QStringList parseLine(const QString& line) const;
  std::vector<QStringList> readRows(int maxRows) const;
  QChar currentDelimiter() const;
  void doImport();
  void routeTags(const QString& rawTags, Word& w) const;

private:
  QLineEdit* m_pathInput;
  QComboBox* m_delimiter;
  QCheckBox* m_hasHeader;
  QCheckBox* m_applyDefaults;
  QTableWidget* m_preview;
  QGridLayout* m_mappingLayout;
  std::vector<QComboBox*> m_fieldMaps;
  const EntryDefaults* m_defaults;
  fs::path m_path;
};
