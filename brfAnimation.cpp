#include <vector>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;

#include "brfAnimation.h"

#include "saveLoad.h"

bool BrfAnimation::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadInt(f , nbones);
  LoadInt(f , nframes);
  Export("tmpAni.txt");
  return true;
}

void BrfAnimation::Export(char* fn){
  FILE* f = fopen(fn,"wt");
  fprintf(f,"%s -- %d bones  %d frames...\n",nbones,nframes);

  fclose(f);
}

BrfAnimation::BrfAnimation()
{
}
