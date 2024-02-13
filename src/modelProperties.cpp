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
#include "modelVars.h"
#include "stimuli.h"
#include "compilerPaths.h"

#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <stdexcept>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QRegularExpression>
#include <QDebug>

//const QRegularExpression ModelProperties::re_uid("[A-Z][A-Za-z0-9_]*");
const QRegularExpression ModelProperties::re_lid("[a-z][a-z0-9_]*");

ModelProperties::ModelProperties(Model *model, MainWindow* parent) : QFrame(parent)
{
    main_window = parent;
    // TODO: we shouldn't need this. Instead, emit a "Modified" signal, caught by MainWindow,
    // when appropriate (just as State and TransitionProperties dialogs do)
    this->model = model;

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);
    //layout->setMinimumWidth(200);

    name_panel = new QGroupBox("Name");
    name_panel->setMaximumHeight(100);
    model_name_field = new QLineEdit();
    model_name_field->setPlaceholderText("Model name");
    layout->addWidget(model_name_field);
    name_panel->setLayout(layout);

    // createInputPanel();
    // createOutputPanel();
    // createVarPanel();

    layout->addWidget(name_panel);
    // layout->addWidget(inp_panel);
    // layout->addWidget(outp_panel);

    vars_panel = new ModelVars("Variables", model);
     
    layout->addWidget(vars_panel);

    QPushButton *dump_button = new QPushButton("dump");
    layout->addWidget(dump_button);
    
    this->setLayout(layout);

    connect(model_name_field, &QLineEdit::editingFinished, this, &ModelProperties::setModelName);
    connect(dump_button, &QPushButton::clicked, this, &ModelProperties::dumpModel);

    io_name_validator = new QRegularExpressionValidator(re_lid);
    assert(main_window);
    assert(main_window->getCompilerPaths());
    CommandExec* executor = main_window->getExecutor();
    assert(executor);
}

ModelProperties::~ModelProperties()
{
}

// [Name] panel

void ModelProperties::setModelName()
{
  assert(model);
  QString name = model_name_field->text().trimmed();
  model->setName(name);
  // main_window->setUnsavedChanges(true);
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
