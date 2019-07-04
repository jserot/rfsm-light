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

#include <QGridLayout>
#include <QLabel>
#include <QDialogButtonBox>

#include "stimuli.h"

Stimuli::Stimuli(QList<FsmIo*> ios, QWidget *parent)
    : QDialog(parent)
{
    mainLayout = new QGridLayout();
    int row = 0;
    mainLayout->addWidget(new QLabel("Name"), row, 0, 1, 1);
    mainLayout->addWidget(new QLabel("Kind"), row, 1, 1, 1);
    mainLayout->addWidget(new QLabel("Parameters"), row, 2, 1, 4);
    row++;
    foreach ( FsmIo* io, ios) {
      addRow(row, io);
      row++;
      }
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    mainLayout->addWidget(buttonBox, row, 0, 1, 5);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(acceptChanges()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    setLayout(mainLayout);
    setWindowTitle(tr("Input stimuli"));
}

void Stimuli::addRow(int row, FsmIo *io)
{
    QLabel* stimulus_name = new QLabel(io->name());
    QComboBox* stimulus_kind = new QComboBox();
    stimulus_kind->addItem("None", Stimulus::None);
    stimulus_kind->addItem("Periodic", Stimulus::Periodic);
    stimulus_kind->addItem("Sporadic", Stimulus::Sporadic);
    stimulus_kind->addItem("ValueChanges", Stimulus::ValueChanges);
    QLineEdit* stimulus_params = new QLineEdit();
    stimulus_kind->setCurrentIndex(io->desc().kind());
    stimulus_params->setText(io->desc().params());
    mainLayout->addWidget(stimulus_name, row, 0, 1, 1);
    mainLayout->addWidget(stimulus_kind, row, 1, 1, 1);
    mainLayout->addWidget(stimulus_params, row, 3, 1, 4);
    rows.append(std::make_tuple(io,stimulus_kind,stimulus_params));
}

void Stimuli::acceptChanges()
{
  for ( auto row: rows ) {
    FsmIo* io = std::get<0>(row);
    QString kind = std::get<1>(row)->currentText();
    QString params = std::get<2>(row)->text();
    // qDebug() << io->name() << ".desc <- " << kind << " " << params;
    Stimulus stim(kind, params);
    io->setDesc(stim);
    }
  QDialog::accept();
}

Stimuli::~Stimuli()
{
}
