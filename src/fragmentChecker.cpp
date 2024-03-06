/***********************************************************************/
/*                                                                     */
/*       This file is part of the RFSM Light software package          */
/*                                                                     */
/*  Copyright (c) 2019-present, Jocelyn SEROT (jocelyn.serot@uca.fr)   */
/*                       All rights reserved.                          */
/*                                                                     */
/*    This source code is licensed under the license found in the      */
/*      LICENSE file in the root directory of this source tree.        */
/*                                                                     */
/***********************************************************************/

#include "fragmentChecker.h"
#include "model.h"
#include "compiler.h"
#include <QFile>
#include <QMessageBox>
#include <QtDebug>

const QString FragmentChecker::tmpFileName = "_rfsm_fragment.fsp";

FragmentChecker::FragmentChecker(Compiler *compiler, Model *model, QWidget *parent)
{
  this->parent = parent;
  this->compiler = compiler;
  this->model = model;
}

bool FragmentChecker::check(QString kind, QString frag)
{
  // Build fragment file
  QFile file(tmpFileName);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  if ( file.error() != QFile::NoError ) {
    QMessageBox::warning(parent, "", "Fragment checker cannot open file " + file.fileName());
    return false;
    }
  QTextStream os(&file);
  os << "-- context" << Qt::endl;
  foreach ( Iov* iov, model->getIos() ) {
    os << Iov::stringOfKind(iov->kind) << " " << iov->name << ": " << Iov::stringOfType(iov->type) << ";" << Qt::endl;
    }
  os << "-- fragment" << Qt::endl;
  os << kind<< " " << frag << ";" << Qt::endl;
  file.close();
  qDebug() << "Created fragment file" << file.fileName();
  QStringList args = { "-check_fragment" };
  // Call compiler
  return compiler->run(tmpFileName, args, ".");
}

bool FragmentChecker::check_state_valuation(QString valuation)
{
  return check("sval", valuation);
}

bool FragmentChecker::check_guard(QString guard)
{
  return check("guard", guard);
}

bool FragmentChecker::check_action(QString action)
{
  return check("action", action);
}

QStringList FragmentChecker::getErrors() 
{
  return compiler->getErrors();
}
  
