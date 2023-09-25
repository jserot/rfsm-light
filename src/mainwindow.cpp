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
#include "syntaxHighlighters.h"
#include "compilerPaths.h"
#include "compilerOptions.h"
#include "debug.h"

#include <QtWidgets>
#include <QVariant>

QString MainWindow::title = "RFSM Light";
QString MainWindow::version = "1.3.0";  // Warning : must also be adjusted manually for the About panel
int MainWindow::canvas_width = 1000;
int MainWindow::canvas_height = 1000;

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
    connect(model, SIGNAL(stateDeleted(State*)), this, SLOT(stateDeleted(State*)));
    connect(model, SIGNAL(transitionInserted(Transition*)), this, SLOT(transitionInserted(Transition*)));
    connect(model, SIGNAL(transitionDeleted(Transition*)), this, SLOT(transitionDeleted(Transition*)));
    connect(model, SIGNAL(stateSelected(State*)), this, SLOT(stateSelected(State*)));
    connect(model, SIGNAL(transitionSelected(Transition*)), this, SLOT(transitionSelected(Transition*)));
    connect(model, SIGNAL(nothingSelected()), this, SLOT(nothingSelected()));
    connect(model, SIGNAL(modelModified()), this, SLOT(modelModified()));

    properties_panel = new PropertiesPanel(this); // Warning: the model must be created before 
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

    QString appDir = QApplication::applicationDirPath();
    qDebug() << "APPDIR=" << appDir;
    compilerPaths = new CompilerPaths(appDir + "/rfsm-light.ini", this);
    compilerOptions = new CompilerOptions(appDir + "/options_spec.txt", this);
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

void MainWindow::modelModified()
{
  setUnsavedChanges(true);
}

void MainWindow::about()
{
    QMessageBox::about(this,
      tr("About RFSM Light"),
      tr("<p>Finite State Diagram Editor, Simulator and Compiler</p>\
          <p>version 1.3.0</p>\
         <p><a href=\"github.com/jserot/rfsm-light\">github.com/jserot/rfsm-light</a></p>\
         <p>(C) J. Sérot (jocelyn.serot@uca.fr), 2019-now"));
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
    checkMenu->addAction(checkSyntaxWithoutStimuliAction);
    checkMenu->addAction(checkSyntaxWithStimuliAction);

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
    
    properties_panel->clear(); // TOFIX : this should not be done before the new FSM is validated
    
    QString fname = QFileDialog::getOpenFileName(this, "Open file", initDir, "FSD file (*.fsd)");
    if ( fname.isEmpty() ) return;
    try {
      model->readFromFile(fname);
      }
    catch(const std::exception& e) {
      QMessageBox::warning(this, "Error", "Unable to import : " + QString(e.what()));
      return;
      }
    //qDebug () << model->getIos();
    properties_panel->update();
    currentFileName = fname;
    setUnsavedChanges(false);
}

// Interface to the external syntax verifier

bool MainWindow::checkSyntax(bool withStimuli)
{
  save();
  QFileInfo fi(currentFileName);
  QString wDir = fi.absolutePath();
  QString compiler = compilerPaths->getPath("SYNTAXCHECKER");
  if ( compiler.isNull() || compiler.isEmpty() ) compiler = "rfsmlint"; // Last chance..
  QStringList args = { currentFileName };
  if ( withStimuli == false ) args.append("-no_stimuli"); 
  compileErrors.clear();
  bool ok = executeCmd(wDir, compiler, args, true);
  qDebug() << "syntax checking result is " << ok << " (" << compileErrors << ")";
  if ( ! ok ) QMessageBox::warning(this, "", compileErrors);
  return ok;
}

bool MainWindow::checkSyntaxWithStimuli() { return checkSyntax(true); }
bool MainWindow::checkSyntaxWithoutStimuli() { return checkSyntax(false); }

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
  //qDebug() << "Action " << m;
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

void MainWindow::generateDot()
{
  QString sFname = getCurrentFileName();
  if ( sFname.isEmpty() ) return;
  QString rFname = changeSuffix(sFname, ".dot");
  QStringList opts = compilerOptions->getOptions("dot");
  model->exportDot(rFname, opts);
  logMessage("Wrote file " + rFname);
  openResultFile(rFname);
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
    QFile gf = QFile(gFile);
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
   if ( ! executeCmd(wDir, cmd, args, false ) ) {
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
  compileErrors.clear();
  if ( executeCmd(wDir, compiler, args << sFname, true) )
    return getOutputFiles(target, wDir);
  else {
    QMessageBox::warning(this, "", "Compilation failed\n" + compileErrors);
    return QStringList();
    }
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
  if ( genOpts.contains("-dot_external_viewer") ) genOpts.removeOne("-dot_external_viewer");
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
  if ( executeCmd(wDir, dotProgram, args, true) )
    return true;
  else {
    //QMessageBox::warning(this, "", "Cannot run DOT program : command " + cmd.toString() + " failed");
    QMessageBox::warning(this, "", "Failed to run DOT program");
    return false;
    }
}

// External command execution

bool MainWindow::executeCmd(QString wDir, QString cmd, QStringList args, bool sync)
{
  qDebug() << "executeCmd: wDir=" << wDir << " cmd=" << cmd << " args=" << args;
  proc.setWorkingDirectory(wDir);
  proc.start(cmd,args);
  if ( proc.error() == QProcess::FailedToStart ) {
    qDebug() << "Command failed to start" << Qt::endl;
    return false;
    }
  if ( sync ) {
    bool r = proc.waitForFinished(); 
    QProcess::ExitStatus s = proc.exitStatus();
    int o = proc.exitCode();
    // qDebug() << "executeCmd: finished=" << r;
    qDebug() << "executeCmd: exit status=" << s;
    qDebug() << "executeCmd: exit code=" << o;
    //qDebug() << "executeCmd: stdout=" << compileMsgs;
    qDebug() << "executeCmd: stderr=" << compileErrors;
    proc.kill();
    proc.close();
    proc.terminate();
    return r && s == QProcess::NormalExit && o == 0;
    }
  else { 
    qDebug() << "executeCmd: async process launched";
    return true;
    }
}

void MainWindow::readProcStdout()
{
  proc.setReadChannel(QProcess::StandardOutput);
  while (proc.canReadLine ()) {
    compileMsgs += QString(proc.readLine());
      // QString r = QString(proc.readLine()).remove('\n').remove ('\r');
      // qDebug() << ">>> " + r;
      }
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
