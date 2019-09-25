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

#include "stimulus.h"
#include <QString>
#include <QStringList>

Stimulus::Stimulus(const QString& txt)
{
  QStringList ss = txt.split(" ");
  QString hd = ss.first();
  ss.removeFirst();
  init(hd, ss.join(" "));
}

Stimulus::Stimulus(const QString& kind, const QString& params)
{
  init(kind, params);
}

Stimulus::Kind kindOfString(QString s)
{
  if ( s == "Periodic" ) return Stimulus::Periodic;
  else if ( s == "Sporadic" ) return Stimulus::Sporadic;
  else if ( s == "ValueChanges" ) return Stimulus::ValueChanges;
  else return Stimulus::None;
}

void Stimulus::init(const QString& kind, const QString& params)
{
  myKind = kindOfString(kind);
  myParams = myKind == Stimulus::None ? "" : params;
}

QString Stimulus::toString()
{
  switch ( myKind ) {
  case Stimulus::Periodic: return "Periodic " + myParams;
  case Stimulus::Sporadic: return "Sporadic " + myParams;
  case Stimulus::ValueChanges: return "ValueChanges " + myParams;
  case Stimulus::None: return "";
  }
}
