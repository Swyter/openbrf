
#include <vector>
#include <set>
#include <stdio.h>
#include <stdlib.h>

#include <vcg/space/box3.h>
#include <vcg/math/matrix44.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

using namespace std;
using namespace vcg;

#include "BrfMesh.h"
#include "BrfSkeleton.h"

#include "saveLoad.h"
typedef vcg::Point3f Pos;

extern int globVersion;

void BrfMesh::TowardZero(float x,float y, float z){
  for (unsigned int f=0; f<frame.size(); f++) {
    for (unsigned int i=0; i<frame[f].pos.size(); i++) {
      frame[f].pos[i][0]+=(frame[f].pos[i][0]<0)?x:-x;
      frame[f].pos[i][1]+=(frame[f].pos[i][1]<0)?y:-y;
      frame[f].pos[i][2]+=(frame[f].pos[i][2]<0)?z:-z;
    }
  }
}

void BrfMesh::SetDefault(){
  flags = 0;
  name[0]=material[0]=0;
  frame.resize(1);
  frame[0].pos.resize(0);
  frame[0].norm.resize(0);
  vert.resize(0);
  face.resize(0);
  rigging.resize(0);
}

void BrfMesh::MakeSingleQuad(float x, float y, float dx, float dy){
  frame.resize(1);
  frame[0].pos.resize(4);
  frame[0].norm.resize(4);
  frame[0].tang.resize(4);
  vcg::Point3f n(0,1,0);
  vcg::Point3f t(0,0,0);
  vert.resize(4);
  for (int i=0; i<4; i++) {
    BrfVert &v(vert[i]);
    v.col=0xFFFFFFFF;
    v.index = i;
    v.tang = t;
    v.__norm = n;
    v.ti=0;
    v.ta=v.tb=vcg::Point2f( i/2, 1-i%2 );
    frame[0].norm[i] = n;
    frame[0].tang[i] = t;
  }
  frame[0].pos[0]=vcg::Point3f(x,   0, y   );
  frame[0].pos[1]=vcg::Point3f(x,   0, y+dy);
  frame[0].pos[2]=vcg::Point3f(x+dx,0, y   );
  frame[0].pos[3]=vcg::Point3f(x+dx,0, y+dy);

  face.resize(2);
  face[0].index[2]=0;
  face[0].index[1]=1;
  face[0].index[0]=3;
  face[1].index[2]=3;
  face[1].index[1]=2;
  face[1].index[0]=0;

  UpdateBBox();
  isRigged = false;

}

void BrfMesh::Scale(float xNeg, float xPos, float yPos, float yNeg, float zNeg, float zPos){
  for (unsigned int f=0; f<frame.size(); f++) {
    for (unsigned int i=0; i<frame[f].pos.size(); i++) {
      frame[f].pos[i][0]*=(frame[f].pos[i][0]<0)?xNeg:xPos;
      frame[f].pos[i][1]*=(frame[f].pos[i][1]<0)?yNeg:yPos;
      frame[f].pos[i][2]*=(frame[f].pos[i][2]<0)?zNeg:zPos;
    }
    for (unsigned int i=0; i<frame[f].norm.size(); i++) {
      int j = vert[i].index;
      frame[f].norm[i][0]/=(frame[f].pos[j][0]<0)?xNeg:xPos;
      frame[f].norm[i][1]/=(frame[f].pos[j][1]<0)?yNeg:yPos;
      frame[f].norm[i][2]/=(frame[f].pos[j][2]<0)?zNeg:zPos;
      frame[f].norm[i].Normalize();
    }
  }

  AdjustNormDuplicates();
}

bool BrfMesh::AddFrameMatchPosOrDie(const BrfMesh &b, int k){
  BrfFrame fa = frame[0];
  Point3f p = bbox.Center();
  BrfFrame fb = b.frame[0];
  for (unsigned int i=0; i<fa.pos.size(); i++) {
    bool found = false;
    for (unsigned int j=0; j<fb.pos.size(); j++) {
      if ((fa.pos[i]-fb.pos[j]).Norm()<0.01) {found=true;}
    }
    if (!found) fa.pos[i] = p;
  }
  frame.insert(frame.begin()+k+1, 1, fa);
  return true;
}

bool BrfMesh::AddFrameMatchTc(const BrfMesh &b, int k){

  BrfFrame nf;
  nf.pos.resize(frame[0].pos.size());
  nf.norm.resize(frame[0].norm.size());
  nf.time = frame[frame.size()-1].time +10;

  for (unsigned int i=0; i<vert.size(); i++) {
    // find vertex closet
    int jMin=0;
    float scoreMinA = 10e20;
    float scoreMinB = 10e20;
    for (unsigned int j=0; j<b.vert.size(); j++) {
      float scoreA = (b.vert[j].ta - vert[i].ta).SquaredNorm();
      float scoreB = (b.frame[0].pos[b.vert[j].index] - frame[0].pos[vert[i].index ]).SquaredNorm() ;
      if ((scoreA<scoreMinA) ||
          ((scoreA==scoreMinA) && (scoreB<scoreMinB)) ) {
        scoreMinA=scoreA;
        scoreMinB=scoreB;
        jMin=j;
      }
    }

    nf.norm[i] = b.frame[0].norm[jMin];
    nf.pos[ vert[i].index ] = b.frame[0].pos[ b.vert[jMin].index ];

  }
  frame.insert(frame.begin()+k+1, 1, nf);
  return true;
}

bool BrfMesh::CopyModification(const BrfMesh& b){
  if (b.frame.size()!=2) return false;

  for (unsigned int i=0; i<frame[0].pos.size(); i++)
    for (unsigned int f=frame.size()-1; f>0; f--){
      for (unsigned int j=0; j<b.frame[0].pos.size(); j++)
        if ((frame[0].pos[i]-b.frame[0].pos[j]).SquaredNorm()<0.0001)
          if (b.frame[1].pos[j]!=b.frame[0].pos[j])
            frame[f].pos[i]=b.frame[1].pos[j];
    }
  return true;
}

bool BrfMesh::AddFrameMatchVert(const BrfMesh &b, int k){

  if (b.vert.size()!=vert.size()) return false;
  BrfFrame nf;
  nf.pos.resize(frame[0].pos.size());
  nf.norm.resize(frame[0].norm.size());
  nf.time = frame[frame.size()-1].time +10;
  for (unsigned int i=0; i<vert.size(); i++) {
    nf.norm[i] = b.frame[0].norm[i];
    nf.pos[ vert[i].index ] = b.frame[0].pos[ b.vert[i].index ];
  }
  frame.insert(frame.begin()+k+1, 1, nf);
  return true;

}

bool BrfMesh::AddFrameDirect(const BrfMesh &b)
{
  if (b.frame[0].pos.size()!=frame[0].pos.size()) return false;
  if (b.frame[0].norm.size()!=frame[0].norm.size()) return false;
  frame.push_back(b.frame[0]);
  int i=frame.size()-1;
  frame[i].time = frame[i-1].time +10;
  return true;
}

void BrfMesh::AddBackfacingFaces(){
  int n=(int)face.size();
  for (int ff=0; ff<n; ff++) {
    BrfFace f = face[ff];
    f.Flip();
    face.push_back( f );
  }
}

void BrfMesh::RemoveBackfacingFaces(){
  int fi=0;
  vector<int> kill(face.size(),0);
  int nsurv=0;
  for (unsigned int ff=0; ff<face.size(); ff++) {
    Point3f a=frame[fi].pos[ vert[face[ff].index[0]].index ];
    Point3f b=frame[fi].pos[ vert[face[ff].index[1]].index ];
    Point3f c=frame[fi].pos[ vert[face[ff].index[2]].index ];
    Point3f na=(c-a)^(b-a); // area weighted norm

    Point3f nb;
    nb.SetZero();
    nb+=frame[fi].norm[ face[ff].index[0] ];
    nb+=frame[fi].norm[ face[ff].index[1] ];
    nb+=frame[fi].norm[ face[ff].index[2] ];

    na.Normalize();
    nb.Normalize();
    if (nb*na<0) kill[ff]=1;
    else nsurv++;
  }
  vector<BrfFace> tmp = face;
  face.resize(0);
  for (unsigned int ff=0; ff<tmp.size(); ff++) if (!kill[ff]) face.push_back(tmp[ff]);
}




void BrfMesh::ComputeNormals(){
  for (unsigned int fi=0; fi<frame.size(); fi++) {
    for (unsigned int vi=0; vi<vert.size(); vi++){
      frame[fi].norm[vi]=Point3f(0,0,0);
    }
    for (unsigned int ff=0; ff<face.size(); ff++){
      Point3f a=frame[fi].pos[ vert[face[ff].index[0]].index ];
      Point3f b=frame[fi].pos[ vert[face[ff].index[1]].index ];
      Point3f c=frame[fi].pos[ vert[face[ff].index[2]].index ];
      Point3f n=(c-a)^(b-a); // area weighted norm
      for (int w=0; w<3; w++)
      frame[fi].norm[ face[ff].index[w] ]+=n;
    }
    for (unsigned int vi=0; vi<vert.size(); vi++){
      frame[fi].norm[vi].Normalize();
    }
  }
  AdjustNormDuplicates();
}
void BrfFrame::Apply(Matrix44f m){
  for (int i=0; i<(int)pos.size(); i++){
    pos[i]=m*pos[i];
  }
  Point3f t = m*Point3f(0,0,0);
  for (int i=0; i<(int)norm.size(); i++){
    norm[i]=m*norm[i] - t;
  }
}

