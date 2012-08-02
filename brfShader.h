#ifndef BRFSHADER_H
#define BRFSHADER_H

#include "brfToken.h"

class BrfShaderOpt{
public:
  BrfShaderOpt();
  int map;
  unsigned int colorOp, alphaOp, flags;
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
};

class BrfShader
{
public:
  static int tokenIndex(){return SHADER;}
  char name[255];
  char technique[255];

  char fallback[255];
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;

  void SetDefault();

  BrfShader();
  unsigned int flags;
  unsigned int requires;
  std::vector<BrfShaderOpt> opt;
};

#endif // BRFSHADER_H
