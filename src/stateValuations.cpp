#include "stateValuations.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

StateValuations::StateValuations(QString title, QStringList& valuations) : DynamicPanel(title)
{
  qDebug() << "StateValuations::StateValuations: valuations=" << valuations;
  foreach ( QString v, valuations )
    addRow((void *)(&v));
}

void StateValuations::addRowFields(QHBoxLayout *row_layout, void *row_data)
{
  int nb_rows = row_layout->count();
  QString name(QString(tr("valuation #%1").arg(nb_rows)));
  //QString *v = (QString*)(row_data);
  QLineEdit *valuation = new QLineEdit();
  valuation->setObjectName(name);
  valuation->setPlaceholderText("<output>=<value>");
  valuation->setMinimumSize(120,valuation->minimumHeight());
  valuation->setFrame(true);
  valuation->setText(row_data ? *(QString *)row_data : "");
  valuation->setCursorPosition(0);
  row_layout->addWidget(valuation);
}

void StateValuations::deleteRowFields(QHBoxLayout *row_layout)
{
  Q_UNUSED(row_layout);
  // Nothing
}

QStringList StateValuations::retrieve()
{
  QStringList valuations;
  for ( int i=0; i<layout->count()-1; i++ ) { // Exclude last row, carrying panel buttons
    QHBoxLayout* row = static_cast<QHBoxLayout*>(layout->itemAt(i));
    QLineEdit* ledit = qobject_cast<QLineEdit*>(row->itemAt(0)->widget());
    QString valuation = ledit->text().trimmed();
    valuations << valuation;
    }
  qDebug () << "StateValuationPanel: valuations=" << valuations;
  return valuations;
}

StateValuations::~StateValuations()
{
}
