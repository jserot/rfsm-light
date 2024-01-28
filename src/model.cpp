/***********************************************************************/
/*                                                                     */
/*       This file is part of the RFSM Light software package          */
/*                                                                     */
/*  Copyright (c) 2019-present, Jocelyn SEROT (jocelyn.serot@uca.fr)   */
/*                       All rights reserved.                          */
/*                                                                     */
/*    This source code is licensed under the license found in the      */
/*      LICENSE file in the root directory of this source tree.        */
/*                                                                     */
/***********************************************************************/

#include "model.h"
#include "transition.h"
#include "include/nlohmann_json.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#ifdef USE_QGV
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#endif

QString Model::statePrefix = "S";
int Model::stateCounter = 0;
QColor Model::lineColor = Qt::lightGray;
QColor Model::boxColor = Qt::black;

Model::Model(QWidget *parent)
    : QGraphicsScene(parent)
{
    mode = SelectItem;
    mainWindow = parent;
}

void Model::setMode(Mode mode)
{
    this->mode = mode;
}

Model::Mode Model::getMode(void)
{
  return mode;
}

Iov* Model::addIo(const QString name, const Iov::IoKind kind, const Iov::IoType type, const Stimulus stim)
{
  qDebug () << "Model::addIo" << name << kind << type << stim.toString() ;
  Iov *io = new Iov(name, kind, type, stim);
  ios.append(io);
  return io;
}

void Model::removeIo(Iov *io)
{
  ios.removeOne(io);
}

void Model::clear(void)
{
  name = "";
  ios.clear();
  stateCounter = 0;
  QGraphicsScene::clear();
}

void Model::addState(State *state)
{
  state->setBrush(boxColor);
  addItem(state);
}

State* Model::addState(QPointF pos, QString id, QString attr)
{
  State* state = new State(id, attr);
  state->setPos(pos);
  addState(state);
  return state;
}

State* Model::addPseudoState(QPointF pos)
{
  State* state = new State(); // Pseudo-state
  state->setPos(pos);
  addState(state);
  return state;
}

QList<State*> Model::states()
{
  QList<State*> states;
  for ( const auto item: items() )
    if ( item->type() == State::Type )
      states.append(qgraphicsitem_cast<State *>(item));
  return states;
}

QList<Transition*> Model::transitions()
{
  QList<Transition*> transitions;
  for ( const auto item: items() )
    if ( item->type() == Transition::Type )
      transitions.append(qgraphicsitem_cast<Transition *>(item));
  return transitions;
}

State* Model::initState()
{
  QList<Transition*> transitions;
  for ( const auto item: items() )
    if ( item->type() == Transition::Type ) {
      Transition *t = qgraphicsitem_cast<Transition *>(item);
      if ( t->isInitial() ) return t->getDstState();
      }
  return NULL;
}

Transition* Model::initTransition()
{
  QList<Transition*> transitions;
  for ( const auto item: items() )
    if ( item->type() == Transition::Type ) {
      Transition *t = qgraphicsitem_cast<Transition *>(item);
      if ( t->isInitial() ) return t;
      }
  return NULL;
}


State* Model::getState(QString id)
{
  foreach ( State* s, states() )
    if ( s->getId() == id ) return s;
  return NULL;
}

QStringList Model::getInputs()
{
  QStringList r;
  for ( const auto io : ios )
    if ( io->kind == Iov::IoIn ) r.append(io->name);
  return r;
}

QStringList Model::getInpEvents()
{
  QStringList r;
  for ( const auto io : ios )
    if ( io->kind == Iov::IoIn && io->type == Iov::TyEvent ) r.append(io->name);
  return r;
}

QStringList Model::getInpNonEvents()
{
  QStringList r;
  for ( const auto io : ios )
    if ( io->kind == Iov::IoIn && io->type != Iov::TyEvent ) r.append(io->name);
  return r;
}

QStringList Model::getOutputs()
{
  QStringList r;
  for ( const auto io : ios )
    if ( io->kind == Iov::IoOut ) r.append(io->name);
  return r;
}

QStringList Model::getVars()
{
  QStringList r;
  for ( const auto io : ios )
    if ( io->kind == Iov::IoVar ) r.append(io->name);
  return r;
}

bool Model::hasPseudoState()
{
  foreach ( State* s, states() )
    if ( s->isPseudo() ) return true;
  return false;
}

