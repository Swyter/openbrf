#include <vector>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
#include <vcg/space/box3.h>
using namespace vcg;

#include "brfSkeleton.h"

#include "saveLoad.h"

BrfSkeleton::BrfSkeleton()
{}

BrfBone::BrfBone()
{}

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
  float h=1.0;
  bbox.Add( vcg::Point3f(h,2*h,h));
  bbox.Add(-vcg::Point3f(h,0,h));
  return true;
}

float* BrfBone::fullMatrix() const{
  static float res[16];
#if 0
  res[0+0*4]=x[0];res[1+0*4]=x[1];res[2+0*4]=x[2];res[3+0*4]=0;
  res[0+1*4]=y[0];res[1+1*4]=y[1];res[2+1*4]=y[2];res[3+1*4]=0;
  res[0+2*4]=z[0];res[1+2*4]=z[1];res[2+2*4]=z[2];res[3+2*4]=0;
  res[0+3*4]=t[0];res[1+3*4]=t[1];res[2+3*4]=t[2];res[3+3*4]=1;
#else
#if 0
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
  return res;

}
