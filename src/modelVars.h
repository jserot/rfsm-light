#pragma once

#include "dynamicPanel.h"

class Model;
class Iov;
class QRegularExpressionValidator;

class ModelVars : public DynamicPanel
{
  Q_OBJECT

public:
  ModelVars(QString title, Model *model, QRegularExpressionValidator *name_validator);
  ~ModelVars();

  QStringList retrieve(); // Not used here

private:
  Model *model;
  QRegularExpressionValidator *name_validator;
  QHash<QWidget*, Iov*> widgetToIo; 

signals:
  void modelModified();

protected slots:
  void addRowFields(QHBoxLayout *row_layout, QString &v);
  void deleteRowFields(QHBoxLayout *row_layout);
  void nameEdited();
  void typeEdited();
};
