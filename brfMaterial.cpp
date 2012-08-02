#include <vector>
#include <vcg/space/point4.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "saveLoad.h"

#include "brfMaterial.h"


BrfMaterial::BrfMaterial()
{
  bbox.SetNull();
  location= UNKNOWN;
}

void BrfMaterial::SetDefault(){
  flags = 0;
  renderOrder = 0;
  sprintf(shader,"simple_shader");
  sprintf(diffuseA,name);
  sprintf(diffuseB,"none");
  sprintf(bump,"none");
  sprintf(enviro,"none");
  sprintf(spec,"none");
  specular = 0;
  r=g=b=1;
}

bool BrfMaterial::Skip(FILE*f
                       ){
  LoadString(f, name);
  ::Skip<int>(f);

  LoadString(f, shader);
  LoadString(f, diffuseA);
  LoadString(f, diffuseB);
  LoadString(f, bump);
  LoadString(f, enviro);
  LoadStringMaybe(f, spec,"none");
  ::Skip(f,16);

  return true;
}


bool BrfMaterial::Load(FILE*f, int verbose){
  LoadString(f, name);
  //if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadUint(f , flags);

  // -8..+7 encoded as 4 bits difference encoding...
  renderOrder = ( flags >>24 ) & 15;
  if (renderOrder>7) renderOrder -= 16;
  assert(renderOrder>=-8 && renderOrder<=7);
  flags &=0xF0FFFFFF;

  LoadString(f, shader);
  LoadString(f, diffuseA);
  LoadString(f, diffuseB);
  LoadString(f, bump);
  LoadString(f, enviro);
  LoadStringMaybe(f, spec,"none");
  LoadFloat(f,specular);
  LoadFloat(f,r);
  LoadFloat(f,g);
  LoadFloat(f,b);
  return true;
}

void BrfMaterial::Save(FILE*f) const{
  SaveString(f, name);
  unsigned int fl = flags;
  int ro = renderOrder;
  assert(ro>=-8 && ro<=7);
  if (ro<0) ro+=16;
  fl&= 0xF0FFFFFF;
  fl+= ro<<24;
  SaveUint(f , fl);
  SaveString(f, shader);
  SaveString(f, diffuseA);
  SaveString(f, diffuseB);
  SaveString(f, bump);
  SaveString(f, enviro);
  SaveStringNotempty(f, spec, "none");
  SaveFloat(f,specular);
  SaveFloat(f,r);
  SaveFloat(f,g);
  SaveFloat(f,b);
}

vcg::Box3f BrfMaterial::bbox;
