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

#include "properties.h"
#include "mainwindow.h"
#include "state.h"
#include "transition.h"
#include "fsm.h"
#include "stimuli.h"

#include <QComboBox>
#include <QFrame>
#include <QGraphicsItem>
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
#include <qDebug>

PropertiesPanel::PropertiesPanel(MainWindow* parent) : QFrame(parent)
{
    main_window = parent;

    createModelPanel();
    createIoPanel();
    createStatePanel();
    createTransitionPanel();
    createInitTransitionPanel();
    // createStimuliPanel();

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignTop);

    layout->addWidget(model_panel);
    layout->addWidget(io_panel);
    layout->addWidget(state_panel);
    layout->addWidget(transition_panel);
    layout->addWidget(itransition_panel);

    this->setLayout(layout);

    connect(model_name_field, &QLineEdit::textEdited, this, &PropertiesPanel::setModelName);
    connect(state_name_field, &QLineEdit::textEdited, this, &PropertiesPanel::setStateName);
    connect(transition_start_state_field, QOverload<int>::of(&QComboBox::activated), this, &PropertiesPanel::setTransitionSrcState);
    connect(transition_end_state_field, QOverload<int>::of(&QComboBox::activated), this, &PropertiesPanel::setTransitionDstState);
    connect(transition_event_field, &QLineEdit::textEdited, this, &PropertiesPanel::setTransitionEvent);
    connect(transition_guard_field, &QLineEdit::textEdited, this, &PropertiesPanel::setTransitionGuard);
    connect(transition_actions_field, &QLineEdit::textEdited, this, &PropertiesPanel::setTransitionActions);
    connect(itransition_end_state_field, QOverload<int>::of(&QComboBox::activated), this, &PropertiesPanel::setITransitionDstState);
    connect(itransition_actions_field, &QLineEdit::textEdited, this, &PropertiesPanel::setTransitionActions);

    model_panel->show();
    io_panel->show();
    state_panel->hide();
    transition_panel->hide();
    itransition_panel->hide();
}

PropertiesPanel::~PropertiesPanel()
{
}

void PropertiesPanel::createModelPanel()
{
    model_panel = new QGroupBox("Model");
    QVBoxLayout* modelPanelLayout = new QVBoxLayout();
    model_panel->setMaximumHeight(100);
    //model_panel->setMinimumWidth(200);

    QLabel* nameLabel = new QLabel("Name");
    model_name_field = new QLineEdit();
    modelPanelLayout->addWidget(nameLabel);
    modelPanelLayout->addWidget(model_name_field);

    model_panel->setLayout(modelPanelLayout);
}

void PropertiesPanel::createIoPanel()
{
    io_panel = new QGroupBox("I/Os and variables");
    QGridLayout* grid = new QGridLayout();
    io_panel->setMaximumHeight(200);
    //io_panel->setMinimumWidth(200);

    grid->addWidget(new QLabel("Name"), 0, 0, 1, 2);
    grid->addWidget(new QLabel("Kind"), 0, 2, 1, 2);
    grid->addWidget(new QLabel("Type"), 0, 4, 1, 2);

    io_name = new QLineEdit();
    io_kind = new QComboBox();
    io_kind->addItem("in", "in");
    io_kind->addItem("out", "out");
    io_kind->addItem("var", "var");
    io_type = new QLineEdit();
    grid->addWidget(io_name, 1, 0, 1, 2);
    grid->addWidget(io_kind, 1, 2, 1, 2);
    grid->addWidget(io_type, 1, 4, 1, 2);

    QStringList ios;
    for(auto io : main_window->getFsm()->ios()) {
      qDebug() << "Adding IO " << io->toString();
      ios << io->toString();
    }
    ios_model = new QStringListModel(ios);

    ios_view = new QListView();
    ios_view->setMaximumHeight(100);
    ios_view->setModel(ios_model);
    ios_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //ios_view->setHeight(50);
    grid->addWidget(ios_view, 2, 0, 1, 6);

    QPushButton* add_button = new QPushButton("Add");
    QPushButton* remove_button = new QPushButton("Remove");
    QPushButton* edit_button = new QPushButton("Stimuli");

    grid->addWidget(add_button, 5, 0, 1, 2);
    grid->addWidget(remove_button, 5, 2, 1, 2);
    grid->addWidget(edit_button, 5, 4, 1, 2);

    io_panel->setLayout(grid);

    connect(io_type, &QLineEdit::returnPressed, this, &PropertiesPanel::addIo);
    connect(add_button, &QPushButton::clicked, this, &PropertiesPanel::addIo);
    connect(edit_button, &QPushButton::clicked, this, &PropertiesPanel::editIos);
    connect(remove_button, &QPushButton::clicked, this, &PropertiesPanel::removeIos);
}

