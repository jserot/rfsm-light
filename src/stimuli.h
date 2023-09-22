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

#ifndef STIMULI_DIALOG_H
#define STIMULI_DIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QFrame>
#include <QPushButton>
#include <QSpacerItem>
#include <QComboBox>
#include "fsmIo.h"

class Stimuli: public QDialog {
    Q_OBJECT

public:
  explicit Stimuli(FsmIo* inp, QWidget *parent = Q_NULLPTR);
  ~Stimuli();

private:
  const int maxTime = 1000;

  QHash<QPushButton*, QHBoxLayout*> mButtonToLayoutMap;
  QList<QHBoxLayout*> rows;
  FsmIo *selectedInp;

private:
  QWidget *centralWidget;
  QVBoxLayout* formLayout;
  QComboBox *selector;
  QFrame *form;
  QVBoxLayout *verticalLayout;
  QVBoxLayout *verticalLayout_2;
  QSpacerItem *verticalSpacer;
  QFrame *line;

protected:
  void addPeriodicRow(QString name, int val, int step, int lo, int hi);

private slots:
  void showForm(QVariant kind);
  void clearForm();
  void _addSporadicRow(int t);
  void addSporadicRow();
  void _addValueChangesRow(QPair<int,int> vc);
  void addValueChangesRow();
  void deleteDynamicRow();
  void acceptChanges();
};

#endif
