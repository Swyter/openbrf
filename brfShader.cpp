#include <vector>
#include <vcg/space/point4.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "brfShader.h"

#include "saveLoad.h"

BrfShader::BrfShader()
{
}

bool BrfShaderOpt::Load(FILE*f, int verbose){
  //map, colorOp, alphaOp, flags
  LoadUint(f,map);
  LoadUint(f,colorOp);
  LoadUint(f,alphaOp);
  LoadUint(f,flags);
  return true;
}

void BrfShaderOpt::Save(FILE*f) const{
  //map, colorOp, alphaOp, flags
  SaveUint(f,map);
  SaveUint(f,colorOp);
  SaveUint(f,alphaOp);
  SaveUint(f,flags);
}

bool BrfShader::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadUint(f , flags);
  LoadUint(f , requires);
  LoadString(f, codename);
  LoadUint(f , myst);
  assert(myst<=1);
  for (unsigned int i=0; i<myst; i++) LoadString(f , fallback);

  LoadVector(f,opt);
  return true;
}

void BrfShader::Save(FILE*f) const{
  SaveString(f, name);
  SaveUint(f , flags);
  SaveUint(f , requires);
  SaveString(f, codename);
  SaveUint(f , myst);
  if(myst) SaveString(f , fallback);
  SaveVector(f,opt);
}
