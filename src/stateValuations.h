#pragma once

#include "dynamicPanel.h"

class StateValuations : public DynamicPanel
{
  Q_OBJECT

public:
  StateValuations(QString title, QStringList& valuations);
  ~StateValuations();

  QStringList retrieve();

protected slots:
  void addRowFields(QHBoxLayout *row_layout, QString &v);
  void deleteRowFields(QHBoxLayout *row_layout);
  // void rowEdited();
};
