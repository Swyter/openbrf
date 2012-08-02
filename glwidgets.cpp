/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include <GL/glew.h>
#include <QtGui>
#include <QtOpenGL>
#include <QGLShaderProgram>

#include <math.h>

#include "brfData.h"
#include "glwidgets.h"

#include "ddsData.h"

#include "bindTexturePatch.h"

#include <vcg/math/matrix44.h>
#include <wrap/gl/space.h>


void GLWidget::setFrameNumber(int i){
  if (i<0) return;
  if (selFrameN==i) return;
  selFrameN = i;
  if (!skeletalAnimation()) runningState=PAUSE;
  update();
}

void GLWidget::setDefaultBgColor(QColor col, bool alsoCurrent){
  defaultBgColor = col;
  if (alsoCurrent){
    currBgColor = col;
    update();
  }
}


void GLWidget::setRefAnimation(int i){

  selRefAnimation = i-1; // -1 for the "none"
  update();
}

void GLWidget::setEditingRef(bool mode){
  if (mode) {
    currBgColor.setRedF(0.45);
    currBgColor.setGreenF(0.5);
    currBgColor.setBlueF(0.35);
  } else {
    currBgColor = defaultBgColor;
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
  if (displaying == MESH ) {
    if (selRefAnimation>=0) {
      BrfAnimation *a=&(reference->animation[selRefAnimation]);
      return reference->getOneSkeleton( int(a->nbones ), selRefSkel );
    }

  }
  if (displaying == ANIMATION ) {
    if (data)
    if ((selected>=0) && (selected<(int)data->animation.size())) {
      BrfAnimation &a( data->animation[selected]);
      return reference->getOneSkeleton( int(a.nbones ), selRefSkel );
    }

  }
  return selRefSkel;
}

int GLWidget::getRefSkelAni() const{
  if (displaying == MESH ) {
    return selRefAnimation;
  }
  return -1;
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

    //glDisable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
#if (0)
    const int H = 550; // floor size
    const float h = -0.1; // floor size
    {
    glBegin(GL_QUADS);
      glColor4f(bg_r,bg_g,bg_b,0.7f);
      glVertex3f(+H, h, -H);
      glVertex3f(+H, h,  H);
      glVertex3f(-H, h,  H);
      glVertex3f(-H, h, -H);
    glEnd();    
    }
#endif

  }
  //glEnable(GL_CULL_FACE);

  //glDisable(GL_FOG);

  const int K = 50; // floor size
  glEnable(GL_BLEND);
  glBlendFunc(GL_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_LIGHTING);
  float bg_r = currBgColor.redF();
  float bg_g = currBgColor.greenF();
  float bg_b = currBgColor.blueF();
  float bg[4]={bg_r,bg_g,bg_b,0.0f};
  float ccc = (bg_r>0.3)?-0.3:+0.5;
  float c[4]={ccc+bg_r,ccc+bg_g,ccc+bg_b,0.5f};
  glColor4fv(c);
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
  glDisable(GL_BLEND);
}

int GLWidget::getFrameNumber() const{
  return selFrameN;
}

void GLWidget::browseTexture(){
  if (!renderedTexture.isEmpty()) {
    //QString f = QUrl::fromLocalFile(renderedTexture).toString();
    //f.truncate(f.lastIndexOf('/')+1);
    //QMessageBox::warning(this,"test", f);
    //QDesktopServices::setUrlHandler("*.*",this,"Explorer");
    //QDesktopServices::openUrl(f);

    QStringList args;
    args << QString("/select,") << QDir::toNativeSeparators(renderedTexture);
    QProcess::startDetached("explorer", args);

  }
}

void GLWidget::renderTexture(const char* name, bool addExtension){
  glDisable(GL_LIGHTING);

  glPolygonMode(GL_FRONT,GL_FILL);

  //char tname[512];
  //sprintf(tname,"%s%s",name,(addExtension)?".dds":"");


  BrfMaterial::Location loc = BrfMaterial::UNKNOWN;
  renderedTexture = locateOnDisk( name, (addExtension)?".dds":"", &loc );
  if (!renderedTexture.isEmpty()) {
    setTextureName(renderedTexture, loc, 0);
  } else {
    setCheckboardTexture();
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
      a = &(reference->animation[selRefAnimation]);
      int si = getRefSkeleton();
      if (si>=0) s=&(reference->skeleton[si]);
      //selRefSkel = si;
    }
  }

  if (a && s) {
    lastSkelAniFrameUsed = fi = floatMod( relTime*runningSpeed , a->frame.size() );
    renderRiggedMesh(p,*s,*a,fi);
  } else {
    renderMesh(p,fi);
  }
}

