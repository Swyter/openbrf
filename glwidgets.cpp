#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <vcg\math\quaternion.h>

#include "brfdata.h"
#include "glwidgets.h"

/*void GLWidget::tabChanged(int k){
  if (k>=0) displaying=TokenEnum(k);
  selectNone();
  update();
}*/

void setShadowMode(bool on){
  if (on) {
    glPushMatrix();
    float c[4]={0.175f,0.175f,0.175f,1};
    glLightfv(GL_LIGHT1,GL_AMBIENT,c);
    glDisable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glTranslatef(0,0,0);
    glScalef(1,0,1);
    glColor3f(1,1,1);
    glDisable(GL_COLOR_MATERIAL);

  }
  else {
    glPopMatrix();
    glEnable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);

  }
}

void setWireframeMode(bool on){
  if (on) {
    float c[4]={0.5f,0.5f,0.5f,1};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,c);
    float d[4]={0.0f,0.0f,0.0f,1};
    glLightfv(GL_LIGHT0,GL_SPECULAR,d);
    glPolygonMode(GL_FRONT,GL_LINE);
    glPolygonOffset(-0.1f,-1);
    glEnable(GL_POLYGON_OFFSET_LINE);
  } else {
    glDisable(GL_POLYGON_OFFSET_LINE);
    float c[4]={0.8,0.8,0.8,1};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,c);
    glLightfv(GL_LIGHT0,GL_SPECULAR,c);
    glPolygonMode(GL_FRONT,GL_FILL);
  }
}
void GLWidget::setSelection(const QModelIndexList &newsel, int k){
  if (k>=0) displaying=TokenEnum(k);

  selectNone();

  for (QModelIndexList::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
    selGroup[i->row() ] = true;
    selected = i->row();
  }

  //x.merge(newsel,QItemSelectionModel::Toggle);
  //int tmp=0;
  /*
  for (QItemSelection::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
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
*/
  update();
}

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
  selectNone();
  phi=theta=0;
  dist = 5;
  displaying = NONE;
  timer = new QTimer();
  timer->setInterval(1000/60);
  timer->setSingleShot(false);
  timer->start();

  connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));

}

GLWidget::~GLWidget()
{
    makeCurrent();
    //glDeleteLists(object, 1);
}

void GLWidget::onTimer(){
  if (displaying==ANIMATION) {
    update();
  }
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

const int N_BONES_COLORS=13;
#define f 0.5f
Point3f boneColor[N_BONES_COLORS]={
  Point3f( 1,1,1),
  Point3f( 1,1,0),
  Point3f( 1,0,1),
  Point3f( 1,0,0),
  Point3f( 0,1,1),
  Point3f( 0,1,0),
  Point3f( 0,0,1),

  Point3f( 1,1,f),
  Point3f( 1,f,1),
  Point3f( 1,f,f),
  Point3f( f,1,1),
  Point3f( f,1,f),
  Point3f( f,f,1),
};
#undef f

void BrfRigging::SetColorGl()const{
  Point3f c(0,0,0);
  for (int j=0; j<4; j++) {
    if (boneIndex[j]>-1) c+= boneColor[ boneIndex[j]%N_BONES_COLORS ]*boneWeight[j];
  }
  glColor3fv((GLfloat*)&c);
  //glColor3f(boneColor[2][0],boneColor[2][1],boneColor[2][2]);
}

void BrfMesh::Render() const {
  int framei=0;
  glEnable(GL_COLOR_MATERIAL);
  glColor3f(1,1,1);
  if (!isRigged) glDisable(GL_COLOR_MATERIAL);

  for (int pass=0; pass<2; pass++) {
  setWireframeMode(pass==0);
  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<face.size(); i++) {
    for (int j=0; j<3; j++) {
      rigging[ vert[face[i].index[j]].index ].SetColorGl();

      //glNormal(vert[face[i].index[j]].__norm);
      glNormal(frame[framei].norm[      face[i].index[j]        ]);
      glVertex(frame[framei].pos [ vert[face[i].index[j]].index ]);
    }
  }
  glEnd();
  }
}

void drawSphereWire(){
  for (int i=0; i<10; i++) {
    glBegin(GL_LINE_STRIP);
    float ci = (float)cos(2.0*i/10.0*3.1415);
    float si = (float)sin(2.0*i/10.0*3.1415);
    for (int j=0; j<10; j++) {
      float cj = (float)cos(2.0*j/10.0*3.1415);
      float sj = (float)sin(2.0*j/10.0*3.1415);
      glVertex3f( ci, si*cj, si*sj );
    }
    glEnd();
  }
  for (int i=0; i<10; i++) {
    glBegin(GL_LINE_STRIP);
    float ci = (float)cos(2.0*i/10.0*3.1415);
    float si = (float)sin(2.0*i/10.0*3.1415);
    for (int j=0; j<10; j++) {
      float cj = (float)cos(2.0*j/10.0*3.1415);
      float sj = (float)sin(2.0*j/10.0*3.1415);
      glVertex3f( cj, sj*ci, sj*si );
    }
    glEnd();
  }
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
  glPushMatrix();
  glTranslate(bone[i].t);
  glMultMatrixf(bone[i].fullMatrix());
  //glMultMatrixf(bone[i].mat);
  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(0.04,0.06,0.12);
    drawOcta();
  glPopMatrix();
  for (unsigned int k=0; k<bone[i].next.size(); k++){
    Render(bone[i].next[k],lvl+1);

  }
  glPopMatrix();
}

//static FILE* tmpHack=NULL;

