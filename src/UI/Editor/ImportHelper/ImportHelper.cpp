#include "ImportHelper.hpp"

#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qgridlayout.h>
#include <qheaderview.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qsettings.h>
#include <qtablewidget.h>

#include <algorithm>
#include <array>
#include <fstream>

#include "../../Theme/Theme.hpp"
#include "EntryDefaults.hpp"

namespace {

constexpr int kPreviewRows = 20;

// Field indexes into m_fieldMaps: one mapping combo per app field.
enum Field {
  kEnglish = 0,
  kArabic,
  kSource,
  kDialect,
  kWordform,
  kCategory,
  kTags,
  kFieldCount
};

// Each mapping combo's user data holds the source column index, or -1 when
// the field is not imported.
constexpr int kNotImported = -1;

const QStringList kFields = {"English", "Arabic", "Source", "Dialect", "Wordform", "Category", "Tags"};

// Parallel to m_fieldMaps: which Word member each field index writes to.
using WordMember = std::string Word::*;
constexpr std::array<WordMember, kFieldCount> kWordMembers = {
  &Word::english, &Word::arabic, &Word::source,
  &Word::dialect, &Word::wordform, &Word::category, &Word::tags};

void clearLayout(QLayout* layout) {
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (QWidget* w = item->widget()) {
      delete w;
    } else if (QLayout* child = item->layout()) {
      clearLayout(child);
    }
    delete item;
  }
}

// strip byte order marks (e.g. on Windows)
std::string stripBom(std::string line) {
  if (line.size() >= 3 && (unsigned char)line[0] == 0xEF &&
      (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
    line.erase(0, 3);
  }
  return line;
}

}  // namespace

