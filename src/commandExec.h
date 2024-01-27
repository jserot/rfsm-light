#ifndef _COMMANDEXEC_H
#define _COMMANDEXEC_H

#include <QString>
#include <QProcess>

class CommandExec : QObject
{
  Q_OBJECT
public:
  CommandExec();
  ~CommandExec();

  bool execute(QString wDir, QString cmd, QStringList args);
  QStringList getOutputs();
  QStringList getErrors();
private slots:
  void readStdout();
  void readStderr();
private:
  QProcess proc; 
  QString outputs;
  QString errors;
};

#endif
