#ifndef BRFSHADER_H
#define BRFSHADER_H

#include "brfToken.h"

class BrfShaderOpt{
public:
  unsigned int map, colorOp, alphaOp, flags;
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
};

class BrfShader
{
public:
  static int tokenIndex(){return SHADER;}
  char name[255];
  char codename[255];

  char fallback[255];
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;


  BrfShader();
  unsigned int flags;
  unsigned int requires;
  unsigned int myst;
  std::vector<BrfShaderOpt> opt;
};

#endif // BRFSHADER_H
