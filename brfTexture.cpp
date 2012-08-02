#include <vector>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "saveLoad.h"

#include "brfTexture.h"

BrfTexture::BrfTexture()
{
}
bool BrfTexture::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadUint(f , flags);
  return true;
}
