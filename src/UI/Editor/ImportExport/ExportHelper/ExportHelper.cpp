#include "ExportHelper.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextStream>
#include <QVBoxLayout>

#include "../../../../Plumbing/Database/Database.hpp"
#include "../../../Theme/Theme.hpp"
#include "../../EditPopup/MultiSelectComboBox/MultiSelectComboBox.hpp"
#include "Word.hpp"

namespace {

const QStringList kFields = {"English", "Arabic", "Source", "Dialect", "Wordform", "Category", "Tags"};

QString csvField(const QString& value) {
  QString s = value;
  if (s.contains(',') || s.contains('"') || s.contains('\n')) {
    s.replace('"', "\"\"");
    return '"' + s + '"';
  }
  return s;
}

QString fieldValue(const Word& w, const QString& field) {
  if (field == "English") return QString::fromStdString(w.english);
  if (field == "Arabic") return QString::fromStdString(w.arabic);
  if (field == "Source") return QString::fromStdString(w.source);
  if (field == "Dialect") return QString::fromStdString(w.dialect);
  if (field == "Wordform") return QString::fromStdString(w.wordform);
  if (field == "Category") return QString::fromStdString(w.category);
  if (field == "Tags") return QString::fromStdString(w.tags);
  return QString();
}

QString columnValue(const Word& w, const QStringList& fields, const QString& sep) {
  QStringList parts;
  for (const QString& field : fields) {
    const QString value = fieldValue(w, field);
    if (!value.isEmpty()) parts << value;
  }
  return parts.join(sep);
}

void clearLayout(QLayout* layout) {
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
      delete widget;
    } else if (QLayout* child = item->layout()) {
      clearLayout(child);
    }
    delete item;
  }
}

}  // namespace

ExportHelper::ExportHelper(Database* db, QWidget* parent) : QDialog(parent), m_db(db) {
  setWindowTitle("Export");
  setStyleSheet("background-color: rgb(40, 40, 40);");
  resize(640, 560);

  QVBoxLayout* stack = new QVBoxLayout(this);

  QLabel* title = new QLabel("Export columns");
  Theme::label(title);
  stack->addWidget(title);

  QScrollArea* scroll = new QScrollArea();
  scroll->setWidgetResizable(true);
  scroll->setStyleSheet("QScrollArea { border: none; background: transparent; }");

  QWidget* container = new QWidget();
  container->setStyleSheet("background: transparent;");

  m_columnsLayout = new QVBoxLayout(container);
  m_columnsLayout->setContentsMargins(0, 0, 0, 0);
  m_columnsLayout->setSpacing(6);

  scroll->setWidget(container);
  stack->addWidget(scroll, 1);

  QPushButton* addColumn = new QPushButton("Add column");
  Theme::button(addColumn);
  addColumn->setMaximumWidth(140);
  connect(addColumn, &QPushButton::clicked, this, [this]() {
    m_config.push_back({});
    rebuildColumns();
  });
  stack->addWidget(addColumn);

  QHBoxLayout* optionsRow = new QHBoxLayout();

  QLabel* sepLabel = new QLabel("Separator");
  Theme::label(sepLabel);

  m_separator = new QComboBox();
  Theme::combo(m_separator);
  m_separator->addItem("Space", " ");
  m_separator->addItem("Comma", ",");
  m_separator->addItem("Pipe", "|");

  m_includeHeader = new QCheckBox("Include header row");
  m_includeHeader->setChecked(true);
  Theme::checkbox(m_includeHeader);

  m_markExported = new QCheckBox("Mark as exported");
  m_markExported->setChecked(false);
  Theme::checkbox(m_markExported);

  optionsRow->addWidget(sepLabel);
  optionsRow->addWidget(m_separator);
  optionsRow->addSpacing(20);
  optionsRow->addWidget(m_includeHeader);
  optionsRow->addSpacing(20);
  optionsRow->addWidget(m_markExported);
  optionsRow->addStretch();
  stack->addLayout(optionsRow);

  QHBoxLayout* buttonRow = new QHBoxLayout();
  QPushButton* exportButton = new QPushButton("Export");
  Theme::button(exportButton);
  QPushButton* cancelButton = new QPushButton("Cancel");
  Theme::button(cancelButton);

  connect(exportButton, &QPushButton::clicked, this, &ExportHelper::doExport);
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

  buttonRow->addStretch();
  buttonRow->addWidget(exportButton);
  buttonRow->addWidget(cancelButton);
  stack->addLayout(buttonRow);

  m_config = {
    {"English"},
    {"Arabic"},
    {"Source", "Dialect", "Wordform", "Category", "Tags"},
  };
  rebuildColumns();
}

void ExportHelper::rebuildColumns() {
  clearLayout(m_columnsLayout);

  for (int i = 0; i < (int)m_config.size(); ++i) {
    QHBoxLayout* row = new QHBoxLayout();

    MultiSelectComboBox* select = new MultiSelectComboBox();
    select->setItems(kFields);
    select->setCheckedItems(m_config[i]);

    const int index = i;
    connect(select, &MultiSelectComboBox::selectionChanged, this, [this, index, select]() {
      m_config[index] = select->checkedItems();
    });

    QPushButton* remove = new QPushButton("✕");
    remove->setStyleSheet(
      "background-color: rgb(55, 55, 55);"
      "color: rgb(245, 245, 245);"
      "font-size: 16px;");
    remove->setFixedWidth(36);
    connect(remove, &QPushButton::clicked, this, [this, index]() {
      m_config.erase(m_config.begin() + index);
      rebuildColumns();
    });

    row->addWidget(select, 1);
    row->addWidget(remove);
    m_columnsLayout->addLayout(row);
  }
}

void ExportHelper::doExport() {
  const std::vector<Word> words = m_db->getUnexportedWords();
  if (words.empty()) {
    emit exported(0);
    accept();
    return;
  }

  QFileDialog dialog(this, "Export to CSV");
  dialog.setOption(QFileDialog::DontUseNativeDialog, true);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter("CSV files (*.csv);;All files (*)");
  dialog.selectFile("output-words.csv");
  Theme::fileDialog(&dialog);

  if (dialog.exec() != QDialog::Accepted) {
    return;  // user cancelled
  }

  const QString path = dialog.selectedFiles().value(0);
  if (path.isEmpty()) {
    return;
  }

  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    emit exported(0);
    return;
  }

  const QString sep = m_separator->currentData().toString();
  QTextStream out(&file);

  if (m_includeHeader->isChecked()) {
    QStringList header;
    for (const QStringList& fields : m_config) {
      header << fields.join(sep);
    }
    for (int i = 0; i < header.size(); ++i) {
      if (i > 0) out << ',';
      out << csvField(header[i]);
    }
    out << '\n';
  }

  for (const Word& w : words) {
    for (int i = 0; i < (int)m_config.size(); ++i) {
      if (i > 0) out << ',';
      out << csvField(columnValue(w, m_config[i], sep));
    }
    out << '\n';
  }
  file.close();

  if (m_markExported->isChecked()) {
    for (const Word& w : words) {
      m_db->setExported(w.id, true);
    }
  }

  emit exported((int)words.size());
  accept();
}