void Model::addTransition(Transition *transition)
{
  State *srcState = transition->getSrcState();
  State *dstState = transition->getDstState();
  srcState->addTransition(transition);
  if ( dstState != srcState ) dstState->addTransition(transition); // Do _not_ add self-transitions twice !
  transition->setZValue(-1000.0);
  addItem(transition);
}

Transition* Model::addTransition(State* srcState,
                               State* dstState,
                               QString event,
                               QStringList guards,
                               QStringList actions,
                               State::Location location)
{
  Transition *transition = new Transition(srcState, dstState, event, guards, actions, location);
  addTransition(transition);
  return transition;
}

bool Model::event(QEvent *event)
{
  //qDebug() << "Got event " << event->type();
  switch ( event->type() ) {
    // Note. The [Enter] and [Leave] events cannot be handled by the model itself
    // because the associated action [setCursor] can only be applied to the _enclosing_ view...
    // This workaround uses signals to delegate 
    case QEvent::Enter:
      emit mouseEnter();
      return true;
    case QEvent::GraphicsSceneLeave:
      emit mouseLeave();
      return true;
    default:
        break;
    }
  return QGraphicsScene::event(event);
}

void Model::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton) return;
    State *state;
    Transition *transition;
    QGraphicsItem *item;
    switch ( mode ) {
        case InsertState:
          state = addState(mouseEvent->scenePos(), statePrefix + QString::number(stateCounter++), "");
            emit stateInserted(state);
            emit fsmModified();
            break;
        case InsertPseudoState:
          if ( ! hasPseudoState() ) {
            startState = addPseudoState(mouseEvent->scenePos());
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            line->setPen(QPen(lineColor, 2));
            addItem(line);
            emit fsmModified();
            }
          else
            QMessageBox::warning(mainWindow, "Error",
                                 "There's alreay one initial transition !\nDelete it first to add another one");
          break;
        case InsertTransition:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            line->setPen(QPen(lineColor, 2));
            addItem(line);
            emit fsmModified();
            break;
        case InsertLoopTransition:
          item = itemAt(mouseEvent->scenePos(), QTransform());
          if ( item != NULL && item->type() == State::Type ) {
            state = qgraphicsitem_cast<State *>(item);
            if ( ! state->isPseudo() ) {
              State::Location location = state->locateEvent(mouseEvent);
              Transition *transition = addTransition(state, state, "", QStringList(), QStringList(), location);
              transition->updatePosition();
              emit fsmModified();
              emit transitionInserted(transition);
              }
            }
            break;
        case DeleteItem:
          item = itemAt(mouseEvent->scenePos(), QTransform());
          if ( item != NULL ) {
            switch ( item->type() ) {
            case Transition::Type: {
              transition = qgraphicsitem_cast<Transition *>(item);
              qDebug() << "Deleting transition" << transition->toString();
              State *srcState = transition->getSrcState();
              State *dstState = transition->getDstState();
              if ( transition->isInitial() ) {
                srcState->removeTransitions();
                removeItem(srcState);
                }
              else {
                srcState->removeTransition(transition);
                dstState->removeTransition(transition);
                removeItem(item);
                delete item;
                }
              emit transitionDeleted(transition);
              emit fsmModified();
              }
              break;
            case State::Type:
              state = qgraphicsitem_cast<State *>(item);
              qDebug() << "Deleting state" << state->getId();
              state->removeTransitions();
              removeItem(item);
              emit stateDeleted(state);
              emit fsmModified();
              delete item;
              break;
            default:
              break;
            }
          }
          break;
        case SelectItem:
          item = itemAt(mouseEvent->scenePos(), QTransform());
          if ( item != NULL ) {
            switch ( item->type() ) {
              case State::Type:
                emit(stateSelected(qgraphicsitem_cast<State *>(item)));
                break;
              case Transition::Type:
                emit(transitionSelected(qgraphicsitem_cast<Transition *>(item)));
                break;
              default:
                break;
              }
            }
          else {
            emit(nothingSelected());
            }
          QGraphicsScene::mousePressEvent(mouseEvent);
          break;
       }
}

