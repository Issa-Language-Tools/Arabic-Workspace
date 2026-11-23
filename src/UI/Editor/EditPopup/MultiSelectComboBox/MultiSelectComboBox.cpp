#include "MultiSelectComboBox.hpp"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
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

  m_popup = new QWidget(this);
  m_popup->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

  QVBoxLayout* popupLayout = new QVBoxLayout(m_popup);
  popupLayout->setContentsMargins(0, 0, 0, 0);
  popupLayout->setSpacing(0);

  m_search = new QLineEdit();
  Theme::searchBox(m_search, "Search…");
  connect(m_search, &QLineEdit::textChanged, this, &MultiSelectComboBox::filterItems);
  popupLayout->addWidget(m_search);

  m_list = new CheckList(m_popup);
  m_list->setFocusPolicy(Qt::NoFocus);
  m_list->setSelectionMode(QAbstractItemView::NoSelection);
  Theme::checkList(m_list);
  popupLayout->addWidget(m_list);

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
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    if (m_popup->isVisible() && keyEvent->key() == Qt::Key_Escape) {
      m_popup->hide();
      // Consume the event so the parent dialog doesn't also close on Escape.
      return true;
    }
  }

  if (event->type() == QEvent::MouseButtonPress) {
    QWidget* widget = qobject_cast<QWidget*>(watched);
    if (widget && m_popup->isVisible() && widget != m_button &&
        !m_popup->isAncestorOf(widget) && !m_button->isAncestorOf(widget)) {
      m_popup->hide();
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

void MultiSelectComboBox::addCheckedItem(const QString& text) {
  for (int i = 0; i < m_list->count(); ++i) {
    QListWidgetItem* entry = m_list->item(i);
    if (entry->text() == text) {
      entry->setCheckState(Qt::Checked);
      updateButtonText();
      return;
    }
  }

  QListWidgetItem* entry = new QListWidgetItem(text, m_list);
  entry->setFlags(entry->flags() | Qt::ItemIsUserCheckable);
  entry->setCheckState(Qt::Checked);
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
  if (m_popup->isVisible()) {
    m_popup->hide();
    return;
  }

  // Reset the filter so the full list is shown each time it opens.
  m_search->clear();
  filterItems("");

  // Size the popup to fit its contents, but never narrower than the box and
  // never taller than a reasonable screen share.
  m_list->adjustSize();
  m_list->setFixedHeight(qMin(650, m_list->sizeHint().height()));
  m_popup->setFixedWidth(qMax(m_button->width(), m_list->sizeHint().width()));
  m_popup->adjustSize();

  // Drop it down directly below the button.
  m_popup->move(m_button->mapToGlobal(QPoint(0, m_button->height())));
  m_popup->show();
  m_popup->raise();
  m_search->setFocus();
}

void MultiSelectComboBox::filterItems(const QString& text) {
  for (int i = 0; i < m_list->count(); ++i) {
    QListWidgetItem* item = m_list->item(i);
    item->setHidden(!item->text().contains(text, Qt::CaseInsensitive));
  }
}

void MultiSelectComboBox::updateButtonText() {
  const QStringList items = checkedItems();
  m_button->setText(items.isEmpty() ? m_placeholder : items.join(", "));
}
