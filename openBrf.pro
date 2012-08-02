# -------------------------------------------------
# Project created by QtCreator 2009-07-25T15:50:47
# -------------------------------------------------
QT += opengl
QT += xml
TARGET = openBrf
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    glwidgets.cpp \
    saveLoad.cpp \
    brfMesh.cpp \
    brfData.cpp \
    selector.cpp \
    tablemodel.cpp \
    brfShader.cpp \
    brfTexture.cpp \
    brfMaterial.cpp \
    brfSkeleton.cpp \
    brfAnimation.cpp \
    brfBody.cpp \
    guipanel.cpp \
    vcgmesh.cpp \
    askBoneDialog.cpp \
    ../../vcglib/wrap/ply/plylib.cpp \
    ../../vcglib/wrap/dae/xmldocumentmanaging.cpp \
    ioSMD.cpp \
    askSkelDialog.cpp \
    askTexturenameDialog.cpp \
    askFlagsDialog.cpp \
    iniData.cpp \
    ioOBJ.cpp
HEADERS += mainwindow.h \
    glwidgets.h \
    saveLoad.h \
    brfMesh.h \
    brfData.h \
    selector.h \
    tablemodel.h \
    brfShader.h \
    brfTexture.h \
    brfToken.h \
    brfMaterial.h \
    brfSkeleton.h \
    brfAnimation.h \
    brfBody.h \
    guipanel.h \
    vcgmesh.h \
    vcgExport.h \
    vcgImport.h \
    askBoneDialog.h \
    ioSMD.h \
    askSkelDialog.h \
    askTexturenameDialog.h \
    askFlagsDialog.h \
    iniData.h
FORMS += guipanel.ui \
    askBoneDialog.ui \
    askSkelDialog.ui \
    askTexturenameDialog.ui \
    askFlagsDialog.ui
INCLUDEPATH += "C:\projects\vcglib"
INCLUDEPATH += "C:\libs\lib3ds-1.3.0"
