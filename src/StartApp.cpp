#include <qboxlayout.h>

#include <QApplication>
#include <QFont>
#include <QMainWindow>
#include <QSettings>
#include <QSplitter>
#include <QTabWidget>
#include <QWidget>

#include "../structs/EntryDefaults.hpp"
#include "./Plumbing/Database/Database.hpp"
#include "UI/Editor/Editor.hpp"
#include "UI/Settings/Settings.hpp"
#include "UI/Theme/Theme.hpp"
#include "UI/TitleBar/TitleBar.hpp"

EntryDefaults loadDefaults();

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
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

  QWidget* rightPanel = new QWidget();
  rightPanel->setStyleSheet("background-color: rgb(55, 55, 55);");
  QVBoxLayout* rightPanelLayout = new QVBoxLayout(rightPanel);

  rightPanelLayout->addWidget(new TitleBar("Language Workspace"));
  QTabWidget* tabs = new QTabWidget();
  Editor* editor = new Editor(&entryDefault);
  Settings* settings = new Settings(&entryDefault);
  tabs->addTab(editor, "Editor");
  tabs->addTab(settings, "Settings");
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
  QSettings settings("config.ini", QSettings::IniFormat);

  EntryDefaults defaults;
  defaults.source = settings.value("defaults/source", "").toString();
  defaults.dialect = settings.value("defaults/dialect", "").toString();
  defaults.wordforms = settings.value("defaults/wordforms", "").toStringList();

  return defaults;
}
