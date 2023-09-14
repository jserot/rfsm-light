#ifndef MISC_H
#define MISC_H

#include <QList>
#include <QSet>

template <typename T>
QSet<T> QListToQSet(const QList<T>& qlist)
{
  return QSet<T> (qlist.constBegin(), qlist.constEnd());
}

template <typename T>
QList<T> remove_duplicates(const QList<T>& l)
{
  return QListToQSet(l).values();
}

#endif

