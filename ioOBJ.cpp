#include <vector>
#include <map>
#include <stdio.h>

#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

using namespace std;
using namespace vcg;

#include "BrfMesh.h"
#include "BrfSkeleton.h"

#include <QString>
#include <QFile>

// defined in vcgmesh.cpp
void setGotNormals(bool b);
void setGotMaterialName(bool b);
void setGotTexture(bool b);
void setGotColor(bool b);

bool BrfMesh::SaveOBJ(char* fn, int nframe) const{
  QFile f(fn);
  QString s;

  if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return false;
  int fr=nframe;
  f.write("#\n# Exported by OpenBRF -- marco tarini\n#\n");

  s = QString("s %1\n").arg(name);
  f.write(s.toAscii());
  s = QString("newmtl %1\nusemtl %1\n").arg(material);
  f.write(s.toAscii());


  for (unsigned int i=0; i<frame[fr].pos.size(); i++) {
    s = QString("v %1 %2 %3\n")
      .arg(-frame[fr].pos[i].X())
      .arg(frame[fr].pos[i].Y())
      .arg(frame[fr].pos[i].Z());
    f.write(s.toAscii());
  }
  for (unsigned int i=0; i<vert.size(); i++) {
    s = QString("vn %1 %2 %3\n")
      .arg(-frame[fr].norm[i].X())
      .arg(frame[fr].norm[i].Y())
      .arg(frame[fr].norm[i].Z());
    f.write(s.toAscii().data());
    s = QString("vt %1 %2\n")
      .arg(vert[i].ta.X())
      .arg(1.0f-vert[i].ta.Y());
    f.write(s.toAscii());
  }


  for (unsigned int i=0; i<face.size(); i++) {
    f.write("f");
    for (int w=0; w<3; w++) {;
      s = QString(" %1/%2/%2")
        .arg(vert[face[i].index[w]].index +1)
        .arg(face[i].index[w] +1);
      f.write(s.toAscii());
    }
    f.write("\n");
  }
  f.close();
  return true;
}

class Triple{
public:
  int a,b,c;
  Triple(int a0,int b0, int c0){a=a0;b=b0;c=c0;}
  bool operator <(const Triple &t) const {
    if (a<t.a) return true;
    if (b<t.b) return true;
    return (c<t.c);
  }
};

bool BrfMesh::LoadOBJ(char* fn){
  name[0]=0;
  frame.resize(1);
  QFile f(fn);
  flags=0;
  isRigged=false;
  material[0]=0;
  if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) return false;
  std::vector<Point3f> norm;
  std::vector<Point2f> ta;
  std::map<Triple,int> map;

  setGotColor(false);
  setGotNormals(false);
  setGotMaterialName(false);
  setGotTexture(false);

  bool norms=false;

  char st[512];
  while (f.readLine(st,511)!=-1) {
    QString s = QString("%1").arg(st);
    s = s.trimmed();
    if (s.startsWith("usemtl ")) {
      char* cp = s.toAscii().data();
      sscanf( cp, "usemtl %s", material);
      setGotMaterialName(true);
    }
    else if (s.startsWith("v ")) {
      char* cp = s.toAscii().data();
      Point3f p;
      sscanf( cp,"v %f %f %f",&(p[0]),&(p[1]),&(p[2])); p[0]=-p[0];
      frame[0].pos.push_back(p);
    }
    else if (s.startsWith("vn ")) {
      char* cp = s.toAscii().data();
      Point3f p;
      sscanf( cp,"vn %f %f %f",&(p[0]),&(p[1]),&(p[2])); p[0]=-p[0];
      norm.push_back(p);
    }
    else if (s.startsWith("vt ")) {
      char* cp = s.toAscii().data();
      Point2f p;
      sscanf( cp,"vt %f %f",&(p[0]),&(p[1])); p[1]=1-p[1];
      ta.push_back(p);
    }
    else if (s.startsWith("s ")) {
      char* cp = s.toAscii().data();
      sscanf( cp, "s %s", name);
    }
    else if (s.startsWith("f ")) {
      char* cp = s.toAscii().data();
      char st[100][100];
      int res=sscanf(cp, "f %s %s %s %s %s %s",st[0],st[1],st[2],st[3],st[4],st[5]);
      int remap[6];
      for (int w=0; w<res; w++) {
        int n=strlen(st[w]);
        int rr[3]; rr[0]=rr[1]=rr[2]=0; int rri=0;
        for(int i=0; i<n; i++) {
          char c=st[w][i];
          if (c=='/') {if (rri<3) rri++;}
          else rr[rri]=rr[rri]*10+(c-'0');
        }
        Triple t ( rr[0]-1,rr[1]-1 , rr[2]-1);

        if (map.find(t)==map.end()) {
          // new a,b,c. Add new vertex
          BrfVert v;
          v.index=t.a;
          assert(t.b<(int)ta.size());
          if (t.b>=0) {
            v.ta=v.tb=ta[t.b];
            setGotTexture(true);
          }
          assert(t.c<(int)norm.size());
          if (t.c>=0) {
            v.__norm=norm[t.c];
            norms=true;
            setGotNormals(true);
          }
          v.col=0xFFFFFFFF;
          vert.push_back(v);
          remap[w] = map[t] = vert.size()-1;
        } else remap[w] = map[t];
      }
      for (int w=2; w<res; w++){
        BrfFace f;
        f.index[0]=remap[0];
        f.index[1]=remap[w-1];
        f.index[2]=remap[w];
        face.push_back(f);
      }

    }

  }
  frame[0].norm.resize( vert.size());
  for (unsigned int i=0; i<vert.size(); i++) frame[0].norm[i]=vert[i].__norm;
  f.close();

  if (!norms) {
    UnifyVert(false);
    ComputeNormals();
  }

  AfterLoad();
  return true;
}
