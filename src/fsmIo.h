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
    QString kind;
    QString type;
    Stimulus stim; // For inputs

  public:
    FsmIo(const QString& name) : name(name), kind(""), type(""), stim() {};
    FsmIo(const QString& name,
             const QString& kind,
             const QString& type) : name(name), kind(kind), type(type), stim() {};
    ~FsmIo();

    QString toString(bool withStim=true);
};

#endif