void Model::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if ( (mode == InsertTransition || mode == InsertPseudoState) && line != 0 ) {
    QLineF newLine(line->line().p1(), mouseEvent->scenePos());
    line->setLine(newLine);
    }
  else if (mode == SelectItem) {
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Model::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if ( line != 0 && (mode == InsertTransition || mode == InsertPseudoState) ) {
    QList<QGraphicsItem *> srcStates = items(line->line().p1());
    if (srcStates.count() && srcStates.first() == line) srcStates.removeFirst();
    QList<QGraphicsItem *> dstStates = items(line->line().p2());
    if (dstStates.count() && dstStates.first() == line) dstStates.removeFirst();
    removeItem(line);
    delete line;
    if (srcStates.count() > 0 && dstStates.count() > 0 &&
        srcStates.first()->type() == State::Type &&
        dstStates.first()->type() == State::Type) {
      State *srcState = qgraphicsitem_cast<State *>(srcStates.first());
      State *dstState = qgraphicsitem_cast<State *>(dstStates.first());
      if ( srcState != dstState ) {
        State::Location location = srcState == dstState ? srcState->locateEvent(mouseEvent) : State::None;
        Transition *transition = addTransition(srcState, dstState, "", QStringList(), QStringList(), location);
        transition->updatePosition();
        emit transitionInserted(transition);
        emit fsmModified();
        }
      }
    else if ( mode == InsertPseudoState && startState != NULL ) {
      // An initial pseudo-state has been created but not connected
      removeItem(startState);
      delete startState;
      }
    }
  line = 0;
  QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool Model::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

// Basic model checking

void Model::report_error(QString msg)
{
  QMessageBox::warning(mainWindow, "", msg);
}

void Model::check_state(State *s)
{
  Q_ASSERT(states().contains(s)); 
}

void Model::check_transition(Transition *t)
{
  check_state(t->getSrcState());
  check_state(t->getDstState());
  if ( ! t->isInitial() && t->getEvent() == "" )
    report_error("No trigerring event for transition " + t->toString());
}

bool Model::check_model(bool withStimuli)
{
  if ( name.isEmpty() ) {
    report_error("No name specified for model");
    return false;
    }
  if ( ! hasPseudoState() ) {
    report_error("No initial state specified");
    return false;
    }

  QStringList inpEvents = getInpEvents();
  if ( inpEvents.length() == 0 ) {
    report_error("There should be at least one input with type event");
    return false;
    }

  if ( withStimuli ) {
    for( Iov* io : ios ) {
      if ( io->kind == Iov::IoIn &&  io->stim.kind == Stimulus::None )  {
        report_error("No stimulus for input " + io->name);
        return false;
        }  
      }
    }
  return true;
}

// Reading and saving

void Model::readFromFile(QString fname)
{
    QFile file(fname);
    qDebug() << "Reading model from file" << file.fileName();
    file.open(QIODevice::ReadOnly);
    if ( file.error() != QFile::NoError ) {
      QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
      return;
      }
    QTextStream is(&file);
    QString txt = is.readAll();
  
    auto json = nlohmann::json::parse(txt.toStdString()); 

    // We cannot directly update the model, because an error can occur when reading the JSON file !
    // Instead, we build lists of IOs, states and transitions by parsing the JSON  file ...

    QString name = QString::fromStdString(json.at("name"));

    QList<Iov *> ios;
    for ( const auto & json_io : json.at("ios") ) {
      std::string name = json_io.at("name");
      std::string kind = json_io.at("kind");
      std::string type = json_io.at("type");
      std::string stim = json_io.at("stim");
      Iov *io = new Iov(QString::fromStdString(name),
                        Iov::ioKindOfString(QString::fromStdString(kind)),
                        Iov::ioTypeOfString(QString::fromStdString(type)),
                        Stimulus(QString::fromStdString(stim)));
      ios.append(io);
      }

    QMap<std::string, State*> states;
    for ( const auto & json_state : json.at("states") ) {
      std::string id = json_state.at("id");
      std::string attr = json_state.at("attr");
      qreal x =  json_state.at("x");
      qreal y =  json_state.at("y");
      State* state;
      if ( id == State::initPseudoId.toStdString() )
        state = new State(QPointF(x,y));
      else 
        state = new State(QString::fromStdString(id),
                          QString::fromStdString(attr),
                          QPointF(x,y));
      states.insert(id, state);
      }   

    QList<Transition *> transitions;
    for ( const auto & json_transition : json.at("transitions") ) {
      std::string src_state = json_transition.at("src_state");
      std::string dst_state = json_transition.at("dst_state");
      std::string event = json_transition.at("event");
      std::string guards = json_transition.at("guard");
      std::string actions = json_transition.at("actions");
      State::Location location;
      switch ( (int)json_transition.at("location") ) {
        case 1: location = State::North; break;
        case 2: location = State::South; break;
        case 3: location = State::East; break;
        case 4: location = State::West; break;
        default: location = State::None; break;
        }
      if ( ! states.contains(src_state) || ! states.contains(dst_state) )
        throw std::invalid_argument("Model::fromString: invalid state id");
      State *srcState = states.value(src_state);
      State *dstState = states.value(dst_state);
      Transition *transition = new Transition(srcState,
                                              dstState,
                                              QString::fromStdString(event),
                                              QString::fromStdString(guards).split(","),
                                              QString::fromStdString(actions).split(","),
                                              location);
      transitions.append(transition);
      }

    // ... and, if (and only if) parsing succeeds, we update the model.

    clear();
    this->name = name;

    foreach ( Iov* io, ios) {
      qDebug () << "Model::readFromFile: adding IO" << io->name << io->kind << io->type << io->stim.toString() ;
      this->ios.append(io);
      }
    foreach ( State *state, states.values() ) {
      qDebug () << "Model::readFromFile: adding state" << state->getId();
      addState(state);
      }
    foreach ( Transition *transition, transitions ) {
      qDebug () << "Model::readFromFile: adding transition" << transition->getSrcState()->getId() << " -> " << transition->getDstState()->getId();
      addTransition(transition);
      transition->updatePosition();
      }
    qDebug() << "Done";
}

void Model::saveToFile(QString fname)
{
    QFile file(fname);
    qDebug() << "Saving model to file" << file.fileName();
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if ( file.error() != QFile::NoError ) {
      QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
      return;
      }

    nlohmann::json json_res;

    json_res["name"] = name.toStdString();

    json_res["ios"] = nlohmann::json::array();
    int cnt = 1;
    for ( const Iov* io: ios ) {
      nlohmann::json json;
      if ( io->name == "" ) {
        QMessageBox::warning(mainWindow, "Warning", tr("IO #%1").arg(cnt) + "has no name. Ignoring it");
        continue;
        }
      json["name"] = io->name.toStdString(); 
      json["kind"] = Iov::stringOfKind(io->kind).toStdString(); 
      json["type"] = Iov::stringOfType(io->type).toStdString(); 
      json["stim"] = io->stim.toString().toStdString(); 
      json_res["ios"].push_back(json);
      }

    json_res["states"] = nlohmann::json::array();
    for ( const auto item: items() ) {
      if ( item->type() == State::Type ) {
        State* state = qgraphicsitem_cast<State *>(item);
        nlohmann::json json;
        json["id"] = state->getId().toStdString(); 
        json["attr"] = state->getAttr().toStdString(); 
        json["x"] = state->scenePos().x(); 
        json["y"] = state->scenePos().y(); 
        json_res["states"].push_back(json);
        }
      }

    json_res["transitions"] = nlohmann::json::array();
    for ( const auto item: items() ) {
      if ( item->type() == Transition::Type ) {
        Transition* transition = qgraphicsitem_cast<Transition *>(item);
        nlohmann::json json;
        json["src_state"] = transition->getSrcState()->getId().toStdString();
        json["dst_state"] = transition->getDstState()->getId().toStdString();
        json["event"] = transition->getEvent().toStdString();
        json["guard"] = transition->getGuards().join(",").toStdString(); // Use "," as separator for compatibility with existing .fsd files
        json["actions"] = transition->getActions().join(",").toStdString(); // Use "," as separator for compatibility with existing .fsd files
        json["location"] = transition->getLocation();
        json_res["transitions"].push_back(json);
        }
      }
    QTextStream os(&file);
    os << QString::fromStdString(json_res.dump(2));
    file.close();
    qDebug () << "Done";
}

// DOT export

QString dotTransitionLabel(QString label, QString lrpad="")
{
  QStringList l = label.split("/");
  if ( l.length() != 2 ) return label;
  int n = std::max(l.at(0).length(), l.at(1).length());
  return lrpad + l.at(0) + lrpad
       + "\n" + lrpad + QString(n, '_') + lrpad  + "\n"
       + lrpad + l.at(1) + lrpad;
}

QString stringOfIos(QList<Iov*> ios)
{
  QString r;
  foreach ( Iov* io, ios)
    r += io->toString() + "\\l";
  return r;
}

void Model::exportDot(QString fname, QStringList options)
{
  QFile file(fname);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  if ( file.error() != QFile::NoError ) {
    QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
    return;
  }
  QTextStream os(&file);

  QString name = this->name.isEmpty() ? "main" : this->name;
  os << "digraph " << name << " {\n";
  os << "layout = dot\n";
  os << "rankdir = UD\n";
  os << "size = \"8.5,11\"\n";
  os << "center = 1\n";
  os << "nodesep = \"0.350000\"\n";
  os << "ranksep = \"0.400000\"\n";
  os << "fontsize = 14\n";
  os << "mindist=1.0\n";
  bool withIoDesc = ! options.contains("-dot_no_captions");
  if ( withIoDesc ) 
    os << "_ios [label=\"" << stringOfIos(ios) << "\", shape=rect, style=solid]\n";
  for ( const auto item: items() ) {
    if ( item->type() == State::Type ) {
      State* state = qgraphicsitem_cast<State *>(item);
      QString id = state->getId();
      if ( state->isPseudo() ) {
        os << id << " [shape=point]\n";
        if ( withIoDesc ) 
          os << "_ios -> " << id << " [style=\"invis\"]\n";
        }
      else {
        QString attr = state->getAttr();
        os << id << " [label=\"" << id << "\\n" << attr <<  "\", shape=circle, style=solid]\n";
        }
      }
    }
  for ( const auto item: items() ) {
    if ( item->type() == Transition::Type ) {
      Transition* transition = qgraphicsitem_cast<Transition *>(item);
      QString src_id = transition->getSrcState()->getId();
      QString dst_id = transition->getDstState()->getId();
      QString label = dotTransitionLabel(transition->getLabel());
      os << src_id << " -> " << dst_id << " [label=\"" << label << "\"]\n";
      }
    }
  os << "}\n";
  file.close();
}

// RFSM export

QString stringOfVarList(QList<Iov*> vs, QString sep=", ", bool withType=true) 
{
  QStringList rs;
  for (const auto& v : vs) {
    if ( withType )
      rs << v->name + ": " + Iov::stringOfType(v->type);
    else
      rs << v->name;
    }
  return rs.join(sep);
}

QString stringOfTransition(Transition *t, bool abbrev=false)
{
  QString ss;
  ss = t->getSrcState()->getId() + " -> " + t->getDstState()->getId();
  if ( ! abbrev ) {
    if ( ! t->getEvent().isEmpty() ) ss += " on " + t->getEvent();
    if ( ! t->getGuards().isEmpty() ) ss += " when " + t->getGuards().join(".");
    if ( ! t->getActions().isEmpty() ) ss += " with " + t->getActions().join(",");
    }
  return ss;
}

QString stringOfState(State *s)
{
  QString ss;
  ss = s->getId();
  if ( s->getAttr() != "" ) 
    ss += " where " + s->getAttr();
  return ss;
}


void Model::export_rfsm_model(QTextStream& os)
{
    QString indent = QString(2, ' ');

    QList<Iov*> gios, lvars;

    for ( const auto io : ios ) {
      if ( io->kind == Iov::IoVar ) lvars.append(io);
      else gios.append(io);
      }

    bool first;
    os << "fsm model " << name << "(";
    if ( gios.length() > 0 ) {
      os << "\n";
      first = true;
      for(const auto io : gios) {
            if(!first) os << "," << "\n";
            os << indent;
            os << Iov::stringOfKind(io->kind) << " " << io->name << ": " << Iov::stringOfType(io->type);
            first = false;
        }
      os << "\n" << indent <<  ")" << "\n";
      }
    else
      os << ")" << "\n";

    os << "{" << "\n";

    os << indent << "states: ";
    first = true;
    for ( State* state : states()) {
        if ( state->isPseudo() ) continue;
        if(!first) os << ", " ;
        os << stringOfState(state);
        first = false;
        }
    os << ";" << "\n";

    if(!lvars.isEmpty()) 
      os << indent << "vars: " << stringOfVarList(lvars) <<  ";" << "\n";

    os << indent;
    os << "trans: ";
    first = true;

    for ( Transition* transition : transitions() ) {
        if ( transition->getSrcState()->isPseudo() ) continue;
        os << indent << "\n  | " << stringOfTransition(transition);
        first = false;
        }
    os << ";" << "\n";

    State* iState = initState();
    Transition* iTransition = initTransition();
    if ( iState == NULL ) throw std::invalid_argument("Initial state undefined");
    if ( iTransition == NULL ) throw std::invalid_argument("Initial transition undefined");
    os << indent << "itrans: " << "\n";
    os << indent << "| -> " << iState->getId();
    if ( ! iTransition->getActions().isEmpty() ) os << " with " << iTransition->getActions().join(";");
    os << ";" << "\n";
    os << "}";
}

QString export_rfsm_stim(Stimulus &st) 
// We cannot use Stimulus::toString() since the syntax is different :(
{
  QString r;
  switch ( st.kind ) {
  case Stimulus::None: return "";
  case Stimulus::Periodic:
    r = "periodic(";
    r +=      QString::number(st.desc.periodic.period);
    r += "," + QString::number(st.desc.periodic.start_time);
    r += "," + QString::number(st.desc.periodic.end_time) + ")";
    break;
  case Stimulus::Sporadic:
    r = "sporadic(";
    for ( const auto t: st.desc.sporadic.dates ) {
      r += " " + QString::number(t);
      r += ",";
      }
    if ( r.endsWith(",") ) r.chop(1);
    r += ")";
    break;
  case Stimulus::ValueChanges: 
    r = "value_changes(";
    for ( const QPair<int,int> &vc: st.desc.valueChanges.vcs ) {
      r += QString::number(vc.first) + ":" + QString::number(vc.second);
      r += ",";
      }
    if ( r.endsWith(",") ) r.chop(1);
    r += ")";
    break;
  }
  return r;
}
void Model::export_rfsm_testbench(QTextStream& os)
{
    QList<Iov*> gios;
    for ( const auto io : ios ) {
      if ( io->kind == Iov::IoIn ) {
        QString ss = export_rfsm_stim(io->stim);
        if ( ss != "" )  {
          os << "input " << io->name << " : " << Iov::stringOfType(io->type) << " = " << ss;;
          gios.append(io);
          os << "\n";
          }
        else {
          QMessageBox::warning(mainWindow, "","No stimulus for input " + io->name);
          return;
          }
        }
      else if ( io->kind == Iov::IoOut ) {
        os << "output " << io->name << " : " << Iov::stringOfType(io->type);
        gios.append(io);
        os << "\n";
        }
      }
    os << "\n";
    os << "fsm " << name << " = " << name << "(";
    bool first = true;
    for(const auto io : gios) {
      if ( !first ) os << ", ";
      os << io->name;
      first = false;
      }
    os << ")";
}

void Model::exportRfsm(QString fname, bool withTestbench)
{
  QFile file(fname);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  if ( file.error() != QFile::NoError ) {
    QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
    return;
  }
  QTextStream os(&file);
  export_rfsm_model(os);
  if ( withTestbench ) {
      os << "\n\n";
      export_rfsm_testbench(os);
      }
  file.close();
}

#ifdef USE_QGV
// Direct DOT rendering using QGV library (since 1.3.0)

void Model::renderDot(QGVScene *dotScene)
{
  dotScene->setGraphAttribute("rankdir", "UD");
  dotScene->setGraphAttribute("nodesep", "0.55");
  dotScene->setGraphAttribute("ranksep", "0.95");
  dotScene->setGraphAttribute("fontsize", "14");
  dotScene->setGraphAttribute("mindist", "1.0");
  dotScene->setNodeAttribute("shape", "circle");
  dotScene->setNodeAttribute("style", "solid");

  QMap<QString,QGVNode*> nodes;

  for ( const auto item: items() ) {
    if ( item->type() == State::Type ) {
      State* state = qgraphicsitem_cast<State *>(item);
      QString id = state->getId();
      QGVNode *node = dotScene->addNode(id);
      if ( state->isPseudo() ) {
        node->setAttribute("shape", "none"); 
        node->setAttribute("label", "");
        }
      nodes.insert(id,node);
      }
    }
  for ( const auto item: items() ) {
    if ( item->type() == Transition::Type ) {
      Transition* transition = qgraphicsitem_cast<Transition *>(item);
      QString src_id = transition->getSrcState()->getId();
      QString dst_id = transition->getDstState()->getId();
      QString label = transition->isInitial() ? "" : dotTransitionLabel(transition->getLabel(),"  ");
      if ( nodes.contains(src_id) && nodes.contains(dst_id) )
        dotScene->addEdge(nodes[src_id], nodes[dst_id], label);
    }
  }
}
#endif
