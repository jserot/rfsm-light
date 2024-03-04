#include "stateProperties.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QFile>

#include "state.h"
#include "model.h"
#include "stateValuations.h"
//#include "syntaxChecker.h"
#include "commandExec.h"

const QRegularExpression StateProperties::re_uid("[A-Z][A-Za-z0-9_]*");

const QString StateProperties::tmpFragmentFileName = "_rfsm_fragment.fsp";

StateProperties::StateProperties(State *state, Model *model, // SyntaxChecker *syntaxChecker,
                                 QString compiler, CommandExec *executor, QWidget *parent)
  : QDialog(parent)
{
  QString id = state->getId();
  setWindowTitle("State " + id);
  QVBoxLayout *layout = new QVBoxLayout();

  QHBoxLayout *row_layout1 = new QHBoxLayout();
  QLabel* name_label = new QLabel("Name");
  state_name_field = new QLineEdit();
  // state_name_field->setPlaceholderText("State identifier");
  state_name_field->setText(id);
  QRegularExpressionValidator *state_name_validator = new QRegularExpressionValidator(re_uid);
  state_name_field->setValidator(state_name_validator);
  row_layout1->addWidget(name_label);
  row_layout1->addWidget(state_name_field);
  layout->addLayout(row_layout1);

  QStringList valuations = state->getAttrs();
  valuations_panel = new StateValuations("Valuations", valuations);
  layout->addWidget(valuations_panel);
  
  QHBoxLayout *row_layout2 = new QHBoxLayout();
  QPushButton *cancel_button = new QPushButton("Cancel");
  QPushButton *accept_button = new QPushButton("Done");
  cancel_button->setDefault(false);
  accept_button->setDefault(true);
  row_layout2->addWidget(cancel_button);
  row_layout2->addWidget(accept_button);
  layout->addLayout(row_layout2);

  setLayout(layout);

  // connect(cancel_button, &QPushButton::clicked, this, &QDialog::reject);
  // connect(accept_button, &QPushButton::clicked, this, &QDialog::accept);
  connect(cancel_button, &QPushButton::clicked, this, &StateProperties::cancel);
  connect(accept_button, &QPushButton::clicked, this, &StateProperties::accept);

  this->state = state;
  this->model = model;
  //this->syntaxChecker = syntaxChecker;
  this->executor = executor;
  this->compiler = compiler;

  setModal(true);
}

void StateProperties::accept()
{
  QString id = state_name_field->text();
  state->setId(id);
  QStringList valuations = valuations_panel->retrieve();
  bool ok = true;
  QSet<QString> lhss;
  QStringList outps = model->getOutpNonEvents();
  qDebug() << "Syntax checking valuations" << valuations << "with outputs=" << outps;
  foreach ( QString valuation, valuations) {
    // SyntaxCheckerResult r = syntaxChecker->check_valuation(model->getOutpNonEvents(), valuation);
    // if ( ! r.ok ) {
    //   QMessageBox::warning(this, "Error", r.msg);
    //   ok = false;
    //   }
    // foreach ( QString o, r.lhs_vars ) {
    //   if ( lhss.contains(o) ) { // Assignation of an already assigned output
    //     QMessageBox::warning(this, "Error", "The output " + o + " is assigned several times by the valuations");
    //     ok = false;
    //     }
    //   else
    //     lhss.insert(o);
    //   }
    if ( ! check_fragment(valuation) ) {
      QStringList compileErrors = executor->getErrors();
      QMessageBox::warning(this, "", "Illegal state valuation: \"" + valuation + "\"\n" + compileErrors.join("\n"));
      ok = false;
      }
    }
  if ( ok ) {
    state->setAttrs(valuations);
    qDebug() << "StateProperties::accept(ok)";
    QDialog::done(Accepted);
    }
  else {
    qDebug() << "StateProperties::accept(nok)";
    // Do not accept and leave dialog opened
  }
}

bool StateProperties::build_fragment_file(QString frag)
{
  // Build fragment file
  QFile file(tmpFragmentFileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  if ( file.error() != QFile::NoError ) {
    QMessageBox::warning(this, "","Cannot open file " + file.fileName());
    return false;
    }
  QTextStream os(&file);
  os << "-- context" << Qt::endl;
  // TBW
  os << "-- fragments" << Qt::endl;
  os << "sval " << frag << ";" << Qt::endl;
  file.close();
  qDebug() << "Created fragment file" << file.fileName();
  return true;
}

bool StateProperties::check_fragment(QString frag)
{
  if ( ! build_fragment_file(frag) ) return false;
  QStringList args;
  return executor->execute(".", compiler, args << "-check_fragment" << tmpFragmentFileName);
}

void StateProperties::cancel()
{
  qDebug() << "StateProperties::cancel";
  QDialog::done(Rejected);
}

StateProperties::~StateProperties()
{
}
