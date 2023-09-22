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

#include "fsm.h"
#include "transition.h"
#include "include/nlohmann_json.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QFile>
#include <QTextStream>

QString Fsm::statePrefix = "S";
int Fsm::stateCounter = 0;
QColor Fsm::lineColor = Qt::lightGray;
QColor Fsm::boxColor = Qt::black;

Fsm::Fsm(QWidget *parent)
    : QGraphicsScene(parent)
{
    mode = SelectItem;
    mainWindow = parent;
}

void Fsm::setMode(Mode mode)
{
    this->mode = mode;
}

FsmIo* Fsm::addIo(const QString name, const QString kind, const QString type)
{
    FsmIo *v = new FsmIo(name, kind, type);
    myIos.insert(name, v);
    return v;
}


FsmIo* Fsm::getIo(QString name)
{
    return myIos.contains(name) ? myIos.value(name) : NULL;
}

void Fsm::removeIo(QString name)
{
  myIos.remove(name);
}

void Fsm::clear(void)
{
  myName = "";
  myIos.clear();
  stateCounter = 0;
  QGraphicsScene::clear();
}

State* Fsm::addState(QPointF pos, QString id, QString attr)
{
  State* state = new State(id, attr);
  state->setBrush(boxColor);
  addItem(state);
  state->setPos(pos);
  return state;
}

State* Fsm::addPseudoState(QPointF pos)
{
   State* state = new State();
   state->setBrush(boxColor);
   addItem(state);
   state->setPos(pos);
   return state;
}

QList<State*> Fsm::states()
{
  QList<State*> states;
  for ( const auto item: items() )
    if ( item->type() == State::Type )
      states.append(qgraphicsitem_cast<State *>(item));
  return states;
}

QList<Transition*> Fsm::transitions()
{
  QList<Transition*> transitions;
  for ( const auto item: items() )
    if ( item->type() == Transition::Type )
      transitions.append(qgraphicsitem_cast<Transition *>(item));
  return transitions;
}

State* Fsm::initState()
{
  QList<Transition*> transitions;
  for ( const auto item: items() )
    if ( item->type() == Transition::Type ) {
      Transition *t = qgraphicsitem_cast<Transition *>(item);
      if ( t->isInitial() ) return t->dstState();
      }
  return NULL;
}

Transition* Fsm::initTransition()
{
  QList<Transition*> transitions;
  for ( const auto item: items() )
    if ( item->type() == Transition::Type ) {
      Transition *t = qgraphicsitem_cast<Transition *>(item);
      if ( t->isInitial() ) return t;
      }
  return NULL;
}


State* Fsm::getState(QString id)
{
  foreach ( State* s, states() )
    if ( s->getId() == id ) return s;
  return NULL;
}

bool Fsm::hasPseudoState()
{
  foreach ( State* s, states() )
    if ( s->isPseudo() ) return true;
  return false;
}

Transition* Fsm::addTransition(State* srcState,
                               State* dstState,
                               QString event,
                               QString guard,
                               QString actions,
                               State::Location location)
{
  Transition *transition = new Transition(srcState, dstState, event, guard, actions, location);
  srcState->addTransition(transition);
  if ( dstState != srcState ) dstState->addTransition(transition); // Do _not_ add self-transitions twice !
  transition->setZValue(-1000.0);
  addItem(transition);
  return transition;
}