#define GL_LIGHT_MODEL_COLOR_CONTROL  0x81F8
#define GL_SEPARATE_SPECULAR_COLOR  0x81FA

bool usingProgram = false;

void GLWidget::enableDefMaterial(){
  float tmps[4]={0,0,0,0};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,tmps);
  glDisable(GL_ALPHA_TEST);
  if (useOpenGL2) {
    if (usingProgram && glUseProgram) glUseProgram(0);
    if (inferMaterial){
      glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE2); glDisable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE0);
    }
  }
  usingProgram = false;

}

#define STRINGIFY(X) #X


QGLShaderProgram* GLWidget::initFramPrograms(int mode, bool green){

	lastUsedShader = mode;
	lastUsedShaderBumpgreen = green;

	if (shaderTried[mode][green]) return NULL;

	QGLShaderProgram * s(shaderProgram[mode][green]);
	QString& log(shaderLog[mode][green]);

	if (s->isLinked()) {
		if (s->bind()) {
			usingProgram = true;
			return s;
		} else return NULL;
	} else {

		QString prefix;
		QString fileVertexSource =  ":/shaders/bump_vertex.cpp";
		QString fileFragmentSource =  ":/shaders/bump_fragment.cpp";

		prefix = (green)?"#define USE_GREEN_NM\n":"";

		switch (mode) {
		case NM_PLAIN:
			break;
		case NM_ALPHA:
			prefix += "#define ALPHA_CUTOUTS\n";
			break;
		case NM_IRON:
			prefix += "#define SPECULAR_ALPHA\n";
			break;
		case NM_SHINE:
			prefix += "#define ALPHA_CUTOUTS\n#define SPECULAR_MAP\n";
			break;
		case SHADER_IRON:
			prefix = "";
			fileFragmentSource = ":/shaders/iron_fragment.cpp";
			fileVertexSource = "";
			break;
    }

		bool res = true;
		if (!fileVertexSource.isEmpty()) {
			QFile file(fileVertexSource);
			file.open(QIODevice::Text|QIODevice::ReadOnly);
			QString src = QString(file.readAll()).arg(prefix);
			res &= s->addShaderFromSourceCode(QGLShader::Vertex,src);
			if (!s->log().isEmpty())
				log += tr("<br />Vertex compilation: <br />")+s->log().replace("\n","<br />");
		}
		if (res) if (!fileFragmentSource.isEmpty()) {
			QFile file(fileFragmentSource);
			file.open(QIODevice::Text|QIODevice::ReadOnly);
			QString src = QString(file.readAll()).arg(prefix);
			res &= s->addShaderFromSourceCode(QGLShader::Fragment, src);
			if (!s->log().isEmpty())
				log += tr("<br />Fragment compilation: <br />")+s->log().replace("\n","<br />");
		}
		if (res) {
			res &= s->link();
			if (!s->log().isEmpty())
				log += tr("<br />Linking: <br />")+s->log().replace("\n","<br />");
		}
		if (res) {
			res &= s->bind();

			if (!s->log().isEmpty())
				log += tr("<br />Binding: <br />")+s->log().replace("\n","<br />");
			usingProgram = true;
		}

		if (!res) {
			shaderTried[mode][green] = true;
			return NULL;
		} else return s;
	}
}


