#include "ImportExport.hpp"

#include <qboxlayout.h>
#include <qpushbutton.h>

#include "../../Theme/Theme.hpp"

ImportExport::ImportExport(QWidget* parent) : QWidget(parent) {
  QHBoxLayout* row = new QHBoxLayout(this);
  row->setContentsMargins(0, 0, 0, 0);
  row->setSpacing(4);

  m_importButton = new QPushButton("Import");
  Theme::button(m_importButton);
  connect(m_importButton, &QPushButton::clicked, this, &ImportExport::importClicked);

  m_exportButton = new QPushButton("Export");
  Theme::button(m_exportButton);
  connect(m_exportButton, &QPushButton::clicked, this, &ImportExport::exportClicked);

  row->addWidget(m_importButton);
  row->addWidget(m_exportButton);
}
