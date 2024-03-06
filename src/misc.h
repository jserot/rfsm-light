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

#pragma once

#include <QList>
#include <QSet>

template <typename T>
QSet<T> QListToQSet(const QList<T>& qlist)
{
#if QT_VERSION >= 0x060000
  return QSet<T> (qlist.constBegin(), qlist.constEnd());
#else
  return QSet<T>::fromList(qlist);
#endif
}

template <typename T>
QList<T> remove_duplicates(const QList<T>& l)
{
  return QListToQSet(l).values();
}


