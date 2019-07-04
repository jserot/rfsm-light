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

#ifndef FSM_H
#define FSM_H

#include <QStringListModel>
#include <QTextStream>
#include <QGraphicsScene>

#include "state.h"
#include "fsmIo.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QColor;
QT_END_NAMESPACE

class Fsm : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {
      InsertState,
      InsertPseudoState,
      InsertTransition,
      InsertLoopTransition,
      SelectItem,
      DeleteItem
      };

    explicit Fsm(QWidget *parent = 0);

    QString name() const { return myName; }
    void setName(QString name) { myName = name; }

    FsmIo* addIo(const QString name, QString kind, QString type, QString desc="");
    FsmIo* getIo(QString name);
    void removeIo(QString name);
    QList<FsmIo*> ios() { return myIos.values(); };

    QList<State*> states();
    QList<Transition*> transitions();
    State* initState();
    Transition* initTransition();

    State* getState(QString id);
    bool hasPseudoState();

    void readFromFile(QString fname);
    void saveToFile(QString fname);

    void check_model(bool withStimuli);

    void exportDot(QString fname, QStringList options);
    void exportRfsm(QString fname, bool withTestbench = false);

public slots:
    void setMode(Mode mode);

signals:
    void stateInserted(State *item);
    void transitionInserted(Transition *item);
    void fsmModified();
    void stateSelected(State *item);
    void transitionSelected(Transition *item);
    void nothingSelected();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void check_state(State* s);
    void check_transition(Transition *t);

    void export_rfsm_model(QTextStream& os);
    void export_rfsm_testbench(QTextStream& os);
    
    static int stateCounter;
    static QString statePrefix;

private:
    bool isItemChange(int type);
    State* addState(QPointF pos, QString id);
    State* addPseudoState(QPointF pos);
    Transition* addTransition(State* srcState, State* dstState,
                              QString event, QString guard, QString actions,
                              State::Location location);

    QString myName;
    QMap<QString, FsmIo*> myIos; // I/Os ans local variables - TODO: split this
    
    Mode mode;
    QGraphicsLineItem *line;  // Line being drawn
    State *startState;

    QWidget *mainWindow;

    static QColor lineColor;
    static QColor boxColor;
};

#endif // FSM_H
