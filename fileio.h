#ifndef FILEIO_H
#define FILEIO_H

#include <iostream>
#include "base.h"

class MeshShape;
class EllipseShape;

class DefaultIO:public FileIO
{

    bool write(Shape*, ostream& outfile);
    bool read(Shape*, const istream&);

    bool write(MeshShape*, ostream&);
    bool read(MeshShape*, const istream&);

public:

    bool load(char *fname);
    bool save(char *fname);

    bool exportOBJ(Shape* pShape, char *fname);
    bool exportEPS(Shape* pShape, char *fname);

};


#endif // FILEIO_H