void GLWidget::enableMaterial(const BrfMaterial &m){

  // try to guess what the shader will do using flags
  bool alphaCutout = false;
  //if (m.flags & (1<<4)) alphaShine = true;
  if (m.flags & ((7<<12) | (1<<6) ) )  alphaCutout = true;

  bool alphaShine = QString(m.shader).contains("iron",Qt::CaseInsensitive);

  bool mapShine = m.HasSpec();
  // if using spec map, no shine from alpha
  if (mapShine) alphaShine = false;

  // no texture, no alpha
  if (!useTexture) alphaShine = alphaCutout = false;

  //if (!alphaShine && !alphaCutout) alphaShine = true;
  if (alphaShine && alphaCutout) alphaShine = false;

  if (alphaCutout) {
    //glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.5);
    if (m.flags & (1<<12)) glAlphaFunc(GL_GREATER,1/127.0);
  }

  //if (m.diffuseA[0]!=0) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);



  //glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,(int)m.specular);
  //float tmps[4]={m.r,m.g,m.b,1};
  //glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,tmps);

  if (QString(m.spec)!="none") alphaShine = false;


  if (bumpmapActivated) {

    if (useOpenGL2) {

      int w;
			if (mapShine) w=NM_SHINE ; //prog = programNormalMap[w=NM_SHINE][bumpmapUsingGreen];
			else if (alphaCutout) w=NM_ALPHA; // prog = programNormalMap[w=NM_ALPHA][bumpmapUsingGreen];
			else if (alphaShine) w=NM_IRON; //prog = programNormalMap[w=NM_IRON][bumpmapUsingGreen];
			else w=NM_PLAIN; //prog = programNormalMap[w=NM_PLAIN][bumpmapUsingGreen];


			QGLShaderProgram* p = initFramPrograms(w, bumpmapUsingGreen);

			if (p)  {
				p->setUniformValue("usePerVertColor", (colorMode==0)?GLfloat(-1):GLfloat(1));
				p->setUniformValue("spec_col",m.r,m.g,m.b);
				p->setUniformValue("spec_exp",m.specular);
				p->setUniformValue("samplRgb",0);
				p->setUniformValue("samplBump",1);
				p->setUniformValue("samplSpec",2);

			}
    }
  } else {

    if (useOpenGL2) {

      // disable bump texture
      glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE2); glDisable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE0);

      if (alphaShine){
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
        float ones[4]={1,1,1,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ones);
        glLightfv(GL_LIGHT0,GL_SPECULAR, ones);
        glMateriali(GL_FRONT_AND_BACK,GL_SHININESS, (int)m.specular);

				QGLShaderProgram* p = initFramPrograms(SHADER_IRON, false);

				if (p) {
					p->setUniformValue("spec_col",m.r,m.g,m.b);
					p->setUniformValue("samplRgb",0);
				}
      }
		}


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


  int si = reference->getOneSkeleton( int(a.nbones ), selRefSkel );
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

    float bg_r = currBgColor.redF();
    float bg_g = currBgColor.greenF();
    float bg_b = currBgColor.blueF();
    float ccc = (bg_r>0.3)?-0.25:+0.25;

    float c[4]={ccc+bg_r,ccc+bg_g,ccc+bg_b,1};
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


void GLWidget::initDefaultTextures(){
  // small checkboard
  const int N = 16;
  QImage im(QSize(16,16),QImage::Format_ARGB32);
  for (int x=0; x<N; x++)
    for (int y=0; y<N; y++)
      if ((x+y)%2) im.setPixel(QPoint(x,y),0xFFFFFFFF);
      else im.setPixel(QPoint(x,y),0xFFAAAAFF);
  checkboardTexture = bindTexture(im);
  //glGetIntegerv(GL_TEXTURE_BINDING_2D, &checkboardTexture);

  tw=th=16;
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);


  {
  QImage im(1,1,QImage::Format_ARGB32);
  im.setPixel(0,0,0xFFEEEEEE);
  dummyRgbTexture = bindTexture(im);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  }

  {
  QImage im(1,1,QImage::Format_ARGB32);
  im.setPixel(0,0,0xFF000000);
  dummySpecTexture = bindTexture(im);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  }
  {
  QImage im(1,1,QImage::Format_ARGB32);
  im.setPixel(0,0,0xFF8080FF); // normal 0,1,0
  dummyNormTexture = bindTexture(im);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  }

}

void GLWidget::setCheckboardTexture(){
  glBindTexture( GL_TEXTURE_2D, checkboardTexture);
}

void GLWidget::setDummyRgbTexture(){
  glBindTexture(GL_TEXTURE_2D, dummyRgbTexture );
}
void GLWidget::setDummySpecTexture(){
  if (!glActiveTexture) return;
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, dummySpecTexture );
  glActiveTexture(GL_TEXTURE0);
}
void GLWidget::setDummyNormTexture(){
  if (!glActiveTexture) return;
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, dummyNormTexture );
  glActiveTexture(GL_TEXTURE0);
  bumpmapActivated = true; bumpmapUsingGreen = false;
}

