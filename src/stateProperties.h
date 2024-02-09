#pragma once

#include <QDialog>
#include <QRegularExpression>

class State;
class QLineEdit;
class StateValuations;

class StateProperties : public QDialog
{
  Q_OBJECT

  static const QRegularExpression re_uid;

public:
  StateProperties(State* state, QWidget *parent);
  ~StateProperties();

private:
  State *state;
  QLineEdit* state_name_field;
  StateValuations *valuations_panel;
    
protected slots:
  void accept();
  void cancel();
};
