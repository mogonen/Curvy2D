#include "shapecontrol.h"
#include "commands.h"
//ShapeControl///////////////////////////////////////////////////////////////////////////////////

ShapeControl::ShapeControl():Draggable(true, &_t0){
    isNormalControl = false;
    _theSelected = 0;
    _theHandler = new TransformHandler();
}

void ShapeControl::startSelect(unsigned int svname){
    int svid = svname & ((1<<SV_ID_BIT)-1);
    isNormalControl = svname & (1 << NORMAL_CONTROL_BIT);
    _theSelected = ShapeVertex::get(svid);
}

void ShapeControl::onDrag(const Vec2& t, int){
    if (_theSelected){

        if (Drag::TOOL == Drag::BREAK){
            _theSelected->unpair();
        }
        _theSelected->drag(t, isNormalControl, Drag::CONT == Drag::C2);
    }
}

