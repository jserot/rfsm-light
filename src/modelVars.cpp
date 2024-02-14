#include "modelVars.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

#include "model.h"
#include "iov.h"

ModelVars::ModelVars(QString title, Model *model, QRegularExpressionValidator *name_validator) : DynamicPanel(title)
{
  this->model = model;
  this->name_validator = name_validator;
}

void setComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled)
{
    auto * model = qobject_cast<QStandardItemModel*>(comboBox->model());
    assert(model);
    if(!model) return;
    auto * item = model->item(index);
    assert(item);
    if(!item) return;
    item->setEnabled(enabled);
}

void ModelVars::addRowFields(QHBoxLayout *row_layout, QString& v)
{
  Q_UNUSED(v); // Here
  int nb_rows = row_layout->count();
  QString row_name(QString(tr("var #%1").arg(nb_rows)));

  assert(model);
  Iov* io = model->addIo("", Iov::IoVar, Iov::TyInt, Stimulus(Stimulus::None));

  QLineEdit *name = new QLineEdit();
  name->setObjectName(row_name);
  name->setMinimumSize(80,name->minimumHeight());
  name->setPlaceholderText("<name>");
  name->setFrame(true);
  name->setCursorPosition(0);
  name->setValidator(name_validator);
  row_layout->addWidget(name);
  widgetToIo.insert((QWidget*)name, io);
  connect(name, &QLineEdit::editingFinished, this, &ModelVars::nameEdited);

  QComboBox *typ = new QComboBox();
  typ->addItem("event", QVariant(Iov::TyEvent));
  typ->addItem("int", QVariant(Iov::TyInt));
  typ->addItem("bool", QVariant(Iov::TyBool));
  setComboBoxItemEnabled(typ, 0, false); // No event type for variables
  typ->setCurrentIndex(1);
  row_layout->addWidget(typ);
  widgetToIo.insert((QWidget*)typ, io);
#if QT_VERSION >= 0x060000
  connect(typ, &QComboBox::currentIndexChanged, this, &ModelVars::typeEdited);
#else
  connect(typ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ModelVars::typeEdited);
#endif
}

void ModelVars::deleteRowFields(QHBoxLayout *row_layout)
{
  QLineEdit *ledit = qobject_cast<QLineEdit*>(row_layout->itemAt(0)->widget());
  assert(ledit);
  Iov* io = widgetToIo.take(ledit);
  qDebug() << "Removing IO" << io->toString();
  model->removeIo(io);
  emit modelModified();
}

void ModelVars::nameEdited()
{
  QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
  assert(ledit);
  QString name = ledit->text().trimmed();
  Iov* var = widgetToIo.value(ledit);
  assert(var);
  qDebug() << "Setting variable name to" << name;
  var->name = name;
  emit modelModified();
}

void ModelVars::typeEdited()
{
  QComboBox* box = qobject_cast<QComboBox*>(sender());
  Iov* io = widgetToIo.value(box);
  assert(io);
  io->type = (Iov::IoType)(box->currentIndex());
  qDebug () << "Setting IO type: " << io->type;
  emit modelModified();
}

QStringList ModelVars::retrieve()
{
  return QStringList();
}

ModelVars::~ModelVars()
{
}
