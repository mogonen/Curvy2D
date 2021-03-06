/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "glwidget.h"
#include "mainwindow.h"
#include "sampleshape.h"
#include "canvas.h"
#include "meshshape/Patch.h"
#include "meshshape/spineshape.h"
#include "meshshape/meshshape.h"
#include "shapecontrol.h"

ControlPoint_p          ControlPoint::_pTheActive = 0;

MainWindow::MainWindow()
{

    Session::init(this);

    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    glWidget = Session::get()->glWidget();

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(50, 50);

    //_options = new CustomDialog("Options", this);
    //_options->setModal(false);

    createActions();
    createMenus();

    initTools();

    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(glWidgetArea, 0, 0);

    centralWidget->setLayout(centralLayout);
    setWindowTitle(tr("Curvy2D"));
    resize(1200, 900);

    // set the number of patch lines
    Patch::setN(16);
}


void MainWindow::initScene(){
    //init scene
}

void MainWindow::initTools()
{

    QToolBar *toolbar = addToolBar("main toolbar");

    //toolbar->addAction(dragAct); 
    toolbar->addSeparator();

    toolbar->addAction(dragAct);
    toolbar->addAction(extrudeEdgeAct);
    toolbar->addAction(extrudeFaceAct);
    toolbar->addAction(insertSegmentAct);
    toolbar->addAction(deleteFaceAct);

    toolbar->addSeparator();
    toolbar->addAction(assignPatternAct);
    toolbar->addAction(setFoldsAct);
    //init tool options dock
    optionsDockWidget = new QDockWidget(QString("Options"), this);

    attrDockWidget = new QDockWidget(QString("Attributes"), this);



//<<<<<<< HEAD

    optionsStackedWidget = new QStackedWidget();
//    QVBoxLayout *layout = new QVBoxLayout;
//    layout->addWidget(optionsStackedWidget);
//    optionsStackedWidget->setVisible(true);

    attrStackedWidget = new QStackedWidget();
//    layout->addWidget(attrStackedWidget);
//    attrStackedWidget->setVisible(true);
//=======
//    optionsStackedWidget = new QStackedWidget(optionsDockWidget);
//    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(optionsStackedWidget,0,0,1,1,Qt::AlignTop);
//    layout->addWidget(0,1,0,1,1,Qt::AlignTop);
//    layout->setRowStretch(1,1);
//    optionsStackedWidget->setVisible(true);

//    attrStackedWidget = new QStackedWidget(attrDockWidget);
//    QVBoxLayout *layout2 = new QVBoxLayout;
//    layout2->addWidget(attrStackedWidget);
//    layout2->addStretch(0);
//    attrStackedWidget->setVisible(true);
//>>>>>>> cf2484ca3603326ba548c6882ec97d8c814f9ca7



    optionsDockWidget->setWidget(optionsStackedWidget);
//    optionsDockWidget->setLayout(layout);
//    optionsDockWidget->setVisible(true);

    attrDockWidget->setWidget(attrStackedWidget);
//<<<<<<< HEAD
//    attrDockWidget->setLayout(layout);
//    attrDockWidget->setVisible(true);

    this->addDockWidget(Qt::LeftDockWidgetArea, optionsDockWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, attrDockWidget);
    this->setDockOptions(!QMainWindow::AllowTabbedDocks);
//=======
//    attrDockWidget->setLayout(layout2);
//    attrDockWidget->setVisible(true);
//>>>>>>> cf2484ca3603326ba548c6882ec97d8c814f9ca7

    addAttrWidget(new QWidget, 0);//default widget
    createAllOptionsWidgets();
}

int MainWindow::addOptionsWidget(QWidget* widget,int key){
    int id = optionsStackedWidget->addWidget(widget);
    _optionWidgetIDs[key] = id;
    return id;
}

