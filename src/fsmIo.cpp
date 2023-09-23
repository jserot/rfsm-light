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

#include "fsmIo.h"

FsmIo::IoKind FsmIo::ioKindOfString(QString s)
{
  if ( s == "in" ) return FsmIo::In;
  else if ( s == "out" ) return FsmIo::Out;
  else if ( s == "var" ) return FsmIo::Var;
  else Q_ASSERT(false);
}

FsmIo::IoType FsmIo::ioTypeOfString(QString s)
{
  if ( s == "int" ) return FsmIo::Int;
  else if ( s == "bool" ) return FsmIo::Bool;
  else if ( s == "event" ) return FsmIo::Event;
  else Q_ASSERT(false);
}

QString FsmIo::stringOfKind(IoKind k)
{
  switch ( k ) {
  case In: return QString("in");
  case Out: return QString("out");
  case Var: return QString("var");
  }
}

QString FsmIo::stringOfType(IoType t)
{
  switch ( t ) {
  case Int: return QString("int");
  case Bool: return QString("bool");
  case Event: return QString("event");
  }
}

QString FsmIo::toString(bool withStim)
{
  QString r;
  r = stringOfKind(kind) + " " + name + " : " + stringOfType(type);
  if ( withStim ) r += " = " + stim.toString(); 
  return r;
}
