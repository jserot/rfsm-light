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


#ifndef DotViewer_H
#define DotViewer_H

#include <QGraphicsView>
#include "QGVScene.h"
#include "model.h"

class DotViewer : public QGraphicsView
{
  Q_OBJECT

public:
  DotViewer(Model *model, int width, int height, QWidget *parent);
  ~DotViewer();

private:
  QGVScene *scene;
};

#endif
