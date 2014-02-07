#include "base.h"
#include "mainwindow.h"
#include "shapecontrol.h"
#include "canvas.h"
#include "glwidget.h"

Session* Session::_pSession = 0;

GLWidget* MainWindow::glWidget = 0;

void MainWindow::updateGL(){
    if (glWidget)
        glWidget->updateGL();
}

void Session::init(MainWindow * pMW){
    _pSession = new Session();
    _pSession->_pMainWindow     = pMW;
    _pSession->_pSelectionMan   = new SelectionManager();
    _pSession->_pController     = new ShapeControl();
    _pSession->_pCanvas         = new Canvas();
    _pSession->_pGlWidget       = new GLWidget(_pSession->_pCanvas);
}

Shape* Session::theShape() const{
    return _pGlWidget->activeShape();
}

void Session::insertShape(Shape* pShape){
    if (!pShape)
        return;
    _pCanvas->insert(pShape);
    _pGlWidget->updateGL();
}

void Session::reset(){
    _pCanvas->clear();
    _pGlWidget->clear();
}

/*void Session::cancel(){
    _pController->cancel();
    _pGlWidget->deactivate();
}

void Session::cancel(){
    _pController->cancel();
    _pGlWidget->deactivate();
}*/
