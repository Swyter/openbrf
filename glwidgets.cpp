#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "brfdata.h"
#include "glwidgets.h"

void GLWidget::tabChanged(int k){
  if (k>=0) displaying=TokenEnum(k);
  selectNone();
  update();
}

void GLWidget::selectionChanged(const QItemSelection &selectedn, const QItemSelection &deselected){
  //selectNone();
  //x.merge(newsel,QItemSelectionModel::Toggle);
  int tmp=0;
  for (QItemSelection::ConstIterator i=selectedn.constBegin(); i!=selectedn.constEnd(); i++){
    QItemSelectionRange r = *i;
    QModelIndexList il = r.indexes();
    for (QModelIndexList::ConstIterator ite=il.constBegin(); ite!=il.constEnd(); ite++){
      selGroup[ite->row() ] = true;
      selected = ite->row();
    }
  }

 for (QItemSelection::ConstIterator i=deselected.constBegin(); i!=deselected.constEnd(); i++){
    QItemSelectionRange r = *i;
    QModelIndexList il = r.indexes();
    for (QModelIndexList::ConstIterator ite=il.constBegin(); ite!=il.constEnd(); ite++){
      selGroup[ite->row() ] = false;
      if (selected==ite->row()) selected=-1;
      tmp++;
    }
  }

  this->update();
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
  selectNone();
  phi=theta=0;
  dist = 5;
  displaying = BODY;//MESH;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    //glDeleteLists(object, 1);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 600);
}

/*void GLWidget::setXRotation(int angle)
{
    updateGL();
}*/


void GLWidget::initializeGL()
{
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

#include<wrap\gl\space.h>

void  GLWidget::selectNone(){
  for (int i=0; i<MAXSEL; i++) selGroup[i]=false;
  selected = -1;
}

void BrfMesh::Render() const {
  int framei=0;
  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<face.size(); i++) {
    for (int j=0; j<3; j++) {
      glNormal(vert[face[i].index[j]].n);
      //glNormal(frame[framei].myster[ face[i].index[j] ]);
      glVertex(frame[framei].pos[ vert[face[i].index[j]].index ]);
    }
  }
  glEnd();
}

void drawOcta(){
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,+1);
  glVertex3f(0,0,+1);
  glNormal3f(0,+1,0);
  glVertex3f(0,+1,0);
  glNormal3f(+1,0,0);
  glVertex3f(+1,0,0);
  glNormal3f(0,-1,0);
  glVertex3f(0,-1,0);
  glNormal3f(-1,0,0);
  glVertex3f(-1,0,0);
  glNormal3f(0,+1,0);
  glVertex3f(0,+1,0);
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,0,-1);
  glVertex3d(0,0,-1);
  glNormal3f(0,+1,0);
  glVertex3d(0,+1,0);
  glNormal3f(-1,0,0);
  glVertex3d(-1,0,0);
  glNormal3f(0,-1,0);
  glVertex3d(0,-1,0);
  glNormal3f(+1,0,0);
  glVertex3d(+1,0,0);
  glNormal3f(0,+1,0);
  glVertex3d(0,+1,0);
  glEnd();

}

void BrfSkeleton::Render(int i, int lvl) const{
  glTranslate(bone[i].t);
  glMultMatrixf(bone[i].fullMatrix());
  //glMultMatrixf(bone[i].mat);
  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(0.04,0.06,0.12);
    drawOcta();
  glPopMatrix();
  for (unsigned int k=0; k<bone[i].next.size(); k++){
    glPushMatrix();
    Render(bone[i].next[k],lvl+1);
    glPopMatrix();
  }
}

void BrfSkeleton::Render() const{
  glEnable(GL_COLOR_MATERIAL);
  //glRotatef(180, 0,0,1);
  //glRotatef(90, 1,0,0);
  Render(root,0);
}

void BrfBody::Render() const{

  glDisable(GL_LIGHTING);
  glColor3f(1,1,1);

  for (unsigned int i=0; i<face.size(); i++) {
    glBegin(GL_LINE_LOOP);
    for (unsigned int j=0; j<face[i].size(); j++)
      glVertex(pos[face[i][j]]);
    glEnd();
  }

}

template<class BrfType>
void GLWidget::renderSelected(const std::vector<BrfType>& v){
  Box3f bbox;
  int max=v.size();
  if (max>MAXSEL) max=MAXSEL;


  for (int i=0; i<max; i++) if (selGroup[i]) {
      bbox.Add( v[i].bbox );
  }

  float s = 5/bbox.Diag();
  glScalef(s,s,s);
  glTranslate(-bbox.Center() );

  for (int i=0; i<max; i++) if (selGroup[i]) v[i].Render();

}


void GLWidget::paintGL()
{
  float k=0.5;
  glClearColor(k,k,k,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (displaying == NONE) return;

  //int side = qMin(w, h);
  //glViewport((w - side) / 2, (h - side) / 2, side, side);
  glViewport(0,0,w,h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-1, +1, +1, -1, 4.0, 15.0);
  gluPerspective(60,float(w)/h,1,100);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,-dist);
  glRotatef(theta, 1,0,0);
  glRotatef(phi, 0,1,0);
  //glRotatef(180, 0,0,1);
  //glRotatef(90, 1,0,0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
  glFrontFace(GL_CW);
  if (data) {

    if (displaying == BODY ) renderSelected(data->body);
    if (displaying == MESH ) renderSelected(data->mesh);
    if (displaying == SKELETON ) renderSelected(data->skeleton);

  }

}

void GLWidget::resizeGL(int width, int height)
{
  w = width;
  h = height;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    phi += dx*2.0;
    theta += dy*1.0;
    if (theta>90) theta=90;
    if (theta<-90) theta=-90;
    lastPos = event->pos();
    this->update();
}

