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
#include <QGraphicsItem>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QPushButton>
#include "iov.h"

class MainWindow;
class State;
class Transition;
class Model;
class ModelVars;

class ModelProperties : public QFrame
{
  Q_OBJECT

  static const QRegularExpression re_uid;
  static const QRegularExpression re_lid;

private:
  MainWindow* main_window;
  Model *model;

  QGroupBox* name_panel;
  QLineEdit* model_name_field;

  ModelVars *vars_panel;
  
  QHash<QWidget*,Iov*> widgetToIo; 

public:
  explicit ModelProperties(Model *model, MainWindow* parent);
  ~ModelProperties();

  QSize sizeHint() const { return QSize(275,300); }; 

public slots:
  void setModelName();
  void fillModelName();
  void clearModelName();

  void update();
  void clear();
  //void clearIos();
  void dumpModel();

private:
  // void _addIo(Model* model, Iov* io);
  // void addIo(Iov::IoKind kind);
  // void _removeIo(Model* model, Iov* io);

  QRegularExpressionValidator *io_name_validator;
};
