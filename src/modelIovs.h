#pragma once

#include "dynamicPanel.h"
#include "iov.h"
#include "rowDesc.h"

class Model;
class QRegularExpressionValidator;

class ModelIovs : public DynamicPanel
{
  Q_OBJECT

public:
  ModelIovs(Iov::IoKind kind, QString title, Model *model, QRegularExpressionValidator *name_validator);
  ~ModelIovs();

  QStringList retrieve(); // Not used here

signals:
  void modelModified();
  
private:
  Iov::IoKind kind; // Input, output or variable
  Model *model;
  QRegularExpressionValidator *name_validator;
  QHash<QWidget*,RowDesc*> widgetToRow; 

  void updateTypeChoices(RowDesc *row_desc);
  void updateStimChoices(RowDesc *row_desc);  // For inputs only

protected slots:
  void addRowFields(QHBoxLayout *row_layout, QString &v);
  void deleteRowFields(QHBoxLayout *row_layout);
  void nameChanged();
  void nameEdited();
  void typeEdited();
  void stimEdited();
};
