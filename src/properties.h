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

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QComboBox>
#include <QFrame>
#include <QGraphicsItem>
#include <QGroupBox>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringListModel>

class MainWindow;
class State;
class Transition;

class PropertiesPanel : public QFrame
{
    Q_OBJECT

  private:
    MainWindow* main_window;

    QGraphicsItem* selected_item;

    QGroupBox* model_panel;
    QLineEdit* model_name_field;

    QGroupBox* io_panel;
    QLineEdit* io_name;
    QComboBox* io_kind;
    QLineEdit* io_type;
    QStringListModel* ios_model;
    QListView* ios_view;

    QGroupBox* state_panel;
    QLineEdit* state_name_field;

    QGroupBox* transition_panel;
    QComboBox* transition_start_state_field;
    QComboBox* transition_end_state_field;
    QLineEdit* transition_event_field;
    QLineEdit* transition_guard_field;
    QLineEdit* transition_actions_field;

    QGroupBox* itransition_panel;
    QComboBox* itransition_end_state_field;
    QLineEdit* itransition_actions_field;

  public:
    explicit PropertiesPanel(MainWindow* parent);
    ~PropertiesPanel();

    void unselectItem();
    void setSelectedItem(State* state);
    void setSelectedItem(Transition* transition);

    void toggleStimuliPanel();                                                    

    QSize sizeHint() const { return QSize(275,300); }; 

  public slots:
    void setModelName(const QString& name);

    void setStateName(const QString& name);

    void addIo();
    void removeIos();
    void editIos();

    void setTransitionSrcState(int index);
    void setTransitionDstState(int index);
    void setTransitionEvent(const QString& event);
    void setTransitionGuard(const QString& guard);
    void setTransitionActions(const QString& actions);
    void setITransitionDstState(int index);

    void clear();

  private:
    void createModelPanel();
    void createIoPanel();
    void createStatePanel();
    void createTransitionPanel();
    void createInitTransitionPanel();

    void fillIoList();
    void fillStimuliList();
    void fillModelName();
};

#endif
