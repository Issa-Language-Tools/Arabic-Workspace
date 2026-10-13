#include "WordList.hpp"

#include <qboxlayout.h>

#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <iostream>

#include "../EditPopup/EditPopup.hpp"
#include "../../Theme/Theme.hpp"

WordList::WordList(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* stack = new QVBoxLayout(this);

  m_table = new QTableWidget(0, 4, this);
  Theme::table(m_table);
  m_table->setHorizontalHeaderLabels({"Arabic", "English", "Source", ""});

  m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
  m_table->setColumnWidth(3, 60);

  stack->addWidget(m_table);
}

// TODO: remove
void WordList::makeSampleData() {
  Word w1; w1.arabic = "مرحبا"; w1.english = "hello"; w1.source = "book";
  Word w2; w2.arabic = "شكرا"; w2.english = "thank you"; w2.source = "convo";
  Word w3; w3.arabic = "شكرا"; w3.english = "thank you"; w3.source = "convo";
  Word w4; w4.arabic = "شكرا"; w4.english = "thank you"; w4.source = "convo";
  Word w5; w5.arabic = "شكرا"; w5.english = "thank you"; w5.source = "convo";
  Word w6; w6.arabic = "شكرا"; w6.english = "thank you"; w6.source = "convo";
  Word w7; w7.arabic = "شكرا"; w7.english = "thank you"; w7.source = "convo";
  Word w8; w8.arabic = "شكرا"; w8.english = "thank you"; w8.source = "convo";
  this->m_words.push_back(w1);
  this->m_words.push_back(w2);
  this->m_words.push_back(w3);
  this->m_words.push_back(w4);
  this->m_words.push_back(w5);
  this->m_words.push_back(w6);
  this->m_words.push_back(w7);
  this->m_words.push_back(w8);
}

void WordList::refreshRow(int r) {
  m_table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(m_words[r].arabic)));
  m_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(m_words[r].english)));
  m_table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(m_words[r].source)));
}

void WordList::populate() {
  makeSampleData();
  m_table->setRowCount((int)m_words.size());

  for (int r = 0; r < (int)m_words.size(); ++r) {
    Word* w = &m_words[r];

    m_table->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(w->arabic)));
    m_table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(w->english)));
    m_table->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(w->source)));

    QPushButton* editButton = new QPushButton("Edit");
    Theme::button(editButton);

    connect(editButton, &QPushButton::clicked, this, [this, w, r]() {
      EditPopup popup(w);

      if (popup.exec() == QDialog::Accepted) {
        std::cout << "edit: " << w->english << " : " << w->arabic << '\n';
        refreshRow(r);
      } else {
        std::cout << "no edit: " << w->english << " : " << w->arabic << '\n';
      }
    });

    m_table->setCellWidget(r, 3, editButton);

    m_table->setRowHeight(r, 40);
  }
}