void BrfMesh::Scale(float f){
  for (unsigned int i=0; i<frame.size(); i++)
    for (unsigned int j=0; j<frame[i].pos.size(); j++)
      frame[i].pos[j]*=f;

  bbox.min*=f;
  bbox.max*=f;
}

void BrfMesh::Transform(float *f){
  vcg::Matrix44f m(f); m.transposeInPlace();
  vcg::Point3f z = m * Point3f(0,0,0);
  for (unsigned int i=0; i<frame.size(); i++) {
    for (unsigned int j=0; j<frame[i].pos.size(); j++)
      frame[i].pos[j]=m*frame[i].pos[j];
    for (unsigned int j=0; j<frame[i].norm.size(); j++)
      frame[i].norm[j]=(m*frame[i].norm[j] - z).Normalize();
  }
  AdjustNormDuplicates();
  UpdateBBox();

}

void BrfMesh::ShrinkAroundBones(const BrfSkeleton& s, int nframe){
  std::vector<vcg::Matrix44f> m = s.GetBoneMatrices();
  vcg::Point3f z(0,0,0);
  for (int pi=0; pi<(int)rigging.size(); pi++){
    BrfRigging &rig(rigging[pi]);
    int j = pi;
    frame[nframe].pos[j]=z;
    for (int i=0; i<4; i++) {
      if (rig.boneWeight[i]>0)
        frame[nframe].pos[j] +=( m[ rig.boneIndex[i] ]*z)*rig.boneWeight[i];
    };
  }
}

void BrfMesh::Unskeletonize(const BrfSkeleton& from){

  vector<Matrix44f> mat = from.GetBoneMatrices();
/*
  for (int bi=0; bi<(int)mat.size(); bi++) {
    Matrix44f inv = vcg::Inverse(mat[bi]);
    mat[bi]=inv;
  }*/
  int k = rigging[0].boneIndex[0];
  Matrix44f m = vcg::Inverse(mat[k]);
  vcg::Transpose(m);
  Transform(&(m[0][0]));
  return;

  if (!rigging.size()) return;
  assert (rigging.size()==frame[0].pos.size());

  for (int pi=0; pi<(int)rigging.size(); pi++){
    Matrix44f matTot;
    matTot.SetZero();
    const BrfRigging &rig(rigging[pi]);
    for (int k=0; k<4; k++) {
      int i = rig.boneIndex[k];
      if (i>=0) matTot+=mat[i]* rig.boneWeight[k];
    }
    for (int fi=0; fi<(int)frame.size(); fi++) {
      BrfFrame &f(frame[fi]);

      f.pos[pi]=matTot*f.pos[pi];

    }
  }
  UpdateBBox();

}

void BrfMesh::Reskeletonize(const BrfSkeleton& from, const BrfSkeleton& to){

  vector<Matrix44f> mat0 = from.GetBoneMatrices();
  vector<Matrix44f> mat = to.GetBoneMatrices();

  for (int bi=0; bi<(int)mat.size(); bi++) {
    Matrix44f inv = vcg::Inverse(mat0[bi]);
    mat[bi]=mat[bi]*inv;
  }
  if (!rigging.size()) return;
  assert (rigging.size()==frame[0].pos.size());

  for (int pi=0; pi<(int)rigging.size(); pi++){
    Matrix44f matTot;
    matTot.SetZero();
    const BrfRigging &rig(rigging[pi]);
    for (int k=0; k<4; k++) {
      int i = rig.boneIndex[k];
      if (i>=0) matTot+=mat[i]* rig.boneWeight[k];
    }
    for (int fi=0; fi<(int)frame.size(); fi++) {
      BrfFrame &f(frame[fi]);

      f.pos[pi]=matTot*f.pos[pi];

    }
  }
  UpdateBBox();
}

void BrfFrame::MakeSlim(float ratioX, float ratioZ, const BrfSkeleton* s){
  vector<Matrix44f> m = s->GetBoneMatrices();
  float y3 = m[ s->FindBoneByName("shoulder.L") ].GetColumn3(3)[1]-0.075;
  float y1 = m[ s->FindBoneByName("abdomen") ].GetColumn3(3)[1];
  float y0 = m[ s->FindBoneByName("calf.L") ].GetColumn3(3)[1];
  //float y1 = (y0*5 + y3)/6;
  //float y2 = (y3*5 + y0)/6;
  for (int i=0; i<(int)pos.size(); i++){
    float y = pos[i][1];
    float t;
    if (y>y1)
      t = (y-y1)/(y3-y1);
    else {
      t = (y-y1)/(y0-y1);
      t = (t<0.5)?t*t:(1-(1-t)*(1-t));
    }
    if (t<0) t=0;
    if (t>1) t=1;
    pos[i][0]*=t + (1-t)*ratioX;
    pos[i][2]*=t + (1-t)*ratioZ;
  }
}

class Grid{
public:
  enum {X=9, Y=10};
  float x[X],y[Y];
  Point3f ToLocal(Point3f p) const;
  Point3f ToGlobal(Point3f p) const;
  void FromSkeleton(const BrfSkeleton &s);
};
void Grid::FromSkeleton(const BrfSkeleton &s){
  vector<Matrix44f> m = s.GetBoneMatrices();

  x[0] = m[ s.FindBoneByName("hand.L") ].GetColumn3(3)[0]-0.05;
  x[1] = m[ s.FindBoneByName("forearm.L") ].GetColumn3(3)[0];
  x[2] = m[ s.FindBoneByName("upperarm.L") ].GetColumn3(3)[0];
  x[3] = m[ s.FindBoneByName("shoulder.L") ].GetColumn3(3)[0];
  x[4] = 0;
  x[5] = m[ s.FindBoneByName("shoulder.R") ].GetColumn3(3)[0];
  x[6] = m[ s.FindBoneByName("upperarm.R") ].GetColumn3(3)[0];
  x[7] = m[ s.FindBoneByName("forearm.R") ].GetColumn3(3)[0];
  x[8] = m[ s.FindBoneByName("hand.R") ].GetColumn3(3)[0]+0.05;

  y[0] = -0.05;
  y[1] = m[ s.FindBoneByName("foot.L") ].GetColumn3(3)[1];
  y[2] = m[ s.FindBoneByName("calf.L") ].GetColumn3(3)[1];
  y[3] = m[ s.FindBoneByName("thigh.L") ].GetColumn3(3)[1];
  //y[3] = m[ s.FindBoneByName("abdomen") ].GetColumn3(3)[1];
  y[4] = m[ s.FindBoneByName("spine") ].GetColumn3(3)[1];
  y[5] = m[ s.FindBoneByName("torax") ].GetColumn3(3)[1];
  y[6] = m[ s.FindBoneByName("shoulder.L") ].GetColumn3(3)[1]-0.075;
  y[7] = m[ s.FindBoneByName("shoulder.L") ].GetColumn3(3)[1];
  y[8] = m[ s.FindBoneByName("shoulder.L") ].GetColumn3(3)[1]+0.15;
  y[9] = m[ s.FindBoneByName("head") ].GetColumn3(3)[1]+0.15;
  //for (int i=0; i<Y; i++) //qDegug();//if (y[i]>=y[i++]) {assert(false);}
}

Point3f Grid::ToGlobal(Point3f p) const{
  int x0 = (int)floor(p[0]), y0 = (int)floor( p[1] );
  float x1 = p[0]-x0, y1 = p[1]-y0;
  //C:\Users\tarini\AppData\Roaming\Skype\Pictures\shotsy1=0;
  return ( Point3f(
    x[x0]*(1-x1)+ x[x0+1]*(x1),
    y[y0]*(1-y1)+ y[y0+1]*(y1),
    //y0*0.2//
    p[2]
  ) );
}
Point3f Grid::ToLocal(Point3f p) const{
  int x0, y0;
  for (x0=0; x0<X-1; x0++) if (x[ x0+1 ]>p[0]) break;
  for (y0=0; y0<Y-1; y0++) if (y[ y0+1 ]>p[1]) break;
  return (Point3f(
    x0 + (p[0]-x[x0])/(x[x0+1]-x[x0]),
    y0 + (p[1]-y[y0])/(y[y0+1]-y[y0]),
    p[2]
  ));
}

void BrfMesh::ReskeletonizeHuman(const BrfSkeleton& from, const BrfSkeleton& to, float bonusArm ){

  Grid a,b;
  a.FromSkeleton(from);
  b.FromSkeleton(to);

  if (bonusArm) {
    b.y[5]-=bonusArm*3/5/2; //0.03*0.5;
    b.y[6]-=bonusArm*3/5;   //0.03
    b.y[8]+=bonusArm*2/5;   //0.02;
    b.y[9]+=bonusArm*2/5;   //0.02;
  }

  int fi=0;

  for (int pi=0; pi<(int)frame[fi].pos.size(); pi++){
    frame[fi].pos[pi] = b.ToGlobal( a.ToLocal( frame[fi].pos[pi] ) );
  }
  UpdateBBox();
}


void BrfMesh::Apply(Matrix44<float> m){
  for (int i=0; i<(int)frame.size(); i++) frame[i].Apply(m);
  this->UpdateBBox();
  this->AdjustNormDuplicates();
}

