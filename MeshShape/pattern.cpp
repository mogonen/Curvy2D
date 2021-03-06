#include "meshshape.h"
#include "patch.h"

int * parsePattern(string patternstr , int &len)
{
    char * cstr = new char [patternstr.length()+1];
    std::strcpy (cstr, patternstr.c_str());

    int * pattern = new int[24];
    char * tok = strtok(cstr,",");
    len = 0;
    while (tok != NULL)
    {
       pattern[len++] = atoi(tok);
       tok = strtok (NULL, ",");
    }
    return pattern;
}

void  MeshShape::assignPattern(Edge_p pE, string patternstr)
{

    if (!pE )
        return;

    //Mesh_p pMesh = pE->mesh();
    Corner* c0 = pE->C0();
    Corner* c1 = pE->C1();

    Face_p endf = (c1)?c1->F():0;

    int len;
    int * pattern = parsePattern(patternstr, len);

    int off = 0;
    while(c0 && c0->F()!=endf)
    {
        Patch4* patch = (Patch4*)c0->F()->pData->pSurface;
        int i = c0->I();
        patch->assignPattern(i%2, off, len, pattern);
        c0 = c0->prev()->vPrev();
        off+= (i%2) ? patch->V() : patch->U();
    }

}

void MeshShape::setFolds(Edge_p pE, int n, double dmin)
{
    if (!pE )
        return;

    //Mesh_p pMesh = pE->mesh();
    Corner* c0 = pE->C0();
    Corner* c1 = pE->C1();

    Face_p endf = (c1)?c1->F():0;

    while(c0 && c0->F()!=endf)
    {
        Patch4* patch = (Patch4*)c0->F()->pData->pSurface;
        int i = c0->I();
        if (i%2)
            patch->init(n, patch->V());
       else
            patch->init(patch->U(), n);
        c0 = c0->prev()->vPrev();
    }
}
