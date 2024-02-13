#pragma once

#include "dynamicPanel.h"

class Model;
class Iov;

class ModelVars : public DynamicPanel
{
  Q_OBJECT

public:
  ModelVars(QString title, Model *model);
  ~ModelVars();

  QStringList retrieve(); // Not used here

private:
  Model *model;
  QHash<QWidget*, Iov*> widgetToIo; 

protected slots:
  void addRowFields(QHBoxLayout *row_layout, QString &v);
  void deleteRowFields(QHBoxLayout *row_layout);
  // void rowEdited(); // Not used here
  void nameEdited();
  void typeEdited();
};