void BrfMesh::SetUniformRig(int nbone){
  isRigged=true;
  int psize = (int)frame[0].pos.size();
  rigging.resize(psize);
  for (int i=0; i<psize; i++)
    for (int j=0; j<4; j++){
      rigging[i].boneIndex[j]=(j)?-1:nbone;
      rigging[i].boneWeight[j]=(j)?0 :1;
    }
}

      class MyIndexClass{
      public:
        static BrfMesh* mesh;
        int ind;
        //MyPair(int a, int b):pair<int,int>(a,b){}
        MyIndexClass(int a):ind(a) {}

        inline bool operator == (MyIndexClass const &b)const
        { return (mesh->frame[0].pos[ind] == mesh->frame[0].pos[b.ind])
          && ( (mesh->rigging.size()) || (mesh->rigging[ind] == mesh->rigging[b.ind])); }
        inline bool operator < (MyIndexClass const &b)const
        {
          if (mesh->frame[0].pos[ind] < mesh->frame[0].pos[b.ind]) return true;
          if (mesh->frame[0].pos[ind] != mesh->frame[0].pos[b.ind]) return false;
          if (mesh->rigging.size()) return mesh->rigging[ind] < mesh->rigging[b.ind];
          return false;
        }

      };
      BrfMesh* MyIndexClass::mesh;

void BrfMesh::UnifyPos(){
  typedef std::set< MyIndexClass > Set;
  Set st;
  MyIndexClass::mesh=this;
  vector<int> map(frame[0].pos.size());

  for (unsigned int pi=0; pi<frame[0].pos.size(); pi++) {
    pair< Set::iterator, bool> res;
    res = st.insert( MyIndexClass(pi) );
    int pin = res.first->ind;
    map[pi]=pin;
  }

  vector<int> rank(frame[0].pos.size(),-1);
  for (int i=0,k=0; i<(int)map.size(); i++) {
    if (map[i]==i) rank[i]=k++;
  }


  for (unsigned int vi=0; vi<vert.size(); vi++) {
    vert[vi].index =  rank[ map[vert[vi].index] ];
  }
  int ns = (int)st.size();

  for (unsigned int i=0; i<frame.size(); i++){
    for (int h=0; h<(int)rank.size(); h++) {
      if (rank[h]>=0) {
        frame[i].pos[ rank[h] ]=frame[i].pos[h];
        if (rigging.size()>0 && i==0)
         rigging[ rank[h] ]=rigging[h];
      }
    }
    frame[i].pos.resize( ns );
    if (rigging.size()>0) rigging.resize( ns );
  }
}


      class MyIndexVertClass{
      public:
        static BrfMesh* mesh;
        static bool careForNormals;
        static float crease;
        int ind;

        MyIndexVertClass(int a):ind(a) {}

        //inline bool operator == (MyIndexClass const &b)const{ return true; }
        //{ return (mesh->vert[ind] == mesh->vert[b.ind]);}
        inline bool operator < (MyIndexVertClass const &b)const
        {
          if (mesh->vert[ind].index <  mesh->vert[b.ind].index) return true;
          if (mesh->vert[ind].index != mesh->vert[b.ind].index) return false;

          if (mesh->vert[ind].col <  mesh->vert[b.ind].col) return true;
          if (mesh->vert[ind].col != mesh->vert[b.ind].col) return false;

          if (mesh->vert[ind].ta <  mesh->vert[b.ind].ta) return true;
          if (mesh->vert[ind].ta != mesh->vert[b.ind].ta) return false;
          if (mesh->vert[ind].tb <  mesh->vert[b.ind].tb) return true;
          if (mesh->vert[ind].tb != mesh->vert[b.ind].tb) return false;

          if (careForNormals)
          for (unsigned int i=0; i<mesh->frame.size(); i++) {
            if ((mesh->frame[i].norm[ind] * mesh->frame[i].norm[b.ind])>crease) return false;
            if (mesh->frame[i].norm[ind] < mesh->frame[i].norm[b.ind]) return true;
            //if (mesh->frame[i].norm[ind] != mesh->frame[i].norm[b.ind]) return false;
          }

          return false;
        }

      };
      BrfMesh* MyIndexVertClass::mesh;
      bool MyIndexVertClass::careForNormals;
      float MyIndexVertClass::crease;

void BrfMesh::DivideVert(){
  std::vector<BrfVert> v = vert;
  vert.resize(face.size()*3);
  for (unsigned int j=0; j<frame.size(); j++) frame[j].norm.resize(face.size()*3);
  for (unsigned int i=0,k=0; i<face.size(); i++)
  for (unsigned int w=0; w<3; w++,k++) {
    vert[k]=v[ face[i].index[w] ];
    face[i].index[w] = k;
  }
}

void BrfMesh::RemoveUnreferenced(){
  vector<bool> present;
  vector<int> rank;
  vector<int> select;

  // remove unreferenced verts
  int r=0;
  present.resize(vert.size(),false);
  rank.resize(vert.size());
  for (unsigned int i=0; i<face.size(); i++) {
    present[ face[i].index[0] ] =
    present[ face[i].index[1] ] =
    present[ face[i].index[2] ] = true;
  }
  for (unsigned int i=0; i<vert.size(); i++) {
    if (present[i]) { rank[i]=r++; select.push_back(i); }
  }
  for (unsigned int i=0; i<face.size(); i++) {
    face[i].index[0] = rank[ face[i].index[0] ];
    face[i].index[1] = rank[ face[i].index[1] ];
    face[i].index[2] = rank[ face[i].index[2] ];
  }
  for (int i=0; i<r; i++) {
    vert[i] = vert[ select[i] ];
    for (unsigned int nf=0; nf<frame.size(); nf++) frame[nf].norm[i]=frame[nf].norm[ select[i] ];
  }
  vert.resize(r);
  for (unsigned int nf=0; nf<frame.size(); nf++) frame[nf].norm.resize(r);

  // remove unreferenced pos

  present.clear();
  rank.clear();
  select.clear();
  r=0;
  assert(frame.size()>0);
  present.resize(frame[0].pos.size(),false);
  rank.resize(frame[0].pos.size());
  for (unsigned int i=0; i<vert.size(); i++) {
    present[ vert[i].index ] = true;
  }
  for (unsigned int i=0; i<frame[0].pos.size(); i++) {
    if (present[i]) { rank[i]=r++; select.push_back(i); }
  }
  assert(r<=(int)frame[0].pos.size());
  for (unsigned int i=0; i<vert.size(); i++) {
    vert[i].index = rank[ vert[i].index ];
  }

  for (int i=0; i<r; i++) {
    for (unsigned int nf=0; nf<frame.size(); nf++) frame[nf].pos[i] = frame[nf].pos[ select[i] ];
  }
  for (unsigned int nf=0; nf<frame.size(); nf++) frame[nf].pos.resize(r);

  UpdateBBox();
}

void BrfMesh::RemoveSeamsFromNormals(double crease)
{
  std::vector<vcg::Point3f> np(frame[0].pos.size()); // norm per pos
  for (unsigned int fi=0; fi<frame.size(); fi++){
    for (unsigned int i=0; i<np.size(); i++) np[i].SetZero();
    for (unsigned int i=0; i<vert.size(); i++) {
      int j = vert[i].index;
      np[ j ]+=frame[fi].norm[i];
    }
    for (unsigned int i=0; i<np.size(); i++) np[i].Normalize();

    for (unsigned int i=0; i<vert.size(); i++) {
      int j = vert[i].index;
      if (np[j]*frame[fi].norm[i]>crease)
        frame[fi].norm[i]= np[j];
    }

  }
  AdjustNormDuplicates();
}

void BrfMesh::UnifyVert(bool careForNormals, float crease){
  typedef std::set< MyIndexVertClass > Set;
  Set st;
  MyIndexVertClass::mesh=this;
  MyIndexVertClass::careForNormals=careForNormals;
  MyIndexVertClass::crease=crease;
  vector<int> map(vert.size());

  for (unsigned int vi=0; vi<vert.size(); vi++) {
    pair< Set::iterator, bool> res;
    res = st.insert( MyIndexVertClass(vi) );
    int pin = res.first->ind;
    map[vi]=pin;
  }

  vector<int> rank(vert.size(),-1);
  for (int i=0,k=0; i<(int)map.size(); i++) {
    if (map[i]==i) rank[i]=k++;
  }


  for (unsigned int fi=0; fi<face.size(); fi++)
  for (int w=0; w<3; w++) {
    face[fi].index[w] =  rank[ map[ face[fi].index[w] ] ];
  }
  int ns = (int)st.size();

  for (int h=0; h<(int)rank.size(); h++) {
    if (rank[h]>=0) {
      vert[ rank[h] ]=vert[h];
      for (unsigned int i=0; i<frame.size(); i++) {
        frame[i].norm[ rank[h] ] = frame[i].norm[ h ];
      }
    }
  }
  vert.resize( ns );
  for (unsigned int i=0; i<frame.size(); i++) frame[i].norm.resize(ns);
}

int SameTri(Pos a0, Pos a1, Pos b0, Pos b1, Pos c0, Pos c1){  
   
  if  (a0==a1 && b0==b1 && c0==c1) return 1;
    //  ||(a0==b1 && b0==c1 && c0==a1) 
  //  ||(a0==c1 && b0==a1 && c0==b1)
  ;
  return 0;
  
}

void BrfMesh::UpdateBBox(){
  bbox.SetNull();
  for (unsigned int i=0; i<frame[0].pos.size(); i++) {
    bbox.Add(frame[0].pos[i]);
  }
}