void MainWindow::setOptionsWidget(int key){

    if (key==Options::NONE && (!optionsDockWidget->isVisible() || !optionsStackedWidget->isVisible()))
        return;

    int id = _optionWidgetIDs[key];
    optionsStackedWidget->setCurrentIndex(id);
    optionsDockWidget->setVisible(true); //modify later
}

int MainWindow::addAttrWidget(QWidget* widget, void* key){
    int id = attrStackedWidget->addWidget(widget);
    _attrWidgetIDs[key] = id;
    return id;
}

void MainWindow::setAttrWidget(void* key){

    if (!attrDockWidget->isVisible() || !attrStackedWidget->isVisible())
        return;

    map<void*, int>::const_iterator it = _attrWidgetIDs.find(key);
    if (it == _attrWidgetIDs.end()){
        attrStackedWidget->setCurrentIndex(0);
        return;
    }
    int id = it->second;
    attrStackedWidget->setCurrentIndex(id);
//    attrDockWidget->setVisible(true); //modify later
}

void MainWindow::removeAttrWidget(void* key){

    if (key==0)
        return;

    map<void*, int>::const_iterator it = _attrWidgetIDs.find(key);
    if (it == _attrWidgetIDs.end())
        return;

    int id = it->second;
    QWidget* widget = attrStackedWidget->find(id);
    attrStackedWidget->removeWidget(widget);
    delete widget;
}