ImportHelper::ImportHelper(const EntryDefaults* defaults, QWidget* parent)
    : QDialog(parent), m_defaults(defaults) {
  setWindowTitle("Import");
  setStyleSheet("background-color: rgb(40, 40, 40);");
  resize(900, 720);

  QVBoxLayout* stack = new QVBoxLayout(this);

  // File picker row.
  QHBoxLayout* fileRow = new QHBoxLayout();
  m_pathInput = new QLineEdit();
  m_pathInput->setReadOnly(true);
  Theme::lineEdit(m_pathInput, "No file selected");

  QPushButton* browse = new QPushButton("Browse");
  Theme::button(browse);

  connect(browse, &QPushButton::clicked, this, [this]() {
    QFileDialog dialog(this, "Import");
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Delimited files (*.csv *.tsv *.txt);;All files (*)");
    Theme::fileDialog(&dialog);

    if (dialog.exec() == QDialog::Accepted) {
      const QStringList selected = dialog.selectedFiles();
      if (!selected.isEmpty()) {
        m_path = selected.first().toStdString();
        m_pathInput->setText(selected.first());
        rebuild(true);
      }
    }
  });

  fileRow->addWidget(m_pathInput, 1);
  fileRow->addWidget(browse);
  stack->addLayout(fileRow);

  // Options row: delimiter + header toggle.
  QHBoxLayout* optionsRow = new QHBoxLayout();
  QLabel* delimiterLabel = new QLabel("Delimiter");
  Theme::label(delimiterLabel);

  m_delimiter = new QComboBox();
  Theme::combo(m_delimiter);
  m_delimiter->addItem("Comma (,)", QString(","));
  m_delimiter->addItem("Tab", QString("\t"));
  m_delimiter->addItem("Semicolon (;)", QString(";"));
  m_delimiter->addItem("Pipe (|)", QString("|"));

  m_hasHeader = new QCheckBox("First row contains field names");
  Theme::checkbox(m_hasHeader);
  {
    QSettings settings("config.ini", QSettings::IniFormat);
    m_hasHeader->setChecked(settings.value("import/firstRowHeader", true).toBool());
  }

  optionsRow->addWidget(delimiterLabel);
  optionsRow->addWidget(m_delimiter);
  optionsRow->addSpacing(20);
  optionsRow->addWidget(m_hasHeader);
  optionsRow->addStretch();
  stack->addLayout(optionsRow);

  // Field mapping rows are rebuilt per file.
  QLabel* mappingLabel = new QLabel("Map fields");
  Theme::label(mappingLabel);
  stack->addWidget(mappingLabel);

  m_mappingLayout = new QGridLayout();
  m_mappingLayout->setHorizontalSpacing(12);
  stack->addLayout(m_mappingLayout);

  // Preview table.
  m_preview = new QTableWidget(0, 0, this);
  Theme::table(m_preview);
  m_preview->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_preview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  stack->addWidget(m_preview, 1);

  // Action buttons.
  QHBoxLayout* buttonRow = new QHBoxLayout();

  m_applyDefaults = new QCheckBox("Apply default source & dialect");
  Theme::checkbox(m_applyDefaults);

  QPushButton* importButton = new QPushButton("Import");
  Theme::button(importButton);
  QPushButton* cancelButton = new QPushButton("Cancel");
  Theme::button(cancelButton);

  buttonRow->addWidget(m_applyDefaults);
  buttonRow->addStretch();
  buttonRow->addWidget(importButton);
  buttonRow->addWidget(cancelButton);
  stack->addLayout(buttonRow);

  connect(m_delimiter, &QComboBox::currentIndexChanged, this, [this](int) { rebuild(false); });
  connect(m_hasHeader, &QCheckBox::toggled, this, [this](bool checked) {
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("import/firstRowHeader", checked);
    rebuild(false);
  });
  connect(importButton, &QPushButton::clicked, this, &ImportHelper::doImport);
  connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ImportHelper::rebuild(bool autoDetect) {
  // Remember the user's current mapping so toggling the header or changing
  // the delimiter doesn't throw away manual selections.
  std::vector<int> previousColumns;
  previousColumns.reserve(m_fieldMaps.size());
  for (QComboBox* combo : m_fieldMaps) {
    previousColumns.push_back(combo->currentData().toInt());
  }

  clearLayout(m_mappingLayout);
  m_fieldMaps.clear();
  m_preview->clear();
  m_preview->setRowCount(0);
  m_preview->setColumnCount(0);

  if (m_path.empty()) return;

  const std::vector<QStringList> rows = readRows(kPreviewRows);
  if (rows.empty()) return;

  int cols = 0;
  for (const QStringList& r : rows) {
    cols = std::max(cols, (int)r.size());
  }
  if (cols == 0) return;

  const bool header = m_hasHeader->isChecked();

  // One mapping row per app field, laid out three-across to stay compact.
  constexpr int kFieldsPerRow = 3;
  for (int f = 0; f < kFieldCount; ++f) {
    QHBoxLayout* cell = new QHBoxLayout();
    QLabel* label = new QLabel(kFields[f]);
    Theme::label(label);

    QComboBox* combo = new QComboBox();
    Theme::combo(combo);
    combo->setFixedHeight(36);
    combo->addItem("(Not imported)", kNotImported);
    for (int c = 0; c < cols; ++c) {
      combo->addItem(QString("Column %1").arg(c + 1), c);
    }

    int selectedColumn = kNotImported;
    if (autoDetect) {
      // Positional default: column 1 -> English, column 2 -> Arabic, etc.
      if (f < cols) {
        selectedColumn = f;
      }
    } else if (!previousColumns.empty()) {
      const int saved = previousColumns[f];
      if (saved >= 0 && saved < cols) {
        selectedColumn = saved;
      }
    }
    // Item 0 is "(Not imported)", so column c lives at index c + 1.
    combo->setCurrentIndex(selectedColumn + 1);

    cell->addWidget(label);
    cell->addWidget(combo, 1);
    m_mappingLayout->addLayout(cell, f / kFieldsPerRow, f % kFieldsPerRow);
    m_fieldMaps.push_back(combo);
  }

  // Preview table. The first row becomes column headers when it is a header.
  m_preview->setColumnCount(cols);
  QStringList headers;
  for (int c = 0; c < cols; ++c) {
    const QString value = header ? rows[0].value(c) : QString();
    headers << (value.isEmpty() ? QString("Column %1").arg(c + 1) : value);
  }
  m_preview->setHorizontalHeaderLabels(headers);

  const int dataStart = header ? 1 : 0;
  m_preview->setRowCount(std::max(0, (int)rows.size() - dataStart));
  for (int r = dataStart; r < (int)rows.size(); ++r) {
    const int tableRow = r - dataStart;
    for (int c = 0; c < cols; ++c) {
      m_preview->setItem(tableRow, c, new QTableWidgetItem(rows[r].value(c)));
    }
  }
}

QStringList ImportHelper::parseLine(const QString& line) const {
  QStringList fields;
  QString current;
  bool insideQuotes = false;

  const QChar delimiter = currentDelimiter();
  const QChar quote = '"';

  for (int i = 0; i < line.size(); ++i) {
    const QChar ch = line.at(i);

    // A delimiter outside quotes ends the current field.
    if (!insideQuotes && ch == delimiter) {
      fields << current.trimmed();
      current.clear();
      continue;
    }

    // Quotes toggle in/out of a quoted field, and "" inside quotes is an
    // escaped quote character.
    if (ch == quote) {
      if (insideQuotes && i + 1 < line.size() && line.at(i + 1) == quote) {
        current += quote;
        ++i;  // skip the second quote of the escaped pair
      } else {
        insideQuotes = !insideQuotes;
      }
      continue;
    }

    current += ch;
  }

  fields << current.trimmed();
  return fields;
}

std::vector<QStringList> ImportHelper::readRows(int maxRows) const {
  std::vector<QStringList> rows;

  std::ifstream in(m_path, std::ios::in);
  std::string line;
  int count = 0;
  while (count < maxRows && std::getline(in, line)) {
    const std::string text = (count == 0) ? stripBom(line) : line;
    rows.push_back(parseLine(QString::fromUtf8(text.c_str())));
    ++count;
  }

  return rows;
}

QChar ImportHelper::currentDelimiter() const {
  return m_delimiter->currentData().toString().at(0);
}

void ImportHelper::doImport() {
  if (m_path.empty() || m_fieldMaps.empty()) return;

  const bool applyDefaults = m_applyDefaults->isChecked();
  const std::string defaultSource = m_defaults->source.toStdString();
  const std::string defaultDialect = m_defaults->dialect.toStdString();

  std::vector<Word> words;

  std::ifstream in(m_path, std::ios::in);
  std::string line;
  int lineNumber = 0;
  while (std::getline(in, line)) {
    if (lineNumber == 0) {
      line = stripBom(line);
      if (m_hasHeader->isChecked()) {
        ++lineNumber;
        continue;
      }
    }

    const QStringList fields = parseLine(QString::fromUtf8(line.c_str()));
    if (fields.isEmpty()) {
      ++lineNumber;
      continue;
    }

    Word w;
    for (int f = 0; f < kFieldCount; ++f) {
      const int col = m_fieldMaps[f]->currentData().toInt();
      if (col < 0 || col >= fields.size()) continue;

      w.*kWordMembers[f] = fields[col].trimmed().toStdString();
    }

    if (applyDefaults) {
      w.source = defaultSource;
      w.dialect = defaultDialect;
    }

    words.push_back(w);
    ++lineNumber;
  }

  emit imported(words);
  accept();
}
