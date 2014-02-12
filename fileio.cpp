#include "fileio.h"
#include "MeshShape/meshshape.h"

bool DefaultIO::save(char *fname){

    ofstream outfile;
    outfile.open (fname);

    outfile.close();

    return true;
}

bool DefaultIO::write(Shape * pShape, ostream& outfile){

   /* outfile <<"shape "<<pShape->P().x<<" "<< pShape->P().y<<" "<<pShape->type()<"/"<<pShape->name()<<"/"<<(pShape->parent()?pShape->parent()->name():0)<<endl;
    SVList verts = pShape->getVertices();
    FOR_ALL_ITEMS(SVList, verts){
        ShapeVertex_p sv = (*it);
        outfile<<"sv "<<sv->P.x<<" "<<sv->P.y<<" "<<sv->N.x<<" "<<sv->N.y<<" "<<sv->N.z<<" "<<sv->id()<<"/"<<(sv->parent()?sv->parent()->id():0)<<endl;
    }*/
}

bool DefaultIO::write(MeshShape * pMS, ostream& outfile){



}
