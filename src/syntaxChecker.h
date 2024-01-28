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

#ifndef _SYNTAX_CHECKER
#define _SYNTAX_CHECKER

#include "commandExec.h"

class SyntaxChecker
{
 public:
  SyntaxChecker(CommandExec *executor, QString syntaxChecker);
  QString check_guard(QStringList inps, QStringList outps, QStringList vars, QString input);
  QString check_action(QStringList inps, QStringList outps, QStringList vars, QString input);
private:
  CommandExec *executor;
  QString checker;
  bool check(QStringList args, QString input, QStringList& lhs_vars, QStringList& rhs_vars);
};
#endif
