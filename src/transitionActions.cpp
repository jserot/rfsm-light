#include "transitionActions.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

TransitionActions::TransitionActions(QString title, QStringList& actions) : DynamicPanel(title)
{
  foreach ( QString v, actions )
    addRow(v);
}

void TransitionActions::addRowFields(QHBoxLayout *row_layout, QString& v)
{
  int nb_rows = row_layout->count();
  QString name(QString(tr("action #%1").arg(nb_rows)));
  QLineEdit *valuation = new QLineEdit();
  valuation->setObjectName(name);
  valuation->setPlaceholderText("<o/v>:=<expr> | o");
  valuation->setMinimumSize(120,valuation->minimumHeight());
  valuation->setFrame(true);
  valuation->setText(v);
  valuation->setCursorPosition(0);
  // connect(valuation, &QLineEdit::editingFinished, this, &TransitionActions::rowEdited);
  row_layout->addWidget(valuation);
}

// void TransitionActions::rowEdited()
// {
//   QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
//   qDebug() << ledit->objectName() << "=" << ledit->text();
// }

void TransitionActions::deleteRowFields(QHBoxLayout *row_layout)
{
  Q_UNUSED(row_layout);
  // Nothing
}

QStringList TransitionActions::retrieve()
{
  QStringList actions;
  for ( int i=0; i<layout->count()-1; i++ ) { // Exclude last row, carrying panel buttons
    QHBoxLayout* row = static_cast<QHBoxLayout*>(layout->itemAt(i));
    QLineEdit* ledit = qobject_cast<QLineEdit*>(row->itemAt(0)->widget());
    QString action = ledit->text().trimmed();
    actions << action;
    }
  qDebug () << "TransitionActions: actions=" << actions;
  return actions;
}

TransitionActions::~TransitionActions()
{
}
