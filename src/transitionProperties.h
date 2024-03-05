#pragma once

#include <QDialog>

class Transition;
class State;
class Model;
class Compiler;
class QComboBox;
class TransitionGuards;
class TransitionActions;

class TransitionProperties : public QDialog
{
  Q_OBJECT

public:
  TransitionProperties(Transition *transition, Model *model, bool isInitial, Compiler *compiler, QWidget *parent);
  ~TransitionProperties();

private:
  Transition *transition;
  Model *model;
  Compiler *compiler;

  QComboBox* start_state_field;
  QComboBox* end_state_field;
  QComboBox* event_field;
  TransitionGuards *guards_panel;
  TransitionActions *actions_panel;
    
protected slots:
  void accept();
  void cancel();
};