void MainWindow::createActions()
{
    //file Actions
    fileNewAct = new QAction(tr("New"), this);
    fileNewAct->setShortcuts(QKeySequence::New);
    connect(fileNewAct, SIGNAL(triggered()), this, SLOT(newFile()));

    fileSaveAct = new QAction(tr("Save"), this);
    fileSaveAct->setShortcuts(QKeySequence::Save);
    connect(fileSaveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    fileSaveAsAct = new QAction(tr("Save As"), this);
    fileSaveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(fileSaveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    fileOpenAct = new QAction(tr("Open"), this);
    fileOpenAct->setShortcuts(QKeySequence::Open);
    connect(fileOpenAct, SIGNAL(triggered()), this, SLOT(openFile()));

    editUndoAct = new QAction(tr("Undo"), this);
    editUndoAct->setShortcuts(QKeySequence::Undo);
    connect(editUndoAct, SIGNAL(triggered()), this, SLOT(undo()));

    editRedoAct = new QAction(tr("Redo"), this);
    editRedoAct->setShortcuts(QKeySequence::Redo);
    connect(editRedoAct, SIGNAL(triggered()), this, SLOT(redo()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    //view
    viewOptionsAct =  new QAction(tr("Show Options"), this);
    connect(viewOptionsAct, SIGNAL(triggered()), this, SLOT(viewOptions()));

    viewAttrAct =  new QAction(tr("Show Attributes"), this);
    connect(viewAttrAct, SIGNAL(triggered()), this, SLOT(viewAttr()));

    patchesOnAct = new QAction(tr("Show &Patches"), this);
    patchesOnAct->setCheckable(true);
    patchesOnAct->setChecked(true);
    connect(patchesOnAct, SIGNAL(triggered()), this, SLOT(togglePatches()));

    dragAct = new QAction(tr("Drag"), this);
    dragAct->setShortcut(Qt::Key_Space);
    connect(dragAct, SIGNAL(triggered()), this, SLOT(flipDrag()));

     //Mesh Shape Actions
    extrudeEdgeAct = new QAction(tr("&Extrude Edge"), this);
    extrudeEdgeAct->setShortcut(tr("Ctrl+E"));
    connect(extrudeEdgeAct, SIGNAL(triggered()), this, SLOT(selectExtrudeEdge()));

    extrudeFaceAct = new QAction(tr("Extrude &Face"), this);
    extrudeFaceAct->setShortcut(tr("Ctrl+Q"));
    connect(extrudeFaceAct, SIGNAL(triggered()), this, SLOT(selectExtrudeFace()));

    insertSegmentAct = new QAction(tr("&Insert Segment"), this);
    insertSegmentAct->setShortcut(tr("Ctrl+I"));
    connect(insertSegmentAct, SIGNAL(triggered()), this, SLOT(selectInsertSegment()));

    deleteFaceAct = new QAction(tr("&Delete Face"), this);
    deleteFaceAct->setShortcut(tr("Ctrl+D"));
    connect(deleteFaceAct, SIGNAL(triggered()), this, SLOT(selectDeleteFace()));

    assignPatternAct = new QAction(tr("Assign &Patern"), this);
    assignPatternAct->setShortcut(tr("Ctrl+P"));
    connect(assignPatternAct, SIGNAL(triggered()), this, SLOT(assignPatternTool()));

    setFoldsAct = new QAction(tr("&Fold"), this);
    setFoldsAct->setShortcut(tr("Ctrl+F"));
    connect(setFoldsAct, SIGNAL(triggered()), this, SLOT(setFoldsTool()));


    QActionGroup* toolset = new QActionGroup(this);

   /* dragAct->setCheckable(true);
    dragAct->setChecked(true);

    renderAct->setCheckable(true);
    renderAct->setChecked(false);
*/
    dragAct->setCheckable(true);
    extrudeEdgeAct->setCheckable(true);
    extrudeFaceAct->setCheckable(true);
    insertSegmentAct->setCheckable(true);
    deleteFaceAct->setCheckable(true);
    assignPatternAct->setCheckable(true);
    setFoldsAct->setCheckable(true);

    extrudeEdgeAct->setActionGroup(toolset);
    extrudeFaceAct->setActionGroup(toolset);
    insertSegmentAct->setActionGroup(toolset);
    deleteFaceAct->setActionGroup(toolset);
    dragAct->setActionGroup(toolset);
    assignPatternAct->setActionGroup(toolset);
    setFoldsAct->setActionGroup(toolset);

    //SHAPE ACTIONS
    shapeInsertTorusAct = new QAction(tr("Torus"), this);
    connect(shapeInsertTorusAct, SIGNAL(triggered()), this, SLOT(newTorus()));

    shapeInsertEllipseAct = new QAction(tr("Ellipse"), this);
    connect(shapeInsertEllipseAct, SIGNAL(triggered()), this, SLOT(insertEllipse()));

    shapeInsertGridAct = new QAction(tr("Grid"), this);
    connect(shapeInsertGridAct, SIGNAL(triggered()), this, SLOT(newGrid()));

    shapeInsert2NGonAct = new QAction(tr("2NGon"), this);
    connect(shapeInsert2NGonAct, SIGNAL(triggered()), this, SLOT(new2NGon()));

    shapeInsertSpineAct = new QAction(tr("Spine"), this);
    connect(shapeInsertSpineAct, SIGNAL(triggered()), this, SLOT(newSpine()));


    shapeDeleteAct = new QAction(tr("Delete"), this);
    shapeDeleteAct->setShortcut(Qt::Key_Delete);
    connect(shapeDeleteAct, SIGNAL(triggered()), this, SLOT(deleteShape()));

    shapeTransformAct = new QAction(tr("Transform"), this);
    shapeTransformAct->setShortcut(tr("Ctrl+T"));
    connect(shapeTransformAct, SIGNAL(triggered()), this, SLOT(transformShape()));

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));   
    fileMenu->addAction(fileNewAct);
    fileMenu->addAction(fileOpenAct);
    fileMenu->addAction(fileSaveAct);
    fileMenu->addAction(fileSaveAsAct);

    fileMenu->addAction("Import");
    fileMenu->addAction("Export");

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editUndoAct);
    editMenu->addAction(editRedoAct);

    viewMenu  = menuBar()->addMenu(tr("View"));
    viewMenu->addAction(viewOptionsAct);
    viewMenu->addAction(viewAttrAct);
    viewMenu->addAction(patchesOnAct);

    insertMenu = menuBar()->addMenu("Create");
    insertMenu->addAction(shapeInsertGridAct);
    insertMenu->addAction(shapeInsert2NGonAct);
    insertMenu->addAction(shapeInsertTorusAct);
    insertMenu->addAction(shapeInsertSpineAct);

    shapeMenu  = menuBar()->addMenu(tr("Shape"));

    shapeMenu->addAction(shapeDeleteAct);

    shapeMenu->addAction(shapeTransformAct);
    shapeMenu->addAction(shapeDeleteAct);


    toolsMenu  = menuBar()->addMenu(tr("Tools"));
    QMenu * GeoTool = toolsMenu->addMenu("Geometry Tools");
    GeoTool->addAction(extrudeEdgeAct);
    GeoTool->addAction(extrudeFaceAct);
    GeoTool->addAction(insertSegmentAct);
    GeoTool->addAction(deleteFaceAct);

    QMenu * PaternTool = toolsMenu->addMenu("Patern Tools");
    PaternTool->addAction(assignPatternAct);
    \
    selectMenu  = menuBar()->addMenu(tr("Select"));
    selectMenu->addAction("Select All");
    selectMenu->addAction("Select Inverse");
    selectMenu->addAction("Grow Selection");
    selectMenu->addAction("Clear Selection");

    windowMenu  = menuBar()->addMenu(tr("Window"));

    helpMenu    = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAct);
}

QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter pixmap size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
            return QSize(width, height);
    }

    return glWidget->size();
}

void MainWindow::keyPressEvent(QKeyEvent *event){

    bool ctrl = (event->modifiers() & Qt::ControlModifier);

    int key = event->key();

    if (key == Qt::Key_H)
    {
        Patch::flipH();
    }

    if (key == Qt::Key_Escape){
        Session::get()->controller()->cancel();
    }

    if (key == Qt::Key_Return){
        Session::get()->controller()->apply();
    }
    glWidget->keyPressEvent(event);
    glWidget->updateGL();
}

//slots

void MainWindow::newFile(){
    Session::get()->reset();
}

void MainWindow::openFile(){

}

void MainWindow::saveFile(){

}

void MainWindow::saveFileAs(){
    QString fname = QFileDialog::getSaveFileName(this,"Save As");
    Session::get()->saveAs(fname.toUtf8().constData());
}

void MainWindow::undo(){
    Session::get()->undo();
}

void MainWindow::redo(){
    Session::get()->redo();
}

void MainWindow::viewAttr(){
    bool ison = true; //= viewAttrAct->isChecked();
    attrDockWidget->setVisible(ison);
    attrStackedWidget->setVisible(ison);
    if (ison)
        setAttrWidget((void*)Session::get()->theShape());
}

void MainWindow::viewOptions(){
    bool ison = true; //viewOptionsAct->isChecked();
    optionsDockWidget->setVisible(ison);
    optionsStackedWidget->setVisible(ison);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Shady"), tr("<b>Shady</b> is an application in progress that implements theoretical framework developed atTexas A&M University for rendering 2D shapes as if they are part of 3D scene."));
}

void MainWindow::unselectDrag()
{
    //dragAct->setChecked(false);
    glWidget->setRender(DRAG_ON, false);
}

void MainWindow::flipDrag()
{
     glWidget->setRender(DRAG_ON, dragAct->isChecked()); //dragAct->isChecked()
     if (dragAct->isChecked())
         setOptionsWidget(Options::DRAG);
}

void MainWindow::togglePatches()
{
     glWidget->setRender(WIREFRAME_ON, patchesOnAct->isChecked()); //dragAct->isChecked()
}

void MainWindow::transformShape(){
    Shape_p shape = Session::get()->theShape();
    if (!shape)
        return;
    Session::get()->controller()->startTransform(shape);
    glWidget->updateGL();
}

void MainWindow::deleteShape(){
   glWidget->clear();
}


