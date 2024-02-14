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

#include "modelProperties.h"
#include "mainwindow.h"
#include "model.h"
#include "modelInps.h"
#include "modelOutps.h"
#include "modelVars.h"
#include "stimuli.h"
#include "compilerPaths.h"

#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
// #include <stdexcept>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

const QRegularExpression ModelProperties::re_lid("[a-z][a-z0-9_]*");

ModelProperties::ModelProperties(Model *model, MainWindow* parent) : QFrame(parent)
{
    this->model = model;

    QRegularExpressionValidator *io_name_validator = new QRegularExpressionValidator(re_lid);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    //layout->setMinimumWidth(200);

    name_panel = new QGroupBox("Name");
    name_panel->setMaximumHeight(100);
    model_name_field = new QLineEdit();
    model_name_field->setPlaceholderText("Model name");
    layout->addWidget(model_name_field);
    name_panel->setLayout(layout);
    layout->addWidget(name_panel);

    inps_panel = new ModelInps("Inputs", model, io_name_validator);
    layout->addWidget(inps_panel);

    outps_panel = new ModelOutps("Outputs", model, io_name_validator);
    layout->addWidget(outps_panel);

    vars_panel = new ModelVars("Variables", model, io_name_validator);
    layout->addWidget(vars_panel);

    QPushButton *dump_button = new QPushButton("dump");  // For debug only
    layout->addWidget(dump_button);
    
    this->setLayout(layout);

    connect(model_name_field, &QLineEdit::editingFinished, this, &ModelProperties::setModelName);
    connect(dump_button, &QPushButton::clicked, this, &ModelProperties::dumpModel);
    connect(inps_panel, SIGNAL(modelModified()), this, SLOT(modelUpdated()));
    connect(outps_panel, SIGNAL(modelModified()), this, SLOT(modelUpdated()));
    connect(vars_panel, SIGNAL(modelModified()), this, SLOT(modelUpdated()));
}

ModelProperties::~ModelProperties()
{
}

void ModelProperties::modelUpdated()
{
  emit modelModified(); // To main window
}

// [Name] panel

void ModelProperties::setModelName()
{
  assert(model);
  QString name = model_name_field->text().trimmed();
  model->setName(name);
  emit modelModified();
}

void ModelProperties::fillModelName()
{
  assert(model);
  model_name_field->setText(model->getName());
}

void ModelProperties::clearModelName()
{
  assert(model);
  model_name_field->setText("");
}

void ModelProperties::update()
{
    fillModelName();
}

void ModelProperties::clear()
{
  clearModelName();
  // clearIos();
}

void ModelProperties::dumpModel()
{
  assert(model);
  model->dump();
}
