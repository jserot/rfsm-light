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

#include "transition.h"
#include "state.h"
#include "model.h"
#include "mainwindow.h"
#include "imageviewer.h"
#include "textviewer.h"
#ifdef USE_QGV
#include "dotviewer.h"
#endif
#include "compilerPaths.h"
#include "compilerOptions.h"
#include "commandExec.h"
#include "syntaxChecker.h"
#include "debug.h"
#include "stimuli.h"
#include "stateProperties.h"
#include "transitionProperties.h"
#include "modelProperties.h"

#include <QtWidgets>
#include <QVariant>

const QString MainWindow::title = "RFSM Light";
const QString MainWindow::version = "1.3.0"; 
const int MainWindow::canvas_width = 500;
const int MainWindow::canvas_height = 1000;
const QList<int> MainWindow::splitterSizes = { 250, 250, 250 };
const double MainWindow::zoomInFactor = 1.25;
const double MainWindow::zoomOutFactor = 0.8;
const double MainWindow::minScaleFactor = 0.2;
const double MainWindow::maxScaleFactor = 2.0;
const QStringList guiOnlyOpts = { "-dot_external_viewer", "-sync_externals" };

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolbars();

    QSplitter *splitter = new QSplitter;

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //sizePolicy.setHorizontalStretch(0);
    //sizePolicy.setVerticalStretch(0);
    //sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
    splitter->setSizePolicy(sizePolicy);
    splitter->setMinimumSize(QSize(0, 300));
    splitter->setOrientation(Qt::Horizontal);

    setCentralWidget(splitter);

    model = new Model(this);
    model->setSceneRect(QRectF(0, 0, canvas_width, canvas_height));
    connect(model, SIGNAL(stateInserted(State*)), this, SLOT(stateInserted(State*)));
    connect(model, SIGNAL(editState(State*)), this, SLOT(editState(State*)));
    connect(model, SIGNAL(transitionInserted(Transition*)), this, SLOT(transitionInserted(Transition*)));
    connect(model, SIGNAL(editTransition(Transition*)), this, SLOT(editTransition(Transition*)));
    connect(model, SIGNAL(mouseEnter()), this, SLOT(updateCursor()));
    connect(model, SIGNAL(mouseLeave()), this, SLOT(resetCursor()));

    QString appDir = QApplication::applicationDirPath();
    qDebug() << "APPDIR=" << appDir;
    compilerPaths = new CompilerPaths(appDir + "/rfsm-light.ini", this);
    compilerOptions = new CompilerOptions(appDir + "/options_spec.txt", this);
    initDir = compilerPaths->getPath("INITDIR");

    executor = new CommandExec();

    QString syntaxCheckerPgm = getCompilerPaths()->getPath("SYNTAXCHECKER");
    syntaxChecker = new SyntaxChecker(executor, syntaxCheckerPgm);

    assert(model);
    properties_panel = new ModelProperties(model,this); 
    properties_panel->setMinimumWidth(280);
    properties_panel->setMaximumWidth(360);

    splitter->addWidget(properties_panel);

    QWidget *editor = new QWidget;
    QVBoxLayout *editLayout = new QVBoxLayout;

    view = new QGraphicsView(model);
    view->setMinimumWidth(300);
    view->setMaximumWidth(400);
    view->setMinimumHeight(400);

    editLayout->addWidget(view);

    editor->setLayout(editLayout);

    splitter->addWidget(editor);

    results = new QTabWidget(splitter);
    results->setMinimumHeight(400);
    results->setMinimumWidth(300);
    results->setDocumentMode(false);
    results->setTabsClosable(true);
    results->setMovable(true);

    connect(results, SIGNAL(tabCloseRequested(int)), this, SLOT(closeResultTab(int)));
    connect(results, SIGNAL(currentChanged(int)), this, SLOT(resultTabChanged(int)));

    statusBar = new QStatusBar;
    setStatusBar(statusBar);

    setWindowTitle(title);
    setUnifiedTitleAndToolBarOnMac(true);

    codeFont.setFamily("Courier");
    codeFont.setFixedPitch(true);
    codeFont.setPointSize(11);

    unsaved_changes = false;
    updateActions();
    
    connect(properties_panel, SIGNAL(modelModified()), this, SLOT(modelModified()));

    splitter->setSizes(splitterSizes); 

    currentScaleFactor = 1.0;
}


