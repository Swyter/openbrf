#include <QGLWidget>

#include <vector>
#include <vcg/simplex/vertex/base.h>
#include <vcg/simplex/face/base.h>
#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/clean.h>

#include "vcgexport.h"
#include "vcgimport.h"
//#include <wrap/io_trimesh/import.h>
//#include <wrap/io_trimesh/export.h>

using namespace vcg;
using namespace std;

#include "vcgmesh.h"

class CEdge;    // dummy prototype never used
class CFace;

class CVertex : public VertexSimp2< CVertex, CEdge, CFace, vertex::BitFlags, vertex::Coord3f,
                                    vertex::Normal3f, vertex::Color4b, vertex::TexCoord2f >{};
class CFace   : public FaceSimp2<   CVertex, CEdge, CFace, face::VertexRef, //face::Normal3f ,
                                    face::BitFlags, face::WedgeTexCoord2f,
                                    //face::Normal3f, // just for obj importing
                                    face::WedgeTexCoord2f> {};
class CMesh   : public vcg::tri::TriMesh< vector<CVertex>, vector<CFace> > {
public:
  void setFace(int i, int vi,int vj,int vk){
    face[i].V(0) = &(vert[vi]);
    face[i].V(1) = &(vert[vj]);
    face[i].V(2) = &(vert[vk]);
  }
};

CMesh mesh;

#include "BrfMesh.h"
#include "BrfSkeleton.h"


VcgMesh::VcgMesh()
{}

static const int mask =
  vcg::tri::io::Mask::IOM_VERTCOLOR |
  //vcg::tri::io::Mask::IOM_WEDGCOLOR |
  vcg::tri::io::Mask::IOM_VERTTEXCOORD|
  //vcg::tri::io::Mask::IOM_WEDGTEXCOORD|
  vcg::tri::io::Mask::IOM_VERTCOORD |
  vcg::tri::io::Mask::IOM_VERTNORMAL |
  vcg::tri::io::Mask::IOM_FACEINDEX
  ;

static int lastMask;
static int lastErr;
static bool lastMeterial=false;

bool VcgMesh::gotColor(){
 return lastMask & vcg::tri::io::Mask::IOM_VERTCOLOR;
}
bool VcgMesh::gotTexture(){
 return lastMask & (vcg::tri::io::Mask::IOM_VERTTEXCOORD | vcg::tri::io::Mask::IOM_WEDGTEXCOORD);
}

bool VcgMesh::gotMaterialName(){
 return lastMeterial;
}
static bool mustUseWT(){
  return
    (lastMask & vcg::tri::io::Mask::IOM_WEDGTEXCOORD)
    &&
    !(lastMask & vcg::tri::io::Mask::IOM_VERTTEXCOORD);
}

static bool mustUseVT(){
  return
    (lastMask & vcg::tri::io::Mask::IOM_VERTTEXCOORD);
}


bool VcgMesh::gotNormals(){
 return lastMask & vcg::tri::io::Mask::IOM_VERTNORMAL;
}

const char *VcgMesh::lastErrString(){
  typedef vcg::tri::io::Importer<CMesh> Imp;
  return Imp::ErrorMsg(lastErr);
}

bool VcgMesh::load(char* filename){
  typedef vcg::tri::io::Importer<CMesh> Imp;
  lastMask=mask;
  vcg::tri::io::Importer<CMesh>::LoadMask(filename,lastMask);
  mesh.Clear();
  return !Imp::ErrorCritical(lastErr=Imp::Open(mesh,filename,lastMask));
}

bool VcgMesh::save(char* filename){
  return (vcg::tri::io::Exporter<CMesh>::Save(mesh,filename,mask)==0);
}

static Color4b Int2Col(unsigned int h){
  return Color4b((h>>0)&255, (h>>8)&255, (h>>16)&255, (h>>24)&255);
}

static unsigned int Col2Int(const Color4b & c){
  return (((unsigned int)(c[3]))<<24) + (((unsigned int)(c[1]))<<16) +
         (((unsigned int)(c[2]))<< 8) + ((unsigned int)(c[0]));
}

