#ifndef BRFMATERIAL_H
#define BRFMATERIAL_H

#include "brfToken.h"

class BrfMaterial
{
public:
  BrfMaterial();
  static int tokenIndex(){return MATERIAL;}
  char name[255];
  unsigned int flags;
  char shader[255];
  char diffuseA[255];
  char diffuseB[255];
  char bump[255];
  char enviro[255];
  char spec[255];
  unsigned int specular;
  float r,g,b;

  bool Load(FILE*f,int verbose=1);
  bool Save(FILE*f,int verbose=1) const;
};

#endif // BRFMATERIAL_H
