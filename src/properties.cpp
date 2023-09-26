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
#include "model.h"
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
#include <QStandardItemModel>
//#include <qDebug>

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
    //layout->setMinimumWidth(200);

    layout->addWidget(model_panel);
    layout->addWidget(io_panel);
    layout->addWidget(state_panel);
    layout->addWidget(transition_panel);
    layout->addWidget(itransition_panel);

    this->setLayout(layout);

    connect(model_name_field, &QLineEdit::textEdited, this, &PropertiesPanel::setModelName);
    connect(state_name_field, &QLineEdit::textEdited, this, &PropertiesPanel::setStateName);
    connect(state_attr_field, &QLineEdit::textEdited, this, &PropertiesPanel::setStateAttr);
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
  QHBoxLayout *rowLayout;
  io_panel = new QGroupBox("I/Os and variables");
  //io_panel->setMaximumHeight(200);
  //io_panel->setMinimumWidth(200);
  ioLayout = new QVBoxLayout();
  ioLayout->setSpacing(4);
  ioLayout->setContentsMargins(11, 11, 11, 11);

  rowLayout = new QHBoxLayout();
  QPushButton* add_button = new QPushButton("Add");
  rowLayout->addWidget(add_button);
  ioLayout->addLayout(rowLayout);
  connect(add_button, &QPushButton::clicked, this, &PropertiesPanel::addIo);
    
  rowLayout = new QHBoxLayout();
  rowLayout->addWidget(new QLabel("Name"));
  rowLayout->addWidget(new QLabel("Kind"));
  rowLayout->addWidget(new QLabel("Type"));
  rowLayout->addWidget(new QLabel("Stim"));
  rowLayout->addWidget(new QLabel("")); // For padding
  ioLayout->addLayout(rowLayout);

  // QSpacerItem *verticalSpacer = new QSpacerItem(20, 64, QSizePolicy::Minimum, QSizePolicy::Expanding);
  // ioLayout->addItem(verticalSpacer);

  io_panel->setLayout(ioLayout);
}

void PropertiesPanel::_addIo(Model* model, Iov* io)
{
  qDebug () << "Adding IO" << io->name << io->kind << io->type;
  Q_ASSERT(model);
  Q_ASSERT(io);
  QHBoxLayout *rowLayout = new QHBoxLayout(io_panel);
  rowLayout->setObjectName("ioRowLayout");

  QLineEdit *io_name = new QLineEdit();
  io_name->setMinimumSize(40,io_name->minimumHeight());
  io_name->setFrame(true);
  io_name->setText(io->name);
  // TODO: use setInputMask to fordid syntax errors on IO names (check rsfm syntax)
  rowLayout->addWidget(io_name);
  widgetToIo.insert((QWidget*)io_name, io);
  widgetToLayout.insert((QWidget*)io_name, rowLayout);
  connect(io_name, &QLineEdit::textChanged, this, &PropertiesPanel::editIoName);

  QComboBox *io_kind = new QComboBox();
  io_kind->addItem("in", QVariant(Iov::IoIn));
  io_kind->addItem("out", QVariant(Iov::IoOut));
  io_kind->addItem("var", QVariant(Iov::IoVar));
  io_kind->setCurrentIndex(io->kind);
  rowLayout->addWidget(io_kind);
  widgetToIo.insert((QWidget*)io_kind, io);
  widgetToLayout.insert((QWidget*)io_kind, rowLayout);
  connect(io_kind, &QComboBox::currentIndexChanged, this, &PropertiesPanel::editIoKind);

  QComboBox *io_type = new QComboBox();
  io_type->addItem("event", QVariant(Iov::TyEvent));
  io_type->addItem("int", QVariant(Iov::TyInt));
  io_type->addItem("bool", QVariant(Iov::TyBool));
  io_type->setCurrentIndex(io->type);
  rowLayout->addWidget(io_type);
  widgetToIo.insert((QWidget*)io_type, io);
  widgetToLayout.insert((QWidget*)io_type, rowLayout);
  connect(io_type, &QComboBox::currentIndexChanged, this, &PropertiesPanel::editIoType);

  QComboBox *io_stim = new QComboBox();
  io_stim->addItem("None", QVariant(Stimulus::None));
  io_stim->addItem("Periodic", QVariant(Stimulus::Periodic));
  io_stim->addItem("Sporadic", QVariant(Stimulus::Sporadic));
  io_stim->addItem("ValueChanges", QVariant(Stimulus::ValueChanges));
  io_stim->setCurrentIndex(io->stim.kind);
  rowLayout->addWidget(io_stim);
  connect(io_stim, &QComboBox::activated, this, &PropertiesPanel::editIoStim);
  widgetToLayout.insert((QWidget*)io_stim, rowLayout);
  widgetToIo.insert((QWidget*)io_stim, io);

  QPushButton *io_delete = new QPushButton();
  io_delete->setIcon(QIcon(":/images/delete.png"));
  rowLayout->addWidget(io_delete);
  connect(io_delete, &QPushButton::clicked, this, &PropertiesPanel::removeIo);
  widgetToLayout.insert((QWidget*)io_delete, rowLayout);
  widgetToIo.insert((QWidget*)io_delete, io);
  ioLayout->insertLayout(-1,rowLayout);
  ioToLayout.insert(io,rowLayout);
}