void MainWindow::stateInserted(State *state)
{
  editState(state);
}

void MainWindow::editState(State *state)
{
  StateProperties* dialog = new StateProperties(state, model, syntaxChecker, this);
  if ( dialog->exec() == QDialog::Accepted ) {
    qDebug() << "state" << state->getId() << "updated";
    getModel()->update();
    setUnsavedChanges(true);
    }
  delete dialog;
}

void MainWindow::transitionInserted(Transition *transition)
{
  editTransition(transition);
}

void MainWindow::editTransition(Transition *transition)
{
  bool isInitial = transition->isInitial();
  QStringList inpEvents = model->getInpEvents();
  if ( !isInitial && inpEvents.isEmpty() ) {
      QMessageBox::warning( this, "Error", "No input event available to trigger this transition. Please define one.");
      qDebug() << "Deleting transition" << transition->toString();
      model->removeTransition(transition);
      //delete transition;
      return;
      }
  TransitionProperties* dialog = new TransitionProperties(transition,model,isInitial,syntaxChecker,this);
  if ( dialog->exec() == QDialog::Accepted ) {
    qDebug() << "Transition" << transition->toString() << "updated";
    getModel()->update();
    setUnsavedChanges(true);
    }
  delete dialog;
}

void MainWindow::modelModified()
{
  qDebug() << "Model modified !";
  setUnsavedChanges(true);
}

void MainWindow::about()
{
    QMessageBox::about(this,
      "About RFSM Light",
      "<p>Finite State Diagram Editor, Simulator and Compiler</p>\
          <p>version " + version + "</p>\
         <p><a href=\"github.com/jserot/rfsm-light\">github.com/jserot/rfsm-light</a></p>\
         <p>(C) J. Sérot (jocelyn.serot@uca.fr), 2019-now");
}

// Actions