void Fsm::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
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
              Transition *transition = addTransition(state, state, "", "", "", location);
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
              //qDebug() << "Deleting transition " << (long)item;
              transition = qgraphicsitem_cast<Transition *>(item);
              State *srcState = transition->srcState();
              State *dstState = transition->dstState();
              if ( srcState->isPseudo() ) {
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
              //qDebug() << "Deleting state " << (long)item;
              state = qgraphicsitem_cast<State *>(item);
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

void Fsm::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if ( (mode == InsertTransition || mode == InsertPseudoState) && line != 0 ) {
    QLineF newLine(line->line().p1(), mouseEvent->scenePos());
    line->setLine(newLine);
    }
  else if (mode == SelectItem) {
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void Fsm::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
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
        Transition *transition = addTransition(srcState, dstState, "", "", "", location);
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

bool Fsm::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

// Checking
// Now carried out externally with the [rfsmlint] tool

// void Fsm::check_state(State *s)
// {
//   if ( ! states().contains(s) ) 
//     throw std::invalid_argument(std::string("Fsm::check_state: invalid state"));
// }

// void Fsm::check_transition(Transition *t)
// {
//   check_state(t->srcState());
//   check_state(t->dstState());
//   if ( ! t->isInitial() && t->getEvent() == "" )
//     throw std::invalid_argument(std::string( "No trigerring event for transition " + t->toString().toStdString()));
// }

// void Fsm::check_model(bool withStimuli)
// {
//   if ( name().isEmpty() ) 
//     throw std::invalid_argument(std::string("No name specified for model"));
//   if ( ! hasPseudoState() )
//     throw std::invalid_argument(std::string("No initial state specified"));

//   QStringList inpEvents;
//   for(const auto io : this->myIos.values()) {
//     if ( ! io->name().at(0).isLower() )
//       throw std::invalid_argument(std::string("Identifier \"" + io->name().toStdString() + "\" does not start with a lowercase letter"));
//     if ( io->kind() == "in" && io->type() == "event" )
//       inpEvents.append(io->name());
//     }
//   if ( inpEvents.length() == 0 )
//     throw std::invalid_argument(std::string("No input event"));

//   for ( Transition* t : transitions() ) {
//       check_transition(t);
//       if (  ! t->isInitial() && ! inpEvents.contains(t->getEvent()) )
//         throw std::invalid_argument(std::string(
//           "Trigerring event for transition " + t->toString().toStdString() + " is not listed as input"));
//     }

//   if ( withStimuli ) {
//     for( FsmIo* io : myIos.values()) {
//       if ( io->kind() == "in" ) {
//         if ( io->desc().kind() == Stimulus::None ) 
//           throw std::invalid_argument(std::string("No stimulus for input " + io->name().toStdString()));
//         }
//       }
//     }
// }

// Reading and saving

void Fsm::readFromFile(QString fname)
{
    QFile file(fname);
    //qDebug() << "Opening file " << file.fileName();
    file.open(QIODevice::ReadOnly);
    if ( file.error() != QFile::NoError ) {
      QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
      return;
      }
    QTextStream is(&file);
    QString txt = is.readAll();
  
    auto json = nlohmann::json::parse(txt.toStdString()); 

    clear(); // TODO : do not do this before parsing successfully completed !!!
    
    QMap<std::string, State*> states;
    stateCounter = 0;

    myName = QString::fromStdString(json.at("name"));

    for ( const auto & json_io : json.at("ios") ) {
      std::string name = json_io.at("name");
      std::string kind = json_io.at("kind");
      std::string type = json_io.at("type");
      std::string desc = json_io.at("desc");
      addIo(QString::fromStdString(name), QString::fromStdString(kind),
            QString::fromStdString(type)); // QString::fromStdString(desc));
      }

    for ( const auto & json_state : json.at("states") ) {
      std::string id = json_state.at("id");
      std::string attr = json_state.at("attr");
      State* state;
      if ( id == State::initPseudoId.toStdString() )
        state = addPseudoState(QPointF(json_state.at("x"), json_state.at("y")));
      else 
        state = addState(QPointF(json_state.at("x"),
                                 json_state.at("y")),
                         QString::fromStdString(id),
                         QString::fromStdString(attr));
      states.insert(id, state);
      stateCounter++;
      }   

    for ( const auto & json_transition : json.at("transitions") ) {
      std::string src_state = json_transition.at("src_state");
      std::string dst_state = json_transition.at("dst_state");
      std::string event = json_transition.at("event");
      std::string guard = json_transition.at("guard");
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
        throw std::invalid_argument("Fsm::fromString: invalid state id");
      State *srcState = states.value(src_state);
      State *dstState = states.value(dst_state);
      Transition *transition = addTransition(srcState, dstState,
                                             QString::fromStdString(event),
                                             QString::fromStdString(guard),
                                             QString::fromStdString(actions),
                                             location);
      transition->updatePosition();
      }
}

void Fsm::saveToFile(QString fname)
{
    QFile file(fname);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if ( file.error() != QFile::NoError ) {
      QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
      return;
      }

    nlohmann::json json_res;

    json_res["name"] = myName.toStdString();

    json_res["ios"] = nlohmann::json::array();
    for ( const FsmIo* io: myIos.values() ) {
      nlohmann::json json;
      json["name"] = io->name.toStdString(); 
      json["kind"] = io->kind.toStdString(); 
      json["type"] = io->type.toStdString(); 
      //json["desc"] = io->desc().toString().toStdString(); 
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
        json["src_state"] = transition->srcState()->getId().toStdString();
        json["dst_state"] = transition->dstState()->getId().toStdString();
        json["event"] = transition->getEvent().toStdString();
        json["guard"] = transition->getGuard().toStdString();
        json["actions"] = transition->getActions().toStdString();
        json["location"] = transition->location();
        json_res["transitions"].push_back(json);
        }
      }
    QTextStream os(&file);
    os << QString::fromStdString(json_res.dump(2));
    file.close();
}

// DOT export

QString dotTransitionLabel(QString label)
{
  QStringList l = label.split("/");
  if ( l.length() != 2 ) return label;
  int n = std::max(l.at(0).length(), l.at(1).length());
  return l.at(0) + "\\n" + QString(n, '_') + "\\n" + l.at(1);
}

QString stringOfIos(QList<FsmIo*> ios)
{
  QString r;
  foreach ( FsmIo* io, ios)
    r += io->toString() + "\\l";
  return r;
}

void Fsm::exportDot(QString fname, QStringList options)
{
  QFile file(fname);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  if ( file.error() != QFile::NoError ) {
    QMessageBox::warning(mainWindow, "","Cannot open file " + file.fileName());
    return;
  }
  QTextStream os(&file);

  QString name = myName.isEmpty() ? "main" : myName;
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
    os << "_ios [label=\"" << stringOfIos(myIos.values()) << "\", shape=rect, style=solid]\n";
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
      QString src_id = transition->srcState()->getId();
      QString dst_id = transition->dstState()->getId();
      QString label = dotTransitionLabel(transition->getLabel());
      os << src_id << " -> " << dst_id << " [label=\"" << label << "\"]\n";
      }
    }
  os << "}\n";
  file.close();
}

