#include <qgl.h>
#include <QDebug>
#include "meshshape/spineshape.h"
#include "meshshape/meshshape.h"
#include "glwidget.h"

#ifdef FACIAL_SHAPE
#include "FacialShape/facialshape.h"
#endif

#include "meshshape/Patch.h"
#include "controlpoint.h"
#include "curve.h"
#include "shapecontrol.h"

#include "canvas.h"


#define theSHAPE Session::get()->theShape()

//selection stuff
bool isInRenderMode(){
    GLint mode;
    glGetIntegerv(GL_RENDER_MODE, &mode);
    return ( mode == GL_RENDER);
}

bool MeshShape::IsSelectMode(SELECTION_e eMode){
    return GetSelectMode() == eMode && !Session::isRender(DRAGMODE_ON);
}

//void Selectable::renderNamed(bool ispush) const{
//    glLoadName(name());
//    render();
//}

void Selectable::render(int mode)
{
    if(mode&DRAG_MODE)
        glLoadName(name());
}

Selectable_p select(GLint hits, GLuint *buff){
   //only one selectable object in the stack for now
   unsigned int name = buff[3];
   Selectable_p pSel =0;
   if ( name & ( 1 << UI_BIT ) )
   {
      pSel =Session::get()->controller();
     Session::get()->controller()->startSelect(name);
   }
   else
      pSel = Session::get()->selectionMan()->get(buff[3]);

   return pSel;
}

inline bool selectionColor(Selectable_p pSel){
    if (pSel->isTheSelected()){
        glColor3f(1.0, 0.1, 0);
        return true;
    }
    else if (pSel->isInSelection())
    {
        glColor3f(1.0, 1.0, 0);
        return true;
    }
    return false;
}
// now all renders here

void ControlPoint::render(int mode) {

    Draggable::render(mode);

    if (isChild() && !isActive())
        return;

    glColor3f(1.0, 1.0, 1.0);
    selectionColor((Selectable_p)this);

    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex3f(P().x, P().y, 0);
    glEnd();


    if (isChild() && isInRenderMode()){
        glBegin(GL_LINES);
        Point p0 = P();
        Point p1 = parent()->P();
        glVertex3f(p0.x, p0.y, 0);
        glVertex3f(p1.x, p1.y, 0);
        glEnd();
    }
}

void Shape::render(int mode)
{
    if(Session::isRender(DRAGMODE_ON))
       Session::get()->controller()->renderControls((Shape_p)this);

    Draggable::render(mode);
    ensureUpToDate();
}

void ShapeControl::renderControls(Shape_p shape)
{
    if (_theHandler->isActive())
        return;
    //could not get pushname popname working!
    glPointSize(4.0);
    SVList verts = shape->getVertices();
    FOR_ALL_CONST_ITEMS(SVList, verts){
        ShapeVertex_p sv = (*it);
        if (sv->parent() && (!_theSelected || (_theSelected != sv->parent() && _theSelected->parent() != sv->parent())) )
            continue;

        unsigned int svname = sv->id() | (1 << UI_BIT);
        if (Session::isRender(NORMALS_ON) && sv->isNormalControl){
            unsigned int svn_name = svname | (1 << NORMAL_CONTROL_BIT);
            Point p1 = sv->P + Point(sv->N*NORMAL_RAD);
            glLoadName(svn_name);
            glBegin(GL_POINTS);
            glColor3f(1.0, 0, 0);
            glVertex3f(p1.x, p1.y, 0);
            glEnd();

            if (isInRenderMode()){
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_LINES);
                glVertex3f(sv->P.x, sv->P.y, 0);
                glVertex3f(p1.x, p1.y, 0);
                glEnd();
            }

        }

        glLoadName(svname);
        glBegin(GL_POINTS);
        glColor3f(0, 0, 0);
        if(sv->flag ==1)
            glColor3f(1.0, 1.0, 0);
        glVertex3f(sv->P.x, sv->P.y, 0);
        glEnd();

        if (isInRenderMode() && sv->parent()){
            glBegin(GL_LINES);
            glVertex3f(sv->P.x, sv->P.y, 0);
            glVertex3f(sv->parent()->P.x, sv->parent()->P.y, 0);
            glEnd();
        }
    }
}

void SpineShape::render(int mode){

    Shape::render(mode);
    if(isInRenderMode()){
        glPointSize(5.0);
        glBegin(GL_POINTS);
            FOR_ALL_CONST_ITEMS(SVertexList, _verts){
                if (!(*it)->pSV)
                    continue;
                Point p = (*it)->P();
                glVertex2f(p.x, p.y);
            }
        glEnd();
    }

    glBegin(GL_LINES);
        FOR_ALL_CONST_ITEMS(SVertexList, _verts){
            if (!(*it)->pSV)
                continue;

            Point p0 = (*it)->P();
            for(SVertexList::iterator itv = (*it)->links.begin(); itv != (*it)->links.end(); itv++)
            {
                Point p1 = (*itv)->P();
                glVertex3f(p0.x, p0.y, 0);
                glVertex3f(p1.x, p1.y, 0);
            }
        }
    glEnd();
}

