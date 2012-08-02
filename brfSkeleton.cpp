#include <vector>
#include <vcg/math/matrix44.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
#include <vcg/space/box3.h>
using namespace vcg;

#include "brfSkeleton.h"
#include "brfAnimation.h"
using namespace std;
#include "brfMesh.h"

#include "saveLoad.h"


void BrfSkeleton::BuildDefaultMesh(BrfMesh & m) const{ // builds a mesh with just an octa x bone...
  int nb = bone.size();
  m.vert.resize(nb*6);
  m.frame.resize(1);
  m.frame[0].pos.resize(nb*6);
  m.frame[0].norm.resize(nb*6);
  m.face.resize(nb*8);
  m.rigging.resize(nb*6);

  float X=0.04,Y=0.06,Z=0.12;
  vcg::Point3f pos[6]={
    vcg::Point3f(+X,0,0), //0
    vcg::Point3f(0,+Y,0), //1
    vcg::Point3f(0,0,+Z), //2
    vcg::Point3f(-X,0,0), //3
    vcg::Point3f(0,-Y,0), //4
    vcg::Point3f(0,0,-Z), //5
  };
  int facei[8][3] = {
    {0,1,2},{0,2,4},{0,4,5},{0,5,1},
    {3,1,5},{3,5,4},{3,4,2},{3,2,1},
  };

  std::vector<vcg::Matrix44f> mat = GetBoneMatrices();

  for (int i=0, pi=0, fi=0; i<nb; i++) {
    // set up rigging...
    for (int j=0; j<6; j++,pi++){
      m.rigging[pi].boneIndex[0]=i;
      m.rigging[pi].boneWeight[0]=1;
      for (int h=1; h<4; h++) {
        m.rigging[pi].boneIndex[h]=-1;
        m.rigging[pi].boneWeight[h]=0;
      }

      // set up pos and norm
      m.frame[0].pos[pi]=mat[i]*pos[j];
      m.frame[0].norm[pi]=mat[i]*(pos[j]/pos[j].Norm()) - mat[i]*Point3f(0,0,0);

      // set up uv coords
      m.vert[pi].index = pi;
      m.vert[pi].ta = m.vert[pi].tb = Point2f(0,0);
      m.vert[pi].col = 0xFFFFFFFF;

    }
    // set up face index
    for (int j=0; j<8; j++, fi++)
    for (int w=0; w<3; w++) {
      m.face[fi].index[w] = facei[j][2-w] + i*6;
    }
  }

  sprintf(m.name,"meshFromSkeleton");
  sprintf(m.material,"none");
  m.AdjustNormDuplicates();
  m.isRigged=true;
  m.hasVertexColor=false;

}

BrfSkeleton::BrfSkeleton()
{
  float h=1;
  bbox.Add( vcg::Point3f(h,2*h,h));
  bbox.Add(-vcg::Point3f(h,0,h));
}

BrfBone::BrfBone()
{}


std::vector<Matrix44f>  BrfSkeleton::GetBoneMatrices(const BrfAnimationFrame &fr) const
{
  std::vector<Matrix44f> res;
  if (fr.rot.size()!=bone.size()) return res;
  //vcg::Matrix44 m; m.Set
  res.resize(fr.rot.size());
  Matrix44f first;
  first.SetTranslate( fr.tra );
  SetBoneMatrices(fr, root, res, first);

  std::vector<Matrix44f> tmp;
  tmp = GetBoneMatrices();

  for (unsigned int i=0; i<tmp.size(); i++) {
    tmp[i] = vcg::Invert( tmp[i] );
    res[i]= res[i] * tmp[i] ;
    //res[i]= tmp[i] * res[i]  ;
  }
  return res;
}

void BrfSkeleton::SetBoneMatrices(const BrfAnimationFrame &fr, int bi,
                                  std::vector<Matrix44f> &output, const Matrix44f &curr) const
{
  Matrix44f tr; tr.SetTranslate( bone[bi].t );
  output[ bi ] = curr * tr * Matrix44f( fr.getRotationMatrix( bi ));

  for (unsigned int k=0; k<bone[bi].next.size(); k++) {
    SetBoneMatrices(fr, bone[bi].next[k] , output, output[bi] );
  }
}
std::vector<Matrix44f>  BrfSkeleton::GetBoneMatrices() const
{
  std::vector<Matrix44f> res;
  res.resize(bone.size());
  Matrix44f first;
  first.SetIdentity( );
  SetBoneMatrices( root, res, first);
  return res;
}

