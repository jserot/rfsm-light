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

#include "syntaxChecker.h"
#include "commandExec.h"
#include <QtDebug>

SyntaxChecker::SyntaxChecker(CommandExec *executor, QString checker)
{
  this->executor = executor;
  this->checker = checker;
}

bool SyntaxChecker::check(QStringList args, QString input, QStringList& lhs_vars, QStringList& rhs_vars)
{
  QString wDir = ".";
  args.append(input);
  assert(executor);
  if ( ! executor->execute(wDir, checker, args) ) return false;
  QStringList outps = executor->getOutputs();
  qDebug() << "Checker output=" << outps; 
  // TODO: use regexp to parse output
  if ( outps.length() < 1 ) return false;
  QStringList res = outps.at(0).split(";", Qt::KeepEmptyParts);
  if ( res.length() != 3 ) return false;
  if ( res.at(0) != "1" ) return false;
  lhs_vars = res.at(1).split(",",Qt::SkipEmptyParts);
  rhs_vars = res.at(2).split(",",Qt::SkipEmptyParts);
  qDebug() << "Guard/action validated. LHS vars=" << lhs_vars << "RHS vars" << rhs_vars;
  return true;
}

QString SyntaxChecker::check_guard(QStringList inps, QStringList outps, QStringList vars, QString guard)
{
  Q_UNUSED(outps);
  QStringList lhs_vars, rhs_vars;
  if ( ! check(QStringList("-guard"), guard, lhs_vars, rhs_vars) ) return "Invalid guard: \"" + guard + "\""; 
  foreach ( QString rhs, rhs_vars ) {
    if ( ! inps.contains(rhs) || ! vars.contains(rhs) )
      return "The symbol \"" + rhs + "\" occuring in the guard \"" + guard + "\" is neither an input nor a variable";
    }
  return "Ok";
}

QString SyntaxChecker::check_action(QStringList inps, QStringList outps, QStringList vars, QString action)
{
  QStringList lhs_vars, rhs_vars;
  if ( ! check(QStringList("-action"), action, lhs_vars, rhs_vars) ) return "Invalid action: \"" + action + "\""; 
  foreach ( QString rhs, rhs_vars ) {
    if ( ! inps.contains(rhs) || ! vars.contains(rhs) )
      return "The symbol \"" + rhs + "\" occuring in the action \"" + action + "\" is neither an input nor a variable";
    }
  foreach ( QString lhs, lhs_vars ) {
    if ( ! outps.contains(lhs) || ! vars.contains(lhs) ) 
      return "The symbol \"" + lhs + "\" modified in the action \"" + action + "\" is neither an output nor a variable";
    }
  return "Ok";
}
