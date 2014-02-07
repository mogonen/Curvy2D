#include "Patch.h"
#include "../curve.h"
#include <QDebug>

int     Patch::N;
int     Patch::Ni;
int     Patch::NN;
int     Patch::NN2;
double  Patch::T;
bool    Patch::isH = true;


Patch::Patch(Face_p pF):Selectable(Renderable::SHAPE){
    _ps = 0;
    _pFace = pF;
    _pFace->pData = new FaceData();
    _pFace->pData->pSurface = this;
    this->pRef = (void*)pF;
}

Patch4::Patch4(Face_p pF):Patch(pF){
    init();
}

void Patch4::init()
{
    if (_ps)
        delete _ps;
    _N = N;
    _ps = new   Point[NN];
}

void Patch4::onUpdate(){

    _pFace->update();

    if (_N != N)
        init();

    //init bezier surface points
    for(int i=0; i<4; i++)
    {
        _K[i]= K(0,i);
        _K[i+12]= K(2,i);
    }

    _K[7] = K(1,1);
    _K[11] = K(1,2);

    _K[4] = K(3,1);
    _K[8] = K(3,2);

    _K[5]  = _K[1]  + _K[4]  - _K[0];
    _K[6]  = _K[2]  + _K[7]  - _K[3];
    _K[9]  = _K[13] + _K[8]  - _K[12];
    _K[10] = _K[11] + _K[14] - _K[15];

    //bezier surface interpolation
    for(int j=0; j<N;j++)
        for(int i=0; i<N; i++){
            Point p;
            for(int bj = 0; bj<4; bj++)
                for(int bi = 0; bi<4; bi++)
                    p = p + cubicBernstein(bi, i*T)*cubicBernstein(bj, j*T)*_K[bi+bj*4];
            _ps[ind(i,j)] = p;
        }


}

int Patch::edgeInd(int ei, int i){
    switch(ei){
        case 0:
            return i;
        break;
        case 1:
            return Ni+i*N;
        break;
        case 2:
            return (Ni-i)+Ni*N;
        break;
        case 3:
            return (Ni-i)*N;
        break;
    }
    return -1;
}

int Patch::edgeUInd(int ei, int i){
    switch(ei){
        case 0:
            return i;
        break;
        case 1:
            return Ni+i*N;
        break;
        case 2:
            return i+Ni*N;
        break;
        case 3:
            return i*N;
        break;
    }
    return -1;
}

Point Patch::K(int ei, int i){
    Corner* ci = C(ei);
    Spline* c = ci->E()->pData->pCurve;
    /*if (!c)
        return (i<2)?(ci->P()*(1-i) + ci->next()->P()*i) : (ci->P()*(i-2) + ci->next()->P()*(1-t));*/

    bool curvedir = (ci->V()->pData->pP() == c->pCV(0));
    bool rev = ((ei>1) && curvedir) || ( (ei<2) && !curvedir);
    return (rev)?c->CV(3-i):c->CV(i);
}
