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
#include <QTemporaryFile>
#include <QMessageBox>
#include <QtDebug>
#include "qt_compat.h"

FragmentChecker::FragmentChecker(Compiler *compiler, Model *model, QWidget *parent)
{
  this->parent = parent;
  this->compiler = compiler;
  this->model = model;
}

bool FragmentChecker::check(QString kind, QString frag)
{
  // Build fragment file
  QTemporaryFile file;
  if ( ! file.open() ) return false;
  QString fname = file.fileName();
  QTextStream os(&file);
  os << "-- context" << QT_ENDL;
  foreach ( Iov* iov, model->getIos() ) {
    os << Iov::stringOfKind(iov->kind) << " " << iov->name << ": " << Iov::stringOfType(iov->type) << ";" << QT_ENDL;
  }
  os << "-- fragment" << QT_ENDL;
  os << kind<< " " << frag << ";" << QT_ENDL;
  file.close();
  QStringList args = { "-check_fragment" };
  // Call compiler
  return compiler->run(fname, args, ".");
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
  