void BrfMesh::FindSymmetry(vector<int> &output){
  int nfound=0,nself=0;
  output.resize(frame[0].pos.size());
  for (unsigned int i=0; i<frame[0].pos.size(); i++) {
    output[i]=i;
  }
  
  for (unsigned int i=0; i<frame[0].pos.size(); i++) {
    for (unsigned int j=0; j<frame[0].pos.size(); j++) {
      Point3f pi = frame[0].pos[i];
      Point3f pj = frame[0].pos[j];
      pj[0]*=-1;
      if ((pi-pj).Norm()<0.01){ 
        output[i]=j;
        if (i==j) nself++; else nfound++;
      }
    }  
  }
  printf("Found %d symmetry (and %d self) on %d",nfound,nself,frame[0].pos.size());
}

void BrfMesh::ApplySymmetry(const vector<int> &input){
  vector<Point3f> oldpos=frame[0].pos;
  for (unsigned int i=0; i<frame[0].pos.size(); i++) {
    for (int k=1; k<3; k++) { // for Y and Z, not X
      frame[0].pos[i][k] = oldpos[input[i]][k];
    }
  }
}

void BrfMesh::CopyPos(int nf, const BrfMesh &brf, const BrfMesh &newbrf){
  int nbad=0, ngood=0;
  
  for (unsigned int i=0; i<vert.size(); i++) {
    int found=false;
    for (unsigned int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[nf].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[0 ].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      if (1
      // && (posA-posB).SquaredNorm()<0.01
      && posA==posB
      //&& uvA==uvB
      ) 
      { 
         frame[nf].pos[vert[i].index] = newbrf.frame[0].pos[newbrf.vert[j].index];
         //frame[nf].norm[vert[i].index] = newbrf.frame[0].norm[brfnew.vert[j].index];
         found=true;
         break;
      }
    }
    if (!found) nbad++; else ngood++;
  }
  printf("NOt changed %d (bad) and changed %d good\n",nbad, ngood);
}

void BrfMesh::FixTextcoord(const BrfMesh &brf,BrfMesh &ref, int fi){
  int nbad=0, ngood=0, nundec=0;
  
  vector<bool> undec;
  undec.resize(vert.size(), false);
  
  vector<int> perm(vert.size(), -1);
  
  for (unsigned int i=0; i<vert.size(); i++) {
    int found=false;
    float score = 100000000.0;
    int bestj=-1;
    for (unsigned int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[fi].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[ 0].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      
      if (1
      //&& (posA-posB).SquaredNorm()<0.01
      && posA==posB
      //&& uvA==uvB
      ) 
      { 
         if (found) undec[i]=true;
         if (perm[j]!=-1) continue;
         found=true;
         float curscore = (ref.vert[i].ta - brf.vert[j].ta).SquaredNorm();
         if (curscore<score) {
            score=curscore;
         } else continue;
         bestj=j;
      }
    }
    
    if (found) perm[bestj]=i;
    if (!found) nbad++; else
      if (undec[i]) nundec++; else
        ngood++;
  }

  printf("Found %d bad, %d good, %d undex\n",nbad, ngood, nundec);
  
  // apply permut
  vector<BrfVert> tmp=vert;
  vector<bool> tmp2=undec;
  for (unsigned int i=0; i<vert.size(); i++) {
    vert[ i ] = tmp[ perm[i] ];
    ref.vert[ i ] = tmp[ perm[i] ];
    undec[i] = tmp2[ perm[i] ];    
  }
  
  vector<int> permInv(vert.size());
  for (unsigned int i=0; i<vert.size(); i++) {
    permInv[ perm[i] ] = i;
  }
  
  
  for (unsigned int i=0; i<face.size(); i++)
  for (int j=0; j<3; j++) {
    face[i].index[j] = permInv[face[i].index[j]];
    ref.face[i].index[j] = permInv[ref.face[i].index[j]];
  }
  
  return;

  // avrg
  for (unsigned int i=0; i<ref.face.size(); i++) {
    int ndec=0, nundec=0;
    Point2f avga,avgb;
    avga=avgb=Point2f(0,0);
    for (int j=0; j<3; j++) {
      int k=ref.face[i].index[j];
      if (undec[k])  { nundec++; }
      else {
        ndec++;
        avga+=ref.vert[k].ta;
        avgb+=ref.vert[k].tb;
      }
    }
    avga/=ndec;
    avgb/=ndec;
    if (nundec>0 && nundec<3) for (int j=0; j<3; j++) {
      int k=ref.face[i].index[j];
      if (undec[k]) {
        ref.vert[k].ta=avga;
        ref.vert[k].tb=avgb;
      }
    }
    
  }
  
}

void BrfMesh::SetName(char* st){
  sprintf(name,st);
}

void BrfMesh::DeleteSelected(){
  printf("  thereis %3dface, %3dpoin, %3dvert\n",face.size(), frame[0].pos.size(), vert.size());


  vector<int> remapv;
  remapv.resize(vert.size(),-1);
  

  vector<bool> survivesp;
  survivesp.resize(frame[0].pos.size(),false);
  
  int deletedV=0, deletedP=0, deletedF=0;
  
  vector<BrfVert> tvert = vert;
  vert.clear();
  
  vector<BrfFrame> tframe = frame;
  for (unsigned int j=0; j<frame.size(); j++) frame[j].norm.clear();
  
  for (unsigned int i=0; i<tvert.size(); i++) {
    if (!selected[i]) {
      vert.push_back(tvert[i]);
      for (unsigned int j=0; j<frame.size(); j++) frame[j].norm.push_back(tframe[j].norm[i]);
      remapv[i]=vert.size()-1;
      
      int j=tvert[i].index;
      survivesp[j]=true;
    } else deletedV++;
  }

  vector<int> remapp;
  remapp.resize(frame[0].pos.size(),-1);
  
  for (unsigned int i=0; i<frame.size(); i++) {
    int k=0;
    deletedP =0;
    for (unsigned int j=0; j<frame[i].pos.size(); j++) {
      if (survivesp[j]) {
        remapp[j]=k;
        frame[i].pos[k]=frame[i].pos[j];
        frame[i].norm[k]=frame[i].norm[j];
        k++;
      } else deletedP++;
    }
    frame[i].pos.resize(k);
  }
  
  for (unsigned int i=0; i<vert.size(); i++) {
   vert[i].index = remapp[vert[i].index];
  }
  
  vector<BrfFace> tface = face;
  face.clear();
  for (unsigned int i=0; i<tface.size(); i++) {
    BrfFace f;
    bool ok=true;
    for (int j=0; j<3; j++) {
      int k=remapv[ tface[i].index[j] ];
      if (k<0) ok=false;
      f.index[j]=k;
    }
    if (ok) face.push_back(f); else deletedF++;
  }
  printf("  deleted %3dface, %3dpoin, %3dvert\n",deletedF, deletedP, deletedV);
  printf("  dsurviv %3dface, %3dpoin, %3dvert\n",face.size(), frame[0].pos.size(), vert.size());
}

void BrfMesh::SelectAbsent(const BrfMesh& brf, int fi){
  int nbad=0, ngood=0;

  selected.clear();
  selected.resize(vert.size(),true);

  for (unsigned int i=0; i<vert.size(); i++) {
    int found=false;
    for (unsigned int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[fi].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[ 0].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      
      if (1
      && (posA-posB).SquaredNorm()<0.0001
      //&& posA==posB
      //&& uvA==uvB
      && (uvA-uvB).SquaredNorm()<0.0001
      ) 
      {
         selected[i]=false;
         found=true; 
         //break;
      }
    }
    if (!found) nbad++; else ngood++;
  }
  printf("Selected %d absent (%d where present)\n",nbad, ngood);
}



void BrfMesh::CopyTextcoord(const BrfMesh &brf, const BrfMesh &newbrf, int fi){
  int nbad=0, ngood=0;
  
  for (unsigned int i=0; i<vert.size(); i++) {
    int found=false;
    for (unsigned int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[fi].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[ 0].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      
      if (1
      && (posA-posB).SquaredNorm()<0.0001
      //&& posA==posB
      //&& uvA==uvB
      && (uvA-uvB).SquaredNorm()<0.0001     ) 
      { 
         vert[i].ta = newbrf.vert[j].ta;
         vert[i].tb = newbrf.vert[j].tb;
         //frame[0].pos[     vert[i].index ] =  newbrf.frame[0].pos[ newbrf.vert[j].index ];
         //printf("%d<->%d ",i,j); 
         //vert[i].col=0xFFFF0000; 
         found=true; 
         //break;
      }
    }
    if (!found) nbad++; else ngood++;
  }
  printf("Found %d bad and %d good\n",nbad, ngood);
}

bool BrfMesh::CheckAssert() const{
  
  bool ok = true;
  for (unsigned int i=0; i<frame.size(); i++) {
    if ( frame[i].norm.size() != vert.size() ) {
      printf("Check failed for frame %d! (%d != %d)\n",
      i,frame[i].norm.size() , vert.size());
      ok=false;
    }
  }
  return ok;
}

int BrfFrame::FindClosestPoint(Point3f to, float *min) const{
  int best=-1;
  //float min=9e10f;
  for (unsigned int i=0; i<pos.size(); i++) {
    float d = (pos[i]-to).SquaredNorm();
    if (d<*min) {
      *min=d;
      best=i;
    }
  }
  return best;
}

void BrfMesh::TransferRigging(const std::vector<BrfMesh>& from, int nf, int nfb){
  rigging.resize( frame[0].pos.size() );
  for (unsigned int i=0; i<frame[nf].pos.size(); i++) {
    float maxdist=9e10f;
    for (unsigned int h=0; h<from.size(); h++)
    {
      int j=from[h].frame[nfb].FindClosestPoint( frame[nf].pos[i], &maxdist );
      if (maxdist==9e10f) j=-1;
      if (j>=0)
        rigging[i]=from[h].rigging[j];
    }
  }
  isRigged = true;
  UpdateMaxBone();
}

