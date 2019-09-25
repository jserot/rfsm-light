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
#include "fsm.h"
#include "mainwindow.h"
#include "command.h"
#include "imageViewer.h"
#include "syntaxHighlighters.h"
#include "compilerPaths.h"
#include "compilerOptions.h"
#include "debug.h"

#include <QtWidgets>
#include <QVariant>

QString MainWindow::title = "RFSM Light";

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

    fsm = new Fsm(this);
    fsm->setSceneRect(QRectF(0, 0, 600, 600));
    connect(fsm, SIGNAL(stateInserted(State*)), this, SLOT(stateInserted(State*)));
    connect(fsm, SIGNAL(stateDeleted(State*)), this, SLOT(stateDeleted(State*)));
    connect(fsm, SIGNAL(transitionInserted(Transition*)), this, SLOT(transitionInserted(Transition*)));
    connect(fsm, SIGNAL(transitionDeleted(Transition*)), this, SLOT(transitionDeleted(Transition*)));
    connect(fsm, SIGNAL(stateSelected(State*)), this, SLOT(stateSelected(State*)));
    connect(fsm, SIGNAL(transitionSelected(Transition*)), this, SLOT(transitionSelected(Transition*)));
    connect(fsm, SIGNAL(nothingSelected()), this, SLOT(nothingSelected()));
    connect(fsm, SIGNAL(fsmModified()), this, SLOT(fsmModified()));

    properties_panel = new PropertiesPanel(this); // Warning: fsm must be created before 
    properties_panel->setMinimumWidth(180);
    properties_panel->setMaximumWidth(360);

    splitter->addWidget(properties_panel);

    QWidget *editor = new QWidget;
    QVBoxLayout *editLayout = new QVBoxLayout;

    view = new QGraphicsView(fsm);
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

#ifdef Q_OS_WIN
    compilerPaths = new CompilerPaths("rfsm-light.ini", this);
    compilerOptions = new CompilerOptions("options_spec.txt", this);
#else
    QString appDir = QApplication::applicationDirPath();
    //qDebug() << "APPDIR=" << appDir;
    compilerPaths = new CompilerPaths(appDir + "/rfsm-light.ini", this);
    compilerOptions = new CompilerOptions(appDir + "/options_spec.txt", this);
#endif
    initDir = compilerPaths->getPath("INITDIR");

    connect(&proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcStdout()));
    connect(&proc, SIGNAL(readyReadStandardError()), this, SLOT(readProcStderr()));
    
    unsaved_changes = false;
    updateActions();
}


void MainWindow::stateInserted(State *state)
{
  properties_panel->setSelectedItem(state);
}

void MainWindow::stateDeleted(State *)
{
  properties_panel->unselectItem();
}

void MainWindow::transitionInserted(Transition *transition)
{
  properties_panel->setSelectedItem(transition);
}

void MainWindow::transitionDeleted(Transition *)
{
  properties_panel->unselectItem();
}

void MainWindow::stateSelected(State *state)
{
  properties_panel->setSelectedItem(state);
}

void MainWindow::transitionSelected(Transition *transition)
{
  properties_panel->setSelectedItem(transition);
}

void MainWindow::nothingSelected()
{
  properties_panel->unselectItem();
}

void MainWindow::fsmModified()
{
  setUnsavedChanges(true);
}

