#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "brfdata.h"
#include "glwidgets.h"

#include <vcg\math\matrix44.h>
#include <wrap\gl\space.h>

void GLWidget::setRefAnimation(int i){

  selRefAnimation = i-1; // -1 for the "none"
  update();
}

void GLWidget::setEditingRef(bool mode){
  if (mode) {
    bg_r=045; bg_g=0.5; bg_b=0.35 ;
  } else {
    bg_r=0.5; bg_g=0.5; bg_b=0.5 ;
  }
}

void GLWidget::setRefSkin(int i){
  selRefSkin = i-1; // -1 for the "none"
  update();
}

static float floatMod(float a,int b){
  if (b<=0) return 0;
  int ia=int((float)floor((double)a));
  return (a-ia) + ia%b; // which is >=0.0f but <b
}

void GLWidget::renderFloor(){
  this->setWireframeLightingMode(false,false,false);

  glDisable(GL_LIGHTING);
  glPushMatrix();
  float c[4]={0.6f*bg_r,0.6f*bg_g,0.6f*bg_b,1.0f};
  float bg[4]={bg_r,bg_g,bg_b,1.0f};
  glColor3fv(c);
  glScalef(0.5,0.5,0.5);
  glEnable(GL_FOG);
  glFogfv(GL_FOG_COLOR,bg);
  glFogf(GL_FOG_DENSITY,0.125f);
  glBegin(GL_LINES);
  int K = 50;
  for (int i=-K; i<=K; i++){
    glVertex3f(-K, 0, i);
    glVertex3f(+K, 0, i);

    glVertex3f(i, 0, +K);
    glVertex3f(i, 0, -K);
  }
  glEnd();
  glPopMatrix();
  glDisable(GL_FOG);
}

void GLWidget::renderBrfItem (const BrfMesh& p){
  float fi = 0;
  if (p.HasVertexAni()) {
    if (runningState==STOP) fi=1;
    else fi = floatMod( relTime*runningSpeed, p.frame.size()-3) +2;
    if (fi>=(float)p.frame.size()) fi=(float)p.frame.size()-1;
  }
  BrfAnimation* a=NULL;
  BrfSkeleton* s=NULL;
  if (p.isRigged) {
    if (selRefAnimation>=0) {
      a=&(reference->animation[selRefAnimation]);
      s=reference->getOneSkeleton( int(a->nbones ) );
    }
  }

  if (a && s) {
    fi = floatMod( relTime*runningSpeed , a->frame.size() );
    renderRiggedMesh(p,*s,*a,fi);
  } else {
    renderMesh(p,fi);
  }
}

