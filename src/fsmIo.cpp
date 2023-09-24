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
  if ( s == "in" ) return FsmIo::IoIn;
  else if ( s == "out" ) return FsmIo::IoOut;
  else if ( s == "var" ) return FsmIo::IoVar;
  else Q_ASSERT(false);
}

FsmIo::IoType FsmIo::ioTypeOfString(QString s)
{
  if ( s == "event" ) return FsmIo::TyEvent;
  else if ( s == "int" ) return FsmIo::TyInt;
  else if ( s == "bool" ) return FsmIo::TyBool;
  else Q_ASSERT(false);
}

QString FsmIo::stringOfKind(IoKind k)
{
  switch ( k ) {
  case IoIn: return QString("in");
  case IoOut: return QString("out");
  case IoVar: return QString("var");
  }
}

QString FsmIo::stringOfType(IoType t)
{
  switch ( t ) {
  case TyEvent: return QString("event");
  case TyInt: return QString("int");
  case TyBool: return QString("bool");
  }
}

QString FsmIo::toString(bool withStim)
{
  QString r;
  r = stringOfKind(kind) + " " + name + " : " + stringOfType(type);
  if ( withStim ) r += " = " + stim.toString(); 
  return r;
}
