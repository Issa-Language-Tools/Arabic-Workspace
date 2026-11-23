#pragma once

#include <QStringList>
#include <QWidget>

class QLineEdit;
class QListWidget;
class QPushButton;
class QEvent;
class QWidget;

class MultiSelectComboBox : public QWidget {
  Q_OBJECT
public:
  explicit MultiSelectComboBox(QWidget* parent = nullptr);
  ~MultiSelectComboBox() override;

  void setItems(const QStringList& items);
  void setPlaceholder(const QString& text);
  void setCheckedItems(const QStringList& items);
  void addCheckedItem(const QString& text);
  QStringList checkedItems() const;

signals:
  void selectionChanged();

protected:
  bool eventFilter(QObject* watched, QEvent* event) override;

private:
  void togglePopup();
  void updateButtonText();
  void filterItems(const QString& text);

private:
  QPushButton* m_button;
  QWidget* m_popup;
  QLineEdit* m_search;
  QListWidget* m_list;
  QString m_placeholder = "Select…";
};