class CoordSyst{
public:
  Point3f base;
  Point3f x,y,z;
  Point3f P(Point3f p) {
    return x*p[0] + y*p[1] + z*p[2] + base;
  }
  Point3f V(Point3f p) {
    return x*p[0] + y*p[1] + z*p[2];
  }
  CoordSyst operator *(const CoordSyst &b){
    return CoordSyst( P(b.base), V(b.x), V(b.y), V(b.z));
  }
  CoordSyst Inverse() {
    CoordSyst tmp;
    tmp.x=Point3f(x[0],y[0],z[0]);
    tmp.y=Point3f(x[1],y[1],z[1]);
    tmp.z=Point3f(x[2],y[2],z[2]);
    tmp.base=Point3f(0,0,0);
    tmp.base = tmp.P(-base);
    return tmp;
  }
  CoordSyst(){}
  CoordSyst(Point3f o, Point3f _x, Point3f _y, Point3f _z){
    base = o; x = _x; y=_y; z=_z;
  }
  CoordSyst(Point3f a, Point3f b, Point3f c){
    base = a;
    b-=a;
    c-=a;
    x=b;
    y=b^c;
    z=x^y;
    x.Normalize();
    z.Normalize();
    y.Normalize();
  }
};


void makeCoordSyst(const BrfSkeleton &s, int bi, vector<CoordSyst> &cs, CoordSyst c0){
  c0= c0*CoordSyst(s.bone[bi].t, s.bone[bi].x,s.bone[bi].y,s.bone[bi].z);
  cs[bi]=c0;
  for (unsigned int k=0; k<s.bone[bi].next.size(); k++){
    makeCoordSyst(s,s.bone[bi].next[k],cs,c0);
  }
}

// save a skeleton as a mesh
void VcgMesh::add(const BrfSkeleton &s){
  vector<CoordSyst> cs(s.bone.size());
  CoordSyst c0;
  c0.base=Point3f(0,0,0);
  c0.x   =Point3f(1,0,0);
  c0.y   =Point3f(0,1,0);
  c0.z   =Point3f(0,0,1);
  makeCoordSyst(s, s.root,  cs, c0);

  mesh.Clear();
  vcg::tri::Allocator<CMesh>::AddFaces( mesh , s.bone.size()*8 );
  vcg::tri::Allocator<CMesh>::AddVertices( mesh , s.bone.size()*6 );
  for (unsigned int i=0; i<s.bone.size(); i++) {
    mesh.setFace(i*8+0, i*6+0, i*6+1, i*6+2);
    mesh.setFace(i*8+1, i*6+3, i*6+2, i*6+1);
    mesh.setFace(i*8+2, i*6+1, i*6+0, i*6+5);
    mesh.setFace(i*8+3, i*6+2, i*6+4, i*6+0);
    mesh.setFace(i*8+4, i*6+5, i*6+4, i*6+3);
    mesh.setFace(i*8+5, i*6+2, i*6+3, i*6+4);
    mesh.setFace(i*8+6, i*6+3, i*6+1, i*6+5);
    mesh.setFace(i*8+7, i*6+4, i*6+5, i*6+0);
    /*
    for (int h=0; h<8; h++)
    for (int w=0; w<8; w++) {
      int a=mesh.face[i*8+h].V(w)-&(mesh.vert[0]);
      mesh.face[i*8+h].WT(w).P() = Point2f(i,a); // encode node id in pos
    }*/
    float X=0.04,Y=0.06,Z=0.12;
    mesh.vert[i*6+0].P()=cs[ i ].P( Point3f(X,0,0) );
    mesh.vert[i*6+1].P()=cs[ i ].P( Point3f(0,Y,0) );
    mesh.vert[i*6+2].P()=cs[ i ].P( Point3f(0,0,Z) );
    mesh.vert[i*6+3].P()=cs[ i ].P(-Point3f(X,0,0) );
    mesh.vert[i*6+4].P()=cs[ i ].P(-Point3f(0,Y,0) );
    mesh.vert[i*6+5].P()=cs[ i ].P(-Point3f(0,0,Z) );

    mesh.vert[i*6+0].N()=cs[ i ].V( Point3f(1,0,0) );
    mesh.vert[i*6+1].N()=cs[ i ].V( Point3f(0,1,0) );
    mesh.vert[i*6+2].N()=cs[ i ].V( Point3f(0,0,1) );
    mesh.vert[i*6+3].N()=cs[ i ].V(-Point3f(1,0,0) );
    mesh.vert[i*6+4].N()=cs[ i ].V(-Point3f(0,1,0) );
    mesh.vert[i*6+5].N()=cs[ i ].V(-Point3f(0,0,1) );

    for (int v=0; v<6; v++)
      mesh.vert[i*6+v].T().P()=Point2f(i,v);

  }
}