void PropertiesPanel::createStatePanel()
{
    state_panel = new QGroupBox("State properties");
    QVBoxLayout* statePanelLayout = new QVBoxLayout();
    state_panel->setMaximumHeight(100);
    //state_panel->setMinimumWidth(200);

    QLabel* nameLabel = new QLabel("Name");
    state_name_field = new QLineEdit();
    statePanelLayout->addWidget(nameLabel);
    statePanelLayout->addWidget(state_name_field);

    state_panel->setLayout(statePanelLayout);
}

void PropertiesPanel::createTransitionPanel()
{
    transition_panel = new QGroupBox("Transition properties");
    QGridLayout* transitionLayout = new QGridLayout();
    transition_panel->setMaximumHeight(180);

    QLabel* startLabel = new QLabel("Start State");
    transition_start_state_field = new QComboBox();
    transitionLayout->addWidget(startLabel, 0, 0, 1, 3);
    transitionLayout->addWidget(transition_start_state_field, 1, 0, 1, 3);

    QLabel* endLabel = new QLabel("End State");
    transition_end_state_field = new QComboBox();
    transitionLayout->addWidget(endLabel, 0, 3, 1, 3);
    transitionLayout->addWidget(transition_end_state_field, 1, 3, 1, 3);

    QLabel* eventLabel = new QLabel("Event");
    transition_event_field = new QLineEdit();
    //transition_event_field->setMinimumWidth(50);
    transitionLayout->addWidget(eventLabel, 2, 0, 1, 2);
    transitionLayout->addWidget(transition_event_field, 2, 2, 1, 4);

    QLabel* guardLabel = new QLabel("Guard");
    transition_guard_field = new QLineEdit();
    transitionLayout->addWidget(guardLabel, 3, 0, 1, 2);
    transitionLayout->addWidget(transition_guard_field, 3, 2, 1, 4);

    QLabel* actionsLabel = new QLabel("Action(s)");
    transition_actions_field = new QLineEdit();
    transitionLayout->addWidget(actionsLabel, 4, 0, 1, 2);
    transitionLayout->addWidget(transition_actions_field, 4, 2, 1, 4);

    transition_panel->setLayout(transitionLayout);
}

void PropertiesPanel::createInitTransitionPanel()
{
    itransition_panel = new QGroupBox("Transition properties");
    QVBoxLayout* transitionLayout = new QVBoxLayout();

    QLabel* endLabel = new QLabel("End State");
    itransition_end_state_field = new QComboBox();
    transitionLayout->addWidget(endLabel);
    transitionLayout->addWidget(itransition_end_state_field);

    QLabel* actionsLabel = new QLabel("Action(s)");
    itransition_actions_field = new QLineEdit();
    transitionLayout->addWidget(actionsLabel);
    transitionLayout->addWidget(itransition_actions_field);

    itransition_panel->setLayout(transitionLayout);
}


void PropertiesPanel::unselectItem()
{
    selected_item = nullptr;
    state_panel->hide();
    transition_panel->hide();
    itransition_panel->hide();
}

void PropertiesPanel::setSelectedItem(State* state)
{
    qDebug() << "State " << state->getId() << " selected";
    transition_panel->hide();
    itransition_panel->hide();
    if ( ! state->isPseudo() ) {
      selected_item = state;
      state_panel->show();
      state_name_field->setText(state->getId());
      }
}

void PropertiesPanel::setSelectedItem(Transition* transition)
{
    qDebug() << "Transition " << transition->toString() << " selected";
    selected_item = transition;
    state_panel->hide();

    if ( transition->isInitial() ) {
      transition_panel->hide();
      itransition_panel->show();
      itransition_end_state_field->clear();
      foreach ( State* state, main_window->getFsm()->states() ) {
        if ( ! state->isPseudo() ) {
          QString id = state->getId();
          itransition_end_state_field->addItem(id, QVariant(id));
          if ( transition->dstState()->getId() == id ) 
            itransition_end_state_field->setCurrentIndex(itransition_end_state_field->count()-1);
          }
        }
      itransition_actions_field->setText(transition->getActions());
      }
    else {
      itransition_panel->hide();
      transition_panel->show();
      transition_start_state_field->clear();
      transition_end_state_field->clear();
      foreach ( State* state, main_window->getFsm()->states() ) {
        if ( ! state->isPseudo() ) {
          QString id = state->getId();
          transition_start_state_field->addItem(id, QVariant(id));
          transition_end_state_field->addItem(id, QVariant(id));
          if ( transition->srcState()->getId() == id )
            transition_start_state_field->setCurrentIndex(transition_start_state_field->count()-1);
          if ( transition->dstState()->getId() == id ) 
            transition_end_state_field->setCurrentIndex(transition_end_state_field->count()-1);
          }
        }
      transition_event_field->setText(transition->getEvent());
      transition_guard_field->setText(transition->getGuard());
      transition_actions_field->setText(transition->getActions());
    }
}