void BrfMesh::UpdateMaxBone(){
  maxBone = 0;
  for (int i=0; i<(int)rigging.size(); i++) {
    for (int j=0; j<4; j++) {
      if (rigging[i].boneWeight[j]>0)
        if (rigging[i].boneIndex[j]>maxBone) maxBone = rigging[i].boneIndex[j];
    }
  }
}

// tmp class to load and save rigging
class TmpRiggingPair{
public:
  int vindex;  // vert index
  float    weight;
  bool Load(FILE*f){
    LoadInt(f,vindex);
    LoadFloat(f,weight);
    return true;
  }
  void Save(FILE*f) const {
    SaveInt(f,vindex);
    SaveFloat(f,weight);
  }
  static unsigned int SizeOnDisk(){return 8;}
};

// tmp class to load and save rigging
class TmpRigging{
public:
  int bindex; // bone index
  vector< TmpRiggingPair > pair;
  bool Load(FILE*f){
    LoadInt(f,bindex);
    if (!LoadVector(f,pair)) return false;
    return true;
  }
  void Save(FILE*f) const {
    SaveInt(f,bindex);
    SaveVector(f,pair);
  }
  static void Skip(FILE*f){
    ::Skip<int>(f);
    SkipVectorF< TmpRiggingPair >(f);
  }
  /*void Export(FILE *f){
    fprintf(f,"(bone %d) ",bindex);
    for (unsigned int i=0; i<pair.size(); i++){
      fprintf(f,"[%d]%f, ",pair[i].vindex, pair[i].weight);
      if ((i+1)%6==0) fprintf(f,"\n");
    }
    fprintf(f,"\n\n");
  }*/

};



bool BrfRigging::operator == (const BrfRigging &b) const{
  for (int i=0; i<4; i++) {
    if  (boneIndex[i]==-1) break;
    if  (boneIndex[i]!=b.boneIndex[i]) return false;
    if  (boneIndex[i]!=b.boneIndex[i]) return false;
    if  (boneWeight[i]!=b.boneWeight[i]) return false;
    if  (boneWeight[i]!=b.boneWeight[i]) return false;
  }
  return true;
}

bool BrfRigging::operator < (const BrfRigging &b) const{
  for (int i=0; i<4; i++) {
    if  (boneIndex[i]<b.boneIndex[i]) return true;
    if  (boneIndex[i]>b.boneIndex[i]) return false;
    if  (boneWeight[i]<b.boneWeight[i]) return true;
    if  (boneWeight[i]>b.boneWeight[i]) return false;
  }
  return false;
}

void Rigging2TmpRigging(const vector<BrfRigging>& vert , vector<TmpRigging>& v) {
  v.clear();
  const int MAX = 200;
  static bool usedBone[MAX];
  static int remap[MAX];
  for (int i=0; i<MAX; i++) usedBone[i]=false;
  int nUsed = 0;
  for (unsigned int i=0; i<vert.size(); i++)
  for (int j=0; j<4; j++){
    int k = vert[i].boneIndex[j];
    if (k>-1) {
      if (!usedBone[k]) {
        usedBone[k]=true;
        nUsed++;
      }
    }
  }
  v.resize(nUsed);
  nUsed=0;
  for (int i=0; i<MAX; i++){
    if (usedBone[i]) {
      v[nUsed].bindex=i;
      v[nUsed].pair.clear();
      remap[i]=nUsed;
      nUsed++;
    }
  }

  for (unsigned int i=0; i<vert.size(); i++)
  for (int j=0; j<4; j++){
    int k = vert[i].boneIndex[j];
    if (k!=-1) {
      TmpRiggingPair pair;
      pair.vindex = i;
      pair.weight = vert[i].boneWeight[j];
      v[ remap[k] ].pair.push_back(pair);
    }
  }

}

float BrfRigging::WeightOf(int i) const{
  for ( int k=0;  k<4; k++) if (boneIndex[k]==i) return boneWeight[k];
  return 0;
}

int BrfRigging::FirstEmpty() const{
  for ( int k=0;  k<4; k++) if (boneIndex[k]==-1) return k;
  return 4;
}

int BrfRigging::LeastIndex() const{
  int min=0;

  for ( int k=1;  k<4; k++) if (boneIndex[k]!=-1)
    if (boneWeight[k]<boneWeight[min]) min=k;
  return min;
}


void BrfRigging::Normalize(){
  float sum=0;

  for ( int k=0;  k<4; k++) if (boneIndex[k]!=-1) sum+=boneWeight[k];
  if (sum==0) return;
  for ( int k=0;  k<4; k++) if (boneIndex[k]!=-1) boneWeight[k]/=sum;

}

void BrfRigging::Add(int bi, float w){
  bool overflow = false;
  int k = FirstEmpty();
  if (k>=4) {
    k=LeastIndex();
    overflow= true;
  }
  boneIndex[k] = bi;
  boneWeight[k] = w;
  if (overflow) Normalize();
}

int TmpRigging2Rigging(const vector<TmpRigging>& v, vector<BrfRigging>&vert){
  int max=0;

  for (unsigned int i=0; i<v.size(); i++)
  for (unsigned int j=0; j<v[i].pair.size(); j++){

    vert[ v[i].pair[j].vindex ].Add( v[i].bindex, v[i].pair[j].weight );
    /*
    int vi = v[i].pair[j].vindex;
    int k = vert[vi].FirstEmpty();
    if (k>=4) {
      k = vert[vi].LeastIndex();
      tooManyBones = true;
    }


    vert[ vi ].boneIndex[k] = v[i].bindex;
    vert[ vi ].boneWeight[k] = v[i].pair[j].weight;
    */
    if (max<v[i].bindex) max =v[i].bindex;


  }
  return max;
}

void BrfMesh::NormalizeRigging(){
  for (unsigned int i=0; i<rigging.size(); i++) {
    rigging[i].Normalize();
  }
}

void BrfMesh::Save(FILE*f) const{
  CheckAssert();

  SaveString(f, name);  
  SaveUint(f , flags);
  SaveString(f, material); // material used

  if (globVersion != 0) {
    if (flags>>16 == 2) globVersion = 2; else globVersion = 1;
  }

  SaveVector(f, frame[0].pos);

  std::vector<TmpRigging> tmpRig;
  Rigging2TmpRigging(rigging, tmpRig);

  SaveVector(f, tmpRig);

  SaveInt(f , frame.size()-1 ); // tho other nframes!
  for (unsigned int i=1; i<frame.size(); i++) frame[i].Save(f);

  SaveVector(f, vert);
  SaveVector(f, face);
  
  return;
}

void BrfMesh::AdjustNormDuplicates(){ // copys normals
  for (unsigned int i=0; i<vert.size(); i++) {
    vert[i].__norm = frame[0].norm[i];
  }
}
/*
BrfMesh& BrfMesh::operator = (const BrfMesh &brf){
  flags=brf.flags;
  sprintf(name,"%s",brf.name);
  sprintf(material,"%s",brf.material);
  
  frame.resize(brf.frame.size() );
  
  Merge(brf);  
  return *this;
}*/

bool BrfMesh::HasVertexAni() const{
  return frame.size()>1;
}

void BrfMesh::GetTimings(std::vector<int> &v){
  v.resize(frame.size());
  for (unsigned int i=0; i<frame.size(); i++){
    v[i]= frame[i].time;
  }
}

void BrfMesh::SetTimings(const std::vector<int> &v){
  int last = 0;
  for (unsigned int i=0; i<frame.size(); i++){
    if (i<v.size()) frame[i].time = last = v[i] ;
    else { last+=10; frame[i].time = last;}
  }
}


bool BrfMesh::SaveAsPly(int frameIndex, const wchar_t* path) const{
  wchar_t filename[255];
  if (frame.size()==0) swprintf(filename,L"%ls%s.ply",path, name);
  else swprintf(filename,L"%ls\%s%02d.ply",path, name,frameIndex);
  FILE* f = _wfopen(filename,L"wt");
  if (!f) { printf("Cannot save \"%ls\"!\n",filename); return false;}
  printf("Saving \"%ls\"...\n",filename);
  fprintf(f,
    "ply\n"
    "format ascii 1.0\n"
    "comment fromBRF (by Marco Tarini)\n"
    "element vertex %d\n"
    "property float x\n"
    "property float y\n"
    "property float z\n"
    "element face %d\n"
    "property list uchar int vertex_indices\n"
    "end_header\n", vert.size(), face.size()
  );
  for (unsigned int i=0; i<vert.size(); i++) {
    fprintf(f,"%f %f %f\n",
      frame[frameIndex].pos[ vert[i].index ].X(),
      frame[frameIndex].pos[ vert[i].index ].Y(),
      frame[frameIndex].pos[ vert[i].index ].Z()
    );
  }
  for (unsigned int i=0; i<face.size(); i++) {
    fprintf(f,"3 %d %d %d\n",
      face[i].index[2],
      face[i].index[1],
      face[i].index[0]
    );
  }
  fclose(f);
  return true;
}