bool GLWidget::fixTextureFormat(QString st){

  FILE* f = _wfopen(st.toStdWString().c_str(),L"rb");
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

void GLWidget::setTextureName(QString s, int origin, int texUnit){
  if (texUnit!=0) {
    if (!glActiveTexture) return;
    glActiveTexture(GL_TEXTURE0 + texUnit);
  }
  glEnable(GL_TEXTURE_2D);
  DdsData data;
  data.location = origin;
  if (myBindTexture( s, data )){
     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
     setMaterialError(0);
     tw=data.sx;
     th=data.sy;
     ta=(data.ddxversion>=3);
     //qDebug("Settexture: ta=%d, name=%s",data.ddxversion,s.toAscii().data());
  } else {
     setMaterialError(3); // format is wrong
     lastMatErr.texName=s;
     setCheckboardTexture();
  }
  emit(setTextureData(data));
  glEnable(GL_TEXTURE_2D);
  if (texUnit!=0) glActiveTexture(GL_TEXTURE0);
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
    bumpmapActivated = false;
    lastMatErr.texName=QString("%1.dds").arg(st);
    QString s;
    if (useTexture) {
      s = locateOnDisk(QString(m->diffuseA),".dds",&(m->rgbLocation));
      if (!s.isEmpty()) setTextureName(s, m->rgbLocation,0 );
      else {
        setMaterialError(2); // file not found
        setCheckboardTexture();
      }
    } else {
      setDummyRgbTexture();
    }

		lastUsedShader = SHADER_FIXEDFUNC;
    if (useOpenGL2 && inferMaterial && glMultiTexCoord3fv) {
      bool useN = useNormalmap && m->HasBump();
      bool useS = useSpecularmap && m->HasSpec();
      if (useN || useS) {
        if (useN) {
          QString b = locateOnDisk(QString(m->bump),".dds",&(m->bumpLocation));
          if (!b.isEmpty()) {
            setTextureName(b, m->bumpLocation ,1 );
            bumpmapActivated = true;
            bumpmapUsingGreen = ta;
          }
        } else setDummyNormTexture();
        if (useS) {
          QString b = locateOnDisk(QString(m->spec),".dds",&(m->specLocation));
          if (!b.isEmpty()) {
            setTextureName(b, m->specLocation ,2 );
            bumpmapActivated = true;
          }
        } else setDummySpecTexture();
      }
      enableMaterial( *m );
    }

  }
  else {
    setMaterialError(1); // material not found
    setCheckboardTexture();
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
void GLWidget::setNormalmap(int i){
  useNormalmap = i; update();
}
void GLWidget::setSpecularmap(int i){
  useSpecularmap = i; update();
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

QString GLWidget::getCurrentShaderDescriptor() const{
	QString st =  (lastUsedShaderBumpgreen)?tr("\"green\" NM"):tr("\"blue\" NM");
	switch (lastUsedShader) {
	case SHADER_FIXEDFUNC: return tr("Deafult (fixed functionality)");
	case SHADER_IRON: return tr("Alpha to Shininess");
	case NM_PLAIN: return tr("Plain NormalMap (%1)").arg(st);
	case NM_ALPHA: return tr("NormalMap + Alpha to Transparency (%1)").arg(st);
	case NM_IRON: return tr("NormalMap + Alpha to Shininiess (%1)").arg(st);
	case NM_SHINE: return tr("NormalMap + ShininessMap (%1)").arg(st);
	default: return "ERROR";
	}
}
QString GLWidget::getCurrentShaderLog() const{

	switch (lastUsedShader) {
	case SHADER_FIXEDFUNC: return "";
	default: return shaderLog[lastUsedShader][lastUsedShaderBumpgreen] ;
		//(shaderProgram[lastUsedShader][lastUsedShaderBumpgreen])->log();
	}

}

GLWidget::GLWidget(QWidget *parent, IniData &_inidata)
    : QGLWidget(parent), inidata(_inidata)
{
  //grabKeyboard ();
  selectNone();
  phi=theta=0;
  dist = 3.9;
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

	lastUsedShader = SHADER_FIXEDFUNC;
	lastUsedShaderBumpgreen = 0;


  keys[0]=keys[1]=keys[2]=keys[3]=keys[4]=false;

	for (int j=0; j<2; j++)
	for (int i=0; i<SHADER_MODES; i++) {
		shaderTried[i][j] = false;
		shaderProgram[i][j] = new QGLShaderProgram();
	}


  useNormalmap = true;
  useSpecularmap = true;

  useTexture=true; useWireframe=false; useLighting=useFloor=true; useRuler=false;
  openGL2ready=false;

  bumpmapActivated = false;
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
  inferMaterial = true;
  useOpenGL2 = false;

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
    emit(displayInfo(tr("Scene mode: navigate with mouse and WASD (levitate with wheel, zoom in with shift)"), 10000));
  } else {
    //setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::WheelFocus);
    if (i==1)
    emit(displayInfo(tr("Helmet mode: for objects with vertical Z axis, like M&B helmets or weapons."), 8000));
    else
    emit(displayInfo(tr("Default mode: rotate objects with mouse, zoom in/out with wheel."), 8000));
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
	if (elapsed<0) {
		lasttime = -1;
		return;
	}
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



QSize GLWidget::minimumSizeHint() const
{
    return QSize(150, 350);
}

QSize GLWidget::sizeHint() const
{
    return QSize(800, 800);
}


void GLWidget::initOpenGL2(){
  if (openGL2ready) return;
  glewInit();
	//initFramPrograms();
	qDebug("Init glew!");
  openGL2ready = true;
}

void GLWidget::initializeGL()
{

  glAttachShader = 0; // just for safety

  openGL2ready = false;
  initDefaultTextures();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void GLWidget::setUseOpenGL2(bool mode){
  if (mode == useOpenGL2) return;
  useOpenGL2 = mode;
  update();
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
  if (useOpenGL2) initOpenGL2();
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
  if (pass==1) setMaterialName(m.material);

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
      const Point3f &norm(m.frame[fv].norm[        m.face[i].index[j]        ]);
      const Point3f &tang(m.vert[                  m.face[i].index[j]        ].tang);
      const int     &tiv (m.vert[                  m.face[i].index[j]        ].ti);
      const Point3f &pos (m.frame[fv].pos [ m.vert[m.face[i].index[j]].index ]);
      Point3f v(0,0,0);
      Point3f n(0,0,0);
      Point3f t(0,0,0);
      int ti = 0;
      for (int k=0; k<4; k++){
        float wieght = rig.boneWeight[k];
        int       bi = rig.boneIndex [k];
        if (bi>=0 && bi<(int)bonepos.size()) {
          v += (bonepos[bi]* pos  )*wieght;
          n += (bonepos[bi]* norm - bonepos[bi]*Point3f(0,0,0) )*wieght;
          if (bumpmapActivated){
            t += (bonepos[bi]* tang - bonepos[bi]*Point3f(0,0,0) )*wieght;
            ti = tiv;
          }
        }
      }
      glNormal(n);
      glTexCoord(m.vert[m.face[i].index[j]].ta);
      if (bumpmapActivated) {
        glMultiTexCoord3fv(GL_TEXTURE1,t.V());
        glMultiTexCoord1i (GL_TEXTURE2,ti );

      }
      glVertex(v);
    }
  }
  glEnd();
  }
  glDisable(GL_TEXTURE_2D);
  enableDefMaterial();
}

void GLWidget::renderMeshSimple(const BrfMesh &m){

  glDisable(GL_POLYGON_OFFSET_LINE);
  glPolygonMode(GL_FRONT,GL_FILL);
  glDisable(GL_LIGHTING);
  glBegin(GL_TRIANGLES);
  for (unsigned int i=0; i<m.face.size(); i++) {
    for (int j=0; j<3; j++) {
      glVertex(m.frame[0].pos [ m.vert[m.face[i].index[j]].index ]);
    }
  }
  glEnd();
  glEnable(GL_LIGHTING);

}

void GLWidget::renderMesh(const BrfMesh &m, float frame){

  if (useOpenGL2) initOpenGL2();

  int framei = (int) frame;

  glEnable(GL_COLOR_MATERIAL);
  glColor3f(1,1,1);
  if ((!m.isRigged && colorMode==2)|| colorMode==0) glDisable(GL_COLOR_MATERIAL);


  for (int pass=(useWireframe)?0:1; pass<2; pass++) {
  setWireframeLightingMode(pass==0, useLighting, useTexture);
  if (pass==1) setMaterialName(m.material);
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

      if (bumpmapActivated) {
        glMultiTexCoord3fv(GL_TEXTURE1,  m.vert[m.face[i].index[j]].tang.V());
        glMultiTexCoord1i (GL_TEXTURE2,  m.vert[m.face[i].index[j]].ti );
      }
      glVertex(m.frame[framei].pos [ m.vert[m.face[i].index[j]].index ]);

    }
  }
  glEnd();
  }
  glDisable(GL_TEXTURE_2D);
  if (inferMaterial && glActiveTexture){
    glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D); glActiveTexture(GL_TEXTURE0);
  }
  enableDefMaterial();
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
  glLineWidth(1);
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
    glPushAttrib(GL_ENABLE_BIT|GL_LIGHTING_BIT);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glBlendFunc(GL_ONE,GL_ONE);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glDisable(GL_FOG);

    for (int pass=0; pass<2; pass++){
      if (pass==0){ glColor3f(0.1f,0,0); glCullFace(GL_FRONT); }
      else        { glColor3f(0,0,0.1f); glCullFace(GL_BACK); }
      for (unsigned int i=0; i<b.face.size(); i++) {
        glBegin(GL_POLYGON);
        for (unsigned int j=0; j<b.face[i].size(); j++)
          glVertex(b.pos[b.face[i][j]]);
        glVertex(b.pos[b.face[i][0]]);
        glEnd();
      }
    }
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
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
void GLWidget::setInferMaterialOn(){
  inferMaterial = true;
  update();
}
void GLWidget::setInferMaterialOff(){
  inferMaterial = false;
  update();
}

