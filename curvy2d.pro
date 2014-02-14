CONFIG +=

HEADERS += glwidget.h \
    mainwindow.h \
    vec/Vector.h \
    vec/Utility.h \
    vec/Quaternion.h \
    vec/Matrix.h \
    base.h \
    canvas.h \
    sampleshape.h \
    controlpoint.h \
    curve.h \
    customdialog.h \
    shape.h \
    meshshape/patch.h \
    meshshape/spineshape.h \
    meshshape/meshshape.h \
    meshshape/meshdata.h \
    shapecontrol.h \
    meshshape/cmesh.h \
    fileio.h \
    meshshape/meshcommands.h \
    commands.h

SOURCES += glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    vec/Vector.cpp \
    vec/Utility.cpp \
    vec/Quaternion.cpp \
    vec/Matrix.cpp \
    curve.cpp \
    customdialog.cpp \
    shape.cpp \
    meshshape/patch.cpp \
    meshshape/spine.cpp \
    meshshape/meshshape.cpp \
    meshshape/cmesh.cpp \
    meshshape/beziershape.cpp \
    meshshape/meshoperations.cpp \
    meshshape/meshprimitives.cpp \
    uicalbacks_meshshape.cpp \
    shapecontrol.cpp \
    glwidget_render.cpp \
    glwidget_canvas.cpp \
    selection.cpp \
    session.cpp \
    render.cpp \
    meshshape/pattern.cpp \
    fileio.cpp \
    meshshape/meshcommands.cpp \
    commands.cpp

QT           += opengl widgets

DEFINES+=MODELING_MODE

