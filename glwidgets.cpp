#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "brfdata.h"
#include "glwidgets.h"

#include "ddsData.h"

#include "bindTexturePatch.h"

#include <vcg\math\matrix44.h>
#include <wrap\gl\space.h>

void GLWidget::setFrameNumber(int i){
  if (i<0) return;
  if (selFrameN==i) return;
  selFrameN = i;
  if (!skeletalAnimation()) runningState=PAUSE;
  update();
}

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

int GLWidget::getRefSkin() const{
  return selRefSkin;
}

int GLWidget::getRefSkeleton() const{
  return selRefSkel;
}

void GLWidget::setRefSkeleton(int i){
  selRefSkel = i;
  update();
}


static float floatMod(float a,int b){
  if (b<=0) return 0;
  int ia=int((float)floor((double)a));
  return (a-ia) + ia%b; // which is >=0.0f but <b
}

void GLWidget::renderRuler(){
  this->setWireframeLightingMode(false,false,false);
  glDisable(GL_LIGHTING);
  float h=0.4f;
  glBegin(GL_LINES);
  for (int i=1; i<=300; i+=1){
    int lvl=0;
    if (i==rulerLenght) continue;
    if (i%5==0) lvl =1;
    if (i%10==0) lvl =2;
    if (i%50==0) lvl =3;
    if (i%100==0)lvl =4;
    float rgb = 0.75f + lvl/16.0;
    glColor3f( rgb, rgb, rgb);
    glVertex3f( 0, h, i*0.01 );
    glVertex3f( 0, h*(6-lvl)/7.0, i*0.01 );
  }
  glColor4f( 1,0,0,0.3 );

  float r = rulerLenght*0.01f;
  glVertex3f(0,-0.2,0);
  glVertex3f(0,h,0);
  glVertex3f(0,-0.2,r);
  glVertex3f(0,h,r);
  glEnd();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  float w=0.1;
  glBegin(GL_QUADS);
  glVertex3f(-w,h,r);
  glVertex3f( w,h,r);
  glVertex3f( w,-0.2,r);
  glVertex3f(-w,-0.2,r);
  glEnd();

  glEnable(GL_CULL_FACE);
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
  glColor4f( 1,1,1,1 );
}

void GLWidget::renderFloor(){
  this->setWireframeLightingMode(false,false,false);

  glDisable(GL_LIGHTING);
  if ( viewmode!=1 ) {

    // solid floor
    //glEnable(GL_FOG);

    const int H = 550; // floor size
    const float h = -0.1; // floor size
    glEnable(GL_BLEND);
    //glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
      glColor4f(bg_r,bg_g,bg_b,0.7f);
      glVertex3f(+H, h, -H);
      glVertex3f(+H, h,  H);
      glVertex3f(-H, h,  H);
      glVertex3f(-H, h, -H);
    glEnd();
    glDisable(GL_BLEND);
  }
  //glEnable(GL_CULL_FACE);

  //glDisable(GL_FOG);

  const int K = 50; // floor size

  glDisable(GL_LIGHTING);
  float c[4]={0.6f*bg_r,0.6f*bg_g,0.6f*bg_b,1.0f};
  float bg[4]={bg_r,bg_g,bg_b,1.0f};
  glColor3fv(c);
  glEnable(GL_FOG);
  glFogfv(GL_FOG_COLOR,bg);
  glFogf(GL_FOG_DENSITY,0.125f);

  glPushMatrix();
  if (displaying == MESH || displaying == BODY)  {
    glRotatef(+90*currViewmodeHelmet,1,0,0);
    glRotatef(-180*currViewmodeHelmet,0,1,0);
  }

  glScalef(0.5,0.5,0.5);
  glBegin(GL_LINES);
  for (int i=-K; i<=K; i++){
    glVertex3f(-K, 0, i);
    glVertex3f(+K, 0, i);

    glVertex3f(i, 0, +K);
    glVertex3f(i, 0, -K);
  }
  glEnd();

  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex3f(0,0,0);
  glEnd();

  glPopMatrix();
  glDisable(GL_FOG);
}

int GLWidget::getFrameNumber() const{
  return selFrameN;
}