void GLWidget::renderBody(const BrfBody& b){
  for (unsigned int i=0; i<b.part.size(); i++) renderBodyPart(b.part[i]);
}


void _subdivideScreen(int nsel,int w,int h, int *ncol, int *nrow){
  if (w<h) { _subdivideScreen( nsel,h,w, nrow, ncol); }
  else {
    int best = 0;
    for (int n=(int)(ceil(sqrt(double(nsel)))); n>=1; n--) {
      int m = (nsel+n-1)/n;
      int score;
      score = (w/m<h/n)? w/m : h/n;
      if (score>best) {
        *ncol = m;
        *nrow = n;
        best=score;
      }

    }
  }
}

float myrand(float min, float max){
  return min+(max-min)*(rand()%1001)/1000;
}

static Point3f randomUpVector(int, float above){
  Point3f res;
  float maxup = 1-above;
  do {
    res = Point3f( myrand(-1,1),myrand(-1,maxup),myrand(-1,1));
    if (res.SquaredNorm()>1) continue;
    res.Normalize();
    if (res.Y()>maxup) continue;
  } while (0);
  return res;
}

void GLWidget::renderAoOnMeshes(float brightness, float fromAbove, bool perface, bool inAlpha){
  if (!data) return;
  if (!viewmodeMult) {
    // ao, combined
    renderAoOnMeshesAllSelected(brightness, fromAbove, perface, inAlpha);
  } else {
    // ao, separated

    bool selGroupBackup[MAXSEL];
    for (int i=0;i<MAXSEL; i++) {
      selGroupBackup[i] = selGroup[i];
      selGroup[i] = false;
    }
    for (uint i=0; i<data->mesh.size(); i++) {
      assert(i<MAXSEL);
      if (!(selGroupBackup[i])) continue;
      selGroup[i] = true;
      renderAoOnMeshesAllSelected(brightness, fromAbove, perface, inAlpha);
      selGroup[i] = false;
    }
    for (int i=0;i<MAXSEL; i++) selGroup[i] = selGroupBackup[i];
  }
}