void BrfSkeleton::SetBoneMatrices(int bi,
                                  std::vector<Matrix44f> &output, const Matrix44f &curr) const
{
  Matrix44f tr; tr.SetTranslate( bone[bi].t );
  output[ bi ] = curr * tr * Matrix44f( bone[bi].getRotationMatrix());

  for (unsigned int k=0; k<bone[bi].next.size(); k++) {
    SetBoneMatrices( bone[bi].next[k] , output, output[bi] );
  }
}


bool BrfBone::Load(FILE*f, int verbose){
  LoadInt(f, attach);
  LoadString(f, name);
  LoadInt(f, b);

  LoadPoint(f,x);
  LoadPoint(f,z);
  LoadPoint(f,y);
  LoadPoint(f,t);

  return true;
}

void BrfBone::Save(FILE*f) const{
  SaveInt(f, attach);
  SaveString(f, name);
  SaveInt(f, b);

  SavePoint(f,x);
  SavePoint(f,z);
  SavePoint(f,y);
  SavePoint(f,t);

}

void BrfBone::Export(FILE*f){
  fprintf(f,"  %s \n  ",name);
  fprintf(f,"  %f, %f, %f\n",x[0],x[1],x[2]);
  fprintf(f,"  %f, %f, %f\n",y[0],y[1],y[2]);
  fprintf(f,"  %f, %f, %f\n",z[0],z[1],z[2]);
  fprintf(f,"  %f, %f, %f\n",t[0],t[1],t[2]);
  fprintf(f,"  attach: %d,  [%d]\n",attach,b);


}

void BrfSkeleton::BuildTree(){
  root = -1;
  for (unsigned int i=0; i<bone.size(); i++)
    bone[i].next.clear();
  for (unsigned int i=0; i<bone.size(); i++){
    int a=bone[i].attach;
    if (a==-1) {
      assert (root==-1);
      root = i;
    }
    else {
      bone[a].next.push_back(i);
    }
  }
  assert(root!=-1);
}


void BrfSkeleton::Export(char* fn){
  FILE* f = fopen(fn,"wt");
  fprintf(f,"%s -- %d bones:\n",name,bone.size());
  for (unsigned int i=0; i<bone.size(); i++){
    fprintf(f,"\n (%d) ",i);
    bone[i].Export(f);
  }
  fclose(f);
}

void BrfSkeleton::Save(FILE*f) const{
  SaveString(f, name);
  SaveVector(f,bone);
}

bool BrfSkeleton::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);

  LoadVector(f,bone);

  BuildTree();
  //Export("tmp.txt");
  return true;
}

void BrfBone::setRotationMatrix(vcg::Matrix44f m){
  x = m.GetRow3(0);
  y = m.GetRow3(1);
  z = m.GetRow3(2);
}

vcg::Matrix44f BrfBone::getRotationMatrix() const{
  float res[16];
#if 0
  res[0+0*4]=x[0];res[1+0*4]=x[1];res[2+0*4]=x[2];res[3+0*4]=0;
  res[0+1*4]=y[0];res[1+1*4]=y[1];res[2+1*4]=y[2];res[3+1*4]=0;
  res[0+2*4]=z[0];res[1+2*4]=z[1];res[2+2*4]=z[2];res[3+2*4]=0;
  res[0+3*4]=t[0];res[1+3*4]=t[1];res[2+3*4]=t[2];res[3+3*4]=1;
#else
#if 1
  res[0+0*4]=x[0];res[0+1*4]=x[1];res[0+2*4]=x[2];res[0+3*4]=0;
  res[1+0*4]=y[0];res[1+1*4]=y[1];res[1+2*4]=y[2];res[1+3*4]=0;
  res[2+0*4]=z[0];res[2+1*4]=z[1];res[2+2*4]=z[2];res[2+3*4]=0;
  res[3+0*4]=0   ;res[3+1*4]=0   ;res[3+2*4]=0   ;res[3+3*4]=1;
#else
  res[0+0*4]=x[0];res[1+0*4]=x[1];res[2+0*4]=x[2];res[3+0*4]=0;
  res[0+1*4]=y[0];res[1+1*4]=y[1];res[2+1*4]=y[2];res[3+1*4]=0;
  res[0+2*4]=z[0];res[1+2*4]=z[1];res[2+2*4]=z[2];res[3+2*4]=0;
  res[0+3*4]=0   ;res[1+3*4]=0   ;res[2+3*4]=0   ;res[3+3*4]=1;
#endif
#endif
  return Matrix44f(res);

}
