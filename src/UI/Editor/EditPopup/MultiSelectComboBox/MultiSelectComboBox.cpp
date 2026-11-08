#include "MultiSelectComboBox.hpp"

#include <QApplication>
#include <QEvent>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QPushButton>
#include <QSet>
#include <QVBoxLayout>

#include "../../../Theme/Theme.hpp"

namespace {
class CheckList : public QListWidget {
public:
  using QListWidget::QListWidget;

protected:
  void mousePressEvent(QMouseEvent* event) override {
    QListWidgetItem* item = itemAt(event->pos());
    if (item && (item->flags() & Qt::ItemIsUserCheckable)) {
      item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }
  }
};
}  // namespace

MultiSelectComboBox::MultiSelectComboBox(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_button = new QPushButton(m_placeholder, this);
  Theme::comboButton(m_button);
  connect(m_button, &QPushButton::clicked, this, &MultiSelectComboBox::togglePopup);
  layout->addWidget(m_button);

  m_list = new CheckList(this);
  m_list->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
  m_list->setAttribute(Qt::WA_ShowWithoutActivating);
  m_list->setFocusPolicy(Qt::NoFocus);
  m_list->setSelectionMode(QAbstractItemView::NoSelection);
  Theme::checkList(m_list);

  connect(m_list, &QListWidget::itemChanged, this, [this](QListWidgetItem*) {
    updateButtonText();
    emit selectionChanged();
  });

  qApp->installEventFilter(this);
}

MultiSelectComboBox::~MultiSelectComboBox() {
  if (qApp) {
    qApp->removeEventFilter(this);
  }
}

bool MultiSelectComboBox::eventFilter(QObject* watched, QEvent* event) {
  // Close the dropdown when the user clicks anywhere that isn't the button
  // or the list itself. Closing happens without consuming the event, so the
  // click still reaches whatever was clicked (e.g. the Save button).
  if (event->type() == QEvent::MouseButtonPress) {
    QWidget* widget = qobject_cast<QWidget*>(watched);
    if (widget && m_list->isVisible() && widget != m_button &&
        !m_list->isAncestorOf(widget) && !m_button->isAncestorOf(widget)) {
      m_list->hide();
    }
  } else if (event->type() == QEvent::ApplicationDeactivate) {
    // Switching to another application also closes it. (We use this instead of
    // WindowDeactivate because showing the popup itself deactivates the dialog,
    // which would otherwise close the popup immediately.)
    if (m_list->isVisible()) {
      m_list->hide();
    }
  }

  return QWidget::eventFilter(watched, event);
}

void MultiSelectComboBox::setItems(const QStringList& items) {
  m_list->clear();
  for (const QString& item : items) {
    QListWidgetItem* entry = new QListWidgetItem(item, m_list);
    entry->setFlags(entry->flags() | Qt::ItemIsUserCheckable);
    entry->setCheckState(Qt::Unchecked);
  }
  updateButtonText();
}

void MultiSelectComboBox::setPlaceholder(const QString& text) {
  m_placeholder = text;
  updateButtonText();
}

void MultiSelectComboBox::setCheckedItems(const QStringList& items) {
  const QSet<QString> checked(items.begin(), items.end());
  for (int i = 0; i < m_list->count(); ++i) {
    QListWidgetItem* entry = m_list->item(i);
    entry->setCheckState(checked.contains(entry->text()) ? Qt::Checked : Qt::Unchecked);
  }
  updateButtonText();
}

QStringList MultiSelectComboBox::checkedItems() const {
  QStringList result;
  for (int i = 0; i < m_list->count(); ++i) {
    QListWidgetItem* entry = m_list->item(i);
    if (entry->checkState() == Qt::Checked) {
      result << entry->text();
    }
  }
  return result;
}

void MultiSelectComboBox::togglePopup() {
  if (m_list->isVisible()) {
    m_list->hide();
    return;
  }

  // Size the popup to fit its contents, but never narrower than the box and
  // never taller than a reasonable screen share.
  m_list->adjustSize();
  m_list->setFixedWidth(qMax(m_button->width(), m_list->sizeHint().width()));
  m_list->setFixedHeight(qMin(400, m_list->sizeHint().height()));

  // Drop it down directly below the button.
  m_list->move(m_button->mapToGlobal(QPoint(0, m_button->height())));
  m_list->show();
  m_list->raise();
}

void MultiSelectComboBox::updateButtonText() {
  const QStringList items = checkedItems();
  m_button->setText(items.isEmpty() ? m_placeholder : items.join(", "));
}
