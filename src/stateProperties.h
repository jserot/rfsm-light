#pragma once

#include <QDialog>
#include <QRegularExpression>

class State;
class Model;
class QLineEdit;
class StateValuations;
//class SyntaxChecker;
class CommandExec;

class StateProperties : public QDialog
{
  Q_OBJECT

  static const QRegularExpression re_uid;
  static const QString tmpFragmentFileName;

public:
  StateProperties(State *state, Model *model, // SyntaxChecker *syntaxChecker,
                                                   QString compiler, CommandExec *executor, QWidget *parent);
  ~StateProperties();

private:
  State *state;
  Model *model;
  //SyntaxChecker *syntaxChecker;
  CommandExec *executor;
  QString compiler;
  bool build_fragment_file(QString frag);
  bool check_fragment(QString frag);

  QLineEdit* state_name_field;
  StateValuations *valuations_panel;
    
protected slots:
  void accept();
  void cancel();
};
