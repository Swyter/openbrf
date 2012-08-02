#ifndef GLWIDGETS_H
#define GLWIDGETS_H

#include <QGLWidget>
#include <QtGui>
#include "brfData.h"
#include "iniData.h"
#include "ddsData.h"

class BrfData;

//typedef std::map< std::string, std::string > MapSS;

class GLWidget : public QGLWidget
{
    Q_OBJECT
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public:
    GLWidget(QWidget *parent, IniData& i);
    ~GLWidget();

    BrfData* data;
    BrfData* reference; // for things that needs to be present to see other things...
                       // e.g. skeletons for animations
    IniData &inidata;

    int selected;
    int selRefAnimation; // animation selected to view rigged mesh
    int selRefSkin; // rigged mesh
    int selRefSkel; // current skeleton
    int selFrameN; // current selected frame of vertex ani

    float extraMatrix[16]; // matric for temp transforms
    void clearExtraMatrix();

    void selectNone();
    void setEditingRef(bool mode);
    TokenEnum displaying;

    typedef struct {
      int type; // 0 = no err;  1 = cannot find material;  2 = cannot find mesh file; 3 = cannot decrypt mesh file
      QString matName;
      QString texName;
    } MaterialError;
    MaterialError lastMatErr;
    void setMaterialError(int newErr);

    void keyPressEvent( QKeyEvent * event );
    void keyReleaseEvent( QKeyEvent * event );
private slots:
   void onTimer();

public slots:
   void setSelection(const QModelIndexList &, int k);
   void setRefAnimation(int i);
   void setRefSkin(int i);
   void setViewmode(int i);
   void setViewmodeMult(int i);
   int getViewmodeMult()const{return viewmodeMult;}
   int  getRefSkin() const;
   void setRefSkeleton(int i);
   int  getRefSkeleton() const;
   void setWireframe(int i);
   void setLighting(int i);
   void setTexture(int i);
   void setNormalmap(int i);
   void setSpecularmap(int i);
   void setFloor(int i);
   void setRuler(int i);
   void setRulerLenght(int i);
   void setPlay();
   void setStop();
   void setPause(int i=-1);
   void setStepon();
   void setStepback();
   void setColorPerVert();
   void setColorPerRig();
   void setColorPerWhite();
   void setFrameNumber(int);
   void setDefaultBgColor(QColor bgColor, bool alsoCurrent);

   void renderAoOnMeshes(float brightness, float fromAbove);

   void browseTexture();

   int  getFrameNumber() const;
   void showMaterialDiffuseA();
   void showMaterialDiffuseB();
   void showMaterialBump();
   void showMaterialEnviro();
   void showMaterialSpecular();
   void showAlphaTransparent();
   void showAlphaPurple();
   void showAlphaNo();
   void setCommonBBoxOn();
   void setCommonBBoxOff();
   void setInferMaterialOn();
   void setInferMaterialOff();
   void setUseOpenGL2(bool mode);
public:

bool useWireframe, useLighting, useTexture , useNormalmap, useFloor, useRuler, useSpecularmap;
bool ghostMode;
bool fixTexturesOnSight;
int colorMode, rulerLenght;
enum{STOP, PAUSE, PLAY} runningState;
enum{DIFFUSEA, DIFFUSEB, BUMP, ENVIRO, SPECULAR } curMaterialTexture;
enum{TRANSALPHA, PURPLEALPHA, NOALPHA} showAlpha;
bool commonBBox;
bool inferMaterial;
bool useOpenGL2;

float runningSpeed;
int relTime; // msec, zeroed at stop.

signals:
    void signalFrameNumber(int);
    void notifyCheckboardChanged();
    void setTextureData(DdsData d);
    void displayInfo(QString st, int howlong);

protected:
    //MapSS *mapMT;
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    // rendering of stuff
    template<class BrfType> void renderSelected(const std::vector<BrfType>& p);
    // unified rendering of BrfItems...
    void renderBrfItem(const BrfMesh& p);
    void renderBrfItem(const BrfAnimation& p);
    void renderBrfItem(const BrfBody& p);
    void renderBrfItem(const BrfSkeleton& p);
    void renderBrfItem(const BrfTexture& p);
    void renderBrfItem(const BrfMaterial& p);

    void enableMaterial(const BrfMaterial& m);
    void enableDefMaterial();

    // basic rendering of Brf Items & c:
    void renderMesh(const BrfMesh& p, float frame);
    void renderMeshSimple(const BrfMesh& p);
    void renderRiggedMesh(const BrfMesh& p,  const BrfSkeleton& s, const BrfAnimation& a, float frame);
    void renderSkeleton(const BrfSkeleton& p);
    void renderAnimation(const BrfAnimation& p, const BrfSkeleton& s, float frame);
    void renderBody(const BrfBody& p);

    void renderBone(const BrfAnimation& p, const BrfSkeleton& s,  float frame, int i, int lvl)const; // recursive
    void renderBone(const BrfSkeleton& p, int i, int lvl) const; // recursive
    void renderBodyPart(const BrfBodyPart &b) const;


    void renderTexture(const char* name, bool addExtension = true);
    void renderSphereWire() const;
    void renderCylWire() const;
    void renderOcta() const;
    void renderFloor();
    void renderFloorMaybe();
    void renderRuler();
    void mySetViewport(int x,int y,int w,int h);

    void renderAoOnMeshesAllSelected(float brightness, float fromAbove);


    void glClearCheckBoard();
    // rendering mode (just changes of openGL status):
    void setShadowMode(bool on) const;
    void setWireframeLightingMode(bool on, bool light, bool text) const;
    void setTextureName(QString st, int origin, int texUnit);
    static bool fixTextureFormat(QString st);
    void setMaterialName(QString st);
    void setCheckboardTexture();
    void setDummyRgbTexture();
    void setDummySpecTexture();
    void setDummyNormTexture();
    void initOpenGL2();
    bool openGL2ready;
    void initDefaultTextures();
    void initializeGL();

    bool skeletalAnimation();


public:

    QString texturePath[3];
    QString locateOnDisk(QString nome, const char*ext, BrfMaterial::Location *loc);
    void forgetChachedTextures();
    enum{MAXSEL=2000};
    bool selGroup[MAXSEL];
    int selIndex() const;

private:
    int w, h; // screen size
    QColor currBgColor, defaultBgColor; // bgcolors
    QPoint lastPos; // mouse pos
    float phi, theta, dist;
    int tw, th; // texture size, when texture is dysplayed
    float cx, cy, zoom; // for texture display
    vcg::Point3f avatP, avatV; // pos, vel of avatat
    bool keys[5];

    QTimer *timer;

    bool animating;
    bool bumpmapActivated;

    int viewmode;
    int viewmodeMult;
    int dummyRgbTexture, dummySpecTexture, dummyNormTexture, checkboardTexture;
    float currViewmodeHelmet;
    float currViewmodeInterior;
    vcg::Point3f lastCenter;
    float lastScale;
    float closingUp;
    QString renderedTexture;
    // fragment programs
    unsigned int
        fragProgramIron,
        programNormalMapPlain, programNormalMapIron,
        programNormalMapAlpha, programNormalMapShineMap;
    void initFramPrograms();
};

#endif // GLWIDGETS_H
