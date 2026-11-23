#pragma once

#include <QDialog>
#include <QStringList>

class QLineEdit;
class QPlainTextEdit;
class MultiSelectComboBox;
class Word;

class EditPopup : public QDialog {
public:
  explicit EditPopup(Word* word, const QStringList& wordforms, const QStringList& tags,
                     QWidget* parent = nullptr);

private:
  QPlainTextEdit* m_inEnglish;
  QPlainTextEdit* m_inArabic;
  QPlainTextEdit* m_inSource;
  QPlainTextEdit* m_inDialect;
  MultiSelectComboBox* m_inWordForm;
  MultiSelectComboBox* m_inTags;
  QLineEdit* m_newTagInput;
  Word* w;
};
