#include "modelInps.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QRegularExpressionValidator>

#include "model.h"
#include "iov.h"
#include "stimuli.h"

ModelInps::ModelInps(QString title, Model *model, QRegularExpressionValidator *name_validator) : DynamicPanel(title)
{
  this->model = model;
  this->name_validator = name_validator;
}

void ModelInps::addRowFields(QHBoxLayout *row_layout, QString& v)
{
  Q_UNUSED(v); // Here
  int nb_rows = row_layout->count();
  QString row_name(QString(tr("inp #%1").arg(nb_rows)));

  assert(model);
  Iov* io = model->addIo("", Iov::IoIn, Iov::TyEvent, Stimulus(Stimulus::None));

  QLineEdit *name = new QLineEdit();
  name->setObjectName(row_name);
  name->setMinimumSize(80,name->minimumHeight());
  name->setPlaceholderText("<name>");
  name->setFrame(true);
  name->setCursorPosition(0);
  name->setValidator(name_validator);
  row_layout->addWidget(name);
  widgetToIo.insert((QWidget*)name, io);
  connect(name, &QLineEdit::editingFinished, this, &ModelInps::nameEdited);

  QComboBox *typ = new QComboBox();
  typ->addItem("event", QVariant(Iov::TyEvent));
  typ->addItem("int", QVariant(Iov::TyInt));
  typ->addItem("bool", QVariant(Iov::TyBool));
  typ->setCurrentIndex(0);
  row_layout->addWidget(typ);
  widgetToIo.insert((QWidget*)typ, io);
#if QT_VERSION >= 0x060000
  connect(typ, &QComboBox::currentIndexChanged, this, &ModelInps::typeEdited);
#else
  connect(typ, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ModelInps::typeEdited);
#endif

  stim = new QComboBox();
  stim->addItem("None", QVariant(Stimulus::None));
  stim->addItem("Periodic", QVariant(Stimulus::Periodic));
  stim->addItem("Sporadic", QVariant(Stimulus::Sporadic));
  stim->addItem("ValueChanges", QVariant(Stimulus::ValueChanges));
  stim->setCurrentIndex(0);
  setStimChoices(io);
  row_layout->addWidget(stim);
#if QT_VERSION >= 0x060000
  connect(stim, &QComboBox::activated, this, &ModelInps::stimEdited);
#else
  connect(stim, QOverload<int>::of(&QComboBox::activated), this, &ModelInps::stimEdited);
#endif
  widgetToIo.insert((QWidget*)stim, io);
}

void enableComboBoxItem(QComboBox* box, int i, bool enabled)
{
  QStandardItemModel *model = qobject_cast<QStandardItemModel*>(box->model());
  Q_ASSERT(model);
  QStandardItem *item = model->item(i);
  Q_ASSERT(item);
  item->setEnabled(enabled);
}

void ModelInps::setStimChoices(Iov *io)
{
  assert(stim);
  qDebug() << "Setting stim choices for io" << io->toString();
  switch ( io->type ) {
  case Iov::TyEvent:
    enableComboBoxItem(stim, 0, true); // TO FIX: we should not use hardcoded index here 
    enableComboBoxItem(stim, 1, true);
    enableComboBoxItem(stim, 2, true);
    enableComboBoxItem(stim, 3, false);
    break;
  case Iov::TyInt:
  case Iov::TyBool:
    enableComboBoxItem(stim, 0, true); // TO FIX: we should not use hardcoded index here 
    enableComboBoxItem(stim, 1, false);
    enableComboBoxItem(stim, 2, false);
    enableComboBoxItem(stim, 3, true);
    break;
    }
}

void ModelInps::deleteRowFields(QHBoxLayout *row_layout)
{
  QLineEdit *ledit = qobject_cast<QLineEdit*>(row_layout->itemAt(0)->widget());
  assert(ledit);
  Iov* io = widgetToIo.take(ledit);
  qDebug() << "Removing IO" << io->toString();
  model->removeIo(io);
  emit modelModified();
}

void ModelInps::nameEdited()
{
  QLineEdit* ledit = qobject_cast<QLineEdit*>(sender());
  assert(ledit);
  QString name = ledit->text().trimmed();
  Iov* io = widgetToIo.value(ledit);
  assert(io);
  qDebug() << "Setting input name to" << name;
  io->name = name;
  emit modelModified();
}

void ModelInps::typeEdited()
{
  QComboBox* box = qobject_cast<QComboBox*>(sender());
  Iov* io = widgetToIo.value(box);
  assert(io);
  io->type = (Iov::IoType)(box->currentIndex());
  qDebug () << "Setting IO type: " << io->type;
  setStimChoices(io);
  emit modelModified();
}

void ModelInps::stimEdited()
{
  QComboBox* selector = qobject_cast<QComboBox*>(sender());
  Iov* io = widgetToIo.value(selector);
  assert(io);
  QString io_name = io->name;
  if ( io_name == "" ) {
    QMessageBox::warning( this, "Error", "Please give a name to this IO before editing it");
    selector->setCurrentIndex(Stimulus::None);
    return;
    }
  Stimulus::Kind kind = (Stimulus::Kind)(selector->currentIndex()); 
  switch ( kind ) {
  case Stimulus::None:
    io->stim = Stimulus(Stimulus::None);
    break;
  default:
    Stimuli* stimDialog = new Stimuli(kind,io,this);
    stimDialog->exec();
    //fillIoList();
    delete stimDialog;
    break;
    }
  emit modelModified();
}

QStringList ModelInps::retrieve()
{
  return QStringList();
}

ModelInps::~ModelInps()
{
}
