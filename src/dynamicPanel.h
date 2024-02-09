#pragma once

#include <QGroupBox>
#include <QMap>

class QPushButton;
class QBoxLayout;
class QVBoxLayout;
class QHBoxLayout;

class DynamicPanel : public QGroupBox
{
  Q_OBJECT

protected:
  QVBoxLayout *layout;
  QPushButton *add_button;
  QPushButton *clear_button;
  QHash<QPushButton*, QBoxLayout*> mButtonToLayoutMap;

  void delete_row(QLayout *row_layout);

public:
  DynamicPanel(QString title);
  ~DynamicPanel();

public slots:
  void clear();
  void addRow(QString value);

signals:
    
protected slots:
  void addNewRow();
  void deleteRow();
  // The following methods must be defined in the derived classes 
protected:
  virtual void addRowFields(QHBoxLayout *row_layout, QString& v) = 0; // Fields shown _before_ the [Delete] button on the row
  virtual void rowEdited() = 0; 
public:
  virtual QStringList retrieve() = 0;
};