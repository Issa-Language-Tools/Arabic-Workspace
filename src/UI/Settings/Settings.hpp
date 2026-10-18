#pragma once

#include <qtmetamacros.h>
#include <QWidget>

class QPlainTextEdit;
class EntryDefaults;

class Settings : public QWidget {
  Q_OBJECT
public:
  explicit Settings(EntryDefaults* ed, QWidget* parent = nullptr);

private:
  void save();

private:
  QPlainTextEdit* m_inSource;
  QPlainTextEdit* m_inDialect;
  QPlainTextEdit* m_inWordform;
  EntryDefaults* m_defaults;

signals:
  void defaultsChanged();
};