// RFSM export

QString stringOfVarList(QList<FsmIo*> vs, QString sep=", ", bool withType=true) 
{
  QStringList rs;
  for (const auto& v : vs) {
    if ( withType )
      rs << v->name + ": " + v->type;
    else
      rs << v->name;
    }
  return rs.join(sep);
}

QString stringOfTransition(Transition *t, bool abbrev=false)
{
  QString ss;
  ss = t->srcState()->getId() + " -> " + t->dstState()->getId();
  if ( ! abbrev ) {
    if ( ! t->getEvent().isEmpty() ) ss += " on " + t->getEvent();
    if ( ! t->getGuard().isEmpty() ) ss += " when " + t->getGuard();
    if ( ! t->getActions().isEmpty() ) ss += " with " + t->getActions();
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


void Fsm::export_rfsm_model(QTextStream& os)
{
    QString indent = QString(2, ' ');

    QList<FsmIo*> gios, lvars;

    for ( const auto v : myIos.values() ) {
      if ( v->kind == "var" ) lvars.append(v);
      else gios.append(v);
      }

    bool first;
    os << "fsm model " << myName << "(";
    if ( gios.length() > 0 ) {
      os << "\n";
      first = true;
      for(const auto io : gios) {
            if(!first) os << "," << "\n";
            os << indent;
            os << io->kind << " " << io->name << ": " << io->type;
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
        if ( transition->srcState()->isPseudo() ) continue;
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
    if ( ! iTransition->getActions().isEmpty() ) os << " with " << iTransition->getActions();
    os << ";" << "\n";
    os << "}";
}

void Fsm::export_rfsm_testbench(QTextStream& os)
{
    QList<FsmIo*> gios;
    for ( const auto s : myIos.values() ) {
      if ( s->kind == "in" ) {
        QString ss = s->stim.toString();
        if ( ss != "" )  {
          os << "input " << s->name << " : " << s->type << " = " << ss;;
          gios.append(s);
          os << "\n";
          }
        else {
          QMessageBox::warning(mainWindow, "","No stimulus for input " + s->name);
          return;
          }
        }
      else if ( s->kind == "out" ) {
        os << "output " << s->name << " : " << s->type;
        gios.append(s);
        os << "\n";
        }
      }
    os << "\n";
    os << "fsm " << myName << " = " << myName << "(";
    bool first = true;
    for(const auto io : gios) {
      if ( !first ) os << ", ";
      os << io->name;
      first = false;
      }
    os << ")";
}

void Fsm::exportRfsm(QString fname, bool withTestbench)
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
