#include "commandExec.h"
#include <QDebug>

CommandExec::CommandExec()
{
  connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
  connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));
}

bool CommandExec::execute(QString wDir, QString cmd, QStringList args)
{
  qDebug() << "CommandExec::execute: wDir=" << wDir << " cmd=" << cmd << " args=" << args;
  outputs.clear();
  errors.clear();
  proc.setWorkingDirectory(wDir);
  proc.start(cmd,args);
  if ( proc.error() == QProcess::FailedToStart ) {
#if QT_VERSION >= 0x060000
    qDebug() << "CommandExec: failed to start" << Qt::endl;
#else
    qDebug() << "CommandExec: failed to start" << endl;
#endif
    return false;
    }
  // qDebug() << "CommandExec: sync process launched. Waiting for termination";
  bool r = proc.waitForFinished(); 
  QProcess::ExitStatus s = proc.exitStatus();
  int o = proc.exitCode();
  qDebug() << "CommandExec: exit status/code=" << s << "," << o;
  // qDebug() << "CommandExec: stdout=" << outputs;
  // qDebug() << "CommandExec: stderr=" << errors;
  proc.kill();
  proc.close();
  proc.terminate();
  return r && s == QProcess::NormalExit && o == 0;
}

QStringList CommandExec::getOutputs()
{
  return outputs.split("\n", Qt::SkipEmptyParts);
}

QStringList CommandExec::getErrors()
{
  return errors.split("\n", Qt::SkipEmptyParts);
}

void CommandExec::readStdout()
{
  proc.setReadChannel(QProcess::StandardOutput);
  outputs = "";
  while (proc.canReadLine ()) {
    outputs += QString(proc.readLine());
      }
}

void CommandExec::readStderr()
{
  proc.setReadChannel ( QProcess::StandardError );
  errors = "";
  while (proc.canReadLine ()) 
    errors += QString(proc.readLine());
}

CommandExec::~CommandExec()
{
}