void BrfAnimation::Render(int i, int lvl) const{
  int fi= (curFrame)%(int)frame.size();

  vcg::Point4f p = frame[fi].rot[i];
  vcg::Quaterniond qua(p[1],p[2],p[3],p[0]);
  qua.Normalize();
  qua.Invert();
  vcg::Matrix44d mat;
  double dva[] = {-1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1};
  vcg::Matrix44d swapYZ(dva);
  double dvb[] = {-1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1};
  vcg::Matrix44d invertXY(dvb);
  qua.ToMatrix(mat);

  mat=invertXY*swapYZ*mat*swapYZ;

 // mat = mat.transpose();

  glPushMatrix();

  glTranslate(skel->bone[i].t);

  if (lvl!=0); glMultMatrixd((const GLdouble *) mat.V());

  //glMultMatrixf(skel->bone[i].fullMatrix());
  /*
  if (tmpHack) {
    float *f1 = skel->bone[i].fullMatrix();
    const double *f2 = (const double*)mat.V();
    fprintf(tmpHack,"[%d]\n\n",i);
    for (int i=0; i<16; i++)
      fprintf(tmpHack, "%6.3f%c" ,f1[i],((i+1)%4)?' ':'\n');
    fprintf(tmpHack,"\n");
    for (int i=0; i<16; i++)
      fprintf(tmpHack, "%6.3lf%c",f2[i],((i+1)%4)?' ':'\n');
    fprintf(tmpHack,"\n\n");
  }*/

  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(0.04,0.06,0.12);
    drawOcta();
  glPopMatrix();
  for (unsigned int k=0; k<skel->bone[i].next.size(); k++){
    Render(skel->bone[i].next[k],lvl+1);
  }
  glPopMatrix();
}


void BrfSkeleton::Render() const{
  glEnable(GL_COLOR_MATERIAL);
  Render(root,0);

  setShadowMode(true);
  Render(root,0);
  setShadowMode(false);
}

void BrfAnimation::Render() const{
  if (!skel) return;
  if (skel->bone.size()!=(unsigned int)nbones) return;

  //static bool once=true;
  //if (once)  tmpHack=fopen("debug.txt","wt");


  //if (curFrame>=(int)frame.size()) return;
  glEnable(GL_COLOR_MATERIAL);

  int fi= (curFrame)%(int)frame.size();
  //if (capped) fi=0;
  glPushMatrix();
  glTranslate(frame[fi].tra);
  Render(skel->root,0);
  glPopMatrix();

  //if (once) {
  //  fclose(tmpHack);
  //  once=false;
  //}
  setShadowMode(true);
  glTranslate(frame[fi].tra);
  Render(skel->root,0);
  setShadowMode(false);
}

void BrfBodyPart::Render() const{

  glDisable(GL_LIGHTING);
  switch(type){
    case MANIFOLD: glColor3f(1,1,1); break;
    case FACE: glColor3f(0.75f,1,0.75f); break;
    case SPHERE: glColor3f(1,0.75f,0.75f); break;
    case CAPSULE: glColor3f(0.75,0.75f,1); break;
    default: break;
  }

  if (type==MANIFOLD || type==FACE) {
    for (unsigned int i=0; i<face.size(); i++) {
      glBegin(GL_LINE_LOOP);
      for (unsigned int j=0; j<face[i].size(); j++)
        glVertex(pos[face[i][j]]);
      glEnd();
    }
  }
  else if (type==SPHERE) {
    glPushMatrix();
    glTranslate(center);
    glScalef(radius,radius,radius);
    drawSphereWire();
    glPopMatrix();
  } else if (type==CAPSULE) {
    glPushMatrix();
    glTranslate((center+dir)/2);
    glMultMatrixf((GLfloat*)GetRotMatrix());
    glScalef(1,radius,radius);
    drawSphereWire();
    glPopMatrix();
  }

}

void BrfBody::Render() const{
  for (unsigned int i=0; i<part.size(); i++) part[i].Render();
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

int GLWidget::SelIndex(){
  for (int i=0; i<MAXSEL; i++) if (selGroup[i]) return i;
  return -1;
}


void GLWidget::paintGL()
{

  switch(displaying){
  case NONE: glClearColor(0.2f,0.2f,0.2f,1); break;
  default:  glClearColor(0.5f,0.5f,0.5f,1); break;
  }
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (displaying == NONE) return;

  //int side = qMin(w, h);
  //glViewport((w - side) / 2, (h - side) / 2, side, side);
  glViewport(0,0,w,h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-1, +1, +1, -1, 4.0, 15.0);
  gluPerspective(60,float(w)/h,1,20);
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
    if ( (displaying == ANIMATION) && reference) {
      int si = SelIndex();
      if (si>=0 && si<(int)data->animation.size()) {
        QTime qtime = QTime::currentTime();
        int time =( qtime.msec()+qtime.second()*1000 +qtime.minute()*60000) /100 ;
        BrfAnimation::curFrame=time;
        BrfAnimation::SetSkeleton( reference->getOneSkeleton( int(data->animation[si].nbones ) ) );
      }
    }

    if (displaying == BODY ) renderSelected(data->body);
    if (displaying == MESH ) renderSelected(data->mesh);
    if (displaying == SKELETON ) renderSelected(data->skeleton);
    if (displaying == ANIMATION )  renderSelected(data->animation);

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

void GLWidget::wheelEvent(QWheelEvent *event)
{
  if (event->delta()>0) {
    dist*=1.1;
  } else {
    dist/=1.1;
  }
  update();
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