void MainWindow::createActions()
{
    newDiagramAction = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newDiagramAction->setShortcuts(QKeySequence::New);
    newDiagramAction->setToolTip(tr("Start a new diagram"));
    connect(newDiagramAction, SIGNAL(triggered()), this, SLOT(newDiagram()));

    openFileAction = new QAction(QIcon(":/images/open.png"), "&Open", this);
    openFileAction->setShortcut(QKeySequence::Open);
    openFileAction->setToolTip(tr("Open an existing diagram"));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));
 
    saveFileAction = new QAction(QIcon(":/images/save.png"), "&Save", this);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveFileAction->setToolTip(tr("Save current diagram"));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(save()));
 
    saveFileAsAction = new QAction( "Save As", this);
    saveFileAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveFileAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
 
    aboutAction = new QAction(tr("About"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    exitAction = new QAction(tr("Q&uit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));

    checkSyntaxWithoutStimuliAction = new QAction(tr("Check model only "), this);
    checkSyntaxWithoutStimuliAction->setShortcut(tr("Ctrl+K"));
    connect(checkSyntaxWithoutStimuliAction, SIGNAL(triggered()), this, SLOT(checkSyntaxWithoutStimuli()));

    checkSyntaxWithStimuliAction = new QAction(tr("Check model and stimuli"), this);
    checkSyntaxWithStimuliAction->setShortcut(tr("Ctrl+Shift+K"));
    connect(checkSyntaxWithStimuliAction, SIGNAL(triggered()), this, SLOT(checkSyntaxWithStimuli()));

    renderDotAction = new QAction(QIcon(":/images/compileDot.png"), tr("Generate DOT representation"), this);
    renderDotAction->setToolTip(tr("Generate DOT representation"));
    renderDotAction->setShortcut(tr("Ctrl+R"));
    connect(renderDotAction, SIGNAL(triggered()), this, SLOT(renderDot()));

    generateRfsmModelAction = new QAction(tr("Generate RFSM code (model only)"), this);
    connect(generateRfsmModelAction, SIGNAL(triggered()), this, SLOT(generateRfsmModel()));

    generateRfsmTestbenchAction = new QAction(tr("Generate RFSM code (model+testbench) "), this);
    connect(generateRfsmTestbenchAction, SIGNAL(triggered()), this, SLOT(generateRfsmTestbench()));

    generateCTaskAction = new QAction(QIcon(":/images/compileCTask.png"),tr("Generate CTask code"), this);
    generateCTaskAction->setToolTip(tr("Generate CTask code"));
    connect(generateCTaskAction, SIGNAL(triggered()), this, SLOT(generateCTask()));

    generateSystemCModelAction = new QAction(QIcon(":/images/compileSystemC.png"),tr("Generate SystemC code (model only)"), this);
    generateSystemCModelAction->setToolTip(tr("Generate SystemC code"));
    connect(generateSystemCModelAction, SIGNAL(triggered()), this, SLOT(generateSystemCModel()));

    generateSystemCTestbenchAction = new QAction(tr("Generate SystemC code (model+testbench)"), this);
    connect(generateSystemCTestbenchAction, SIGNAL(triggered()), this, SLOT(generateSystemCTestbench()));

    generateVHDLModelAction = new QAction(QIcon(":/images/compileVHDL.png"),tr("Generate VHDL code (model only)"), this);
    generateVHDLModelAction->setToolTip(tr("Generate VHDL code"));
    connect(generateVHDLModelAction, SIGNAL(triggered()), this, SLOT(generateVHDLModel()));

    generateVHDLTestbenchAction = new QAction(tr("Generate VHDL code (model+testbench)"), this);
    connect(generateVHDLTestbenchAction, SIGNAL(triggered()), this, SLOT(generateVHDLTestbench()));

    runSimulationAction = new QAction(QIcon(":/images/runSimulation.png"),tr("Run simulator"), this);
    runSimulationAction->setToolTip(tr("Simulate and open VCD viewer"));
    connect(runSimulationAction, SIGNAL(triggered()), this, SLOT(runSimulation()));

    zoomInAction = new QAction(tr("Zoom In"), this);
    zoomInAction->setShortcut(tr("Ctrl++"));
    connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAction = new QAction(tr("Zoom Out"), this);
    zoomOutAction->setShortcut(tr("Ctrl+-"));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAction = new QAction(tr("Normal size (100%)"), this);
    connect(normalSizeAction, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAction = new QAction(tr("Fit to Window"), this);
    fitToWindowAction->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAction, SIGNAL(triggered()), this, SLOT(fitToWindow()));
    fitToWindowAction->setCheckable(true);

    closeResultsAction = new QAction(tr("Close all results"), this);
    connect(closeResultsAction, SIGNAL(triggered()), this, SLOT(closeResults()));

    pathConfigAction = new QAction(tr("Compiler and tools"), this);
    connect(pathConfigAction, SIGNAL(triggered()), this, SLOT(setCompilerPaths()));

    compilerOptionsAction = new QAction(tr("Compiler options"), this);
    connect(compilerOptionsAction, SIGNAL(triggered()), this, SLOT(setCompilerOptions()));

    fontConfigAction = new QAction(tr("Code font"), this);
    QObject::connect(fontConfigAction, SIGNAL(triggered()), this, SLOT(setCodeFont()));
    
    diagramActions = new QActionGroup(this);

    selectItemAction = new QAction(QIcon(":/images/select.png")," Select item", diagramActions);
    addStateAction = new QAction(QIcon(":/images/state.png")," Add state", diagramActions);
    addInitStateAction = new QAction(QIcon(":/images/initstate.png")," Add initial state", diagramActions);
    addTransitionAction = new QAction(QIcon(":/images/transition.png")," Add transition", diagramActions);
    addSelfTransitionAction = new QAction(QIcon(":/images/loop.png")," Add self transition", diagramActions);
    deleteItemAction = new QAction(QIcon(":/images/delete.png")," Delete item", diagramActions);

    selectItemAction->setData(QVariant::fromValue((int)Model::SelectItem));
    addStateAction->setData(QVariant::fromValue((int)Model::InsertState));
    addInitStateAction->setData(QVariant::fromValue((int)Model::InsertPseudoState));
    addTransitionAction->setData(QVariant::fromValue((int)Model::InsertTransition));
    addSelfTransitionAction->setData(QVariant::fromValue((int)Model::InsertLoopTransition));
    deleteItemAction->setData(QVariant::fromValue((int)Model::DeleteItem));

    selectItemAction->setCheckable(true);
    addStateAction->setCheckable(true);
    addInitStateAction->setCheckable(true);
    addTransitionAction->setCheckable(true);
    addSelfTransitionAction->setCheckable(true);
    deleteItemAction->setCheckable(true);

    selectItemAction->setChecked(true);
    addStateAction->setChecked(false);
    addInitStateAction->setChecked(false);
    addTransitionAction->setChecked(false);
    addSelfTransitionAction->setChecked(false);
    deleteItemAction->setChecked(false);

    connect(diagramActions, SIGNAL(triggered(QAction*)), this, SLOT(editDiagram(QAction*)));
}

void MainWindow::updateActions()
{
  updateViewActions(); // Nothing else for now
}

void MainWindow::updateViewActions()
{
  QWidget *widget;
  QString kind;
  if ( results->count() == 0 ) {
    closeResultsAction->setEnabled(false);
    goto unselect;
    }
  closeResultsAction->setEnabled(true);
  widget = results->widget(results->currentIndex());  
  if ( widget == NULL ) goto unselect;
  kind = widget->metaObject()->className();
  if ( kind == "ImageViewer" ) {
    ImageViewer* viewer = static_cast<ImageViewer*>(widget);
    bool b = viewer->isFittedToWindow();
    fitToWindowAction->setEnabled(true);
    fitToWindowAction->setChecked(b);
    zoomInAction->setEnabled(!b);
    zoomOutAction->setEnabled(!b);
    normalSizeAction->setEnabled(!b);
    return;
    }
  else if ( kind == "DotViewer" ) {
    fitToWindowAction->setEnabled(true);
    fitToWindowAction->setChecked(true);
    zoomInAction->setEnabled(true);
    zoomOutAction->setEnabled(true);
    normalSizeAction->setEnabled(true);
    return;
    }
 unselect:
    fitToWindowAction->setEnabled(false);
    zoomInAction->setEnabled(false);
    zoomOutAction->setEnabled(false);
    normalSizeAction->setEnabled(false);
}

// Menus

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newDiagramAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveFileAsAction);
    fileMenu->addAction(aboutAction);
    fileMenu->addAction(exitAction);

    checkMenu = menuBar()->addMenu(tr("&Check"));
    checkMenu->addAction(checkSyntaxWithoutStimuliAction);
    checkMenu->addAction(checkSyntaxWithStimuliAction);

    buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(renderDotAction);
    buildMenu->addAction(generateCTaskAction);
    buildMenu->addAction(generateSystemCModelAction);
    buildMenu->addAction(generateVHDLModelAction);
    buildMenu->addSeparator();
    buildMenu->addAction(generateSystemCTestbenchAction);
    buildMenu->addAction(generateVHDLTestbenchAction);
    buildMenu->addSeparator();
    buildMenu->addAction(generateRfsmModelAction);
    buildMenu->addAction(generateRfsmTestbenchAction);
    buildMenu->addSeparator();
    buildMenu->addAction(runSimulationAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(zoomInAction);
    viewMenu->addAction(zoomOutAction);
    viewMenu->addAction(normalSizeAction);
    viewMenu->addAction(fitToWindowAction);
    viewMenu->addSeparator();
    viewMenu->addAction(closeResultsAction);

    configMenu = menuBar()->addMenu("&Configuration");
    configMenu->addAction(pathConfigAction);
    configMenu->addAction(compilerOptionsAction);
    configMenu->addAction(fontConfigAction);
}

void MainWindow::createToolbars()
{
     QWidget *spacer1 = new QWidget(this);
     spacer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     QWidget *spacer2 = new QWidget(this);
     spacer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

     fileToolBar = addToolBar(tr("File"));
     fileToolBar->addAction(newDiagramAction);
     fileToolBar->addAction(openFileAction);
     fileToolBar->addAction(saveFileAction);

     editToolBar = addToolBar(tr("Edit"));
     editToolBar->addWidget(spacer1); 
     editToolBar->addAction(selectItemAction);
     editToolBar->addAction(addStateAction);
     editToolBar->addAction(addInitStateAction);
     editToolBar->addAction(addTransitionAction);
     editToolBar->addAction(addSelfTransitionAction);
     editToolBar->addAction(deleteItemAction);

     compileToolBar = addToolBar(tr("Compile"));
     compileToolBar->addWidget(spacer2);
     compileToolBar->addAction(renderDotAction);
     compileToolBar->addAction(generateCTaskAction);
     compileToolBar->addAction(generateSystemCModelAction);
     compileToolBar->addAction(generateVHDLModelAction);
     compileToolBar->addAction(runSimulationAction);
}

void MainWindow::createPropertiesPanel()
{
  properties_panel = new ModelProperties(model,this);
  properties_panel->setMinimumWidth(180);
  properties_panel->setMaximumWidth(360);
}

// File IO

void MainWindow::setUnsavedChanges(bool unsaved_changes)
{
    this->unsaved_changes = unsaved_changes;
    setWindowTitle(unsaved_changes ? title + " (Unsaved changes)" : title);
}

void MainWindow::checkUnsavedChanges()
{
    if ( unsaved_changes ) {
        QMessageBox save_message;
        save_message.setText("Do you want to save your changes?");
        save_message.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        save_message.setDefaultButton(QMessageBox::Save);
        switch( save_message.exec() ) {
            case QMessageBox::Cancel: return;
            case QMessageBox::Save: save();
            }
        }
}

void MainWindow::openFile()
{
    checkUnsavedChanges();
    
    QString fname = QFileDialog::getOpenFileName(this, "Open file", initDir, "FSD file (*.fsd)");
    if ( fname.isEmpty() ) return;
    try {
      model->readFromFile(fname);
      }
    catch(const std::exception& e) {
      QMessageBox::warning(this, "Error", "Unable to import : " + QString(e.what()));
      return;
      }
    view->ensureVisible(model->itemsBoundingRect());
    //QRectF bb = view->scene()->itemsBoundingRect();
    //view->centerOn(bb.center());
    currentFileName = fname;
    properties_panel->update();
    setUnsavedChanges(false);
}

bool MainWindow::checkSyntax(bool withStimuli)
{
  return model ? model->check_model(withStimuli) : true;
}

bool MainWindow::checkSyntaxWithStimuli()
{ 
  return model ? model->check_model(false) : true;
  }

bool MainWindow::checkSyntaxWithoutStimuli()
{ 
  return model ? model->check_model(true) : true;
}

void MainWindow::newDiagram()
{
  checkUnsavedChanges();
  properties_panel->clear();
  model->clear();
  currentFileName.clear();
  setUnsavedChanges(false);
}

void MainWindow::saveToFile(QString fname)
{
  model->saveToFile(fname);
  logMessage("Saved file " + fname);
  setUnsavedChanges(false);
}

void MainWindow::save()
{
    if ( currentFileName.isEmpty() ) saveAs();
    else saveToFile(currentFileName);
}

void MainWindow::saveAs()
{
  QString fname = QFileDialog::getSaveFileName( this, "Save to file", "", "FSM file (*.fsd)");
  if ( fname.isEmpty() ) return;
  saveToFile(fname);
  currentFileName = fname;
}

// Diagram editing

void MainWindow::editDiagram(QAction *action)
{
  Model::Mode m = static_cast<Model::Mode>(action->data().value<int>());
  model->setMode(m);
}

// Generating result files

QString MainWindow::getCurrentFileName()
{
  if ( currentFileName.isEmpty() )
    return QFileDialog::getSaveFileName( this, "Please save source file before compiling", "", "FSD file (*.fsd)");
  else
    return currentFileName;
}

QString changeSuffix(QString fname, QString suffix)
{
  QFileInfo f(fname);
  return f.path() + "/" + f.completeBaseName() + suffix;
  // return f.completeBaseName() + suffix;
}

void MainWindow::renderDot()
{
#ifdef USE_QGV
    addDotTab();
#else
    QString sFname = getCurrentFileName();
    if ( sFname.isEmpty() ) return;
    QString rFname = changeSuffix(sFname, ".dot");
    QStringList opts = compilerOptions->getOptions("dot");
    model->exportDot(rFname, opts);
    logMessage("Wrote file " + rFname);
    openResultFile(rFname);
#endif
}

QString MainWindow::generateRfsm(bool withTestbench ) // TODO : factorize
{
  if ( ! checkSyntax(withTestbench) ) return "";
  QString sFname = getCurrentFileName();
  if ( sFname.isEmpty() ) return "";
  QString rFname = changeSuffix(sFname, ".fsm");
  model->exportRfsm(rFname, withTestbench);
  return rFname;
}

void MainWindow::generateRfsmModel()
{
  QString rFname = generateRfsm(false);
  if ( ! rFname.isEmpty() ) {
    logMessage("Wrote file " + rFname);
    openResultFile(rFname);
    }
}

void MainWindow::generateRfsmTestbench()
{
  QString rFname = generateRfsm(true);
  if ( ! rFname.isEmpty() ) {
    logMessage("Wrote file " + rFname);
    openResultFile(rFname);
    }
}

// Displaying result files

void MainWindow::addResultTab(QString fname)
{
  QFile file(fname);
  if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      QMessageBox::warning(this,"Error:","cannot open file:\n"+fname);
      return;
    }
  QFileInfo f(fname);
  QString tabName = f.suffix() == "gif" ? changeSuffix(f.fileName(),".dot") : f.fileName();
  for ( int i=0; i<results->count(); i++ )
    if ( results->tabText(i) == tabName ) closeResultTab(i); // Do not open two tabs with the same name
  if ( f.suffix() == "gif" ) {
    QPixmap pixmap(f.filePath());
    ImageViewer *viewer = new ImageViewer(pixmap, results);
    results->addTab(viewer, tabName);
    } 
  else {
    TextViewer *viewer = new TextViewer(file, codeFont, results);
    results->addTab(viewer, tabName);
    }
  results->setCurrentIndex(results->count()-1);
}

void MainWindow::closeResultTab(int index)
{
  QWidget *w = results->widget(index);
  results->removeTab(index);
  if ( w ) delete w; // removeTab does _not_ delete the tabbed widget
}

void MainWindow::closeResults()
{
  while ( results->count() > 0 )
    closeResultTab(results->currentIndex());
  updateActions();
}

void MainWindow::openResultFile(QString fname)
{
  QFileInfo f(fname);
  QString wDir = f.canonicalPath();
  qDebug() << "Displaying file : " << fname;
  QStringList genOpts = compilerOptions->getOptions("general");
  QStringList args = { fname };
  if ( f.suffix() == "dot" ) {
    if ( genOpts.contains("-dot_external_viewer") )
      customView("DOTVIEWER", args, wDir);
    else {
      if ( dotTransform(f, wDir) )
        openResultFile(changeSuffix(fname, ".gif"));
      }
    }
  else if ( f.suffix() == "vcd" ) {
    QString gFile = changeSuffix(fname, ".gtkw");
    QFile gf(gFile);
    if ( gf.exists() ) args << gFile;
    customView("VCDVIEWER", args, wDir);
    }
  else {
    addResultTab(fname);
    }
}

void MainWindow::customView(QString toolName, QStringList args, QString wDir)
{
   QString cmd = compilerPaths->getPath(toolName);
   if ( cmd.isNull() || cmd.isEmpty() ) {
     QMessageBox::warning(this, "", "No path specified for " + toolName);
     return;
     }
  QStringList genOpts = compilerOptions->getOptions("general");
  // bool sync = genOpts.contains("-sync_externals");
  // if ( ! executeCmd(wDir, cmd, args, sync ) ) {
  if ( executor->execute(wDir, cmd, args) ) {
     QMessageBox::warning(this, "", "Failed to launch external program " + toolName);
     //addResultTab(fname);
     }
}

void MainWindow::resultTabChanged(int index)
{
  Q_UNUSED(index);
  updateActions();
}

// Interface to RFSMC compiler

QStringList MainWindow::compile(QString target, QString wDir, QString sFname, QStringList args)
{
  if ( sFname.isEmpty() ) return QStringList();
  qDebug() << "compile: srcFile=" << sFname << " wDir=" << wDir;
  QString compiler = compilerPaths->getPath("COMPILER");
  if ( compiler.isNull() || compiler.isEmpty() ) compiler = "rfsmc"; // Last chance..
  // if ( targetDir != "" ) dir.mkdir(targetDir);
  // Clean target directory
  // removeFiles(wDir + "/" + targetDir, eraseFirst);
  if ( executor->execute(wDir, compiler, args << "-gui" << sFname) )
    return getOutputFiles(target, wDir);
  else {
    QStringList compileErrors = executor->getErrors();
    QMessageBox::warning(this, "", "Error when compiling model\n" + compileErrors.join("\n"));
    return QStringList();
    }
  return QStringList();
}

QStringList MainWindow::getOutputFiles(QString target, QString wDir)
{
  QString rfile = wDir + "/rfsm.output";
  QFile ff(rfile);
  QStringList res;
  qDebug() << "Output files: rfile=" << rfile;
  if ( ! ff.exists() ) {
      QMessageBox::warning(NULL, "", "Cannot open file " + rfile);
      return res;
    }
  ff.open(QIODevice::ReadOnly | QIODevice::Text);
  if ( target == "sim" )
    res.append(wDir+"/"+"main.vcd");  // TO FIX : this is a hack while waiting rfsmc to correctly write rsfm.output
  else {
    QTextStream is(&ff);
    while( ! is.atEnd() ) {
      QString of = is.readLine(); // One file per line
      QFileInfo f(of);
      if (  (target == "systemc" && (f.suffix() == "cpp" || f.suffix() == "h"))
            || (target == "vhdl" && f.suffix() == "vhd")
            || (target == "ctask" && f.suffix() == "c")
            || (target == "dot" && f.suffix() == "dot") 
            || (target == "sim" && f.suffix() == "vcd") )
        res.append(wDir+"/"+of);
      }
    }
  ff.close();
  qDebug() << "Output files: " << res;
  return res;
}

void MainWindow::generate(QString target, bool withTestbench)
{
  QString fname = generateRfsm(withTestbench);
  QFileInfo fi(fname);
  if ( fname.isEmpty() ) return;
  qDebug () << "generate.fname = " << fname;
  QString wDir = QFileInfo(fname).absolutePath();
  QStringList genOpts = compilerOptions->getOptions("general");
  QString targetDir = ".";
  if ( target != "sim" && genOpts.contains("-target_dirs") ) {
    targetDir = target;
    QString targetPath = wDir + "/" + target; // TO FIX : do not use raw, OS-dependent "/" in file path
    genOpts.removeOne("-target_dirs");
    QDir dir(targetPath);
    if ( ! dir.exists() ) {
      qDebug() << "Creating directory " << targetPath;
      QDir().mkdir(targetPath);
      }
    }
  foreach ( QString opt, genOpts)
    if ( genOpts.contains(opt) ) genOpts.removeOne(opt);
  QStringList args =
    QStringList()
    << "-" + target
    << "-target_dir" << targetDir
    << genOpts
    << compilerOptions->getOptions(target);
  //if ( target == "sim" ) args << "-main" <<  fi.baseName();
  if ( target == "ctask" || target == "systemc" ) args << "-show_models";
  QStringList resFiles = compile(target, wDir, fi.fileName(), args);
  if ( ! resFiles.isEmpty() ) {
    logMessage("Generated file(s) : " + resFiles.join(", "));
    foreach ( QString rFile, resFiles) 
      openResultFile(rFile);
    }
  updateActions();
}

void MainWindow::generateCTask() { generate("ctask", false); }

void MainWindow::generateSystemCModel() { generate("systemc", false); }
void MainWindow::generateSystemCTestbench() { generate("systemc", true); }

void MainWindow::generateVHDLModel() { generate("vhdl", false); }
void MainWindow::generateVHDLTestbench() { generate("vhdl", true); }

void MainWindow::runSimulation() { generate("sim", true); }

bool MainWindow::dotTransform(QFileInfo f, QString wDir)
{
  QString dotProgram = compilerPaths->getPath("DOTPROGRAM");
  if ( dotProgram.isNull() || dotProgram.isEmpty() ) dotProgram = "dot"; // Last chance..
  QString srcFile = f.filePath();
  QString dstFile = changeSuffix(srcFile, ".gif");
  //QString opts = ""; // getOption("-dot_options");
  QString wdir = f.canonicalPath();
  QStringList args = { "-Tgif",  "-o", dstFile, srcFile };
  if ( executor->execute(wDir, dotProgram, args) )
    return true;
  else {
    //QMessageBox::warning(this, "", "Cannot run DOT program : command " + cmd.toString() + " failed");
    QMessageBox::warning(this, "", "Failed to run DOT program");
    return false;
    }
}

// VIEW operations 

void MainWindow::setCodeFont()
{
  bool ok;
  QFont font = QFontDialog::getFont(&ok, QFont("Courier", 10), this);
  //qDebug() << "Got font " << font.toString();
  if ( ok ) {
    for ( int i=0; i<results->count(); i++ )
      (static_cast<QPlainTextEdit*>(results->widget(i)))->document()->setDefaultFont(font);
    codeFont = font;
    }
}

void MainWindow::zoomIn()
{
  scaleImage(zoomInFactor);
}


void MainWindow::zoomOut()
{
  scaleImage(zoomOutFactor);
}

void MainWindow::normalSize()
{
  QWidget *w = selectedTab();
  QString k = w->metaObject()->className();
  if ( k == "ImageViewer" ) {
    ImageViewer* viewer = static_cast<ImageViewer*>(w);
    if ( viewer == NULL ) return;
    viewer->normalSize();
    }
  // updateSelectedTabTitle(); // TODO ? 
}

void MainWindow::fitToWindow()
{
  QWidget *w = selectedTab();
  QString k = w->metaObject()->className();
  if ( k == "ImageViewer" ) {
    ImageViewer* viewer = static_cast<ImageViewer*>(w);
    if ( viewer == NULL ) return;
    viewer->fitToWindow(fitToWindowAction->isChecked() );
    }
  // updateSelectedTabTitle(); // TODO ? 
  //updateViewActions(viewer);
}

QWidget* MainWindow::selectedTab()
{
  int i = results->currentIndex();
  if ( i < 0 ) return NULL;
  QWidget *tab = results->widget(i);
  return tab;
}

void MainWindow::scaleImage(double factor)
{
  QWidget *w = selectedTab();
  QString k = w->metaObject()->className();
  currentScaleFactor = factor * currentScaleFactor;
  if ( k == "ImageViewer" ) {
    ImageViewer* viewer = static_cast<ImageViewer*>(w);
    if ( viewer == NULL ) return;
    viewer->scaleImage(currentScaleFactor); // Absolute scaling
   }
  else if ( k == "DotViewer" ) {
    QGraphicsView* dotView = static_cast<QGraphicsView*>(w);
    if ( dotView == NULL ) return;
    dotView->scale(factor, factor); // Relative scaling
  }
  zoomInAction->setEnabled(currentScaleFactor < maxScaleFactor);
  zoomOutAction->setEnabled(currentScaleFactor > minScaleFactor);
  // updateSelectedTabTitle(); // TODO ? 
}

// In-app DOT rendering (since version 1.3.0)

#ifdef USE_QGV
void MainWindow::addDotTab(void)
{
  QString tabName = "dot";
  for ( int i=0; i<results->count(); i++ )
    if ( results->tabText(i) == tabName ) closeResultTab(i); // Do not open two tabs with the same name
  DotViewer *view = new DotViewer(model, 200, 400, results); // TODO: let the dotViewer class decide of the canvas dimensions ?
  results->addTab(view, tabName);
  results->setCurrentIndex(results->count()-1);
}
#endif

// Dynamic cursor handling (since 1.3.0)

QCursor cursorOf(Model::Mode mode)
{
  switch ( mode ) {
  case Model::InsertState: return QCursor(QPixmap(":cursors/state.png"),0,0);
  case Model::InsertPseudoState: return QCursor(QPixmap(":cursors/initstate.png"),0,0);
  case Model::InsertTransition: return QCursor(QPixmap(":cursors/transition.png"),0,0);
  case Model::InsertLoopTransition: return QCursor(QPixmap(":cursors/loop.png"),0,0);
  case Model::DeleteItem: return QCursor(QPixmap(":cursors/delete.png"),0,0);
  default: return Qt::ArrowCursor;
  }
}

void MainWindow::updateCursor()
{
  setCursor(cursorOf(model->getMode()));
}

void MainWindow::resetCursor()
{
  setCursor(Qt::ArrowCursor);
}

// Configuration

void MainWindow::setCompilerPaths()
{
  compilerPaths->edit(this);
}

void MainWindow::setCompilerOptions()
{
  compilerOptions->edit(this);
  QStringList opts = compilerOptions->getOptions("general");
  traceMode = opts.contains("-debug");
  //if ( traceMode ) qDebug() << "Debug mode activated";
  //else qDebug() << "Debug mode desactivated";
}

// Logging 

void MainWindow::logMessage(QString msg)
{
  statusBar->showMessage(msg);
}

// Bye

void MainWindow::quit()
{
    checkUnsavedChanges();
    close();
}
