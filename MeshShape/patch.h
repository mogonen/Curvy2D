#ifndef PATCH_H
#define PATCH_H

#include <string>
#include "../Base.h"
#include "CMesh.h"
#include "MeshData.h"

using namespace dlfl;

class Patch:public Selectable {

protected:
   Point* _ps;
   Face_p _pFace;
   Point  _K[16]; //bezier surface
   Point   K(int ei, int i);
   int _N;

public:

    Patch(Face_p);
    Corner* C(int i) const {return _pFace->C(i);}

    static bool isH;

    static int N, Ni, NU, NV;
    static double T;


public:

    static void setN(int n){
        N = n;
        Ni = N-1;
        T = 1.0/Ni;
    }
    static void flipH(){
        isH=!isH;
    }

};

class Patch4:public Patch{

    inline int ind(int uv, int n, int i){return i + uv*(_nU*N) + n*N;}
    inline Point P(int uv, int n, int i)const{return _ps[i + uv*(_nU*N) + n*N];}
    int    _nU, _nV;
    int*   _pattern;

protected:

    void onUpdate();

public:

    void render(int mode = 0);
    void init(int nu, int nv);

    void assignPattern(int uv, int off, int len, int * data);

    int U() const {return _nU;}
    int V() const {return _nV;}

    Patch4(Face_p);
    //~Patch4();

};

static double H(double t)
{
    return 3*t*t - 2*t*t;
}

#endif // PATCH_H
