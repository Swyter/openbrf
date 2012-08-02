/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

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
  void Save(FILE*f) const;
  static bool IsAnimable() { return false; }
  static Box3f bbox;

  void SetDefault();

};

#endif // BRFTEXTURE_H
