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
   //void tabChanged(int k);
signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);


    template<class BrfType> void renderSelected(const std::vector<BrfType>& p);
private:
    int w, h; // screen size
    QPoint lastPos; // mouse pos

    enum{MAXSEL=500};

    bool selGroup[MAXSEL];
    float phi, theta, dist;
    int SelIndex();
    QTimer *timer;

};

#endif // GLWIDGETS_H
