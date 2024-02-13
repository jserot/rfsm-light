#include "transitionGuards.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

TransitionGuards::TransitionGuards(QString title, QStringList& guards) : DynamicPanel(title)
{
  foreach ( QString v, guards )
    addRow(v);
}

void TransitionGuards::addRowFields(QHBoxLayout *row_layout, QString& v)
{
  int nb_rows = row_layout->count();
  QString name(QString(tr("guard #%1").arg(nb_rows)));
  QLineEdit *valuation = new QLineEdit();
  valuation->setObjectName(name);
  valuation->setPlaceholderText("<bool expr>");
  valuation->setMinimumSize(120,valuation->minimumHeight());
  valuation->setFrame(true);
  valuation->setText(v);
  valuation->setCursorPosition(0);
  // connect(valuation, &QLineEdit::editingFinished, this, &TransitionGuards::rowEdited);
  row_layout->addWidget(valuation);
}

// void TransitionGuards::rowEdited()
// {
//   QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
//   qDebug() << ledit->objectName() << "=" << ledit->text();
// }

void TransitionGuards::deleteRowFields(QHBoxLayout *row_layout)
{
  Q_UNUSED(row_layout);
  // Nothing
}

QStringList TransitionGuards::retrieve()
{
  QStringList guards;
  for ( int i=0; i<layout->count()-1; i++ ) { // Exclude last row, carrying panel buttons
    QHBoxLayout* row = static_cast<QHBoxLayout*>(layout->itemAt(i));
    QLineEdit* ledit = qobject_cast<QLineEdit*>(row->itemAt(0)->widget());
    QString guard = ledit->text().trimmed();
    guards << guard;
    }
  qDebug () << "TransitionGuards: guards=" << guards;
  return guards;
}

TransitionGuards::~TransitionGuards()
{
}
