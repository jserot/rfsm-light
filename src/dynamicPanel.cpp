#include "dynamicPanel.h"

#include <QGroupBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QHBoxLayout>

DynamicPanel::DynamicPanel(QString title) : QGroupBox(title)
{
  layout = new QVBoxLayout();
  QHBoxLayout *top_row_layout = new QHBoxLayout();
  top_row_layout->setObjectName("top row");
  add_button = new QPushButton("Add");
  clear_button = new QPushButton("Clear");
  top_row_layout->addWidget(add_button);
  top_row_layout->addWidget(clear_button);
  layout->addLayout(top_row_layout);
  setLayout(layout);
  connect(add_button, &QPushButton::clicked, this, &DynamicPanel::addNewRow);
  connect(clear_button, &QPushButton::clicked, this, &DynamicPanel::clear);
  add_button->setDefault(false);
  clear_button->setDefault(false);
}

void DynamicPanel::addRow(void *row_data) {
  QHBoxLayout* row_layout = new QHBoxLayout();
  int nb_rows = layout->count();
  row_layout->setObjectName("row" + QString::number(nb_rows));
  QString name(QString(tr("#%1:").arg(nb_rows)));
  addRowFields(row_layout, row_data); // DynamicPanel specific items added here 
  QPushButton *delButton = new QPushButton();
  row_layout->addWidget(delButton);
  delButton->setIcon(QIcon(":/images/delete.png"));
  connect(delButton, &QPushButton::clicked, this, &DynamicPanel::deleteRow);
  mButtonToLayoutMap.insert(delButton, row_layout);
  layout->insertLayout(0, row_layout); // Add to top
}

void DynamicPanel::addNewRow()
{
  addRow(nullptr);
}

void DynamicPanel::delete_row(QHBoxLayout *row_layout)
{
  deleteRowFields(row_layout);
  while (row_layout->count() != 0) {
    QLayoutItem* item = row_layout->takeAt(0);
    delete item->widget();
    delete item;
  }
  delete row_layout;
}

void DynamicPanel::deleteRow()
{
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  QHBoxLayout* row_layout = static_cast<QHBoxLayout*>(mButtonToLayoutMap.take(button));
  DynamicPanel::delete_row(row_layout);
}

void DynamicPanel::clear()
{
  while ( layout->count() > 1 ) { // Do not erase last row !
    QHBoxLayout* row = static_cast<QHBoxLayout*>(layout->takeAt(0));
    assert(row);
    qDebug() << "DynamicPanel: deleting row" << row->objectName();
    delete_row(row);
  }
}

DynamicPanel::~DynamicPanel()
{
  // TBC
}
