#pragma once

#include "dynamicPanel.h"

class Model;
class Iov;
class QRegularExpressionValidator;

class ModelOutps : public DynamicPanel
{
  Q_OBJECT

public:
  ModelOutps(QString title, Model *model, QRegularExpressionValidator *name_validator);
  ~ModelOutps();

  QStringList retrieve(); // Not used here

signals:
  void modelModified();
  
private:
  Model *model;
  QRegularExpressionValidator *name_validator;
  QHash<QWidget*, Iov*> widgetToIo; 

protected slots:
  void addRowFields(QHBoxLayout *row_layout, QString &v);
  void deleteRowFields(QHBoxLayout *row_layout);
  void nameEdited();
  void typeEdited();
};