void BrfMesh::AdaptToRes(const BrfMesh& ref){
  //int np = frame[0].pos.size();
  
  for (unsigned int i=0; i<frame.size(); i++) {
    for (unsigned int j=0; j< frame[i].pos.size(); j++)
    frame[i].pos[j]= ref.frame[0].pos[j+4];
//     frame[i].pos.push_back( ref.frame[0].pos[j]);
   // for (int j=frame[i].pos.size(); j< ref.frame[0].pos.size(); j++) 
   //  frame[i].pos.push_back( ref.frame[0].pos[j]);
  }
  vector<int> newv(ref.vert.size(), -1);
  
/*  for (unsigned int i=0; i<ref.vert.size(); i++) {
    if (ref.vert[i].index>=np) {
      newv[i]=vert.size();
      vert.push_back(vert[i]);
    }
 }
 */

/*
  for (unsigned int i=0; i<face.size(); i++) {
    if (face[i].index>refnp) {
      int v0= newv[ face[i].index[0] ];
      int v1= newv[ face[i].index[1] ];
      int v2= newv[ face[i].index[2] ];
      if (v0>=0) {
        face.push_back(vert[i]);
      }
    }
  }
*/
 
//  vert=ref.vert;
//  face=ref.face;
}


BrfFrame BrfFrame::Average(BrfFrame& b, float t){
  BrfFrame res=*this;
  for (unsigned int i=0; i<pos.size(); i++) {
    res.pos[i]=pos[i]*(t) + b.pos[i]*(1.0f-t);
  }
  return res;
}

BrfFrame BrfFrame::Average(BrfFrame& b, float t, const vector<bool> &sel){
  BrfFrame res=*this;
  for (unsigned int i=0; i<pos.size(); i++) {
     if (sel[i]) res.pos[i]=pos[i]*(t) + b.pos[i]*(1.0f-t);
  }
  return res;
}




  
void BrfMesh::DiminishAni(float t){
  BrfFrame orig=frame[3];
  for (unsigned int i=2; i<frame.size(); i++) {
    unsigned int j=i+1;
    frame[i]=frame[i].Average((j==frame.size())?orig:frame[j],t);
  }
}

void BrfMesh::DiminishAniSelected(float t){
  vector<BrfFrame> orig=frame;
  for (unsigned int i=2; i<frame.size(); i++) {
    unsigned int j1=i+1; if (j1>=frame.size()) j1=2;
    unsigned int j2=i-1; if (j1<2) j1+=frame.size()-2;
    BrfFrame b = orig[j1];
    b=b.Average(orig[j2],0.5,selected);
    for (unsigned int k=0; k<frame[i].pos.size(); k++) {
     if (selected[k]) frame[i].pos[k]=b.pos[k];
    }
    frame[i].Average(orig[i],t,selected);
  }
}


void BrfMesh::KeepSelctedFixed(int asInFrame, double howmuch){
  for (unsigned int i=2; i<frame.size(); i++) if ((int)i!=asInFrame) {
    frame[i]=frame[i].Average(frame[asInFrame],howmuch,selected);
  }
}


int BrfMesh::GetFirstSelected(int after) const{
  for (unsigned int i=after+1; i<selected.size(); i++) if (selected[i]) return i;
  return -1;
}

Point3f BrfMesh::GetASelectedPos(int framei) const{
  return frame[framei].pos[ vert[ GetFirstSelected()] .index ];
}

Point3f BrfMesh::GetAvgSelectedPos(int framei) const{
  Point3f res=Point3f(0,0,0);
  int n=0;
  for (unsigned int i=0; i<frame[framei].pos.size(); i++) {
    if (selected[i]) {
      res+=frame[framei].pos[i];
      n++;
    }
  }
//  printf("(sel=%d on %d, vert=%d)\n",n,selected.size(), frame[framei].pos.size());
  return res/n;
}


void BrfMesh::FollowSelected(const BrfMesh &brf, int bf){
  int sel=brf.GetFirstSelected(); // first selected point
  if (sel<0) return;
  for (unsigned int j=0; j<frame.size(); j++) {
    vcg::Point3f d = brf.frame[j].pos[sel] - brf.frame[bf].pos[sel];
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      frame[j].pos[i]+=d;
    }
  }
  
}

void BrfMesh::CopySelectedFrom(const BrfMesh &brf){
  for (unsigned int j=0; j<frame.size(); j++) {
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      if (selected[i])
       frame[j].pos[i]=brf.frame[j].pos[i];
    }
  }
}



void BrfMesh::Hasten(float timemult){
  for (unsigned int j=0; j<frame.size(); j++) {
     int t= frame[j].time;
     if (t>=100) //t=100+int((t-100)/timemult);
     t=140-int((140-t)/timemult);
     frame[j].time=t;
  }
}

void BrfMesh::Flip(){
  for (unsigned int j=0; j<frame.size(); j++) {
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      frame[j].pos[i].X()*=-1;
    }
    for (unsigned int i=0; i<frame[j].norm.size(); i++){
      frame[j].norm[i].X()*=-1;
    }
  }
  for (unsigned int i=0; i<face.size(); i++) face[i].Flip();
  AdjustNormDuplicates();
}

class CoordSist{
public:
  Point3f base;
  Point3f x,y,z;
  Point3f operator() (Point3f p) {
    return x*p[0] + y*p[1] + z*p[2] + base;
  };
  CoordSist Inverse() {
    CoordSist tmp;
    tmp.x=Point3f(x[0],y[0],z[0]);
    tmp.y=Point3f(x[1],y[1],z[1]);
    tmp.z=Point3f(x[2],y[2],z[2]);
    tmp.base=Point3f(0,0,0);
    tmp.base = tmp(-base);
    return tmp;
  }
  CoordSist(){}
  CoordSist(Point3f a, Point3f b, Point3f c){
    base = a;
    b-=a;
    c-=a;
    x=b;
    y=b^c;
    z=x^y;
    x.Normalize();
    z.Normalize();
    y.Normalize();
  };
};

void BrfMesh::FollowSelectedSmart(const BrfMesh &brf, int bf){
  int p0=brf.GetFirstSelected(); 
  int p1=brf.GetFirstSelected(p0); 
  int p2=brf.GetFirstSelected(p1);
  if (p0<0 || p1<0 || p1<1 ) return;
  
  CoordSist direct( brf.frame[bf].pos[p0], brf.frame[bf].pos[p1], brf.frame[bf].pos[p2]);
  CoordSist inverse=direct.Inverse();
  
  for (unsigned int j=0; j<frame.size(); j++) {
    if ((int)j==bf) continue;
    
    CoordSist local( brf.frame[j].pos[p0], brf.frame[j].pos[p1], brf.frame[j].pos[p2]);
    
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      frame[j].pos[i] = local( inverse ( frame[j].pos[i] ) );
    }
  }
  
}

void BrfMesh::PropagateDeformations(int bf, const BrfMesh &ori){
  int p0=GetFirstSelected(); 
  int p1=GetFirstSelected(p0); 
  int p2=GetFirstSelected(p1);
  if (p0<0 || p1<0 || p1<1 ) return;

  CoordSist direct( ori.frame[bf].pos[p0], ori.frame[bf].pos[p1], ori.frame[bf].pos[p2]);
  CoordSist inverse=direct.Inverse();
  
  for (unsigned int j=1; j<frame.size(); j++) {
    if ((int)j==bf) continue;
    
    CoordSist local( frame[j].pos[p0], frame[j].pos[p1], frame[j].pos[p2]);
    
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      if (frame[bf].pos[i] != ori.frame[bf].pos[i] )
      frame[j].pos[i] = local( inverse ( frame[bf].pos[i] ) );
    }
  }
}


void BrfMesh::ClearSelection(){
  selected.clear();
  selected.resize(frame[0].pos.size(),false);
}

void BrfMesh::CopySelection(const BrfMesh& ref){
  ClearSelection();
  for (unsigned int i=0; i<ref.selected.size(); i++) if (ref.selected[i]) selected[i]=true;
//  selected = ref.selected;
}

void BrfMesh::SelectRed(const BrfMesh &brf){
  selected.clear();
  selected.resize(frame[0].pos.size(),false);
  int count=0;
  for (unsigned int i=0; i<vert.size(); i++) {
    if ( brf.vert[i].col == 0xFFFF0000 ) { 
      int j=vert[i].index; 
      if (!selected[j]) count++;
      selected[j]=true; 
    }
  }
  printf("Selected %d points\n",count);
}

void BrfMesh::SelectRand(){
  selected.clear();
  selected.resize(frame[0].pos.size(),false);
  int count=0;
  for (int i=0; i<42; i++) rand();
  for (int i=0; i<3; i++) {
    int j=rand()%(frame[0].pos.size()); 
    if (!selected[j]) count++;
    selected[j]=true; 
  }
  printf("Selected %d random points\n",count);
}

BrfMesh BrfMesh::SingleFrame(int i) const {
  BrfMesh res = *this;
  res.frame.clear();
  res.frame.push_back(frame[i]);
  return res;
}


void BrfMesh::FindRefPoints(){
  for (unsigned int i=0; i<vert.size(); i++) {
    if ( vert[i].col == 0xFF800080 ) refpoint.push_back(vert[i].index);
  }

  printf("Found %d ref points\n",refpoint.size());
}

Point3f OnCylinder(Point3f p, float range){
  float angle = p[2]/range;
  Point3f sol;
  sol[0] = cos(angle)*(range+p[0]);
  sol[2] = sin(angle)*(range+p[0]);
  sol[1] = p[1];
  return sol;
}

Point3f BrfFrame::MinPos(){
  Point3f res(0,0,0);
  if (pos.size()) res=pos[0];
  for (unsigned int i=1; i<pos.size(); i++)
    for (int k=0; k<3; k++) if (pos[i][k]<res[k]) res[k]=pos[i][k];
  return res;
}
Point3f BrfFrame::MaxPos(){
  Point3f res(0,0,0);
  if (pos.size()) res=pos[0];
  for (unsigned int i=1; i<pos.size(); i++)
    for (int k=0; k<3; k++) if (pos[i][k]>res[k]) res[k]=pos[i][k];
  return res;
}

