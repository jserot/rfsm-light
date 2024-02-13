#include "stateValuations.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

StateValuations::StateValuations(QString title, QStringList& valuations) : DynamicPanel(title)
{
  foreach ( QString v, valuations )
    addRow(v);
}

void StateValuations::addRowFields(QHBoxLayout *row_layout, QString& v)
{
  int nb_rows = row_layout->count();
  QString name(QString(tr("valuation #%1").arg(nb_rows)));
  QLineEdit *valuation = new QLineEdit();
  valuation->setObjectName(name);
  valuation->setPlaceholderText("<output>=<value>");
  valuation->setMinimumSize(120,valuation->minimumHeight());
  valuation->setFrame(true);
  valuation->setText(v);
  valuation->setCursorPosition(0);
  // connect(valuation, &QLineEdit::editingFinished, this, &StateValuations::rowEdited);
  row_layout->addWidget(valuation);
}

// void StateValuations::rowEdited()
// {
//   QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
//   qDebug() << ledit->objectName() << "=" << ledit->text();
// }

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
