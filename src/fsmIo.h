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
  protected:
    QString myName;
    QString myKind;
    QString myType;
    Stimulus myDesc; // For inputs

  public:
    FsmIo(const QString& name) : myName(name), myKind(""), myType(""), myDesc("") {};
    FsmIo(const QString& name,
             const QString& kind,
             const QString& type, 
             const QString& desc = "") : myName(name), myKind(kind), myType(type), myDesc(desc) {};

    QString name() const { return myName; };
    QString kind() const { return myKind; };
    QString type() const { return myType; };
    Stimulus desc() const { return myDesc; };

    void setName(const QString& name) { myName = name; };
    void setKind(const QString& kind) { myKind = kind; };
    void setType(const QString& type) { myType = type; };
    void setDesc(const Stimulus& desc) { myDesc = desc; };

    QString toString(bool withDesc=true);
};

#endif
