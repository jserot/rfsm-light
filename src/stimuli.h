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
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include "fsmIo.h"
#include <tuple>

class Stimuli: public QDialog {
    Q_OBJECT

public:
    explicit Stimuli(QList<FsmIo*> ios, QWidget *parent = 0);
    ~Stimuli();

private slots:
  void acceptChanges();

private:
    QGridLayout* mainLayout;
    void addRow(int row, FsmIo* io);
    QList<std::tuple<FsmIo*,QComboBox*,QLineEdit*>> rows;
};

#endif