void GLWidget::renderBrfItem (const BrfBody& p){
  renderBody(p);
}
void GLWidget::renderBrfItem (const BrfAnimation& a){
  float fi = floatMod( relTime*runningSpeed , a.frame.size() );
  if (!reference) return;
  BrfSkeleton *s=reference->getOneSkeleton( int(a.nbones ) );
  if (selRefSkin>=0) {
    for (unsigned int i=0; i<reference->mesh.size(); i++){
      if (reference->mesh[i].name[4]==char('A'+selRefSkin))
        renderRiggedMesh(reference->mesh[i],*s,a,fi);
    }
  } else {
    // naked bones
    renderAnimation(a,*s,fi);
  }
}
void GLWidget::renderBrfItem (const BrfSkeleton& p){
  renderSkeleton(p);
}
void GLWidget::setShadowMode(bool on) const{
  if (on) {
    glPushMatrix();
    float c[4]={0.35f*bg_r,0.35f*bg_g,0.35f*bg_b,1};
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

static float* vecf(float ff){
  static float f[4]; f[0]=f[1]=f[2]=ff; f[3]=1; return f;
}

void GLWidget::setWireframeLightingMode(bool wf, bool light, bool tex) const{
  if (tex) {
    glEnable(GL_TEXTURE_2D);
  }
  else glDisable(GL_TEXTURE_2D);

  if (wf) {
    glLightfv(GL_LIGHT0,GL_DIFFUSE, (light && !tex)?vecf(0.6f):vecf(0.0f) );
    glLightfv(GL_LIGHT0,GL_SPECULAR, vecf(0) ) ;

    glLightfv(GL_LIGHT0,GL_AMBIENT,(light || tex)?vecf(0.0) :vecf(0.55));

    //if (tex) glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,vecf(1.0));
    //    else glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,vecf(0.0));

    glPolygonMode(GL_FRONT,GL_LINE);
    glPolygonOffset(-0.1f,-1);
    glEnable(GL_POLYGON_OFFSET_LINE);
  } else {
    glDisable(GL_POLYGON_OFFSET_LINE);

    glLightfv(GL_LIGHT0,GL_DIFFUSE, (light)?vecf(0.75):vecf(0.0));
    glLightfv(GL_LIGHT0,GL_SPECULAR,(light)?vecf(0.15):vecf(0.0));
    glLightfv(GL_LIGHT0,GL_AMBIENT, (light)?vecf(0.1 ):vecf(1  ));
    glPolygonMode(GL_FRONT,GL_FILL);
  }
}

void GLWidget::setTextureName(const char *tn){

    std::string s = (*mapMT)[ tn ];
    if (s!="") {

      glEnable(GL_TEXTURE_2D);
      //QGLContext c; c = *context();
      //c.
      std::string cazz = std::string(texturePath.toAscii().data()) +"\\"+s+std::string(".dds");
      const char* textname = cazz.c_str();
      if (!bindTexture( textname ))
      {
        // small checkboard
        QImage im(QSize(16,16),QImage::Format_ARGB32);
        for (int x=0; x<16; x++)
          for (int y=0; y<16; y++)
            if ((x+y)%2) im.setPixel(QPoint(x,y),0xFFFFFFFF);
            else im.setPixel(QPoint(x,y),0xFF8888FF);
        bindTexture(im);
      }
      glEnable(GL_TEXTURE_2D);
    } else
    glDisable(GL_TEXTURE_2D);

}

// SLOTS
void GLWidget::setWireframe(int i){
  useWireframe = i; update();
}
void GLWidget::setLighting(int i){
  useLighting = i; update();
}
void GLWidget::setTexture(int i){
  useTexture = i; update();
}
void GLWidget::setFloor(int i){
  useFloor = i; update();
}
void GLWidget::setPlay(){
  runningState=PLAY;
  update();
}
void GLWidget::setPause(){
  runningState=PAUSE;
  update();
}
void GLWidget::setStop(){
  runningState=STOP;
  relTime=0;
  update();
}
void GLWidget::setColorPerVert(){
  colorMode=1; update();
}
void GLWidget::setColorPerRig(){
  colorMode=2; update();
}
void GLWidget::setColorPerWhite(){
  colorMode=0; update();
}


void GLWidget::setSelection(const QModelIndexList &newsel, int k){
  if (k>=0) displaying=TokenEnum(k);

  selectNone();

  for (QModelIndexList::ConstIterator i=newsel.constBegin(); i!=newsel.constEnd(); i++){
    selGroup[i->row() ] = true;
    selected = i->row();
  }
  if (k==ANIMATION) {
    runningState= PLAY;
    relTime=0;
  }

  update();
}

GLWidget::GLWidget(QWidget *parent, MapSS* mm)
    : QGLWidget(parent)
{
  mapMT=mm;
  selectNone();
  phi=theta=0;
  dist = 5;
  displaying = NONE;
  timer = new QTimer();
  timer->setInterval(1000/60);
  timer->setSingleShot(false);
  timer->start();

  useTexture=useWireframe=false; useLighting=useFloor=true;

  colorMode=1;
  selRefAnimation = -1;
  selRefSkin = -1;


  relTime=0;
  runningState = STOP;
  runningSpeed = 1/75.0f;

  connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

GLWidget::~GLWidget()
{
    makeCurrent();
    //glDeleteLists(object, 1);
}

void GLWidget::onTimer(){

  QTime qtime = QTime::currentTime();
  int time =( qtime.msec()+qtime.second()*1000 +qtime.minute()*60000);
  static int lasttime=-1;
  if (lasttime==-1) lasttime=time;
  //BrfAnimation::curFrame = BrfMesh::curFrame = time;

  if (animating && runningState==PLAY) {
    relTime += (time-lasttime);
    update();
  }
  lasttime=time;
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(150, 350);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 800);
}


void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}



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


void GLWidget::renderRiggedMesh(const BrfMesh& m,  const BrfSkeleton& s, const BrfAnimation& a, float frame){


  if ((int)s.bone.size()!=a.nbones || m.maxBone>a.nbones) {
    renderMesh(m,frame); // give up rigging mesh
    return;
  }

  glEnable(GL_COLOR_MATERIAL);
  glColor3f(1,1,1);
  if ((!m.isRigged && colorMode==2)|| colorMode==0) glDisable(GL_COLOR_MATERIAL);


  int fi= (int)frame;
  vector<Matrix44f> bonepos = s.GetBoneMatrices( a.frame[fi] );

  int fv =0;
  for (int pass=(useWireframe)?0:1; pass<2; pass++) {
  setWireframeLightingMode(pass==0, useLighting, useTexture);
  if (useTexture) setTextureName(m.material);

  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<m.face.size(); i++) {
    for (int j=0; j<3; j++) {

      const BrfRigging &rig (m.rigging[ m.vert[ m.face[i].index[j] ].index ]);
      if (colorMode==2) rig.SetColorGl();
      else glColor3ubv( (GLubyte*)&m.vert[ m.face[i].index[j] ].col );

      //glNormal(vert[face[i].index[j]].__norm);
      const Point3f &norm(m.frame[fv].norm[      m.face[i].index[j]        ]);
      const Point3f &pos(m.frame[fv].pos [ m.vert[m.face[i].index[j]].index ]);
      Point3f v(0,0,0);
      Point3f n(0,0,0);
      for (int k=0; k<4; k++){
        float wieght = rig.boneWeight[k];
        int       bi = rig.boneIndex [k];
        if (bi>=0) {
          v += (bonepos[bi]* pos  )*wieght;
          n += (bonepos[bi]* norm - bonepos[bi]*Point3f(0,0,0) )*wieght;
        }
      }
      glNormal(n);
      glTexCoord(m.vert[m.face[i].index[j]].ta);
      glVertex(v);
    }
  }
  glEnd();
  }
}
void GLWidget::renderMesh(const BrfMesh &m, float frame){

  int framei = (int) frame;



  glEnable(GL_COLOR_MATERIAL);
  glColor3f(1,1,1);
  if ((!m.isRigged && colorMode==2)|| colorMode==0) glDisable(GL_COLOR_MATERIAL);


  for (int pass=(useWireframe)?0:1; pass<2; pass++) {
  setWireframeLightingMode(pass==0, useLighting, useTexture);
  if (useTexture) setTextureName(m.material);
  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<m.face.size(); i++) {
    for (int j=0; j<3; j++) {
      if (colorMode==2)
        m.rigging[ m.vert[ m.face[i].index[j] ].index ].SetColorGl();
      else glColor3ubv( (GLubyte*)&m.vert[ m.face[i].index[j] ].col );

      //glNormal(vert[face[i].index[j]].__norm);
      glNormal(m.frame[framei].norm[      m.face[i].index[j]        ]);
      glTexCoord(m.vert[m.face[i].index[j]].ta);
      glVertex(m.frame[framei].pos [ m.vert[m.face[i].index[j]].index ]);

    }
  }
  glEnd();
  }
  glDisable(GL_TEXTURE_2D);
}

