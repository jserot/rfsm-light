#pragma once

#include "dynamicPanel.h"

class TransitionActions : public DynamicPanel
{
  Q_OBJECT

public:
  TransitionActions(QString title, QStringList& actions);
  ~TransitionActions();

  QStringList retrieve();

protected slots:
  void addRowFields(QHBoxLayout *row_layout, void *v);
  void deleteRowFields(QHBoxLayout *row_layout);
  // void rowEdited();
};
