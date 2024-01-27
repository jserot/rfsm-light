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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "state.h"
#include "properties.h"

#include <QMainWindow>
#include <QFileInfo>
#include <QFrame>
#include <QStatusBar>


QT_BEGIN_NAMESPACE
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
class QTabWidget;
class QActionGroup;
class ImageViewer;
class CompilerPaths;
class CompilerOptions;
class Model;
class CommandExec;
class SyntaxChecker;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
   MainWindow();

private slots:
    void stateInserted(State *);
    void transitionInserted(Transition *);
    void stateDeleted(State *);
    void transitionDeleted(Transition *);
    void stateSelected(State *);
    void transitionSelected(Transition *);
    void nothingSelected();
    void modelModified();
    void save();
    void saveAs();
    void openFile();
    void newDiagram();
    void editDiagram(QAction *);    
    void quit();
    void about();
    bool checkSyntax(bool withStimuli);
    bool checkSyntaxWithStimuli();
    bool checkSyntaxWithoutStimuli();
    void renderDot();
    void generateRfsmModel();
    void generateRfsmTestbench();
    void generateCTask();
    void generateSystemCModel();
    void generateSystemCTestbench();
    void generateVHDLModel();
    void generateVHDLTestbench();
    void runSimulation();
    void closeResultTab(int index);
    void resultTabChanged(int index);
    void zoomIn(); 
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void closeResults();
    void setCompilerPaths();
    void setCompilerOptions();
    void setCodeFont();
    void updateCursor();
    void resetCursor();

private:
    void createActions();
    void updateActions();
    void updateViewActions();
    void createMenus();
    void createToolbars();
    void createPropertiesPanel();

    void checkUnsavedChanges();
    QString getCurrentFileName();
    void saveToFile(QString fname);
    QString generateRfsm(bool withTestbench);
    void addResultTab(QString fname);
#ifdef USE_QGV
    void addDotTab(void);
#endif
    void openResultFile(QString fname);
    
    Model *model;
    QFrame *toolbar;
    QButtonGroup *buttons;
    QGraphicsView *view;
    PropertiesPanel* properties_panel;
    QTabWidget *results;
    CompilerPaths *compilerPaths;
    CompilerOptions *compilerOptions;
    QStatusBar *statusBar;
    CommandExec *executor;
    SyntaxChecker *syntaxChecker;

    QAction *newDiagramAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *saveFileAsAction;
    QAction *aboutAction;
    QAction *exitAction;
    QAction *checkSyntaxWithoutStimuliAction;
    QAction *checkSyntaxWithStimuliAction;
    QAction *renderDotAction;
    QAction *generateRfsmModelAction;
    QAction *generateRfsmTestbenchAction;
    QAction *generateCTaskAction;
    QAction *generateSystemCModelAction;
    QAction *generateSystemCTestbenchAction;
    QAction *generateVHDLModelAction;
    QAction *generateVHDLTestbenchAction;
    QAction *runSimulationAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *normalSizeAction;
    QAction *fitToWindowAction;
    QAction *closeResultsAction;
    QAction *pathConfigAction;
    QAction *compilerOptionsAction;
    QAction *fontConfigAction;
    QActionGroup *diagramActions;
    QAction* selectItemAction;
    QAction* addStateAction;
    QAction* addInitStateAction;
    QAction* addTransitionAction;
    QAction* addSelfTransitionAction;
    QAction* deleteItemAction;

    QMenu *aboutMenu;
    QMenu *fileMenu;
    QMenu *checkMenu;
    QMenu *buildMenu;
    QMenu *viewMenu;
    QMenu *configMenu;

    QToolBar *fileToolBar;
    QToolBar *compileToolBar;
    QToolBar *editToolBar;

    QStringList compile(QString target, QString wDir, QString srcFile, QStringList args);
    QStringList getOutputFiles(QString target, QString wdir);
    void generate(QString target, bool withTestbench);
    void customView(QString toolName, QStringList args, QString wDir);
    void customView(QString toolName, QString fname, QString wDir);
    void exportDot();
    void exportRfsmModel();
    void exportRfsmTestbench();
    bool dotTransform(QFileInfo f, QString wDir);
    bool executeCmd(QString wDir, QString cmd, QStringList args, bool sync=true);
    void scaleImage(double factor);

    bool unsaved_changes;
    QString currentFileName;
    QString initDir;
    QWidget* selectedTab();
    double currentScaleFactor;

    QFont codeFont;
    static const QString title;
    static const QString version;
    static const int canvas_width;
    static const int canvas_height;
    static const QList<int> splitterSizes;
    static const double zoomInFactor;
    static const double zoomOutFactor;
    static const double minScaleFactor;
    static const double maxScaleFactor;
    static QStringList guiOnlyOpts;

    QString compileMsgs;
    QString compileErrors;

    void logMessage(QString msg);

public:
  CompilerPaths *getCompilerPaths() const { return compilerPaths; }
  CompilerOptions *getCompilerOptions() const { return compilerOptions; }
  Model* getModel() const { return model; }
  CommandExec *getExecutor() { return executor; }
  SyntaxChecker *getSyntaxChecker() { return syntaxChecker; }
  void setUnsavedChanges(bool unsaved_changes = true);

};

#endif // MAINWINDOW_H
