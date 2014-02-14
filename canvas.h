#ifndef __CANVAS_H__
#define __CANVAS_H__

#include <Qlist>
#include <Qhash>
#include <algorithm>
#include "Shape.h"

#define UNITZ 0.01
#define MAX_LIGHTS 8

class Canvas{

    Shape_p         _pShape;
    friend class    GLWidget;

public:

    Canvas()
    {
        _pShape = 0;
    }

    void insert(Shape_p sp);
    void remove(Shape_p sp);
    void clear();

	int saveTo(const char * fname);
	int loadFrom(const char * fname);

    /*void setImagePlane(const string &filename)
    {
        QImage img_data = QGLWidget::convertToGLFormat(QImage(QString::fromStdString(filename)));
        glGenTextures(30, texture);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA,
                      img_data.width(), img_data.height(),
                      0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.bits() );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }
    */

};

#endif
