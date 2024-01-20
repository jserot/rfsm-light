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

#ifndef MODEL_H
#define MODEL_H

#include <QStringListModel>
#include <QTextStream>
#include <QGraphicsScene>

#include "state.h"
#include "iov.h"

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QColor;
class QGVScene;
QT_END_NAMESPACE

class Model : public QGraphicsScene
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

    explicit Model(QWidget *parent = 0);

    QString getName() const { return name; }
    void setName(QString n) { name = n; }

    void clear(void);

    Iov* addIo(const QString name, const Iov::IoKind kind, const Iov::IoType type, const Stimulus stim);
    void removeIo(Iov *io);
    QList<Iov*> getIos() { return ios; };
    QStringList getInpEvents();

    QList<State*> states();
    QList<Transition*> transitions();
    State* initState();
    Transition* initTransition();

    State* getState(QString id);
    bool hasPseudoState();

    void readFromFile(QString fname);
    void saveToFile(QString fname);

    bool check_model(bool withStimuli);

    void renderDot(QGVScene *scene);
    void exportDot(QString fname, QStringList options);
    void exportRfsm(QString fname, bool withTestbench = false);

public slots:
    void setMode(Mode mode);
    Mode getMode();

signals:
    void stateInserted(State *item);
    void transitionInserted(Transition *item);
    void stateDeleted(State *item);
    void transitionDeleted(Transition *item);
    void fsmModified();
    void stateSelected(State *item);
    void transitionSelected(Transition *item);
    void nothingSelected();
    void mouseEnter(void);
    void mouseLeave(void);

protected:
    bool event(QEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    void check_state(State* s);
    void check_transition(Transition *t);
    void report_error(QString msg);

    void export_rfsm_model(QTextStream& os);
    void export_rfsm_testbench(QTextStream& os);
    
    static int stateCounter;
    static QString statePrefix;

private:
    bool isItemChange(int type);
    State* addState(QPointF pos, QString id, QString attr);
    State* addPseudoState(QPointF pos);
    Transition* addTransition(State* srcState, State* dstState,
                              QString event, QString guard, QString actions,
                              State::Location location);

    QString name;
    QList<Iov*> ios; // I/Os and local variables - TODO: split this ?
    
    Mode mode;
    QGraphicsLineItem *line;  // Line being drawn
    State *startState;

    QWidget *mainWindow;

    static QColor lineColor;
    static QColor boxColor;
};

#endif // MODEL_H
