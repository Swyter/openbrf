#include <vector>
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

bool BrfShader::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadUint(f , flags);
  LoadUint(f , requires);
  LoadString(f, codename);
  LoadUint(f , myst);
  assert(myst<=1);
  for (unsigned int i=0; i<myst; i++) LoadString(f , fallback);

  int k;
  LoadInt(f, k);
  opt.clear();
  for (int i=0; i<k; i++) {
    BrfShaderOpt o;
    o.Load(f,verbose);
    opt.push_back(o);
  }
  return true;
}
