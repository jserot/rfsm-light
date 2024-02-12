#include "transitionProperties.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
//#include <QRegularExpressionValidator>

#include "transition.h"
#include "state.h"
#include "model.h"
#include "transitionGuards.h"
#include "transitionActions.h"
#include "syntaxChecker.h"

// const QRegularExpression TransitionProperties::re_uid("[A-Z][A-Za-z0-9_]*");

TransitionProperties::TransitionProperties(
  Transition *transition, Model *model, bool isInitial, SyntaxChecker *syntaxChecker, QWidget *parent)
  : QDialog(parent)
{
  setWindowTitle("Transition");

  QVBoxLayout *layout = new QVBoxLayout();

  QGridLayout* layout1 = new QGridLayout();

  QLabel *start_state_label = new QLabel("Start State");
  start_state_field = new QComboBox();
  layout1->addWidget(start_state_label, 0, 0, 1, 3);
  layout1->addWidget(start_state_field, 1, 0, 1, 3);

  QLabel *end_state_label = new QLabel("End State");
  end_state_field = new QComboBox();
  layout1->addWidget(end_state_label, 0, 3, 1, 3);
  layout1->addWidget(end_state_field, 1, 3, 1, 3);

  QLabel *event_label = new QLabel("Event");
  event_field = new QComboBox();
  layout1->addWidget(event_label, 2, 0, 1, 2);
  layout1->addWidget(event_field, 2, 2, 1, 4);

  layout->addLayout(layout1);

  QStringList guards = transition->getGuards();
  guards_panel = new TransitionGuards("Guards", guards);
  layout->addWidget(guards_panel);
  
  QStringList actions = transition->getActions();
  actions_panel = new TransitionActions("Actions", actions);
  layout->addWidget(actions_panel);
  
  QHBoxLayout *layout2 = new QHBoxLayout();
  QPushButton *cancel_button = new QPushButton("Cancel");
  QPushButton *accept_button = new QPushButton("Done");
  layout2->addWidget(cancel_button);
  layout2->addWidget(accept_button);
  layout->addLayout(layout2);
  cancel_button->setDefault(false);
  accept_button->setDefault(true);

  if ( isInitial ) {
    start_state_label->hide();
    start_state_field->hide();
    event_label->hide();
    event_field->hide();
    guards_panel->hide();
    }

  // Fill fields
  start_state_field->clear();
  end_state_field->clear();
  if ( isInitial ) {
    foreach ( State* state, model->states() ) {
      if ( ! state->isPseudo() ) {
          QString id = state->getId();
          end_state_field->addItem(id, QVariant(id));
          if ( transition->getDstState()->getId() == id ) 
            end_state_field->setCurrentIndex(end_state_field->count()-1);
          }
        }
      }
  else {
    foreach ( State* state, model->states() ) {
      if ( ! state->isPseudo() ) {
        QString id = state->getId();
        start_state_field->addItem(id, QVariant(id));
        end_state_field->addItem(id, QVariant(id));
        if ( transition->getSrcState()->getId() == id )
          start_state_field->setCurrentIndex(start_state_field->count()-1);
        if ( transition->getDstState()->getId() == id ) 
          end_state_field->setCurrentIndex(end_state_field->count()-1);
      }
    }
  }
  event_field->clear();
  QStringList inpEvents = model->getInpEvents();
  for ( auto ev: inpEvents ) 
    event_field->addItem(ev, QVariant(ev));
  QString event = transition->getEvent();
  if ( event == "" ) {
    event_field->setCurrentIndex(0);
    transition->setEvent(event);
    }
  else {
    if ( inpEvents.contains(event) ) 
      event_field->setCurrentText(event);
    else
      QMessageBox::warning( this, "Error", "The triggering event for this transition is no longer listed in the model inputs");
      }

  setLayout(layout);

  connect(cancel_button, &QPushButton::clicked, this, &TransitionProperties::cancel);
  connect(accept_button, &QPushButton::clicked, this, &TransitionProperties::accept);

  this->transition = transition;
  this->model = model;
  this->syntaxChecker = syntaxChecker;
}

void TransitionProperties::accept()
{
  State *srcState, *dstState;
  QString event;
  QStringList guards;
  QStringList actions;
  
  bool isInitial = transition->isInitial();
  
  if ( ! isInitial ) {
    srcState = model->getState(start_state_field->currentText());
    assert(srcState);
    }

  dstState = model->getState(end_state_field->currentText());
  assert(dstState);

  if ( ! isInitial ) 
    event = event_field->currentText();

  bool guards_ok = true;
  if ( ! isInitial ) {
    guards = guards_panel->retrieve();
    foreach ( QString guard, guards) {
      qDebug() << "Syntax checking guard" << guard << "with NEInputs=" << model->getInpNonEvents() << "and vars=" << model->getVars();
      QString msg = syntaxChecker->check_guard(model->getInpNonEvents(), model->getOutputs(), model->getVars(), guard);
      if ( msg != "Ok" ) {
        QMessageBox::warning(this, "Error", msg);
        guards_ok = false;
        }
      }
  // if ( guards.length() >= 2 ) {
  //   for ( int i = 0; i<guards.length(); i++ ) {
  //     QString guard = guards.at(i);
  //     guards.replace(i, "(" + guard + ")");
  //   }
  // }
    }

  bool actions_ok = true;
  actions = actions_panel->retrieve();
  foreach ( QString action, actions) {
    qDebug() << "Syntax checking action" << action << "with NEInputs=" << model->getInpNonEvents() << "and vars=" << model->getVars();
    QString msg = syntaxChecker->check_action(model->getInpNonEvents(), model->getOutputs(), model->getVars(), action);
    if ( msg != "Ok" ) {
      QMessageBox::warning(this, "Error", msg);
      actions_ok = false;
      }
    }

  if ( guards_ok && actions_ok ) {
    transition->setDstState(dstState);
    transition->setActions(actions);
    if ( ! isInitial ) {
      transition->setSrcState(srcState);
      transition->setEvent(event);
      transition->setGuards(guards);
    }
    QDialog::accept();
    }
  else {
    // Do not accept and leave dialog opened
  }
}

void TransitionProperties::cancel()
{
  QDialog::reject();
}

TransitionProperties::~TransitionProperties()
{
}
