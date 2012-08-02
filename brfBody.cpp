#include <vector>
#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;

#include "brfBody.h"

#include "saveLoad.h"


BrfBody::BrfBody()
{
}

float* BrfBodyPart::GetRotMatrix() const{
  vcg::Point3f dx, dy, dz(1,1.1,1.3);
  dx = (dir-center)/2.0;
  dy = dz ^ dx;
  dz = dx ^ dy;
  dy.Normalize();
  dz.Normalize();
  static float res[16];
  for (int i=0; i<4; i++) {
    res[i]    = (i==3)?0:dx[i];
    res[i+4]  = (i==3)?0:dy[i];
    res[i+8]  = (i==3)?0:dz[i];
    res[i+12] = (i==3)?1:0;
  }
  return res;
}

void BrfBodyPart::UpdateBBox(){
  bbox.SetNull();
  vcg::Point3f r(radius, radius, radius);
  if (type==MANIFOLD || type==FACE) {
    for (unsigned int i=0; i<pos.size(); i++) {
      bbox.Add(pos[i]);
    }
  }
  if (type==SPHERE) {
    bbox.Add(center+r);
    bbox.Add(center-r);
  }
  if (type==CAPSULE) {
    bbox.Add(center+r);
    bbox.Add(center-r);
    bbox.Add(dir+r);
    bbox.Add(dir-r);
  }
}

void BrfBody::UpdateBBox(){
  bbox.SetNull();
  for (unsigned int i=0; i<part.size(); i++) {
    bbox.Add(part[i].bbox);
  }
}



bool BrfBodyPart::Load(FILE*f, char* _firstWord, int verbose ){
  char firstWord[255];

  if (!_firstWord) LoadString(f,firstWord);
  else sprintf(firstWord,_firstWord);

  //static FILE* ftmp=NULL; //if (!ftmp) ftmp=fopen("debug.txt","wt");

  if (!strcmp(firstWord,"manifold")) {
    type=MANIFOLD;
    LoadVector(f, pos);// # points

    int k;
    LoadInt(f,k); // # faces
    for (int i=0; i<k; i++) {
      int h;
      LoadInt(f,ori); // orientation? -1 or 1 apparently.
      assert(ori==1 || ori==-1);
      LoadInt(f,h);
      assert(h==0);
      LoadInt(f,h); // # verts
      std::vector<int> v;
      for (int j=0; j<h; j++) {
        int pp;
        LoadInt(f,pp);
        v.push_back(pp);
      }
      face.push_back(v);
    }
  } else if (!strcmp(firstWord,"capsule")) {
    type=CAPSULE;
    LoadFloat(f,radius);
    LoadPoint(f,center);
    LoadPoint(f,dir);
    LoadUint(f,flags);
    //fprintf(ftmp,"%f %f %f %f %f %f %f\n",radius,center[0],center[2],center[1],dir[0],dir[2],dir[1]);
    //fflush(ftmp);
  } else if (!strcmp(firstWord,"sphere")) {
    type=SPHERE;
    LoadFloat(f,radius);
    LoadPoint(f,center);
    LoadUint(f,flags);
  } else if (!strcmp(firstWord,"face")) {
    type=FACE;
    LoadVector(f,pos);

    int k = pos.size();

    // default face
    std::vector<int> aface;
    for (int i=0; i<k; i++) aface.push_back(i);
    face.push_back(aface);

    LoadUint(f,flags);

  } else {
    printf("Unknown body (collision mesh) type `%s`\n",firstWord);
    assert(0);
  }

  UpdateBBox();
  return true;
}


void BrfBodyPart::Save(FILE *f) const {
  switch(type) {
  case MANIFOLD:
      SaveString(f,"manifold");
      SaveVector(f,pos);

      SaveUint(f,face.size());
      for (unsigned int i=0; i<face.size(); i++) {
        SaveInt(f,ori);
        SaveInt(f,0);
        SaveUint(f,face[i].size());
        for (unsigned int j=0; j<face[i].size(); j++) {
          SaveInt(f,face[i][j]);
        }
      }
      break;
  case CAPSULE:
      SaveString(f,"capsule");
      SaveFloat(f,radius);
      SavePoint(f,center);
      SavePoint(f,dir);
      SaveUint(f,flags);
      break;
  case SPHERE:
      SaveString(f,"sphere");
      SaveFloat(f,radius);
      SavePoint(f,center);
      SaveUint(f,flags);
      break;
  case FACE:
      SaveString(f,"face");
      assert(face.size()==1);
      SaveVector(f,pos);
      SaveUint(f,flags);
      break;
  default: assert(0);
  }
}

void BrfBody::Save(FILE*f) const{
  SaveString(f,name);
  if (part.size()==1) {
    part[0].Save(f);
  } else {
    SaveString(f,"composite");
    SaveVector(f,part);
  }
}

bool BrfBody::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);

  char str[255];
  LoadString(f, str);

  if (!strcmp(str,"composite")) {
    LoadVector(f, part);
  } else {
    BrfBodyPart b;
    b.Load(f,str,verbose);
    part.push_back(b);
  }
  UpdateBBox();

  return true;
}
