#include "glwidget.h"
#include "canvas.h"

#define UPDATE_GL Session::get()->glWidget()->updateGL();

GLuint texture[30];
void Canvas::insert(Shape_p sp)
{
    _pShape = sp;
    UPDATE_GL
}

void Canvas::remove(Shape_p sp)
{
    clear();
}

void Canvas::clear(){
    delete _pShape;
    _pShape = 0;
}

//NOW GL WIDGET ////////////////////////////////////////////
void GLWidget::clear(){
    _pActiveShape = 0;
    _pCanvas->clear();
    updateGL();
}

void GLWidget::insertShape(Shape_p pShape)
{
    if (!pShape)
        return;

    _pCanvas->insert(pShape);

    Session::get()->activate(pShape);
    updateGL();
}
