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

Stimulus::Stimulus()
{
  kind = None;
  desc.periodic = Periodic_stim();
}

Stimulus::Stimulus(Kind kind, QList<int> &params)
{
  this->kind = kind;
  switch ( kind ) {
  case None:
    break;
  case Periodic:
    desc.periodic = Periodic_stim(params.at(0), params.at(1), params.at(2));
    break;
  case Sporadic:
    desc.sporadic = Sporadic_stim(params);
    break;
  case ValueChanges:
    QList<QPair<int,int>> vcs;
    while ( params.length() >= 2 ) {
      int t = params.takeFirst();
      int v = params.takeFirst();
      vcs.append(QPair<int,int>(t,v));
      }
    desc.valueChanges = ValueChanges_stim(vcs);
    break;
  }
}

QString Stimulus::toString()
{
  switch ( kind ) {
  case None: return "None";
  case Periodic: return "Periodic";
  case Sporadic: return "Sporadic";
  case ValueChanges: return "ValueChanges";
  }
}
