#ifndef GLWIDGETS_H
#define GLWIDGETS_H

#include <QGLWidget>
#include <QTGui>
#include "brfData.h"
class BrfData;

typedef std::map< std::string, std::string > MapSS;

class GLWidget : public QGLWidget
{
    Q_OBJECT
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public:
    GLWidget(QWidget *parent, MapSS* mapMT);
    ~GLWidget();

    BrfData* data;
    BrfData* reference; // for things that needs to be present to see other things...
                       // e.g. skeletons for animations

    int selected;
    int selRefAnimation; // animation selected to view rigged mesh
    int selRefSkin; // rigged mesh
    int selRefSkel; // current skeleton
    int selFrameN; // current selected frame of vertex ani

    void selectNone();
    void setEditingRef(bool mode);
    TokenEnum displaying;

private slots:
   void onTimer();

public slots:
   void setSelection(const QModelIndexList &, int k);
   void setRefAnimation(int i);
   void setRefSkin(int i);
   int  getRefSkin() const;
   void setRefSkeleton(int i);
   int  getRefSkeleton() const;
   void setWireframe(int i);
   void setLighting(int i);
   void setTexture(int i);
   void setFloor(int i);
   void setPlay();
   void setStop();
   void setPause();
   void setColorPerVert();
   void setColorPerRig();
   void setColorPerWhite();
   void setFrameNumber(int);
   int  getFrameNumber() const;
public:
bool useWireframe, useLighting, useTexture , useFloor;
int colorMode;
QString texturePath;

enum{STOP, PAUSE, PLAY} runningState;
float runningSpeed;
int relTime; // msec, zeroed at stop.

signals:
    void signalFrameNumber(int);
protected:
    MapSS *mapMT;
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    // rendering of stuff
    template<class BrfType> void renderSelected(const std::vector<BrfType>& p);
    // unified renering of BrfItems...
    void renderBrfItem(const BrfMesh& p);
    void renderBrfItem(const BrfAnimation& p);
    void renderBrfItem(const BrfBody& p);
    void renderBrfItem(const BrfSkeleton& p);


    // basic rendering of Brf Items & c:
    void renderMesh(const BrfMesh& p, float frame);
    void renderRiggedMesh(const BrfMesh& p,  const BrfSkeleton& s, const BrfAnimation& a, float frame);
    void renderSkeleton(const BrfSkeleton& p);
    void renderAnimation(const BrfAnimation& p, const BrfSkeleton& p, float frame);
    void renderBody(const BrfBody& p);

    void renderBone(const BrfAnimation& p, const BrfSkeleton& p,  float frame, int i, int lvl)const; // recursive
    void renderBone(const BrfSkeleton& p, int i, int lvl) const; // recursive
    void renderBodyPart(const BrfBodyPart &b) const;

    void renderSphereWire() const;
    void renderOcta() const;
    void renderFloor();
    
    // rendering mode (just changes of openGL status):
    void setShadowMode(bool on) const;
    void setWireframeLightingMode(bool on, bool light, bool text) const;
    void setTextureName(const char* text=NULL);
    void initializeGL();

    bool skeletalAnimation();

public:

    enum{MAXSEL=500};
    bool selGroup[MAXSEL];
    int selIndex() const;

private:
    int w, h; // screen size
    float bg_r, bg_g, bg_b; // bgcolor
    QPoint lastPos; // mouse pos
    float phi, theta, dist;
    QTimer *timer;

    bool animating;



};

#endif // GLWIDGETS_H
