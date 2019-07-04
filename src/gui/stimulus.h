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

#ifndef STIMULUS_H
#define STIMULUS_H

#include <QString>

class Stimulus
{
  public:
    enum Kind { None, Periodic, Sporadic, ValueChanges };
    
    Stimulus(Kind kind, QString params) : myKind(kind), myParams(params) {};
    Stimulus(const QString& kind, const QString& params);
    Stimulus(const QString& txt);

    Kind kind() const { return myKind; };
    QString params() const { return myParams; };

    void setKind(const Kind kind) { myKind = kind; };
    void setParams(const QString& params) { myParams = params; };

    QString toString();

  protected:
    void init(const QString& kind, const QString& params);
    Kind myKind;
    QString myParams;
};

#endif