// bends as if on a cylinder
void BrfMesh::Bend(int j, float range){
  float max=GetTopPos(j,2);
  float dx = OnCylinder( Point3f(0,0,max*10/16) , range)[0];
  for (unsigned int i=0; i<frame[j].pos.size(); i++) {
    frame[j].pos[i] = OnCylinder( frame[j].pos[i] , range);
    frame[j].pos[i][0] -=dx;
  }
}


unsigned int BrfVert::SizeOnDisk(){
  if (globVersion == 0 )
    return 4+4+12+8+8;
  else if (globVersion == 1 )
    return 4+4+12+12+1+8;
  else {
    return 4+4+12+8;
  }
}


bool BrfVert::Load(FILE*f){
  if (globVersion == 0){
    // M&B files
    LoadInt(f , index);
    LoadUint(f , col ); // color x vert! as 4 bytes AABBGGRR
    LoadPoint(f, __norm );
    LoadPoint(f, ta ); ta[1]=1-ta[1];
    LoadPoint(f, tb ); tb[1]=1-tb[1];
  }
  else if (globVersion == 1) {
    // warband files
    LoadInt(f , index);
    LoadUint(f , col );
    LoadPoint(f,__norm);

    // only old warband files has the following 2:
    LoadPoint(f,tang);
    LoadByte(f,ti);

    //static FILE*_f =wfopen("testLoadV.txt","wt"); fprintf(_f,"%d ",int(p2));//TEST

    LoadPoint(f,ta); ta[1]=1-ta[1]; tb = ta;
  } else if (globVersion == 2) {
    // warband files
    LoadInt(f , index);
    LoadUint(f , col );
    LoadPoint(f,__norm);
    LoadPoint(f,ta); ta[1]=1-ta[1]; tb = ta;
  }
  return true;
}
void BrfVert::Save(FILE*f) const{
  if (globVersion == 0) {
    SaveInt(f , index);
    SaveUint(f , col ); // color x vert! as 4 bytes AABBGGRR
    SavePoint(f, __norm );
    SavePoint(f, vcg::Point2f(ta[0],1-ta[1]) );
    SavePoint(f, vcg::Point2f(tb[0],1-tb[1]) );
  } else if (globVersion == 1){
    SaveInt(f , index);
    SaveUint(f , col );
    SavePoint(f,__norm);
    Point3f t = tang; if (t == Point3f(0,0,0)) t = __norm^Point3f(0,0,1);
    SavePoint(f,t);
    unsigned char tj = ti;
    if (tj==255) tj=0;
    SaveByte(f,tj);  //fprintf(_f,"%d ",int(p2));
    SavePoint(f, vcg::Point2f(ta[0],1-ta[1]) );
  } else {
    SaveInt(f , index);
    SaveUint(f , col );
    SavePoint(f,__norm);
    SavePoint(f, vcg::Point2f(ta[0],1-ta[1]) );
  }
}

unsigned int BrfFace::SizeOnDisk(){
  return 4+4+4;
}

bool BrfFace::Load(FILE*f){
  LoadInt(f, index[0] );
  LoadInt(f, index[1] );
  LoadInt(f, index[2] );
  return true;
}

void BrfFace::Save(FILE*f) const{
  SaveInt(f, index[0] );
  SaveInt(f, index[1] );
  SaveInt(f, index[2] );
}

bool BrfFrame::Load(FILE*f)
{
  LoadInt(f , time);
  if (!LoadVector(f,pos)) return false;
  if (!LoadVector(f,norm)) return false;
  return true;
}

bool BrfFrame::Skip(FILE*f){
  ::Skip<int>(f);
  SkipVectorB< Point3f > (f);
  SkipVectorB< Point3f > (f);
  return true;
}


void BrfFrame::Save(FILE*f) const
{
    SaveInt(f , time);
    SaveVector(f,pos);
    SaveVector(f,norm);
}

void BrfMesh::InvertPosOrder(){
  vector<Point3f> p=frame[0].pos;
  int N = p.size()-1;
  for (unsigned int i=0; i<frame[0].pos.size(); i++) frame[0].pos[i]=p[N-i];
}

void BrfMesh::FixPosOrder(const BrfMesh &b){
  vector<int> best(frame[0].pos.size(),0);
  vector<float> bestscore(frame[0].pos.size(),99999999.0);
  for (unsigned int i=0; i<vert.size(); i++)
    for (unsigned int j=0; j<b.vert.size(); j++) {
    if ((vert[i].ta == b.vert[j].ta)  && (vert[i].tb == b.vert[j].tb) ) {
      int i2=vert[i].index;
      int j2=b.vert[j].index;
      float score=(frame[0].pos[i2]-b.frame[0].pos[j2]).Norm();
      if (score<bestscore[i2]) {
        bestscore[i2]=score;
        best[i2]=j2;
      }
    }
  }
  
  vector<Point3f> p=frame[0].pos;
  for (unsigned int i=0; i<frame[0].pos.size(); i++) frame[0].pos[i]=p[best[i]];
}

BrfVert::BrfVert(){
  tang = Point3f(0,0,0);
  ti = 255;
}

BrfRigging::BrfRigging(){
  boneIndex[0]=boneIndex[1]=boneIndex[2]=boneIndex[3]=-1;
  boneWeight[0]=boneWeight[1]=boneWeight[2]=boneWeight[3]=0;
}

bool BrfMesh::Skip(FILE* f){
  if (!LoadString(f, name)) return false;
  //printf(" -skipping \"%s\"...\n",name);
  //SkipString(f);
  //::Skip<int>(f); // flags
  unsigned int _flags;
  LoadUint(f,_flags);
  if (globVersion != 0) {
    if (_flags>>16 == 2) globVersion = 2; else globVersion = 1;
  }

  if (!LoadString(f, material)) return false;
  SkipVectorB< Point3f >(f); // pos
  SkipVectorR< TmpRigging >(f);
  SkipVectorR< BrfFrame >(f);
  SkipVectorF< BrfVert >(f);
  SkipVectorF< BrfFace >(f);
  return true;
}

void BrfMesh::DiscardRigging(){
  rigging.clear();
  isRigged = false;
}


void BrfMesh::KeepOnlyFrame(int i){
  if (i<=0 || i>=(int)frame.size()) frame.resize(1); else {
    frame[0]=frame[i];
    frame.resize(1);
    AdjustNormDuplicates();
  }
}

void BrfMesh::AfterLoad(){
  UpdateBBox();
  hasVertexColor=false;
  for(unsigned int i=0; i<vert.size(); i++) if (vert[i].col!=0xFFFFFFFF) hasVertexColor=true;
}

bool BrfMesh::Load(FILE*f){
  if (!LoadString(f, name)) return false;

  LoadUint(f , flags);
  if (!LoadString(f, material)) return false; // material used
  frame.resize(1); // first frame (and only one, iff no vertex animation)
  frame[0].time =0;

  if (globVersion != 0) {
    if (flags>>16 == 2) globVersion = 2; else globVersion = 1;
  }

  if (!LoadVector(f, frame[0].pos)) return false;

  vector<TmpRigging> tmpRig;

  if (!LoadVector(f,tmpRig)) return false;


  int k;
  // if vertex animation, there are other frames...
  LoadInt(f , k  ); // nframes!
  frame.resize(k+1);
  for (unsigned int i=1; i<frame.size(); i++) frame[i].Load(f);
  
  if (!LoadVector(f,vert)) return false;
  if (!LoadVector(f,face)) return false;
  
  // let's make the structure so that M&B is happy
  frame[0].norm.resize(vert.size());
  for (unsigned int i=0; i<vert.size(); i++) {
    frame[0].norm[i]=vert[i].__norm;
  }
  
  isRigged = tmpRig.size()>0;
  rigging.resize(frame[0].pos.size());
  maxBone = TmpRigging2Rigging(tmpRig, rigging);
  NormalizeRigging();

  AfterLoad();
  return true;
}

bool BrfMesh::IsAnimable() const{
  return (frame.size()>1) || isRigged;
}

void BrfMesh::CopyTimesFrom(const BrfMesh &b){
  if (frame.size()!=b.frame.size()) {
    printf("WARNING: different number of frames %d!=%d\n",frame.size(),b.frame.size());
  }
  for (unsigned int j=0; j<frame.size(); j++) {
    frame[j].time=b.frame[j].time;
  }
}

void BrfMesh::Average(const BrfMesh &brf){

  //int nvert = vert.size();
  for (unsigned int j=0; j<frame.size(); j++) {
    for (unsigned int i=0; i<brf.frame[j].pos.size(); i++){
      frame[j].pos[i] = (frame[j].pos[i] + brf.frame[j].pos[i])/2.0;
    }
    for (unsigned int i=0; i<brf.frame[j].norm.size(); i++){
      frame[j].norm[i] = (frame[j].norm[i] + brf.frame[j].norm[i])/2.0;
    }
  }
}


class Rope{
public:
  vector<Point3f> pos;
  Point3f width;
  float lenght;
  
  void Fall(){
    for (unsigned int i=1; i<pos.size()-1; i++) {
      pos[i][1]-=0.01;
    }
  }
  
  void SetMinLenght(Point3f a, Point3f b){
    float tmp =(a-b).Norm();
    if (lenght<tmp) lenght=tmp;
  }
  
