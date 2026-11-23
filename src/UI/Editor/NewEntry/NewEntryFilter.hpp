#pragma once

#include <QObject>
#include <QString>

class QEvent;
class QPlainTextEdit;

// Watches the Arabic/English entry fields and turns key presses into search,
// focus, and save actions.
class NewEntryFilter : public QObject {
  Q_OBJECT
public:
  NewEntryFilter(QPlainTextEdit* arabic, QPlainTextEdit* english, QObject* parent = nullptr);

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;

private:
  void handleTabSearch(QObject* field);

signals:
  void searchRequested(const QString& query);
  void searchCleared();
  void saveRequested();

private:
  QPlainTextEdit* m_arabic;
  QPlainTextEdit* m_english;
};
