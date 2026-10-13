#pragma once

#include <QWidget>

class QPlainTextEdit;

class NewEntry : public QWidget {
public:
  explicit NewEntry(QWidget* parent = nullptr);
  void clear();  // clear fields
  void save();   // print current editors


private:
  QPlainTextEdit* _arabic;
  QPlainTextEdit* _english;
};