void GLWidget::renderTexture(const char* name, bool addExtension){
  glDisable(GL_LIGHTING);

  glPolygonMode(GL_FRONT,GL_FILL);

  //char tname[512];
  //sprintf(tname,"%s%s",name,(addExtension)?".dds":"");


  QString fulltname = locateOnDisk( name, (addExtension)?".dds":"" );
  if (!fulltname.isEmpty()) {
    setTextureName(fulltname);
  } else {
    setCheckboard();
    lastMatErr.texName=QString("%1%2").arg(name).arg((addExtension)?".dds":"");
    setMaterialError(2); // file not found
  }


  float w=1,h=1;
  if (tw*th) {
    if (tw>th) h = th/float(tw);
    if (tw<th) w = tw/float(th);
  }
  if (showAlpha==PURPLEALPHA) {
    glDisable(GL_TEXTURE_2D);
    glColor3f(1,0,1);
    glBegin(GL_QUADS);
    glVertex2f(-w,-h);
    glVertex2f( w,-h);
    glVertex2f( w, h);
    glVertex2f(-w, h);
    glEnd();
  }

  glEnable(GL_TEXTURE_2D);

  glColor3f(1,1,1);

  if (showAlpha==TRANSALPHA ){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  if (showAlpha==PURPLEALPHA){
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
  }
  glDisable(GL_DEPTH_TEST);
  glBegin(GL_QUADS);
  glTexCoord2f(0,1);glVertex2f(-w,-h);
  glTexCoord2f(1,1);glVertex2f( w,-h);
  glTexCoord2f(1,0);glVertex2f( w, h);
  glTexCoord2f(0,0);glVertex2f(-w, h);
  glEnd();

  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

}

void GLWidget::renderBrfItem (const BrfMaterial& t){
  const char *st;
  switch (curMaterialTexture){
  default:
  case DIFFUSEA: st= t.diffuseA; break;
  case DIFFUSEB: st= t.diffuseB; break;
  case BUMP: st= t.bump; break;
  case ENVIRO: st= t.enviro; break;
  case SPECULAR: st= t.spec; break;
  }
  if (!strcmp(st,"none")) st =t.diffuseA;
  renderTexture(st);
}
void GLWidget::renderBrfItem (const BrfTexture& t){
  renderTexture(t.name,false);
}

void GLWidget::renderBrfItem (const BrfMesh& p){
  float fi = 0;
  if (p.HasVertexAni()) {
    if (!skeletalAnimation() && runningState==PLAY) {
      fi = floatMod( relTime*runningSpeed, p.frame.size()-3) +2;
      if (fi<0) fi=0;
      if (selFrameN != (int)fi) {
        selFrameN = (int)fi;
        if (selFrameN>=(int)p.frame.size()) selFrameN = p.frame.size()-1;
        emit(signalFrameNumber(selFrameN));
      }
    } else fi=selFrameN;
    if (fi>=(float)p.frame.size()) fi=(float)p.frame.size()-1;
    if (fi<0) fi=0;

  }
  BrfAnimation* a=NULL;
  BrfSkeleton* s=NULL;
  if (p.isRigged) {
    if (selRefAnimation>=0) {
      a=&(reference->animation[selRefAnimation]);
      int si = reference->getOneSkeleton( int(a->nbones ), getRefSkeleton() );
      if (si>=0) s=&(reference->skeleton[si]);
      //selRefSkel = si;
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
  int fii = int(fi);
  static int lastFii = -1;
  if (fii!=lastFii) emit(signalFrameNumber(fii+1));
  lastFii = fii;

  //if (runningState==STOP) fi=a.frame.size()-1;
  if (!reference) return;


  int si = reference->getOneSkeleton( int(a.nbones ), getRefSkeleton() );
  if (si==-1) return ; // no skel, no render
  const BrfSkeleton &s(reference->skeleton[si]);


  if (selRefSkin>=0) {
    for (unsigned int i=0; i<reference->mesh.size(); i++){
      if (reference->mesh[i].name[4]==char('A'+selRefSkin))
        renderRiggedMesh(reference->mesh[i],s,a,fi);
    }
    //BrfMesh tmp = reference->GetCompleteSkin(selRefSkin);
    //renderRiggedMesh(tmp,s,a,fi);
  } else {
    // naked bones
    renderAnimation(a,s,fi);
  }
}
void GLWidget::renderBrfItem (const BrfSkeleton& p){
  renderFloor();
  if (selRefSkin>=0) {
    ghostMode=true;
    for (unsigned int i=0; i<reference->mesh.size(); i++){
      if (reference->mesh[i].name[4]==char('A'+selRefSkin))
        renderMesh(reference->mesh[i],0);
    }
    ghostMode=false;
    glClear(GL_DEPTH_BUFFER_BIT);
  }
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
  glEnable(GL_LIGHTING);
  if (ghostMode) {
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_LIGHTING);
    glEnable(GL_FOG);

    glLightfv(GL_LIGHT0,GL_DIFFUSE, vecf(0.5f));
    glLightfv(GL_LIGHT0,GL_SPECULAR, vecf(0) ) ;

    glLightfv(GL_LIGHT0,GL_AMBIENT, vecf(0.5f));

    //glFogf(GL_FOG_DENSITY,0.15f);
    //float c[4]={0,0,0,0};
    //glFogfv(GL_FOG_COLOR,c);
    return;
  }
  if (tex) {
    glEnable(GL_TEXTURE_2D);
  }
  else glDisable(GL_TEXTURE_2D);
  glDisable(GL_FOG);

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



void GLWidget::setCheckboard(){
    // small checkboard
    const int N = 16;
    QImage im(QSize(N,N),QImage::Format_ARGB32);
    for (int x=0; x<N; x++)
      for (int y=0; y<N; y++)
        if ((x+y)%2) im.setPixel(QPoint(x,y),0xFFFFFFFF);
        else im.setPixel(QPoint(x,y),0xFFAAAAFF);
    bindTexture(im);
    tw=th=16;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}

bool GLWidget::fixTextureFormat(QString st){

  FILE* f = fopen(st.toAscii().data(),"r+b");
  if (!f) return false;
  unsigned int h[22]; // header
  fread(h,4 , 22,f); // 4 = sizeof uint
  if (h[0]!=0x20534444) {fclose(f); return false;}// "DDS "
  bool dxt1 = (h[21] ==  0x31545844);
  bool dxt5 = (h[21] ==  0x35545844);
  if (!dxt1 && !dxt5 ) {fclose(f); return false;}
  if (h[5]!=0) {fclose(f); return false;} // problem was not the one expected
  h[5] = h[3]*h[4] / ((dxt1)?2:1);
  // to fix llod:
  if (h[7]==0) h[7] = 1; // at least one mipmap lvl
  rewind(f);
  if (fwrite(h,4,8,f)!=8) {fclose(f); return false;}
  fclose(f);
  return true;
}

void GLWidget::forgetChachedTextures(){
  ::forgetChachedTextures();
}

void GLWidget::setTextureName(QString s){
  glEnable(GL_TEXTURE_2D);
  DdsData data;
  if (myBindTexture( s, data )){
     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
     setMaterialError(0);
     tw=data.sx;
     th=data.sy;
  } else {/*
     if (fixTexturesOnSight) {
       if (fixTextureFormat(s)) {
         myBindTexture( s );
         glEnable(GL_TEXTURE_2D);
         return;
       }
     }*/
     setMaterialError(3); // format is wrong
     lastMatErr.texName=s;
     setCheckboard();
  }
  emit(setTextureData(data));
  glEnable(GL_TEXTURE_2D);
}

void GLWidget::setMaterialError(int i){
  if (lastMatErr.type!=i) {
    lastMatErr.type=i;
    emit(notifyCheckboardChanged());
  }
}


void GLWidget::setMaterialName(QString st){
  lastMatErr.matName=QString(st);

  BrfMaterial *m = inidata.findMaterial(st);
  if (m) {
    lastMatErr.texName=QString("%1.dds").arg(st);
    QString s = locateOnDisk(QString(m->diffuseA),".dds",&(m->location));
    if (!s.isEmpty()) setTextureName(s);
    else {
      setMaterialError(2); // file not found
      setCheckboard();
    }
  }
  else {
    setMaterialError(1); // material not found
    setCheckboard();
  }
}

// SLOTS
void GLWidget::setWireframe(int i){
  useWireframe = i; update();
}
void GLWidget::setRuler(int i){
  useRuler = i; update();
}
void GLWidget::setRulerLenght(int i){
  rulerLenght = i; update();
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
void GLWidget::setPause(int i){
  if (i!=-1) {
    relTime = (int)((i-1+runningSpeed/2)/runningSpeed);
  }
  runningState=PAUSE;
  update();
}
void GLWidget::setStepon(){
  relTime=relTime+int(1.0/runningSpeed);
  runningState=PAUSE;
  update();
}
void GLWidget::setStepback(){
  relTime=relTime-int(1.0/runningSpeed);
  runningState=PAUSE;
  selFrameN--;
  update();
}
void GLWidget::clearExtraMatrix(){
  for (int i=0; i<16; i++) extraMatrix[i]=((i%5)==0);
}

void GLWidget::setStop(){
  runningState=STOP;
  relTime=0;
  if (!skeletalAnimation()) {
    selFrameN = 1;
    emit(signalFrameNumber(selFrameN));
  }
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

GLWidget::GLWidget(QWidget *parent, IniData &_inidata)
    : QGLWidget(parent), inidata(_inidata)
{
  //grabKeyboard ();
  selectNone();
  phi=theta=0;
  dist = 5;
  zoom = 1;
  cx = cy = 0;
  currViewmodeHelmet=currViewmodeInterior=viewmode=0;
  avatP.SetZero();
  avatV.SetZero();
  displaying = NONE;
  timer = new QTimer();
  timer->setInterval(1000/60);
  timer->setSingleShot(false);
  timer->start();

  lastCenter.SetZero();
  lastScale = 1;
  closingUp = 0;

  keys[0]=keys[1]=keys[2]=keys[3]=keys[4]=false;

  useTexture=true; useWireframe=false; useLighting=useFloor=true; useRuler=false;
  rulerLenght = 100;
  ghostMode = false;
  curMaterialTexture = DIFFUSEA;

  colorMode=1;
  selRefAnimation = -1;
  selRefSkin = -1;
  selFrameN = 0;
  selRefSkel = 0;
  showAlpha=NOALPHA;
  commonBBox = false;

  relTime=0;
  runningState = STOP;
  runningSpeed = 1/75.0f;

  clearExtraMatrix();

  connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
  tw=th=1;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    //glDeleteLists(object, 1);
}

void GLWidget::setViewmodeMult(int i){
  viewmodeMult=i;
  update();
}
void GLWidget::setViewmode(int i){
  viewmode=i;
  closingUp = 0;
  if (i==2) {
    setFocusPolicy(Qt::WheelFocus);
    this->setFocus();
    float ph = phi*M_PI/180.0;
    float K = 1;
    avatP=
      lastCenter +
      vcg::Point3f(sin(ph)*dist/lastScale*K,0.6,cos(ph)*dist/lastScale*K);
    //qDebug("Scale = %f",lastScale);
    emit(displayInfo("Scene mode: navigate with mouse and WASD (levitate with wheel, zoom in with shift)", 10000));
  } else {
    //setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::WheelFocus);
    if (i==1)
    emit(displayInfo("Helmet mode: for objects with vertical Z axis, like M&B helmets or weapons.", 8000));
    else
    emit(displayInfo("Default mode: rotate objects with mouse, zoom in/out with wheel.", 8000));
  }
  update();
}

void GLWidget::onTimer(){

  QTime qtime = QTime::currentTime();
  int time =( qtime.msec()+qtime.second()*1000 +qtime.minute()*60000);
  static int lasttime=-1;
  static int done;
  if (lasttime==-1) {lasttime=time; done=time;}
  int elapsed = time-lasttime;
  lasttime=time;


  //BrfAnimation::curFrame = BrfMesh::curFrame = time;
  bool needUpdate=false;
  if (animating && runningState==PLAY) {
    relTime += elapsed;
    needUpdate=true;
  }
  if (viewmode==2) {
    if (currViewmodeInterior<1) {
      currViewmodeInterior+=0.125;
      needUpdate=true;
    }
  } else {
    if (currViewmodeInterior>0) {
      currViewmodeInterior-=0.125;
      needUpdate=true;
    }
  }
  if (viewmode==1) {
    if (currViewmodeHelmet<1) {
      currViewmodeHelmet+=0.125;
      needUpdate=true;
    }
  } else {
    if (currViewmodeHelmet>0) {
      currViewmodeHelmet-=0.125;
      needUpdate=true;
    }
  }
  if (viewmode==2) {
    const float acc = 0.006;
    float ph = phi*M_PI/180.0;
    for (; done<time; done+=10) {
      if (keys[0]) avatV-=vcg::Point3f(sin(ph),0,cos(ph))*acc*0.8 ;
      if (keys[1]) avatV+=vcg::Point3f(sin(ph),0,cos(ph))*acc*0.8 ;
      if (keys[2]) avatV+=vcg::Point3f(cos(ph),0,-sin(ph))*acc ;
      if (keys[3]) avatV-=vcg::Point3f(cos(ph),0,-sin(ph))*acc*0.7 ;
      if (!keys[4]) {
        if (closingUp>0) {
          closingUp-=1/16.0;
          needUpdate=true;
        }
      } else {
        if (closingUp<1) {
          closingUp+=1/16.0;
          needUpdate=true;
        }
      }
      if (avatV.SquaredNorm()>0.00001) {
        needUpdate=true;
      }
      avatV*=0.95;
      avatP+=avatV;

    }
  }
  if (needUpdate) update();
}

void GLWidget::keyPressEvent( QKeyEvent * e ){
  if (e->key()==Qt::Key_W || e->key()==Qt::Key_Forward) keys[0]=true;
  if (e->key()==Qt::Key_S || e->key()==Qt::Key_Back) keys[1]=true;
  if (e->key()==Qt::Key_A || e->key()==Qt::Key_Left) keys[2]=true;
  if (e->key()==Qt::Key_D || e->key()==Qt::Key_Right) keys[3]=true;
  if (e->key()==Qt::Key_Shift) keys[4]=true;
  else QWidget::keyPressEvent(e);
}

void GLWidget::keyReleaseEvent( QKeyEvent * e ){
  if (e->key()==Qt::Key_W || e->key()==Qt::Key_Forward) keys[0]=false;
  if (e->key()==Qt::Key_S || e->key()==Qt::Key_Back) keys[1]=false;
  if (e->key()==Qt::Key_A || e->key()==Qt::Key_Left) keys[2]=false;
  if (e->key()==Qt::Key_D || e->key()==Qt::Key_Right) keys[3]=false;
  if (e->key()==Qt::Key_Shift) keys[4]=false;
  else QWidget::keyReleaseEvent(e);
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

bool GLWidget::skeletalAnimation(){
  if (displaying==MESH) {
    int max=data->mesh.size();
    if (max>MAXSEL) max=MAXSEL;

    for (int i=0; i<max; i++) if (selGroup[i]) {
      if (data->mesh[i].isRigged) return true;
    }
  }
  return false;

}

void GLWidget::renderRiggedMesh(const BrfMesh& m,  const BrfSkeleton& s, const BrfAnimation& a, float frame){
  int fv =selFrameN;

  if (fv>=(int)m.frame.size()) fv= m.frame.size()-1;
  if (fv<0) fv= 0;

  if ((int)s.bone.size()!=a.nbones || m.maxBone>a.nbones) {

    renderMesh(m,fv); // give up rigging mesh
    return;
  }

  glEnable(GL_COLOR_MATERIAL);
  glColor3f(1,1,1);
  if ((!m.isRigged && colorMode==2)|| colorMode==0) glDisable(GL_COLOR_MATERIAL);


  int fi= (int)frame;
  vector<Matrix44f> bonepos = s.GetBoneMatrices( a.frame[fi] );

  for (int pass=(useWireframe)?0:1; pass<2; pass++) {
  setWireframeLightingMode(pass==0, useLighting, useTexture);
  if (useTexture) setMaterialName(m.material);

  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<m.face.size(); i++) {
    for (int j=0; j<3; j++) {

      const BrfRigging &rig (m.rigging[ m.vert[ m.face[i].index[j] ].index ]);
      if (colorMode==2) rig.SetColorGl();
      else {
        GLubyte* c = (GLubyte*)&m.vert[ m.face[i].index[j] ].col;
        glColor3ub( c[2],c[1],c[0] );
      }

      //glNormal(vert[face[i].index[j]].__norm);
      const Point3f &norm(m.frame[fv].norm[      m.face[i].index[j]        ]);
      const Point3f &pos(m.frame[fv].pos [ m.vert[m.face[i].index[j]].index ]);
      Point3f v(0,0,0);
      Point3f n(0,0,0);
      for (int k=0; k<4; k++){
        float wieght = rig.boneWeight[k];
        int       bi = rig.boneIndex [k];
        if (bi>=0 && bi<(int)bonepos.size()) {
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
  if (useTexture) setMaterialName(m.material);
  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<m.face.size(); i++) {
    for (int j=0; j<3; j++) {
      if (colorMode==2)
        m.rigging[ m.vert[ m.face[i].index[j] ].index ].SetColorGl();
      else {
        GLubyte* c = (GLubyte*)&m.vert[ m.face[i].index[j] ].col;
        glColor3ub( c[2],c[1],c[0] );
      }

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

void GLWidget::renderCylWire() const{
  glEnable(GL_LIGHTING);
  const int N = 10;
  for (int i=0; i<N; i++) {
    float ci = (float)cos(2.0*i/N*3.1415);
    float si = (float)sin(2.0*i/N*3.1415);
    glBegin(GL_LINE_LOOP);
      glNormal3f( 1, si, ci );
      glVertex3f( 1, si, ci );
      glNormal3f( 1, -si, -ci );
      glVertex3f( 1, -si, -ci );
      glNormal3f(-1, -si, -ci );
      glVertex3f(-1, -si, -ci );
      glNormal3f(-1, si, ci );
      glVertex3f(-1, si, ci );
    glEnd();
  }
  for (int j=-3; j<=+3; j++) {
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<N; i++) {
      float ci = (float)cos(2.0*i/N*3.1415);
      float si = (float)sin(2.0*i/N*3.1415);
      glNormal3f(0,ci,si);
      glVertex3f( j/3.0f, si, ci );
    }
    glEnd();
  }
  glDisable(GL_LIGHTING);
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

  glMultMatrixf((const GLfloat *) mat.V());
  //glMultMatrixf(bone[i].mat);
  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(BrfSkeleton::BoneSizeX(),BrfSkeleton::BoneSizeY(),BrfSkeleton::BoneSizeZ());


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

  glPushMatrix();
  glTranslate(s.bone[i].t);
  if (lvl!=0); glMultMatrixf((const GLfloat *) mat.V());

  glPushMatrix();
    glColor3ub(255-lvl*30,255-lvl*30,255);
    glScalef(BrfSkeleton::BoneSizeX(),BrfSkeleton::BoneSizeY(),BrfSkeleton::BoneSizeZ());
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
  setWireframeLightingMode(true,false,false);
  glLineWidth(2.0);
  glEnable(GL_FOG);
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
    renderCylWire();
    glPopMatrix();
  }
  glDisable(GL_FOG);
  glLineWidth(1.0);

}

void GLWidget::setCommonBBoxOn(){
  commonBBox = true;
  update();
}
void GLWidget::setCommonBBoxOff(){
  commonBBox = false;
  update();
}

void GLWidget::renderBody(const BrfBody& b){
  for (unsigned int i=0; i<b.part.size(); i++) renderBodyPart(b.part[i]);
}


template<class BrfType>
void GLWidget::renderSelected(const std::vector<BrfType>& v){
  Box3f bbox;
  bbox.SetNull();
  int max=v.size();
  if (max>MAXSEL) max=MAXSEL;

  int nsel=0;
  for (int i=0; i<max; i++) if (selGroup[i] || commonBBox) {
    bbox.Add( v[i].bbox );
    if (selGroup[i]) nsel++;
  }
  animating=false;

  if (displaying == MESH || displaying == BODY) {
    glRotatef(180*currViewmodeHelmet,0,1,0);
    glRotatef(-90*currViewmodeHelmet,1,0,0);
  }

  glTranslate(-avatP *currViewmodeInterior);


  if (!bbox.IsNull()) {
    float s = 5/bbox.Diag();
    lastScale = s;
    s = s*(1-currViewmodeInterior)+currViewmodeInterior;
    glScalef(s,s,s);

    Point3f ta = -bbox.Center(),  // center on object
            tb(0,0,-rulerLenght/100.0); // center on ruler
    lastCenter = -ta;

    // interpolate between the two centers
    static float k=1.0;

    if (useRuler && displaying == MESH)
    { if (k!=0) { animating =true; k-=0.1; if (k<0) k=0;} }
    else
    { if (k!=1) { animating =true; k+=0.1; if (k>1) k=1;} }

    glTranslate( (ta*k + tb*(1-k))*(1-currViewmodeInterior));

  }

  glPushMatrix();
  glMultMatrixf(extraMatrix);

  bool _viewmodeMult = viewmodeMult;
  if (displaying==TEXTURE || displaying==MATERIAL) _viewmodeMult=true;

  for (int i=0,seli=0; i<max; i++) if (selGroup[i]) {
    {
      if (_viewmodeMult) {
        int ncol; ncol = (int)(ceil(sqrt(nsel)));
        int nrow = ncol;
        if (ncol*(ncol-1)>=nsel) {
          if (w<h) ncol--; else nrow--;
        }
        // multiple object view mode
        int x = seli%ncol;
        int y = seli/ncol;
        int qx = this->size().width()/ncol;
        int qy = this->size().height()/nrow;

        mySetViewport(x*qx,(nrow-1-y)*qy,qx,qy);
        seli++;
      }
      else
      {
        mySetViewport(0,0,this->size().width(),this->size().height());
      }
    }
    renderBrfItem(v[i]);
    if (v[i].IsAnimable()) animating=true;

    if (_viewmodeMult) renderFloorMaybe();
  }
  if (!_viewmodeMult) renderFloorMaybe();

  glPopMatrix();


}

int GLWidget::selIndex() const{
  for (int i=0; i<MAXSEL; i++) if (selGroup[i]) return i;
  return -1;
}

void GLWidget::glClearCheckBoard(){
  float K = 0.075;
  glColor3f(bg_r+K,bg_g+K,bg_b+K);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0,w,0,h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  int N =16;
  glBegin(GL_QUADS);
  for (int x=0; x<(w+N-1)/N; x++)
  for (int y=0; y<(h+N-1)/N; y++) {
    if ((x+y)&1){
    glVertex2i(x*N+N,y*N+N);
    glVertex2i(x*N,  y*N+N);
    glVertex2i(x*N,  y*N);
    glVertex2i(x*N+N,y*N);
    }
  }
  glEnd();

}

void GLWidget::mySetViewport(int x,int y,int w,int h){
  glViewport(x,y,w,h);
  float wh = float(w)/h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (displaying==TEXTURE || displaying==MATERIAL) {
    if (w<h)
      gluOrtho2D(-1,+1,-1/wh,+1/wh);
    else
      gluOrtho2D(-wh,+wh,-1,+1);
  } else {
    gluPerspective(60-closingUp*40,wh,0.2,20+currViewmodeInterior*500);
  }

  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
  glViewport(0,0,w,h);

  glClearColor(bg_r,bg_g,bg_b,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (displaying==TEXTURE || displaying==MATERIAL) {
    if (showAlpha==TRANSALPHA) glClearCheckBoard();
    currViewmodeInterior=0;
  }

  if (displaying == NONE) return;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (displaying==TEXTURE || displaying==MATERIAL) {
    glScalef(zoom, zoom, zoom);
    float x=cx,y=cy;
    float lim = 1-1/zoom;
    if (x<-lim) x=-lim;
    if (x>+lim) x=lim;
    if (y<-lim) y=-lim;
    if (y>+lim) y=lim;
    glTranslatef( x,-y,0);
  } else {
    glTranslatef(0,0,-dist*(1-currViewmodeInterior));

    glRotatef(theta, 1,0,0);
    glRotatef(phi, 0,1,0);

    glScalef(-1,1,1);
    //glFrontFace(GL_CW);
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);


  if (data) {
    if (displaying == BODY ) renderSelected(data->body);
    if (displaying == MESH ) renderSelected(data->mesh);
    if (displaying == SKELETON ) renderSelected(data->skeleton);
    if (displaying == ANIMATION )  renderSelected(data->animation);
    if (displaying == TEXTURE )  renderSelected(data->texture);
    if (displaying == MATERIAL )  renderSelected(data->material);
  }

}

void GLWidget::renderFloorMaybe(){
  if ( displaying != SKELETON)
  if (((useFloor &&  (displaying == MESH || displaying == BODY) )
       || displaying == ANIMATION  ))
    renderFloor();
  if (useRuler && displaying == MESH)
    renderRuler();
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
    if (displaying==TEXTURE || displaying==MATERIAL) zoom/=1.2; else {
      if(viewmode==2) avatP[1]+=0.2;
      else dist*=1.1;

    }
  } else {
    if (displaying==TEXTURE || displaying==MATERIAL) zoom*=1.2; else {
      if(viewmode==2) avatP[1]-=0.2;
      else dist/=1.1;
    }
  }
  if (zoom<1.0) zoom = 1.0;
  if (zoom>32.0) zoom = 32.0;
  if (dist>18.0) dist=18.0;
  if (dist<0.5) dist=0.5;

  update();
}

QString GLWidget::locateOnDisk(QString nome, const char *ext, BrfMaterial::Location *loc){
  BrfMaterial::Location aloc=BrfMaterial::UNKNOWN;
  if (!loc) loc = &aloc;
  QString tname = QString(nome);
  if (ext[0]) if (!tname.endsWith(".dds",Qt::CaseInsensitive)) tname+=ext;
  if (*loc == BrfMaterial::UNKNOWN) {
     if (QDir(this->texturePath[2]).exists(tname)) *loc = BrfMaterial::LOCAL;
     else if (QDir(this->texturePath[1]).exists(tname)) *loc = BrfMaterial::MODULE;
     else if (QDir(this->texturePath[0]).exists(tname)) *loc = BrfMaterial::COMMON;
     else *loc = BrfMaterial::NOWHERE;
  }

  if (*loc == BrfMaterial::COMMON) return texturePath[0]+"/"+tname;
  if (*loc == BrfMaterial::MODULE) return texturePath[1]+"/"+tname;
  if (*loc == BrfMaterial::LOCAL ) return texturePath[2]+"/"+tname;
  return QString();
}

void GLWidget::showMaterialDiffuseA(){curMaterialTexture = DIFFUSEA; update();}
void GLWidget::showMaterialDiffuseB(){curMaterialTexture = DIFFUSEB;update();}
void GLWidget::showMaterialBump(){curMaterialTexture = BUMP;update();}
void GLWidget::showMaterialEnviro(){curMaterialTexture = ENVIRO;update();}
void GLWidget::showMaterialSpecular(){curMaterialTexture = SPECULAR;update();}

void GLWidget::showAlphaTransparent(){ showAlpha = TRANSALPHA; update();}
void GLWidget::showAlphaPurple(){ showAlpha = PURPLEALPHA; update();}
void GLWidget::showAlphaNo(){showAlpha = NOALPHA; update();}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (displaying==TEXTURE || displaying==MATERIAL) {
      cx += 1/zoom*dx*0.01;
      cy += 1/zoom*dy*0.01;
      if (cx<-1.0) cx=-1.0;
      if (cx>1.0) cx=1.0;
      if (cy<-1.0) cy=-1.0;
      if (cy>1.0) cy=1.0;
    } else {
      float sensib = 0.75;
      if(viewmode==2) sensib*=0.15;
      phi += dx*2.0*sensib;
      theta += dy*1.0*sensib;
      if (theta>90) theta=90;
      if (theta<-90) theta=-90;
    }
    lastPos = event->pos();
    this->update();
}

