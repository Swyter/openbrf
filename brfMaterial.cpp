#include <vector>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "saveLoad.h"

#include "brfMaterial.h"


BrfMaterial::BrfMaterial()
{
}


bool BrfMaterial::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadUint(f , flags);
  LoadString(f, shader);
  LoadString(f, diffuseA);
  LoadString(f, diffuseB);
  LoadString(f, bump);
  LoadString(f, enviro);
  LoadString(f, spec);
  LoadUint(f,specular);
  LoadFloat(f,r);
  LoadFloat(f,g);
  LoadFloat(f,b);
  return true;
}
