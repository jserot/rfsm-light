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

#ifndef FSM_IO
#define FSM_IO

#include <QString>
#include "stimulus.h"

class FsmIo
{
  public:
    QString name;
    QString kind;  // TO FIX : use enum 
    QString type;  // TO FIX : use enum 
    Stimulus stim; // For inputs

  public:
    // FsmIo() { };
    FsmIo(const QString& name,
          const QString& kind,
          const QString& type,
          const Stimulus& stim):
      name(name), kind(kind), type(type), stim(stim) {};
    ~FsmIo();

    QString toString(bool withStim=true);
};

#endif
