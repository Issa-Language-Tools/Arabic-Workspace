#pragma once

#include <QDialog>
#include <QStringList>

class QPlainTextEdit;
class MultiSelectComboBox;
class Word;

class EditPopup : public QDialog {
public:
  explicit EditPopup(Word* word, const QStringList& wordforms, QWidget* parent = nullptr);

private:
  QPlainTextEdit* m_inEnglish;
  QPlainTextEdit* m_inArabic;
  QPlainTextEdit* m_inSource;
  QPlainTextEdit* m_inDialect;
  MultiSelectComboBox* m_inWordForm;
  Word* w;
};
