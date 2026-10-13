#pragma once

#include <QDialog>

class QPlainTextEdit;
class QString;
class QVBoxLayout;
struct Word;

class EditPopup : public QDialog {
public:
  explicit EditPopup(Word* word, QWidget* parent = nullptr);

private:
  QPlainTextEdit* addRow(const QString& label, const QString& value, QVBoxLayout* stack);
  QPlainTextEdit* _inEnglish;
  QPlainTextEdit* _inArabic;
  QPlainTextEdit* _inSource;
  QPlainTextEdit* _inDialect;
  QPlainTextEdit* _inWordform;
  Word* w;
};
