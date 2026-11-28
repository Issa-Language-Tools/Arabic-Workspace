#pragma once

#include <QJsonArray>
#include <QWidget>

class QVBoxLayout;

class DictionaryPanel : public QWidget {
  Q_OBJECT
public:
  explicit DictionaryPanel(QWidget* parent = nullptr);

public slots:
  void setResults(const QJsonArray& results);

private:
  QVBoxLayout* m_cardsLayout;
};
