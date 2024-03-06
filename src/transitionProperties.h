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

#pragma once

#include <QDialog>

class Transition;
class State;
class Model;
class Compiler;
class QComboBox;
class TransitionGuards;
class TransitionActions;

class TransitionProperties : public QDialog
{
  Q_OBJECT

public:
  TransitionProperties(Transition *transition, Model *model, bool isInitial, Compiler *compiler, QWidget *parent);
  ~TransitionProperties();

private:
  Transition *transition;
  Model *model;
  Compiler *compiler;

  QComboBox* start_state_field;
  QComboBox* end_state_field;
  QComboBox* event_field;
  TransitionGuards *guards_panel;
  TransitionActions *actions_panel;
    
protected slots:
  void accept();
  void cancel();
};
