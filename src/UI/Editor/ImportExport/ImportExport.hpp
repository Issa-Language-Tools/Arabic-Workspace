#pragma once

#include <QWidget>

class QPushButton;


class ImportExport : public QWidget {
  Q_OBJECT
public:
  explicit ImportExport(QWidget* parent = nullptr);

signals:
  void importClicked();
  void exportClicked();

private:
  QPushButton* m_importButton;
  QPushButton* m_exportButton;
};