  void Resist(){
    vector<Point3f> post=pos;
    float k=lenght / (pos.size()-1);
    for (unsigned int i=0; i<pos.size()-1; i++) {
      Point3f v=pos[i]-pos[i+1];
      float l=v.Norm();
      v=-v.Normalize()*((k-l)*(0.5));
      post[i]-=v;
      post[i+1]+=v;
    }
    for (unsigned int i=1; i<pos.size()-1; i++) {
      pos[i]=post[i];
    }
  }
  
  void Simulation(int n){
    for (int i=0; i<n; i++) {
      Fall();
      Resist();
    }
  }
  
  void Init(int n){
    pos.resize(n);
    lenght=0;
  }

  void SetPos(Point3f a, Point3f b, float w){
    for (unsigned int i=0; i<pos.size(); i++) {
      float t=float(i)/(pos.size()-1);
      pos[i]=a*t+b*(1-t);
    } 
    Point3f tmp = (a-b);  tmp[1]=0;
    tmp=tmp.Normalize() * w;
    width = Point3f( tmp[2], tmp[1], -tmp[0]);
  }

  void AddTo(BrfFrame &f){
    for (unsigned int i=0; i<pos.size(); i++) {
      f.pos.push_back(pos[i]+width);
      f.pos.push_back(pos[i]-width);
      f.norm.push_back( Point3f(0,1,0) );
      f.norm.push_back( Point3f(0,1,0) );
    }
  }
  
  void AddTo(BrfMesh &b){
    int base = b.vert.size();
    
    for (unsigned int i=0; i<pos.size(); i++) {
      for (int k=0; k<2; k++) {
        BrfVert v;
        v.index=b.frame[0].pos.size()+i*2+k;
        v.__norm = Point3f(0,1,0);
        v.col = 0xffffffff;
        v.ta = v.tb = Point2f(
          479+4*k,
          512-(343+i/float(pos.size()-1) * 32)
        )/512.0;
        b.vert.push_back( v );
      }
    }
    
    for (unsigned int i=0; i<pos.size()-1; i++) {
      int i0=base+0, i1=base+1, i2=base+3, i3=base+2;
      b.face.push_back( BrfFace(i0,i1,i2) );
      b.face.push_back( BrfFace(i2,i3,i0) );
      b.face.push_back( BrfFace(i2,i1,i0) );
      b.face.push_back( BrfFace(i0,i3,i2) );
      base+=2;
    }
  }
};

void BrfMesh::AddRope(const BrfMesh &to, int nseg, float width){
  BrfMesh from=*this;
  Rope rope;
  rope.Init(nseg);
  
  for (unsigned int i=1; i<frame.size(); i++) {
    rope.SetMinLenght(from.GetAvgSelectedPos(i), to.GetAvgSelectedPos(i));
  }
  
  rope.AddTo(*this);
  for (unsigned int i=0; i<frame.size(); i++) {
    rope.SetPos(from.GetAvgSelectedPos(i), to.GetAvgSelectedPos(i), width);
    rope.Simulation(100);
    rope.AddTo(frame[i]);
  }
}

bool BrfMesh::Merge(const BrfMesh &b)
{

  if (frame.size()!=b.frame.size()) return false;
  if ( (rigging.size()!=0) != (b.rigging.size()!=0) ) return false;

  bbox.Add(b.bbox);
  if (maxBone<b.maxBone) maxBone = b.maxBone;

  int npos = frame[0].pos.size();
  int nvert = vert.size();
  
  for (unsigned int j=0; j<frame.size(); j++) {
    for (unsigned int i=0; i<b.frame[j].pos.size(); i++){
      frame[j].pos.push_back( b.frame[j].pos[i]);
    }
    for (unsigned int i=0; i<b.frame[j].norm.size(); i++){
      frame[j].norm.push_back( b.frame[j].norm[i]);
    }
  }
    
  for (unsigned int i=0; i<b.vert.size(); i++) {
    vert.push_back(b.vert[i] + npos);
  }

  for (unsigned int i=0; i<b.rigging.size(); i++) {
    rigging.push_back(b.rigging[i]);
  }

  for (unsigned int i=0; i<b.face.size(); i++) {
    face.push_back(b.face[i] + nvert);
  }
  /*
  for (unsigned int i=0; i<b.selected.size(); i++) {
    selected.push_back(b.selected[i]);
  }
  */
  return true;
}

Point3f Mirror(Point3f p) {
  return Point3f(-p[0],p[1],p[2]);
}

BrfFace Mirror(BrfFace f) {
  BrfFace r;
  r.index[0] = f.index[2];
  r.index[1] = f.index[1];
  r.index[2] = f.index[0];
  return r;
}

float BrfMesh::GetTopPos(int j, int axis) const{
  float min=-100;
  for (unsigned int i=0; i<frame[j].pos.size(); i++){
    if (min< frame[j].pos[i][axis] ) min = frame[j].pos[i][axis];
  }
  return min;
}

void AddRope(const BrfMesh &from, const BrfMesh &to, int nseg, double lenghtProp){
}


void BrfMesh::AlignToTop(BrfMesh& a, BrfMesh& b){
   
  for (unsigned int j=0; j<a.frame.size(); j++) {
      float dy = a.GetTopPos(j) - b.GetTopPos(j);
      if (dy<0) {
        for (unsigned int i=0; i<a.frame[j].pos.size(); i++)
          a.frame[j].pos[i][1]-=dy;
      } else {
        for (unsigned int i=0; i<b.frame[j].pos.size(); i++)
          b.frame[j].pos[i][1]+=dy;
      }
  }
}

void BrfMesh::MergeMirror(const BrfMesh &bb)
{
  BrfMesh b = bb;
  AlignToTop(*this,b);
  
  printf("MErging %d fotograms\n",frame.size() );
  int npos = frame[0].pos.size();
  int nvert = vert.size();
  
  for (unsigned int j=0; j<frame.size(); j++) {
    
    float dy = 0 ;// GetTopPos(j) - b.GetTopPos(j);
    
    for (unsigned int i=0; i<b.frame[j].pos.size(); i++){
      Point3f p = Mirror( b.frame[j].pos[i] );
      p[1]+=dy;
      
      // merge points near the junction
      if (fabs(p[0])<0.02) {
        p[0]=0.0;
        p = (p + frame[j].pos[i])/2.0;
        frame[j].pos[i] = p;
      } else {
        // hack to fix tail
        if (selected[i]) {
          float y,z;
          y = (p[1] + frame[j].pos[i][1])/2.0;
          z = (p[2] + frame[j].pos[i][2])/2.0;
          frame[j].pos[i][1] = p[1] = y;
          frame[j].pos[i][2] = p[2] = z;
        }
      }
      
      frame[j].pos.push_back( p );
    }
    for (unsigned int i=0; i<b.frame[j].norm.size(); i++){
      frame[j].norm.push_back( b.frame[j].norm[i]);
    }
  }
  for (unsigned int i=0; i<b.vert.size(); i++) {
    vert.push_back(b.vert[i] + npos);
  }

  for (unsigned int i=0; i<b.face.size(); i++) {
    face.push_back(Mirror( b.face[i] + nvert ));
  }
}

void BrfMesh::DuplicateFrames(const BrfMesh &b)
{
  DuplicateFrames(b.frame.size());
}

void BrfMesh::DuplicateFrames(int nf)
{
  frame.resize(nf);
  
  int time =0;
  for (unsigned int j=1; j<frame.size(); j++) {
    frame[j]=frame[0];
    frame[j].time=time;
    time+=10;
  }
}

void BrfMesh::PaintAll(int r, int g, int b){
  for (unsigned int i=0; i<vert.size(); i++) vert[i].col=0xFFFFFFFF;
}


void BrfMesh::Translate(Point3f p){
  for (unsigned int j=0; j<frame.size(); j++)
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      //if (selected[i] || !selectedOnly) 
      frame[j].pos[i]+=p;
  }
  bbox.min+=p;
  bbox.max+=p;
}

void BrfMesh::ColorAll(unsigned int newcol){
  //unsigned int col =(unsigned int)r<<8+(unsigned int)g<<16+(unsigned int)b<<24+(unsigned int)a;
  for (int i=0; i<(int)vert.size(); i++){
    vert[i].col = newcol;
  }
  hasVertexColor = (newcol != 0xFFFFFFFF);
}


void BrfMesh::CollapseBetweenFrames(int fi,int fj){
  Point3f point = frame[0].MinPos();
  if (fj>(int)frame.size()-1) fj=(int)frame.size()-1;
  for (int j=fi; j<=fj; j++) 
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      //if (selected[i] || !selectedOnly) 
      frame[j].pos[i]=point;
  }
}


/*
void BrfMesh::Scale(float p){
  for (unsigned int j=0; j<frame.size(); j++)
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      //if (selected[i] || !selectedOnly) 
      frame[j].pos[i]*=p;
  }
}
*/

void BrfMesh::TranslateSelected(Point3f p){
  for (unsigned int j=0; j<frame.size(); j++) {
    for (unsigned int i=0; i<frame[j].pos.size(); i++){
      if (selected[i]) frame[j].pos[i]+=p;
    }
  }
}

void BrfMesh::CycleFrame(int i){
  //int n=frame.size() - 2;
  for (int k=0; k<i; k++) {
    BrfFrame tmp = frame[3];
    unsigned int j;
    for (j=3; j<frame.size()-1; j++)
      frame[j]=frame[j+1];
    frame[j]=tmp;
    frame[2]=frame[j];
  }
}