void GLWidget::renderAoOnMeshesAllSelected(float brightness, float howMuchFromAbove, bool perFace, bool inAlpha){
  makeCurrent();
  if (!data) return;

  std::vector<BrfMesh>& v(data->mesh);

  // set color to at all meshes black
  for (uint i=0; i<v.size(); i++) if (selGroup[i]) {
    for (uint j=0; j<v[i].vert.size(); j++) v[i].vert[j].__norm = vcg::Point3f(0,0,0);
    //v[i].ColorAll(0);
  }

  const int RES = 255;
  const int NPASS = 256;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix(); glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  Box3f bbox;bbox.SetNull();
  for (uint i=0; i<v.size(); i++) if (selGroup[i]) {
    bbox.Add( v[i].bbox );
  }

  float maxLight = 0;
  for (int n=0; n<NPASS; n++)  {

    glClear(GL_DEPTH_BUFFER_BIT);

    // set a view direction for shadows
    // ld = light dir
    Point3f ld = randomUpVector(n, howMuchFromAbove),
                 dx(0,0,1), dy;
    maxLight += std::max(-ld.X(),0.0f);
    ld.Normalize();
    dy = (ld^dx).normalized();
    dx = (dy^ld).normalized();
    //double mat[16]={ dx[0],dx[1],dx[2],0,  dy[0],dy[1],dy[2],0,  ld[0],ld[1],ld[2],0, 0,0,0,1};
    double mat[16]={ dx[0],dy[0],ld[0],0,  dx[1],dy[1],ld[1],0,  dx[2],dy[2],ld[2],0, 0,0,0,1};
    glLoadIdentity();
    glMultMatrixd(mat);
    glScale(2.0/bbox.Diag());
    glTranslate(-bbox.Center());
    double matMV[16],matPR[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    int VP[4]={0,0,RES,RES};
    glGetDoublev(GL_MODELVIEW_MATRIX,matMV);
    glViewport(0,0,RES,RES);
    //glWriteMask(GL_FALSE);

    for (uint i=0; i<v.size(); i++) if (selGroup[i]) renderMeshSimple(v[i]);
    float depthbuf[RES*RES];
    glReadPixels(0, 0, RES,RES,GL_DEPTH_COMPONENT, GL_FLOAT, depthbuf);

    if (perFace) {
      for (uint i=0; i<v.size(); i++) if (selGroup[i]) {
        BrfMesh &m(v[i]);
        for (uint fj=0; fj<m.face.size(); fj++)
        for (uint wj=0; wj<3; wj++) {
          int j0 = m.face[fj].index[wj];
          int j1 = m.face[fj].index[(wj+1)%3];
          int j2 = m.face[fj].index[(wj+2)%3];
          Point3f p =
            m.frame[0].pos[ m.vert[j0].index ] * (4.0/6.0)+
            m.frame[0].pos[ m.vert[j1].index ] * (1.0/6.0)+
            m.frame[0].pos[ m.vert[j2].index ] * (1.0/6.0);
          Point3f n =
            m.frame[0].norm[ j0 ] * (4.0/6.0)+
            m.frame[0].norm[ j1 ] * (1.0/6.0)+
            m.frame[0].norm[ j2 ] * (1.0/6.0);
          n = n.Normalize();

          double rx,ry,rz;
          gluProject(p.X(),p.Y(),p.Z(),matMV, matPR, VP, &rx,&ry,&rz);
          float depth = depthbuf[RES*int(ry)+int(rx)];

          if (depth+0.005>rz) { \
            float diff = -ld*n;
            if (diff<0) diff = 0;
            m.vert[j0].__norm[0] += diff;
          }
          m.vert[j0].__norm[1] += 1;
        }
      }
    } else {
      for (uint i=0; i<v.size(); i++) if (selGroup[i]) {
        BrfMesh &m(v[i]);
        for (uint j=0; j<m.vert.size(); j++) {
          Point3f p = m.frame[0].pos[ m.vert[j].index ];
          Point3f n = m.frame[0].norm[ j ];
          double rx,ry,rz;
          gluProject(p.X(),p.Y(),p.Z(),matMV, matPR, VP, &rx,&ry,&rz);
          float depth = depthbuf[RES*int(ry)+int(rx)];

          if (depth+0.005>rz) {
            float diff = -ld*n;
            if (diff<0) diff = 0;
            m.vert[j].__norm[0] += diff;

          }
        }
      }
    }

  }

  // find final color
  for (uint i=0; i<v.size(); i++) if (selGroup[i]) {
    BrfMesh &m(v[i]);
    for (uint j=0; j<m.vert.size(); j++) {
      
      float ao =  m.vert[j].__norm[0] /(0.95*maxLight) ;
      if (perFace) ao *= NPASS/(m.vert[j].__norm[1]) ;
      ao = brightness + ao*(1-brightness);
      int k = (uint)floor(ao*255+0.5);
      if (k>255) k=255;
      if (k<0) k=0;
      if (inAlpha)
        m.vert[j].col = ( m.vert[j].col & 0x00FFFFFF) | k<<24;
      else
        m.vert[j].col = k | k<<8 | k<<16 | (m.vert[j].col & 0xFF000000);
    }
    m.AdjustNormDuplicates();
    m.hasVertexColor = true;
  }


  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  //swapBuffers();
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


  bool _viewmodeMult = viewmodeMult;
  if (displaying==TEXTURE || displaying==MATERIAL) _viewmodeMult=true;

  int ncol=1, nrow=1;
  _subdivideScreen(nsel,w,h, &ncol, &nrow);
  for (int i=0,seli=0; i<max; i++) if (selGroup[i]) {
    glPushMatrix();
    glMultMatrixf(extraMatrix);
    {

      if (_viewmodeMult) {
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
    glPopMatrix();
    if (v[i].IsAnimable()) animating=true;

    if (_viewmodeMult) renderFloorMaybe();
  }
  if (!_viewmodeMult) renderFloorMaybe();



}

int GLWidget::selIndex() const{
  for (int i=0; i<MAXSEL; i++) if (selGroup[i]) return i;
  return -1;
}

void GLWidget::glClearCheckBoard(){
  float K = (currBgColor.redF()<0.9)?+0.15:-0.075;
  glColor3f(currBgColor.redF()+K,currBgColor.greenF()+K,currBgColor.blueF()+K);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  glViewport(0,0,w,h);
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
		gluPerspective(60-closingUp*40,wh,0.02,20+currViewmodeInterior*500);
    if (wh<1) glScalef(wh,wh,1);
    glScalef(0.2,0.2,0.2);
  }

  glMatrixMode(GL_MODELVIEW);
}

void GLWidget::paintGL()
{
  if (!isValid ()) return;
  glViewport(0,0,w,h);

  glClearColor(currBgColor.redF(),currBgColor.greenF(),currBgColor.blueF(),1);
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



QString GLWidget::locateOnDisk(QString nome, const char *ext, BrfMaterial::Location *loc){
  BrfMaterial::Location aloc=BrfMaterial::UNKNOWN;
  if (!loc) loc = &aloc;
  QString tname = QString(nome);
  if (ext[0]) if (!tname.endsWith(".dds",Qt::CaseInsensitive)) tname+=ext;
  if (*loc == BrfMaterial::UNKNOWN) {
    if (QDir(this->texturePath[1]).exists(tname)) *loc = BrfMaterial::MODULE;
     else if (QDir(this->texturePath[0]).exists(tname)) *loc = BrfMaterial::COMMON;
     else if (QDir(this->texturePath[2]).exists(tname)) *loc = BrfMaterial::LOCAL;
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

		if (event->modifiers()&Qt::ShiftModifier) {
			if (displaying==TEXTURE || displaying==MATERIAL)
				zoom*=(1.0+dy*0.02);
			else dist*=(1.0+dy*0.01);

			if (zoom<1.0) zoom = 1.0;
			if (zoom>32.0) zoom = 32.0;
			if (dist>18.0) dist=18.0;
			if (dist<0.5) dist=0.5;
		} else {
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
		}
    lastPos = event->pos();
    this->update();
}


void GLWidget::keyPressEvent( QKeyEvent * e ){
	if (e->key()==Qt::Key_W || e->key()==Qt::Key_Forward) keys[0]=true;
	if (e->key()==Qt::Key_S || e->key()==Qt::Key_Back) keys[1]=true;
	if (e->key()==Qt::Key_A || e->key()==Qt::Key_Left) keys[2]=true;
	if (e->key()==Qt::Key_D || e->key()==Qt::Key_Right) keys[3]=true;
	if (e->key()==Qt::Key_Shift) keys[4]=true;

	if(viewmode==2) {
		if (e->key()==Qt::Key_R) { avatP[1]+=0.2; update(); }
		if (e->key()==Qt::Key_F) { avatP[1]-=0.2; update(); }
	}

	QWidget::keyPressEvent(e);
}

void GLWidget::keyReleaseEvent( QKeyEvent * e ){
	if (e->key()==Qt::Key_W || e->key()==Qt::Key_Forward) keys[0]=false;
	if (e->key()==Qt::Key_S || e->key()==Qt::Key_Back) keys[1]=false;
	if (e->key()==Qt::Key_A || e->key()==Qt::Key_Left) keys[2]=false;
	if (e->key()==Qt::Key_D || e->key()==Qt::Key_Right) keys[3]=false;
	if (e->key()==Qt::Key_Shift) keys[4]=false;
	else QWidget::keyReleaseEvent(e);
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