void PropertiesPanel::addIo()
{
  Model* model = main_window->getModel();
  Q_ASSERT(model != 0);
  Iov* io = model->addIo("", Iov::IoIn, Iov::TyInt, Stimulus(Stimulus::None));
  _addIo(model, io);
}

void delete_io_row(QLayout *layout)
{
  while (layout->count() != 0) {
    QLayoutItem* item = layout->takeAt(0);
    delete item->widget();
    delete item;
  }
  delete layout;
}

void PropertiesPanel::_removeIo(Model* model, Iov* io)
{
  Q_ASSERT(model);
  Q_ASSERT(io);
  QHBoxLayout* row_layout = ioToLayout.take(io);
  Q_ASSERT(row_layout);
  QLineEdit *io_name = qobject_cast<QLineEdit*>(row_layout->itemAt(0)->widget());
  Q_ASSERT(io_name);
  QString name = io_name->text().trimmed();
  qDebug() << "Removing IO" << name;
  delete_io_row(row_layout);
  model->removeIo(io);
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::removeIo()
{
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  Iov* io = widgetToIo.take(button);
  Model* model = main_window->getModel();
  _removeIo(model, io);
}

void PropertiesPanel::editIoName()
{
  QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
  Iov* io = widgetToIo.value(ledit);
  Q_ASSERT(io);
  QString name = ledit->text().trimmed();
  // Model* model = main_window->getModel();
  // if ( model->getIo(name) != NULL ) {
  //     QMessageBox::warning( this, "Error", "IO or variable " + name + " is already defined");
  //     return;
  //     }
  qDebug () << "Setting IO name: " << name;
  io->name = name;
  main_window->setUnsavedChanges(true);
}

void setComboBoxItemEnabled(QComboBox* comboBox, int index, bool enabled)
// From : https://stackoverflow.com/questions/38915001/disable-specific-items-in-qcombobox
{
    auto * model = qobject_cast<QStandardItemModel*>(comboBox->model());
    Q_ASSERT(model);
    if(!model) return;

    auto * item = model->item(index);
    Q_ASSERT(item);
    if(!item) return;
    item->setEnabled(enabled);
}

void PropertiesPanel::editIoKind()
{
  QComboBox* box = qobject_cast<QComboBox*>(sender());
  Iov* io = widgetToIo.value(box);
  QHBoxLayout* row_layout = widgetToLayout.value(box);
  Q_ASSERT(row_layout);
  Q_ASSERT(io);
  io->kind = (Iov::IoKind)(box->currentIndex());
  qDebug () << "Setting IO kind: " << io->kind;
  QComboBox *stim_box = qobject_cast<QComboBox*>(row_layout->itemAt(3)->widget());
  Q_ASSERT(stim_box);
  switch ( io->kind ) {
  case Iov::IoIn: 
    for ( int i=0; i<stim_box->count(); i++ ) 
      setComboBoxItemEnabled(stim_box, i, true);
    break;
  default:
    for ( int i=0; i<stim_box->count(); i++ ) 
      setComboBoxItemEnabled(stim_box, i, false);
    break;
  }
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::editIoType()
{
  QComboBox* box = qobject_cast<QComboBox*>(sender());
  Iov* io = widgetToIo.value(box);
  Q_ASSERT(io);
  io->type = (Iov::IoType)(box->currentIndex());
  qDebug () << "Setting IO type: " << io->type;
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::editIoStim()
{
  QComboBox* selector = qobject_cast<QComboBox*>(sender());
  Iov* io = widgetToIo.value(selector);
  Q_ASSERT(io);
  QString io_name = io->name;
  if ( io_name == "" ) {
    QMessageBox::warning( this, "Error", "Please give a name to this IO before editing it");
    selector->setCurrentIndex(Stimulus::None);
    return;
    }
  // NO LONGER REQUIRED : the stimuli combobox will be automatically devalidated for non inputs
  // if ( io->kind != Iov::IoIn ) {
  //   QMessageBox::warning( this, "Error", "Stimuli can only be attached to inputs");
  //   selector->setCurrentIndex(Stimulus::None);
  //   return;
  //   }
  Stimulus::Kind kind = (Stimulus::Kind)(selector->currentIndex()); 
  //if ( kind == io->stim.kind ) return; // No change
  switch ( kind ) {
  case Stimulus::None:
    io->stim = Stimulus(Stimulus::None);
    break;
  default:
    Stimuli* stimDialog = new Stimuli(kind,io);
    stimDialog->exec();
    //fillIoList();
    delete stimDialog;
    break;
    }
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::createStatePanel()
{
    state_panel = new QGroupBox("State properties");
    QGridLayout* statePanelLayout = new QGridLayout();
    state_panel->setMaximumHeight(100);
    //state_panel->setMinimumWidth(200);

    QLabel* nameLabel = new QLabel("Name");
    state_name_field = new QLineEdit();
    statePanelLayout->addWidget(nameLabel, 0, 0, 1, 1);
    statePanelLayout->addWidget(state_name_field, 0, 1, 1, 1);

    QLabel* attrLabel = new QLabel("Output valuations");
    state_attr_field = new QLineEdit();
    statePanelLayout->addWidget(attrLabel, 1, 0, 1, 1);
    statePanelLayout->addWidget(state_attr_field, 1, 1, 1, 1);

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
    //qDebug() << "State " << state->getId() << " selected";
    transition_panel->hide();
    itransition_panel->hide();
    if ( ! state->isPseudo() ) {
      selected_item = state;
      state_panel->show();
      state_name_field->setText(state->getId());
      state_attr_field->setText(state->getAttr());
      }
}

void PropertiesPanel::setSelectedItem(Transition* transition)
{
    //qDebug() << "Transition " << transition->toString() << " selected";
    selected_item = transition;
    state_panel->hide();

    if ( transition->isInitial() ) {
      transition_panel->hide();
      itransition_panel->show();
      itransition_end_state_field->clear();
      foreach ( State* state, main_window->getModel()->states() ) {
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
      foreach ( State* state, main_window->getModel()->states() ) {
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
  Model* model = main_window->getModel();
  model->setName(name.trimmed());
  main_window->setUnsavedChanges(true);
}


void PropertiesPanel::setStateName(const QString& name)
{
    State* state = qgraphicsitem_cast<State*>(selected_item);
    if(state != nullptr) {
        state->setId(name);
        main_window->getModel()->update();
        main_window->setUnsavedChanges(true);
    }
}

void PropertiesPanel::setStateAttr(const QString& attr)
{
    State* state = qgraphicsitem_cast<State*>(selected_item);
    if(state != nullptr) {
        state->setAttr(attr);
        main_window->getModel()->update();
        main_window->setUnsavedChanges(true);
    }
}

void PropertiesPanel::setTransitionSrcState(int index)
{
  if ( index == -1 ) return;
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  QString state_id = transition_start_state_field->itemText(index);
  State* state = main_window->getModel()->getState(state_id);
  if ( state == nullptr )
    throw std::invalid_argument(std::string("No state found with id : ") + state_id.toStdString());
  transition->setSrcState(state);
  main_window->getModel()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionDstState(int index)  // TODO: factorize with above code
{
  if ( index == -1 ) return;
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  QString state_id = transition_end_state_field->itemText(index);
  State* state = main_window->getModel()->getState(state_id);
  if ( state == nullptr )
    throw std::invalid_argument(std::string("No state found with id : ") + state_id.toStdString());
  transition->setDstState(state);
  main_window->getModel()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setITransitionDstState(int index)  // TODO: factorize with above code
{
  if ( index == -1 ) return;
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  QString state_id = itransition_end_state_field->itemText(index);
  State* state = main_window->getModel()->getState(state_id);
  if ( state == nullptr )
    throw std::invalid_argument(std::string("No state found with id : ") + state_id.toStdString());
  transition->setDstState(state);
  main_window->getModel()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionEvent(const QString& event)
{
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  transition->setEvent(event);
  main_window->getModel()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionGuard(const QString& guard)
{
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  transition->setGuard(guard);
  main_window->getModel()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::setTransitionActions(const QString& actions)
{
  Transition* transition = qgraphicsitem_cast<Transition*>(selected_item);
  if ( transition == nullptr ) return;
  transition->setActions(actions);
  main_window->getModel()->update();
  main_window->setUnsavedChanges(true);
}

void PropertiesPanel::update()
{
    fillIos();
    fillModelName();
}

void PropertiesPanel::fillModelName()
{
    Model* model = main_window->getModel();
    if ( model == NULL ) return;
    model_name_field->setText(model->getName());
}

void PropertiesPanel::fillIos()
{
  qDebug() << "Filling IOs";
  Model* model = main_window->getModel();
  Q_ASSERT(model);
  if ( model == NULL ) return;
  QStringList ios;
  qDebug() << ios.length();
  for (auto io: model->getIos())
    _addIo(model, io);
}

void PropertiesPanel::clear()
{
  qDebug() << "Clearing properties panel";
  selected_item = nullptr;

  state_panel->hide();
  transition_panel->hide();
  itransition_panel->hide();

  clearIos();
  clearModelName();
}

void PropertiesPanel::clearModelName()
{
    Model* model = main_window->getModel();
    if ( model == NULL ) return;
    model_name_field->setText("");
}

void PropertiesPanel::clearIos()
{
    Model* model = main_window->getModel();
    if ( model == NULL ) return;
    QStringList ios;
    for (auto io: model->getIos())
      _removeIo(model, io);
}

