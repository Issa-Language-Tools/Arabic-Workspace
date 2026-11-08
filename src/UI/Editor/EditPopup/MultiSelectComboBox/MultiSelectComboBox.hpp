#pragma once

#include <QStringList>
#include <QWidget>

class QListWidget;
class QPushButton;
class QEvent;

class MultiSelectComboBox : public QWidget {
  Q_OBJECT
public:
  explicit MultiSelectComboBox(QWidget* parent = nullptr);
  ~MultiSelectComboBox() override;

  void setItems(const QStringList& items);
  void setPlaceholder(const QString& text);
  void setCheckedItems(const QStringList& items);
  QStringList checkedItems() const;

signals:
  void selectionChanged();

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;

private:
  void togglePopup();
  void updateButtonText();

private:
  QPushButton* m_button;
  QListWidget* m_list;
  QString m_placeholder = "Select…";
};