void GLWidget::renderSphereWire() const{
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
void GLWidget::renderOcta() const{
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

void GLWidget::renderBone(const BrfSkeleton &s, int i, int lvl) const{
  glPushMatrix();
  glTranslate(s.bone[i].t);
  Matrix44f mat = s.bone[i].getRotationMatrix();
  mat = mat.transpose();
  glMultMatrixf((const GLfloat *) mat.V());
  //glMultMatrixf(bone[i].mat);
  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(0.04,0.06,0.12);
    renderOcta();
  glPopMatrix();
  for (unsigned int k=0; k<s.bone[i].next.size(); k++){
    renderBone(s,s.bone[i].next[k],lvl+1);

  }
  glPopMatrix();
}


void GLWidget::renderBone(const BrfAnimation &a,const BrfSkeleton &s, float frame, int i, int lvl) const{
  int fi= (int) frame;
  //int fi= (glWidget->frame/100)%(int)frame.size();
  vcg::Matrix44f mat = a.frame[fi].getRotationMatrix(i);
  mat = mat.transpose();

  glPushMatrix();

  glTranslate(s.bone[i].t);

  if (lvl!=0); glMultMatrixf((const GLfloat *) mat.V());


  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(0.04,0.06,0.12);
    renderOcta();
  glPopMatrix();
  for (unsigned int k=0; k<s.bone[i].next.size(); k++){
    renderBone(a,s,  frame,  s.bone[i].next[k],lvl+1);
  }
  glPopMatrix();
}


void GLWidget::renderSkeleton(const BrfSkeleton &s){
  glEnable(GL_COLOR_MATERIAL);
  setWireframeLightingMode(false,true,false);

  renderBone(s,s.root,0);

  setShadowMode(true);
  renderBone(s,s.root,0);
  setShadowMode(false);

}



void GLWidget::renderAnimation(const BrfAnimation &a, const BrfSkeleton &s, float frame){
  setWireframeLightingMode(false,true,false);

  //if (!skel) return;
  if (s.bone.size()!=(unsigned int)a.nbones) return;

  //static bool once=true;
  //if (once)  tmpHack=fopen("debug.txt","wt");


  //if (curFrame>=(int)frame.size()) return;
  glEnable(GL_COLOR_MATERIAL);

  int fi= (int)frame;
  //if (capped) fi=0;
  glPushMatrix();
  glTranslate(a.frame[fi].tra);
  renderBone(a,s,frame,s.root,0);
  glPopMatrix();

  //if (once) {
  //  fclose(tmpHack);
  //  once=false;
  //}
  setShadowMode(true);
  glTranslate(a.frame[fi].tra);
  renderBone(a,s,frame,s.root,0);
  setShadowMode(false);

}

void GLWidget::renderBodyPart(const BrfBodyPart &b) const{

  glDisable(GL_LIGHTING);
  switch(b.type){
    case BrfBodyPart::MANIFOLD: glColor3f(1,1,1); break;
    case BrfBodyPart::FACE: glColor3f(0.75f,1,0.75f); break;
    case BrfBodyPart::SPHERE: glColor3f(1,0.75f,0.75f); break;
    case BrfBodyPart::CAPSULE: glColor3f(0.75,0.75f,1); break;
    default: break;
  }

  if (b.type==BrfBodyPart::MANIFOLD || b.type==BrfBodyPart::FACE) {
    for (unsigned int i=0; i<b.face.size(); i++) {
      glBegin(GL_LINE_LOOP);
      for (unsigned int j=0; j<b.face[i].size(); j++)
        glVertex(b.pos[b.face[i][j]]);
      glEnd();
    }
  }
  else if (b.type==BrfBodyPart::SPHERE) {
    glPushMatrix();
    glTranslate(b.center);
    glScalef(b.radius,b.radius,b.radius);
    renderSphereWire();
    glPopMatrix();
  } else if (b.type==BrfBodyPart::CAPSULE) {
    glPushMatrix();
    glTranslate((b.center+b.dir)/2);
    glMultMatrixf((GLfloat*)b.GetRotMatrix());
    glScalef(1,b.radius,b.radius);
    renderSphereWire();
    glPopMatrix();
  }

}

void GLWidget::renderBody(const BrfBody& b){
  for (unsigned int i=0; i<b.part.size(); i++) renderBodyPart(b.part[i]);
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


  animating=false;
  for (int i=0; i<max; i++) if (selGroup[i]) {
    renderBrfItem(v[i]);
    if (v[i].IsAnimable()) animating=true;
  }


}

int GLWidget::selIndex() const{
  for (int i=0; i<MAXSEL; i++) if (selGroup[i]) return i;
  return -1;
}


void GLWidget::paintGL()
{

  //switch(displaying){
  //case NONE: glClearColor(0.2f,0.2f,0.2f,1); break;
  //default:
  glClearColor(bg_r,bg_g,bg_b,1); //break;
  //}
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
      int si = selIndex();
      if (si>=0 && si<(int)data->animation.size()) {
        BrfAnimation::SetSkeleton( reference->getOneSkeleton( int(data->animation[si].nbones ) ) );
      }
    }

    if (displaying == BODY ) renderSelected(data->body);
    if (displaying == MESH ) renderSelected(data->mesh);
    if (displaying == SKELETON ) renderSelected(data->skeleton);
    if (displaying == ANIMATION )  renderSelected(data->animation);

    if (useFloor || displaying == ANIMATION || displaying == SKELETON || displaying == BODY)
      renderFloor();
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

