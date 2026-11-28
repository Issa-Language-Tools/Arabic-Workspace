#pragma once

#include <QDialog>
#include <QStringList>
#include <vector>

class Database;
class MultiSelectComboBox;
class QCheckBox;
class QComboBox;
class QVBoxLayout;

class ExportHelper : public QDialog {
  Q_OBJECT
public:
  explicit ExportHelper(Database* db, QWidget* parent = nullptr);

signals:
  void exported(int count);

private:
  void rebuildColumns();
  void doExport();

  Database* m_db;
  QVBoxLayout* m_columnsLayout;
  QComboBox* m_separator;
  QCheckBox* m_includeHeader;
  QCheckBox* m_markExported;
  std::vector<QStringList> m_config;
};
