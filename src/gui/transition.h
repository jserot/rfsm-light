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

#ifndef TRANSITION_H
#define TRANSITION_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSimpleTextItem>
#include "state.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Transition : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 4 };

    Transition(State *srcState, State *dstState,
               QString event, QString guards, QString actions,
               State::Location location, QGraphicsItem *parent=0);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    State *srcState() const { return mySrcState; }
    State *dstState() const { return myDstState; }
    QString getEvent() const { return myEvent; }
    QString getGuard() const { return myGuard; }
    QString getActions() const { return myActions; }
    QString getLabel();
    void setSrcState(State *s) { mySrcState = s; }
    void setDstState(State *s) { myDstState = s; }
    void setEvent(QString s) { myEvent = s; }
    void setGuard(QString s) { myGuard = s; }
    void setActions(QString s) { myActions = s; }
    State::Location location() const { return myLocation; }
    bool isInitial();

    void updatePosition();

    QString toString();
    
    static QColor selectedColor;
    static QColor unSelectedColor;
    static double arrowSize;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    State *mySrcState;
    State *myDstState;
    QString myEvent;
    QString myGuard;
    QString myActions;
    QPolygonF arrowHead;
    QGraphicsSimpleTextItem *myLabel;
    State::Location myLocation;
};

#endif // TRANSITION_H