void MeshShape::render(int mode) {

    Shape::render(mode);

    if ( (isInRenderMode() || (!isInRenderMode()&&IsSelectMode(EDGE)) ))
    {
        EdgeList edges = _control->edges();
        FOR_ALL_CONST_ITEMS(EdgeList, edges)
        {
            render(*it);
        }
    }

    //too messy, fix it!
    if (isInRenderMode() || (!isInRenderMode()&&IsSelectMode(FACE)) )
    {
        qreal r, g, b;
        //diffuse.getRgbF(&r,&g,&b);

        GLfloat mat_diff[]   = { (float)r, (float)g, (float)b, 1.0 };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);

        FaceList faces = _control->faces();
        FOR_ALL_CONST_ITEMS(FaceList, faces)
        {
            render(*it, mode);
        }
    }
}

void MeshShape::render(Edge_p pEdge) const{
    //if (this != theSHAPE)return;

    if (pEdge->pData->pCurve){
        pEdge->pData->pCurve->render(DRAG_MODE);
        return;
    }
    //non selectable line representation
    glColor3f(1.0,1.0, 0);
    Point p0 = P0(pEdge);
    Point p1 = P1(pEdge);

    glBegin(GL_LINES);
    glVertex3f(p0.x, p0.y, 0);
    glVertex3f(p1.x, p1.y, 0);
    glEnd();
}

void MeshShape::render(Face_p pFace, int mode) const
{
    if (pFace->pData->pSurface)
    {
        pFace->pData->pSurface->render(mode);
    }
}

void Curve::render(int mode) {

    Selectable::render(mode);
    glColor3f(0.05, 0.05, 0.05);
    selectionColor((Selectable_p)this);
    if (isTheSelected())
        glColor3f(1.0, 0, 0);

    glBegin(_isClosed? GL_LINE_LOOP : GL_LINE_STRIP);
    FOR_ALL_I(_size){
        Point p = this->P(i);
        glVertex3f(p.x, p.y, 0);
    }
    glEnd();
}

void Patch4::render(int mode)
{
    Patch::render(mode);
    /*for(int j=0; j < Ni; j++){
        for(int i = 0; i< Ni; i++){

            Point p[4];
            p[0] = P(i, j);
            p[1] = P(i+1, j);
            p[2] = P(i+1, j+1);
            p[3] = P(i, j+1);

            if (isInSelection() || !isInRenderMode())
            {
                selectionColor((Selectable_p)this);
                glBegin(GL_POLYGON);
                for(int k=0; k<4; k++){
                    glVertex3f(p[k].x, p[k].y, 0);
                }
                glEnd();
                continue;
            }

            if (isInRenderMode() && Session::isRender(WIREFRAME_ON))
            {
                glColor3f(1.0,1.0,1.0);
                glLineWidth(0.5);
                glBegin(GL_LINE_LOOP);
                for(int k=0; k<4; k++)
                    glVertex3f(p[k].x, p[k].y, 0);
                glEnd();

            }

        }
    }*/


    //glColor3f(0, 1.0 ,0);

    for(int u=0; u < _nU; u++)
    {
        if (_pattern[u] == 1)
            glColor3f(0, 1.0 ,0);
        else if (_pattern[u] == 2)
            glColor3f(1.0, 0 ,0);
        else
            glColor3f(0.8, 0.8, 0.8);

        glBegin(GL_LINE_STRIP);
        for(int i = 0; i< N; i++)
        {               
            Point p = P(0, u, i);
            glVertex3f(p.x, p.y, 0);
        }
        glEnd();
    }

    //glColor3f(1.0, 0, 0);

    for(int v=0; v < _nV; v++)
    {
        if (_pattern[_nU + v] == 1)
            glColor3f(0, 1.0 ,0);
        else if (_pattern[_nU + v] == 2)
            glColor3f(1.0, 0 ,0);
        else
            glColor3f(0.8, 0.8, 0.8);

        glBegin(GL_LINE_STRIP);
        for(int i = 0; i < N; i++)
        {
            Point p = P(1, v, i);
            glVertex3f(p.x, p.y, 0);
        }
        glEnd();
    }

}

void TransformHandler::render() const{

    if (!_pShape || _pShape!=theSHAPE)
        return;

    _handles[0]->render(DRAG_MODE);
    _handles[1]->render(DRAG_MODE);
    //_rotHandle->renderNamed();

    if (isInRenderMode()){
        glColor3f(1.0, 1.0, 1.);
        glBegin(GL_LINE_LOOP);
        glVertex2f(_bbox.P[0].x, _bbox.P[0].y);
        glVertex2f(_bbox.P[0].x, _bbox.P[1].y);
        glVertex2f(_bbox.P[1].x, _bbox.P[1].y);
        glVertex2f(_bbox.P[1].x, _bbox.P[0].y);
        glEnd();
    }
}
