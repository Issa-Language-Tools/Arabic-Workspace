#include "DictionaryPanel.hpp"

#include <QFrame>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

#include "../Theme/Theme.hpp"

namespace {

QWidget* makeCard(const QString& word, const QString& meaning, const QString& source) {
  QFrame* frame = new QFrame();
  Theme::cardFrame(frame);

  QVBoxLayout* layout = new QVBoxLayout(frame);
  layout->setContentsMargins(8, 6, 8, 6);
  layout->setSpacing(2);

  QLabel* wordLabel = new QLabel(word);
  Theme::cardWord(wordLabel);

  QLabel* meaningLabel = new QLabel(meaning);
  meaningLabel->setWordWrap(true);
  Theme::cardMeaning(meaningLabel);

  QLabel* sourceLabel = new QLabel(source);
  Theme::cardSource(sourceLabel);

  layout->addWidget(wordLabel);
  layout->addWidget(meaningLabel);
  layout->addWidget(sourceLabel);

  return frame;
}

}  // namespace

DictionaryPanel::DictionaryPanel(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* outer = new QVBoxLayout(this);
  outer->setContentsMargins(0, 0, 0, 0);

  QScrollArea* scroll = new QScrollArea();
  Theme::scrollArea(scroll);

  QWidget* container = new QWidget();
  Theme::transparent(container);

  m_cardsLayout = new QVBoxLayout(container);
  m_cardsLayout->setContentsMargins(8, 8, 8, 8);
  m_cardsLayout->setSpacing(8);
  m_cardsLayout->addStretch();

  scroll->setWidget(container);
  outer->addWidget(scroll);
}

void DictionaryPanel::setResults(const QJsonArray& results) {
  // Clear existing cards, leaving only the trailing stretch item.
  while (m_cardsLayout->count() > 1) {
    QLayoutItem* item = m_cardsLayout->takeAt(0);
    if (QWidget* widget = item->widget()) {
      delete widget;
    }
    delete item;
  }

  for (const QJsonValue& resultValue : results) {
    const QJsonArray dictionaries =
      resultValue.toObject()["dictionaries"].toArray();

    for (const QJsonValue& dictValue : dictionaries) {
      const QJsonObject dict = dictValue.toObject();
      const QString source = dict["name"].toString();

      const QJsonArray roots = dict["roots"].toArray();
      for (const QJsonValue& rootValue : roots) {
        const QJsonArray words = rootValue.toObject()["words"].toArray();

        for (const QJsonValue& wordValue : words) {
          const QJsonObject word = wordValue.toObject();
          QWidget* card = makeCard(
            word["word"].toString(),
            word["meaning"].toString(),
            source);

          m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
        }
      }
    }
  }
}
