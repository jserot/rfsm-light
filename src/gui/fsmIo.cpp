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

QString FsmIo::toString(bool withDesc)
{
  QString r;
  r = myKind + " " + myName + " : " + myType;
  if ( withDesc ) r += " = " + myDesc.toString(); 
  return r;
}
