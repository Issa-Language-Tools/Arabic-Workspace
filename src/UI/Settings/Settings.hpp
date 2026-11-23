#pragma once

#include <qtmetamacros.h>
#include <QWidget>

class QPlainTextEdit;
class QShowEvent;
class EntryDefaults;

class Settings : public QWidget {
  Q_OBJECT
public:
  explicit Settings(EntryDefaults* ed, QWidget* parent = nullptr);

protected:
  void showEvent(QShowEvent* event) override;

private:
  void save();

private:
  QPlainTextEdit* m_inSource;
  QPlainTextEdit* m_inDialect;
  QPlainTextEdit* m_inWordform;
  QPlainTextEdit* m_inTags;
  EntryDefaults* m_defaults;

signals:
  void defaultsChanged();
};
