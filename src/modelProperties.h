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

#include <QComboBox>
#include <QFrame>
#include "iov.h"

class MainWindow;
class State;
class Transition;
class Model;
class ModelIovs;
class QLineEdit;
class QGroupBox;

class ModelProperties : public QFrame
{
  Q_OBJECT

  static const QRegularExpression re_lid;

private:
  MainWindow* main_window;
  Model *model;

  QGroupBox* name_panel;
  QLineEdit* model_name_field;
  ModelIovs *inps_panel;
  ModelIovs *outps_panel;
  ModelIovs *vars_panel;
  
public:
  explicit ModelProperties(Model *model, MainWindow* parent);
  ~ModelProperties();

  QSize sizeHint() const { return QSize(275,300); }; 

signals:
  void modelModified();
  
public slots:
  void setModelName();
  void fillModelName();
  void clearModelName();

  void update();
  void clear();
  //void clearIos();
  void modelUpdated();
  void dumpModel(); // for debug only

private:
  QRegularExpressionValidator *io_name_validator;
};
