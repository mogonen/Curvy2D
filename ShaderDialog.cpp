#include "ShaderDialog.h"

ShaderDialog::ShaderDialog(ImageShape* imgS, QString title, QWidget *parent, char* execLabel, void (*callback)(), bool * ischeck)
    : CustomDialog(title, parent, execLabel,callback,ischeck)
{
    m_imgShape = imgS;
    m_imgShape->SetPenal(this);
    Initialize();
}

void ShaderDialog::Initialize()
{
    //m_returnWidth = this->addDblSpinBoxF("Width:", 0, 2, &m_imgShape->m_width, 2);
    //m_returnHeight = this->addDblSpinBoxF("Height:", 0,2, &m_imgShape->m_height, 2);

    //this->addDblSpinBoxF("Alpha:", 0, 1, &m_imgShape->m_alpha_th, 2);
    //this->addDblSpinBoxF("Strech:", -10, 10, &m_imgShape->m_stretch, 1);
    //this->addDblSpinBoxF("Depth:", 0, 1, &m_imgShape->m_assignedDepth, 2);
    //this->addCheckBox("Shadow Creator:", &m_imgShape->m_shadowCreator);
    this->addComboBox("Cur Texture", "ShapeMap|Dark|Bright");

    QPushButton *texButton = new QPushButton("Set Texture");
    layoutNextElement->addWidget(texButton);
    connect(texButton,SIGNAL(clicked()),this,SLOT(LoadTextureImage()));
}

void ShaderDialog::SetNewSize(double w, double h)
{
    /*m_returnWidth->blockSignals(true);
     *m_returnHeight->blockSignals(true);
     *m_returnWidth->setValue(w);
     *m_returnHeight->setValue(h);
     *m_returnWidth->blockSignals(false);
     *m_returnHeight->blockSignals(false);
    */
}

void ShaderDialog::LoadTextureImage(int cur)
{
    m_imgShape->m_curTexture = cur;
    LoadTextureImage();
}

void ShaderDialog::LoadTextureImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        QPushButton::tr("Open Image"), "/home/", QPushButton::tr("Image Files (*.png *.jpg *.bmp)"));

    switch(m_imgShape->m_curTexture)
    {
        case 0:
            m_imgShape->m_SMFile = fileName;
            m_imgShape->m_texUpdate = ImageShape::UPDATE_SM;
            break;
        case 1:
            m_imgShape->m_DarkFile = fileName;
            m_imgShape->m_texUpdate = ImageShape::UPDATE_DARK;
            break;
        case 2:
            m_imgShape->m_BrightFile = fileName;
            m_imgShape->m_texUpdate = ImageShape::UPDATE_BRIGHT;
            break;
    }
}
