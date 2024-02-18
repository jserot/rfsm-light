#pragma once

#include "dynamicPanel.h"

class TransitionGuards : public DynamicPanel
{
  Q_OBJECT

public:
  TransitionGuards(QString title, QStringList& guards);
  ~TransitionGuards();

  QStringList retrieve();

protected slots:
  void addRowFields(QHBoxLayout *row_layout, void *row_data);
  void deleteRowFields(QHBoxLayout *row_layout);
  // void rowEdited();
};
