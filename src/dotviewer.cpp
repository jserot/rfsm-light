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


#include <QtWidgets>
#include "dotviewer.h"

DotViewer::DotViewer(Model *model, int width, int height, QWidget *parent) : QGraphicsView(parent)
{
  scene = new QGVScene("DOT", this);
  scene->setSceneRect(QRectF(0, 0, width, height));
  setScene(scene);
  setMinimumWidth(200); // TOFIX : use app-level cst here
  setMinimumHeight(400); // TOFIX : use app-level cst here
  model->renderDot(scene);
  setWhatsThis("dotView");
  scene->applyLayout();
  //view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
  ensureVisible(scene->itemsBoundingRect());
}

DotViewer::~DotViewer()
{
  if ( scene ) delete scene;
}
