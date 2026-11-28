#include <qboxlayout.h>
#include <qpushbutton.h>

#include <QApplication>
#include <QFont>
#include <QMainWindow>
#include <QSettings>
#include <QSplitter>
#include <QTabWidget>
#include <QWidget>

#include "../structs/EntryDefaults.hpp"
#include "./Plumbing/AppPaths.hpp"
#include "./Plumbing/Database/Database.hpp"
#include "UI/Dictionary/DictionaryPanel.hpp"
#include "UI/Editor/EditPopup/MultiSelectComboBox/MultiSelectComboBox.hpp"
#include "UI/Editor/Editor.hpp"
#include "UI/Editor/ImportExport/ExportHelper/ExportHelper.hpp"
#include "UI/Editor/ImportExport/ImportExport.hpp"
#include "UI/Editor/ImportExport/ImportHelper/ImportHelper.hpp"
#include "UI/GatewayClient/GatewayClient.hpp"
#include "UI/Settings/Settings.hpp"
#include "UI/Theme/Theme.hpp"
#include "UI/TitleBar/TitleBar.hpp"

EntryDefaults loadDefaults();

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("Issa-Language-Tools");
  QCoreApplication::setApplicationName("Language-Workspace");
  app.setFont(QFont("Noto Sans", 14));

  // Run Migrations:
  Database db;

  EntryDefaults entryDefault = loadDefaults();

  QMainWindow window;
  window.resize(1280, 720);

  QSplitter* split = new QSplitter(Qt::Horizontal);

  QWidget* leftPanel = new QWidget();
  leftPanel->setStyleSheet("background-color: rgb(48, 48, 48);");
  leftPanel->setMinimumWidth(200);
  leftPanel->setMaximumWidth(500);
  QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
  leftLayout->setContentsMargins(0, 0, 0, 0);

  DictionaryPanel* dictionaryPanel = new DictionaryPanel();
  leftLayout->addWidget(dictionaryPanel);

  QWidget* rightPanel = new QWidget();
  rightPanel->setStyleSheet("background-color: rgb(55, 55, 55);");
  QVBoxLayout* rightPanelLayout = new QVBoxLayout(rightPanel);

  rightPanelLayout->addWidget(new TitleBar("Language Workspace"));
  QTabWidget* tabs = new QTabWidget();
  GatewayClient* gateway = new GatewayClient(&window);
  Editor* editor = new Editor(&db, &entryDefault, gateway);
  QObject::connect(gateway, &GatewayClient::resultsReady,
                   dictionaryPanel, &DictionaryPanel::setResults);
  Settings* settings = new Settings(&entryDefault);
  ImportExport* buttons = new ImportExport();
  tabs->addTab(editor, "Editor");
  tabs->addTab(settings, "Settings");
  tabs->setCornerWidget(buttons, Qt::TopRightCorner);
  QObject::connect(buttons, &ImportExport::importClicked, &window, [&window, editor, &entryDefault]() {
    ImportHelper popup(&entryDefault, &window);
    QObject::connect(&popup, &ImportHelper::imported, editor,
                     [editor](const std::vector<Word>& words) { editor->addImportedWords(words); });
    popup.exec();
  });
  QObject::connect(buttons, &ImportExport::exportClicked, &window, [&window, editor, &db]() {
    ExportHelper popup(&db, &window);
    QObject::connect(&popup, &ExportHelper::exported, editor,
                     [editor](int count) { editor->refreshWords(); });
    popup.exec();
  });
  Theme::tabs(tabs);

  QObject::connect(settings, &Settings::defaultsChanged, tabs, [tabs, editor]() {
    tabs->setCurrentWidget(editor);
  });

  rightPanelLayout->addWidget(tabs, 1);

  split->addWidget(leftPanel);
  split->addWidget(rightPanel);
  split->setSizes({300, 900});

  split->setStretchFactor(0, 1);
  split->setStretchFactor(1, 3);

  window.setCentralWidget(split);
  window.show();

  return app.exec();
}

EntryDefaults loadDefaults() {
  QSettings settings(AppPaths::appDataDir() + "/config.ini", QSettings::IniFormat);

  EntryDefaults defaults;
  defaults.source = settings.value("defaults/source", "").toString();
  defaults.dialect = settings.value("defaults/dialect", "").toString();
  defaults.wordforms = settings.value("defaults/wordforms", "").toStringList();
  defaults.tags = settings.value("defaults/tags", "").toStringList();

  return defaults;
}
