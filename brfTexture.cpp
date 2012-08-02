#include <vector>
#include <vcg/space/box3.h>
#include <vcg/space/point4.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "saveLoad.h"

#include "brfTexture.h"

vcg::Box3f BrfTexture::bbox;

BrfTexture::BrfTexture()
{
  bbox.SetNull();
}

bool BrfTexture::Load(FILE*f, int verbose){
  if (!LoadString(f, name)) return false;
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadUint(f , flags);
  return true;
}

void BrfTexture::Save(FILE* f) const{
  SaveString(f, name);
  SaveUint(f , flags);
}

void BrfTexture::SetDefault(){
  sprintf(name, "%s.dds" ,name);
  flags=0x000AA000;
}
