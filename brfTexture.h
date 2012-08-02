#ifndef BRFTEXTURE_H
#define BRFTEXTURE_H

#include "brfToken.h"

class BrfTexture
{
public:
  BrfTexture();

  static int tokenIndex(){return TEXTURE;}
  char name[255];
  unsigned int flags;
  bool Load(FILE*f,int verbose=1);
  bool Save(FILE*f,int verbose=1) const;

};

#endif // BRFTEXTURE_H
