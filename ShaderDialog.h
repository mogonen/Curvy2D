#ifndef SHADERDIALOG_H
#define SHADERDIALOG_H

#include "shape.h"
#include "customdialog.h"

class ShaderDialog : public CustomDialog
{

    Q_OBJECT

public:

    ShaderDialog(ImageShape* imgS, QString title, QWidget *parent = 0, char* execLabel = 0, void (*callback)()=0, bool * ischeck = 0);
    void Initialize();

private slots:

    void LoadTextureImage(int );
    void LoadTextureImage();

private:

    Shape_p         m_Shape;

};



#endif // SHADERDIALOG_H
