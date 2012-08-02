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
    int selected;

    void selectNone();
    TokenEnum displaying;

public slots:
   void selectionChanged(const QItemSelection &,const QItemSelection &);
   void tabChanged(int k);
signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    template<class BrfType> void renderSelected(const std::vector<BrfType>& p);
private:
    int w, h; // screen size
    QPoint lastPos; // mouse pos

    enum{MAXSEL=500};

    bool selGroup[MAXSEL];
    float phi, theta, dist;

};

#endif // GLWIDGETS_H
