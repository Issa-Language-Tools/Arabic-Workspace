#pragma once

#include <qlabel.h>

class TitleBar : public QLabel {
  public:
  explicit TitleBar(const QString& text, QWidget* parent = nullptr);
};
