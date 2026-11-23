#include "NewEntryFilter.hpp"

#include <QEvent>
#include <QKeyEvent>
#include <QPlainTextEdit>

NewEntryFilter::NewEntryFilter(QPlainTextEdit* arabic, QPlainTextEdit* english, QObject* parent)
    : QObject(parent), m_arabic(arabic), m_english(english) {}

bool NewEntryFilter::eventFilter(QObject* watched, QEvent* event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent* key = static_cast<QKeyEvent*>(event);
    const bool ctrl = key->modifiers() & Qt::ControlModifier;

    // Tab triggers search; it does not move focus.
    if (key->key() == Qt::Key_Tab && !ctrl && !(key->modifiers() & Qt::ShiftModifier)) {
      handleTabSearch(watched);
      return true;
    }

    // Vim-style focus movement.
    if (ctrl && key->key() == Qt::Key_H) {
      m_arabic->setFocus();
      return true;
    }
    if (ctrl && key->key() == Qt::Key_L) {
      m_english->setFocus();
      return true;
    }

    if (ctrl && key->key() == Qt::Key_S) {
      emit saveRequested();
      return true;
    }

    // Escape clears the search view without clearing the entry.
    if (key->key() == Qt::Key_Escape && !ctrl) {
      emit searchCleared();
      return true;
    }
  }

  return QObject::eventFilter(watched, event);
}

void NewEntryFilter::handleTabSearch(QObject* field) {
  if (field == m_arabic) {
    // Arabic always searches (duplicate check while adding).
    const QString query = m_arabic->toPlainText().trimmed();
    if (!query.isEmpty()) {
      emit searchRequested(query);
    }
  } else if (field == m_english) {
    // English only searches when Arabic is empty (lookup mode).
    const bool arabicEmpty = m_arabic->toPlainText().trimmed().isEmpty();
    if (arabicEmpty) {
      const QString query = m_english->toPlainText().trimmed();
      if (!query.isEmpty()) {
        emit searchRequested(query);
      }
    }
  }
}
