#include <qboxlayout.h>

#include <QApplication>
#include <QFont>
#include <QMainWindow>
#include <QSplitter>
#include <QWidget>

#include "UI/TitleBar/TitleBar.hpp"
#include "UI/Editor/Editor.hpp"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  app.setFont(QFont("Noto Sans", 14));

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
  rightPanelLayout->addWidget(new Editor());
  rightPanelLayout->addStretch();

  split->addWidget(leftPanel);
  split->addWidget(rightPanel);
  split->setSizes({300, 900});

  split->setStretchFactor(0, 1);
  split->setStretchFactor(1, 3);

  window.setCentralWidget(split);
  window.show();
  return app.exec();
}