void PropertiesPanel::setModelName(const QString& name)
{
  Fsm* model = main_window->getFsm();
  model->setName(name.trimmed());
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::addIo()
{
    QString name = io_name->text().trimmed();
    QString kind = io_kind->currentText().trimmed();
    QString type = io_type->text().trimmed();

    if ( name == "" ) return;
    if ( kind == "" ) { QMessageBox::warning( this, "Error", "No kind specified for io " + name); return; }
    if ( type == "" ) { QMessageBox::warning( this, "Error", "No type specified for io " + name); return; }

    Fsm* model = main_window->getFsm();

    if ( model->getIo(name) != NULL ) {
        QMessageBox::warning( this, "Error", "IO or variable " + name + " is already defined");
        return;
        }

    FsmIo* io = model->addIo(io_name->text(), io_kind->currentText(), io_type->text());

    QStringList string_list = ios_model->stringList();
    string_list << io->toString();
    ios_model->setStringList(string_list);
    main_window->setUnsavedChanges(true);
}

QString getStringPiece(QString s, int n, int i, QString sep=" ")
{
  QStringList l = s.split(sep);
  if ( l.length() < n ) throw std::invalid_argument("PropertiesPanel::getStringPiece");
  return l.at(i).trimmed();
}

void PropertiesPanel::removeIos()
{
    QList<QModelIndex> model_idxs = ios_view->selectionModel()->selectedIndexes();
    QList<int> idxs;
    for(auto& model_index : model_idxs)
      idxs.append(model_index.row());

    std::sort(idxs.begin(), idxs.end());

    QStringList string_list = ios_model->stringList();
    Fsm* model = main_window->getFsm();

    for(auto i = idxs.rbegin(); i != idxs.rend(); i++) {
       // TBR : the model should give a more straightforward access to the name of the item list
        QString v = getStringPiece(string_list.at(*i), 3, 1);
        model->removeIo(v);
        string_list.removeAt(*i);
    }

    ios_model->setStringList(string_list);
    main_window->setUnsavedChanges(true);
}

void PropertiesPanel::editIos()
{
    QList<QModelIndex> model_idxs = ios_view->selectionModel()->selectedIndexes();
    QList<int> idxs;
    for(auto& model_index : model_idxs) idxs.append(model_index.row());
    std::sort(idxs.begin(), idxs.end());
    Fsm* model = main_window->getFsm();
    QList<FsmIo*> ios;
    QStringList string_list = ios_model->stringList();
    for(auto i = idxs.rbegin(); i != idxs.rend(); i++) {
      QString name = getStringPiece(string_list.at(*i), 3, 1);
      FsmIo* io = model->getIo(name);
      if ( io->kind() == "in" ) ios.append(io);
      }
    if ( ! ios.empty() ) {
      Stimuli* stimDialog = new Stimuli(ios);
      stimDialog->exec();
      fillIoList();
      delete stimDialog;
      }
}

void PropertiesPanel::setStateName(const QString& name)
{
    State* state = qgraphicsitem_cast<State*>(selected_item);
    if(state != nullptr) {
        state->setId(name);
        main_window->getFsm()->update();
        main_window->setUnsavedChanges(true);
    }
}

void PropertiesPanel::setTransitionSrcState(int index)
{
  if ( index == -1 ) return;
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  QString state_id = transition_start_state_field->itemText(index);
  State* state = main_window->getFsm()->getState(state_id);
  if ( state == nullptr )
    throw std::invalid_argument(std::string("No state found with id : ") + state_id.toStdString());
  transition->setSrcState(state);
  main_window->getFsm()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionDstState(int index)  // TODO: factorize with above code
{
  if ( index == -1 ) return;
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  QString state_id = transition_end_state_field->itemText(index);
  State* state = main_window->getFsm()->getState(state_id);
  if ( state == nullptr )
    throw std::invalid_argument(std::string("No state found with id : ") + state_id.toStdString());
  transition->setDstState(state);
  main_window->getFsm()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setITransitionDstState(int index)  // TODO: factorize with above code
{
  if ( index == -1 ) return;
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  QString state_id = itransition_end_state_field->itemText(index);
  State* state = main_window->getFsm()->getState(state_id);
  if ( state == nullptr )
    throw std::invalid_argument(std::string("No state found with id : ") + state_id.toStdString());
  transition->setDstState(state);
  main_window->getFsm()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionEvent(const QString& event)
{
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  transition->setEvent(event);
  main_window->getFsm()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionGuard(const QString& guard)
{
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  transition->setGuard(guard);
  main_window->getFsm()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionActions(const QString& actions)
{
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  transition->setActions(actions);
  main_window->getFsm()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::clear()
{
    selected_item = nullptr;

    state_panel->hide();
    transition_panel->hide();
    itransition_panel->hide();

    fillIoList();
    fillModelName();
}

void PropertiesPanel::fillModelName()
{
    Fsm* model = main_window->getFsm();
    if ( model == NULL ) return;
    model_name_field->setText(model->name());
}

void PropertiesPanel::fillIoList()
{
    QStringList ios;
    Fsm* model = main_window->getFsm();

    if ( model == NULL ) return;

    for(auto io: model->ios())
      ios << io->toString();
    delete ios_model;
    ios_model = new QStringListModel(ios);
    ios_view->setModel(ios_model);
}