void MainWindow::about()
{
    QMessageBox::about(this,
      tr("About RFSM Light"),
      tr("<p>Finite State Diagram Editor, Simulator and Compiler</p>\
         <p><a href=\"github.com/jserot/rfsm-light\">github.com/jserot/rfsm-light</a></p>\
         <p>(C) J. Sérot (jocelyn.serot@uca.fr), 2019"));
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

    checkModelAction = new QAction(tr("Check model"), this);
    checkModelAction->setShortcut(tr("Ctrl+K"));
    connect(checkModelAction, SIGNAL(triggered()), this, SLOT(checkModelOnly()));

    checkTestbenchAction = new QAction(tr("Check testbench "), this);
    checkTestbenchAction->setShortcut(tr("Ctrl+Shift+K"));
    connect(checkTestbenchAction, SIGNAL(triggered()), this, SLOT(checkTestbench()));

    generateDotAction = new QAction(QIcon(":/images/compileDot.png"), tr("Generate DOT representation"), this);
    generateDotAction->setToolTip(tr("Generate DOT representation"));
    connect(generateDotAction, SIGNAL(triggered()), this, SLOT(generateDot()));

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
    generateVHDLModelAction->setToolTip(tr("Simulate and open VCD viewer"));
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

    selectItemAction->setData(QVariant::fromValue((int)Fsm::SelectItem));
    addStateAction->setData(QVariant::fromValue((int)Fsm::InsertState));
    addInitStateAction->setData(QVariant::fromValue((int)Fsm::InsertPseudoState));
    addTransitionAction->setData(QVariant::fromValue((int)Fsm::InsertTransition));
    addSelfTransitionAction->setData(QVariant::fromValue((int)Fsm::InsertLoopTransition));
    deleteItemAction->setData(QVariant::fromValue((int)Fsm::DeleteItem));

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
  if ( results->count() == 0 ) {
    closeResultsAction->setEnabled(false);
    goto unselect;
    }
  closeResultsAction->setEnabled(true);
  widget = results->widget(results->currentIndex());  
  if ( widget == NULL ) goto unselect;
  // qDebug() << widget->whatsThis();
  if ( widget->whatsThis() == "ImageViewer" ) {
    //qDebug() << "ImageViewer currently selected";
    ImageViewer* viewer = static_cast<ImageViewer*>(widget);
    bool b = viewer->isFittedToWindow();
    fitToWindowAction->setEnabled(true);
    fitToWindowAction->setChecked(b);
    zoomInAction->setEnabled(!b);
    zoomOutAction->setEnabled(!b);
    normalSizeAction->setEnabled(!b);
    return;
    }
 unselect:
    //qDebug() << "No imageViewer currently selected";
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
    checkMenu->addAction(checkModelAction);
    checkMenu->addAction(checkTestbenchAction);

    buildMenu = menuBar()->addMenu(tr("&Build"));
    buildMenu->addAction(generateDotAction);
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
     compileToolBar->addAction(generateDotAction);
     compileToolBar->addAction(generateCTaskAction);
     compileToolBar->addAction(generateSystemCModelAction);
     compileToolBar->addAction(generateVHDLModelAction);
     compileToolBar->addAction(runSimulationAction);
}

void MainWindow::createPropertiesPanel()
{
    properties_panel = new PropertiesPanel(this);
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
      fsm->readFromFile(fname);
      }
    catch(const std::exception& e) {
      QMessageBox::warning(this, "Error", "Unable to import : " + QString(e.what()));
      return;
      }
    properties_panel->clear();
    currentFileName = fname;
    setUnsavedChanges(false);
}

// Model checking

void MainWindow::checkModelOnly() { checkModel(false); }
void MainWindow::checkTestbench() { checkModel(true); }

bool MainWindow::checkModel(bool withTestBench)
{
  try {
    fsm->check_model(withTestBench);
    }
  catch (const std::exception& e) {
    QMessageBox::warning(this, "Checking failed", QString(e.what()));
    return false;
    }
  return true;
}

void MainWindow::newDiagram()
{
  checkUnsavedChanges();
  fsm->clear();
  properties_panel->clear();
  currentFileName.clear();
  setUnsavedChanges(false);
}

