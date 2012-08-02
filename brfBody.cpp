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

void BrfBody::UpdateBBox(){
  bbox.SetNull();
  for (unsigned int i=0; i<pos.size(); i++) {
    bbox.Add(pos[i]);
  }
}




bool BrfBody::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  int k;
  LoadString(f, manifoldName);
  LoadInt(f,k); // # points
  for (int i=0; i<k; i++) {
    vcg::Point3f p;
    LoadPoint(f,p);
    pos.push_back(p);
  }

  LoadInt(f,k); // # faces
  for (int i=0; i<k; i++) {
    int h;
    LoadInt(f,h);
    assert(h==-1);
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

  UpdateBBox();

  return true;
}