void VcgMesh::add(const BrfMesh &b, int fi){
  mesh.Clear();
  if ((int)b.frame.size()>=fi) fi=b.frame.size()-1;

  mesh.textures.push_back(b.material);
  CMesh::FaceIterator f=vcg::tri::Allocator<CMesh>::AddFaces( mesh , b.face.size() );
#if 0
  // one vcg::vert per brf::pos
  CMesh::VertexIterator v=vcg::tri::Allocator<CMesh>::AddVertices( mesh , b.frame[0].pos.size() );
  int k=0;
  for (CMesh::FaceIterator f=mesh.face.begin(); f!=mesh.face.end(); f++,k++) {
    for (int h=0; h<3; h++) {
      int vi = b.face[k].index[2-h];
      f->V(h) = &(mesh.vert[0]) + b.vert[ vi ].index;
      f->WC(h) = Int2Col(b.vert[ vi ].col);
      f->WT(h).P() = b.vert[vi].ta;

      int pi = b.vert[ vi ].index;
      f->V(h)->N() = b.frame[0].norm[ pi ];
      f->V(h)->P() = b.frame[0].pos [ pi ];
    }
  }
#else
  // one vcg::vert per brf::vert
  CMesh::VertexIterator v=vcg::tri::Allocator<CMesh>::AddVertices( mesh , b.vert.size() );
  int k=0;
  for (CMesh::FaceIterator f=mesh.face.begin(); f!=mesh.face.end(); f++,k++) {
    for (int h=0; h<3; h++) {
      int vi = b.face[k].index[h];
      f->V(2-h) = &(mesh.vert[0]) + vi;

      //f->WT(2-h).P() = b.vert[vi].ta; // texture also x wedge, always

    }
  }
  k=0;
  for (CMesh::VertexIterator v=mesh.vert.begin();v!=mesh.vert.end(); v++,k++) {
      int pi = b.vert[ k ].index;
      v->N() = b.frame[fi].norm[ k ];
      v->P() = b.frame[fi].pos [ pi ];
      v->C() = Int2Col( b.vert[ k ].col );
      v->T().P() =  b.vert[ k ].ta;
  }
#endif

}

BrfMesh VcgMesh::toBrfMesh(){

  BrfMesh b;

  if (lastMeterial=(mesh.textures.size()>0))

    sprintf(b.material,"%s",mesh.textures[0].c_str());
  else
    sprintf(b.material,"undefined");

  b.face.resize( mesh.fn );
  b.vert.resize( mesh.vn ); // one vert x wedge!


  b.frame.resize(1);
  b.frame[0].pos.resize( mesh.vn );
  b.frame[0].norm.resize( mesh.vn );
//        b.frame[0].pos.push_back( f->V(h)->P() );
//      b.frame[0].norm.push_back( f->V(h)->N() );

  int k;
  // copy faces

  k=0;
  for (CMesh::FaceIterator f=mesh.face.begin(); f!=mesh.face.end(); f++,k++) if (!f->IsD()) {
    for (int h=0; h<3; h++) {

      int vi  = b.face[k].index[2-h] = f->V(h)-&(mesh.vert[0]);
      if (mustUseWT()) b.vert[ vi ] .ta = b.vert[ vi ] .tb = f->WT(h).P(); // text x wedge
    }
  }

  k=0;
  for (CMesh::VertexIterator v=mesh.vert.begin();v!=mesh.vert.end(); v++,k++) {
          //int vi = f->V(h)-&mesh.vert[0];
    b.vert[k].col = //0xFFFFFFFF;
    (gotColor())? Col2Int( v->C() )
               :0xFFFFFFFF;
    b.vert[k].index = k;
    b.frame[0].norm[k] = v->N();
    b.frame[0].pos[k] = v->P();
    if (mustUseVT())
      b.vert[k].ta = b.vert[k].tb  = v->T().P(); // text x vert?
  }

  b.AdjustNormDuplicates();
  b.isRigged = false;
  b.flags=0;
  if (!gotNormals()); b.ComputeNormals();
  b.UnifyPos();
  b.AfterLoad();
  return b;
}
