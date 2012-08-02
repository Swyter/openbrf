#ifndef GLWIDGETS_H
#define GLWIDGETS_H

#include <QGLWidget>
#include <QTGui>
#include "brfData.h"
class BrfData;

class GLWidget : public QGLWidget
{
    Q_OBJECT
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    BrfData* data;
    BrfData* reference; // for things that needs to be present to see other things...
                       // e.g. skeletons for animations

    int selected;

    void selectNone();
    TokenEnum displaying;

private slots:
   void onTimer();

public slots:
   void setSelection(const QModelIndexList &, int k);
   void setWireframe(int i);
   void setLighting(int i);
   void setTexture(int i);
   void setPlay();
   void setStop();
   void setPause();
   void setColorPerVert();
   void setColorPerRig();
   void setColorPerWhite();
public:
bool useWireframe, useLighting, useTexture;
int colorMode;

enum{STOP, PAUSE, PLAY} runningState;
float runningSpeed;
int relTime; // msec, zeroed at stop.

signals:

protected:
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
    void renderMesh(const BrfMesh& p, float frame)const;
    void renderSkeleton(const BrfSkeleton& p) const;
    void renderAnimation(const BrfAnimation& p, const BrfSkeleton& p, float frame)const;
    void renderBody(const BrfBody& p) const;

    void renderBone(const BrfAnimation& p, const BrfSkeleton& p,  float frame, int i, int lvl)const; // recursive
    void renderBone(const BrfSkeleton& p, int i, int lvl) const; // recursive
    void renderBodyPart(const BrfBodyPart &b) const;

    void renderSphereWire() const;
    void renderOcta() const;
    
    // rendering mode (just changes of openGL status):
    void setShadowMode(bool on) const;
    void setWireframeLightingTextureMode(bool on, bool light, bool texture) const;
    void initializeGL();


public:

    enum{MAXSEL=500};
    bool selGroup[MAXSEL];
    int selIndex() const;

private:
    int w, h; // screen size
    QPoint lastPos; // mouse pos
    float phi, theta, dist;
    QTimer *timer;

    bool animating;



};

#endif // GLWIDGETS_H
