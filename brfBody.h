#ifndef BRFBODY_H
#define BRFBODY_H

// Body: collision objects...

#include "brfToken.h"

class BrfBodyPart{
public:
  bool Load(FILE*f,int verbose=1);
  enum {manifold, face, sphere, } type;
};

class BrfBody
{
public:
  BrfBody();

  static int tokenIndex(){return BODY;}
  char name[255];
  char manifoldName[255];
  bool Load(FILE*f,int verbose=1);
  bool Save(FILE*f,int verbose=1) const;

  std::vector<vcg::Point3f> pos;
  std::vector< std::vector<int> > face;

  void Render() const;
  Box3f bbox;
private:
  void UpdateBBox();
};

#endif // BRFBODY_H
