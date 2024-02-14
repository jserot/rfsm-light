#pragma once

#include "dynamicPanel.h"

class Model;
class Iov;
class QRegularExpressionValidator;
class QComboBox;

class ModelInps : public DynamicPanel
{
  Q_OBJECT

public:
  ModelInps(QString title, Model *model, QRegularExpressionValidator *name_validator);
  ~ModelInps();

  QStringList retrieve(); // Not used here

signals:
  void modelModified();
  
private:
  Model *model;
  QComboBox *stim;

  QRegularExpressionValidator *name_validator;
  QHash<QWidget*, Iov*> widgetToIo; 

  void setStimChoices(Iov *io);

protected slots:
  void addRowFields(QHBoxLayout *row_layout, QString &v);
  void deleteRowFields(QHBoxLayout *row_layout);
  void nameEdited();
  void typeEdited();
  void stimEdited();
};
