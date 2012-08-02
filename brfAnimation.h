#ifndef BRFANIMATION_H
#define BRFANIMATION_H

#include "brfToken.h"

class BrfAnimation
{
public:
  BrfAnimation();
  static int tokenIndex(){return ANIMATION;}
  char name[255];
  unsigned int flags;
  int nbones, nframes;
  bool Load(FILE*f,int verbose=1);
  bool Save(FILE*f,int verbose=1) const;
  void Export(char *f);
};

#endif // BRFANIMATION_H
