#include "WordList.hpp"

#include <QBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include "../../Theme/Theme.hpp"
#include "../EditPopup/EditPopup.hpp"
#include "EntryDefaults.hpp"

namespace {
constexpr int kRowHeight = 40;
constexpr int kVisibleRows = 10;
}  // namespace

WordList::WordList(const EntryDefaults* ed, QWidget* parent) : QWidget(parent), m_defaults(ed) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_table = new QTableWidget(0, 4, this);
  m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
  Theme::table(m_table);
  m_table->setHorizontalHeaderLabels({"Arabic", "English", "Tags", ""});

  m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
  m_table->setColumnWidth(3, 60);

  const int headerHeight = m_table->horizontalHeader()->sizeHint().height();
  m_table->setFixedHeight(headerHeight + kVisibleRows * kRowHeight);

  stack->addWidget(m_table);
}

void WordList::setWords(const std::vector<Word>& words) {
  m_words = words;

  m_table->setRowCount((int)m_words.size());
  for (int r = 0; r < (int)m_words.size(); ++r) {
    refreshRow(r);
  }
}

void WordList::refreshRow(int r) {
  const Word& w = m_words[r];

  m_table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(w.arabic)));
  m_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(w.english)));
  m_table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(w.tags)));

  QPushButton* editButton = new QPushButton("Edit");
  Theme::button(editButton);

  // Capture only the row index, then look up the Word when the button is clicked.
  connect(editButton, &QPushButton::clicked, this, [this, r]() {
    Word* w = &m_words[r];

    EditPopup popup(w, m_defaults->wordforms, m_defaults->tags);

    if (popup.exec() == QDialog::Accepted) {
      refreshRow(r);
      emit wordEdited(*w);
    } else {
    }
  });

  m_table->setCellWidget(r, 3, editButton);
  m_table->setRowHeight(r, kRowHeight);
}
