#pragma once

#include <QDialog>
//#include <QRegularExpression>

class Transition;
class State;
class Model;
class QComboBox;
class TransitionGuards;
class TransitionActions;
class SyntaxChecker;

class TransitionProperties : public QDialog
{
  Q_OBJECT

  // static const QRegularExpression re_uid;

public:
  TransitionProperties(Transition *transition, Model *model, bool isInitial, SyntaxChecker *syntaxChecker, QWidget *parent);
  ~TransitionProperties();

private:
  Transition *transition;
  Model *model;
  SyntaxChecker *syntaxChecker;

  QComboBox* start_state_field;
  QComboBox* end_state_field;
  QComboBox* event_field;
  TransitionGuards *guards_panel;
  TransitionActions *actions_panel;
    
protected slots:
  void accept();
  void cancel();
};