void MainWindow::saveToFile(QString fname)
{
  fsm->saveToFile(fname);
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
  Fsm::Mode m = static_cast<Fsm::Mode>(action->data().value<int>());
  //qDebug() << "Action " << m;
  fsm->setMode(m);
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

void MainWindow::generateDot()
{
  QString sFname = getCurrentFileName();
  if ( sFname.isEmpty() ) return;
  QString rFname = changeSuffix(sFname, ".dot");
  QStringList opts = compilerOptions->getOptions("dot");
  fsm->exportDot(rFname, opts);
  logMessage("Wrote file " + rFname);
  openResultFile(rFname);
}

QString MainWindow::generateRfsm(bool withTestbench) // TODO : factorize
{
  if ( ! checkModel(withTestbench) ) return "";
  QString sFname = getCurrentFileName();
  if ( sFname.isEmpty() ) return "";
  QString rFname = changeSuffix(sFname, ".fsm");
  fsm->exportRfsm(rFname, withTestbench);
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

SyntaxHighlighter* makeSyntaxHighlighter(QString suffix, QTextDocument* doc)
{
    if ( suffix == "fsm" ) return new FsmSyntaxHighlighter(doc);
    if ( suffix == "c" || suffix == "h" || suffix == "cpp" ) return new CTaskSyntaxHighlighter(doc);
    return NULL;
}


void MainWindow::addResultTab(QString fname)
{
  QFile file(fname);
  QFileInfo f(fname);
  if ( ! file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
      QMessageBox::warning(this,"Error:","cannot open file:\n"+fname);
      return;
    }
  QString tabName = f.suffix() == "gif" ? changeSuffix(f.fileName(),".dot") : f.fileName();
  for ( int i=0; i<results->count(); i++ )
    if ( results->tabText(i) == tabName ) closeResultTab(i); // Do not open two tabs with the same name
  if ( f.suffix() == "gif" ) {
    QPixmap pixmap(f.filePath());
    ImageViewer *viewer = new ImageViewer();
    viewer->setWhatsThis("ImageViewer");
    viewer->setPixmap(pixmap);
    results->addTab(viewer, tabName);
    //AppFile* openedFile = new AppFile(fname, true, viewer, NULL);
    //openedFiles.insert(edit, openedFile);
    viewer->scaleImage(1);
    viewer->setProperty("attachedSyntaxHighlighter", QVariant::fromValue(static_cast<void*>(0)));
    // QSize sz1 = pixmap.size();
    // QSize sz2 = results->currentWidget()->size();
    //viewer->scaleImage((double)sz2.height()/sz1.height());
    } 
  else {
    QPlainTextEdit* edit = new QPlainTextEdit();
// #ifdef Q_OS_MACOS
    edit->setFont(codeFont);
// #endif
    edit->setPlainText(QString::fromUtf8(file.readAll()));
    edit->setReadOnly(true);
    edit->setWhatsThis("TextViewer");
    //edit->setObjectName(fname);
    results->addTab(edit, tabName);
    SyntaxHighlighter* highlighter = makeSyntaxHighlighter(f.suffix(), edit->document());
    edit->setProperty("attachedSyntaxHighlighter", QVariant::fromValue(static_cast<void*>(highlighter)));
    // We need to keep track of the allocated highlighters to delete them when the corresponding tab is closed !
    }
  results->setCurrentIndex(results->count()-1);
}

void MainWindow::closeResultTab(int index)
{
  //qDebug() << "Closing tab " << index;
  QWidget *edit = results->widget(index);
  if ( edit == NULL ) return; 
  QVariant v = edit->property("attachedSyntaxHighlighter");
  SyntaxHighlighter *hl = static_cast<SyntaxHighlighter*>(v.value<void*>());
  if ( hl != NULL ) delete hl;
  results->removeTab(index);
}

void MainWindow::closeResults()
{
  while ( results->count() > 0 )
    closeResultTab(results->currentIndex());
  updateActions();
}

void MainWindow::openResultFile(QString fname)
{
  // QFileInfo f(wDir + "/" + fname);
  // QString fullName = f.canonicalFilePath();
  QFileInfo f(fname);
  QString wDir = f.canonicalPath();
  //qDebug() << "Displaying file : " << fname;
  QStringList genOpts = compilerOptions->getOptions("general");
  if ( f.suffix() == "dot" ) {
    if ( genOpts.contains("-dot_external_viewer") )
      customView("DOTVIEWER", fname, wDir);
    else {
      dotTransform(f, wDir);
      openResultFile(changeSuffix(fname, ".gif"));
      }
    }
  else if ( f.suffix() == "vcd" ) {
    QString vcdFileName = changeSuffix(fname, ".vcd");
    customView("VCDVIEWER", vcdFileName, wDir);
    }
  else {
    addResultTab(fname);
    }
}

void MainWindow::customView(QString toolName, QString fname, QString wDir)
{
   QString toolPath = compilerPaths->getPath(toolName);
   if ( toolPath.isNull() || toolPath.isEmpty() ) {
     QMessageBox::warning(this, "", "No path specified for " + toolName);
     return;
     }
   QFileInfo f(fname);
   QString sName = f.path () + "/" + f.baseName() + ".gtkw";
   QFile sFile(sName);
   QString args = sFile.exists() ? fname + " " + sName : fname;
   CommandLine cmd(toolPath, args);
   //qDebug() << "customView cmd: " << cmd.toString() << endl;
   if ( ! executeCmd(wDir, cmd.toString() ) ) {
     QMessageBox::warning(this, "", "Could not start " + toolName);
     addResultTab(fname);
     }
}

void MainWindow::resultTabChanged(int index)
{
  Q_UNUSED(index);
  updateActions();
}

// Interface to RFSMC compiler

QStringList MainWindow::compile(QString type, QString sFname, QString baseCmd)
{
  Q_UNUSED(type);
  if ( sFname.isEmpty() ) return QStringList();
  QString rFname = changeSuffix(sFname, ".c");
  QFileInfo fi(sFname);
  QString wDir = fi.absolutePath();
  // QDir dir(wDir);
  //qDebug() << "compile: srcFile=" << sFname << " wDir=" << wDir;
  QString compiler = compilerPaths->getPath("COMPILER");
  if ( compiler.isNull() || compiler.isEmpty() ) compiler = "rfsmc"; // Last chance..
  // if ( targetDir != "" ) dir.mkdir(targetDir);
  // Clean target directory
  // removeFiles(wDir + "/" + targetDir, eraseFirst);
  CommandLine cmd(compiler, baseCmd + " " + sFname);
  qDebug() << "compile command: " << cmd.toString();
  compileErrors.clear();
  if ( executeCmd(wDir, cmd.toString()) )
    return getOutputFiles(type, wDir);
  else {
    QMessageBox::warning(this, "", "Compilation failed\n" + compileErrors);
    return QStringList();
    }
}

QStringList MainWindow::getOutputFiles(QString type, QString wDir)
{
  QString rfile = wDir + "/rfsm.output";
  QFile ff(rfile);
  QStringList res;
  if ( ! ff.exists() ) {
      QMessageBox::warning(NULL, "", "Cannot open file " + rfile);
      return res;
    }
  ff.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream is(&ff);
  while( ! is.atEnd() ) {
      QString of = is.readLine(); // One file per line
      QFileInfo f(of);
      if (  (type == "systemc" && (f.suffix() == "cpp" || f.suffix() == "h"))
         || (type == "vhdl" && f.suffix() == "vhd")
         || (type == "ctask" && f.suffix() == "c")
         || (type == "dot" && f.suffix() == "dot") 
         || (type == "sim" && f.suffix() == "vcd") )
      res.append(wDir+"/"+of);
    }
  ff.close();
  //qDebug() << "Output files: " << res;
  return res;
}

void MainWindow::generate(QString target, QString targetOption, bool withTestbench)
{
  QString fname = generateRfsm(withTestbench);
  if ( fname.isEmpty() ) return;
  QStringList genOpts = compilerOptions->getOptions("general");
  QString targetDirOpt;
  if ( genOpts.contains("-target_dirs") ) {
    QString targetDir = "./" + target;
    targetDirOpt += "-target_dir ./" + target;
    genOpts.removeOne("-target_dirs");
    QDir dir(targetDir);
    if ( ! dir.exists() ) {
      qDebug() << "Creating directory " << targetDir;
      QDir().mkdir(targetDir);
      }
  }
  else
    targetDirOpt += "-target_dir .";
  if ( genOpts.contains("-dot_external_viewer") ) genOpts.removeOne("-dot_external_viewer");
  QStringList opts = genOpts + compilerOptions->getOptions(target);
  if ( target == "sim" ) {
    QFileInfo fi(fname);
    opts << " -main " + fi.baseName();
    }
  QStringList resFiles = compile(target, fname, " " + targetOption + " " + targetDirOpt + opts.join(" "));
  if ( ! resFiles.isEmpty() ) {
    logMessage("Generated file(s) : " + resFiles.join(", "));
    foreach ( QString rFile, resFiles) 
      openResultFile(rFile);
    }
  updateActions();
}

void MainWindow::generateCTask() { generate("ctask", "-ctask", false); }

void MainWindow::generateSystemCModel() { generate("systemc", "-systemc", false); }
void MainWindow::generateSystemCTestbench() { generate("systemc", "-systemc", true); }

void MainWindow::generateVHDLModel() { generate("vhdl", "-vhdl", false); }
void MainWindow::generateVHDLTestbench() { generate("vhdl", "-vhdl", true); }

void MainWindow::runSimulation() { generate("sim", "-sim", true); }

void MainWindow::dotTransform(QFileInfo f, QString wDir)
{
  QString dotProgram = compilerPaths->getPath("DOTPROGRAM");
  if ( dotProgram.isNull() || dotProgram.isEmpty() ) dotProgram = "dot"; // Last chance..
  QString srcFile = f.filePath();
  QString dstFile = changeSuffix(srcFile, ".gif");
  QString opts = ""; // getOption("-dot_options");
  QString wdir = f.canonicalPath();
  CommandLine cmd(dotProgram, opts + " -Tgif -o " + dstFile + " " + srcFile);
  if ( ! executeCmd(wDir, cmd.toString()) )
    QMessageBox::warning(this, "", "Cannot run DOT program");
}

// External command execution

bool MainWindow::executeCmd(QString wDir, QString cmd, bool sync)
{
  bool r = false;
  //qDebug() << "Executing command \"" << cmd << "\" in " << wDir;
  proc.setWorkingDirectory(wDir);
  proc.start(cmd);
  if ( proc.error() == QProcess::FailedToStart ) {
    //qDebug() << "Command failed to start" << endl;
    return false;
    }
  if ( sync ) {
    //qDebug() << "executeCmd: waiting for process to finish" << endl;
    r = proc.waitForFinished(-1);  // No time out
    if ( r == true ) r = proc.exitStatus() == QProcess::NormalExit && proc.exitCode() == 0;
    proc.kill();
    proc.close();
    proc.terminate();
    return r;
    }
  else { // This does not work :(
    //qDebug() << "executeCmd: async process launched" << endl;
    return true;
    }
}

void MainWindow::readProcStdout()
{
  // proc.setReadChannel(QProcess::StandardOutput);
  // while (proc.canReadLine ()) {
  //     QString r = QString(proc.readLine()).remove('\n').remove ('\r');
  //     qDebug() << ">>> " + r;
  //     }
}

void MainWindow::readProcStderr()
{
  proc.setReadChannel ( QProcess::StandardError );
  while (proc.canReadLine ()) 
    compileErrors += QString(proc.readLine());
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
  scaleImage(1.25);
}


void MainWindow::zoomOut()
{
  scaleImage(0.8);
}

void MainWindow::normalSize()
{
  ImageViewer *viewer = selectedImageViewer();
  if ( viewer == NULL ) return;
  viewer->adjustImageSize();
  // updateSelectedTabTitle(); // TODO ? 
}

void MainWindow::fitToWindow()
{
  ImageViewer *viewer = selectedImageViewer();
  if ( viewer == NULL ) return;
  viewer->fitToWindow(fitToWindowAction->isChecked() );
  //updateViewActions(viewer);
}

ImageViewer* MainWindow::selectedImageViewer()
{
  int i = results->currentIndex();
  if ( i < 0 ) return NULL;
  QWidget *tab = results->widget(i);
  return tab->whatsThis() == "ImageViewer" ? (ImageViewer *)tab : NULL;
}

void MainWindow::scaleImage(double factor)
{
  ImageViewer *viewer = selectedImageViewer();
  if ( viewer == NULL ) return;
  double newScaleFactor = factor * viewer->getScaleFactor();
  viewer->scaleImage(newScaleFactor);
  zoomInAction->setEnabled(newScaleFactor < 3.0);
  zoomOutAction->setEnabled(newScaleFactor > 0.33);
  // updateSelectedTabTitle(); // TODO ? 
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
