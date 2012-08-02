#ifndef BRFBODY_H
#define BRFBODY_H

// Body: collision objects...

#include "brfToken.h"

class BrfBodyPart{
public:
  bool Load(FILE*f,char* firstWord=NULL, int verbose=1);
  void Save(FILE*f) const;
  typedef enum {MANIFOLD, FACE, CAPSULE, SPHERE, N_TYPE } Type;
  Type type;
  static char* typeName[N_TYPE];

  // for manyfolds
  std::vector<vcg::Point3f> pos;
  std::vector< std::vector<int> > face;


  // for capsule / spheres
  Point3f center;
  float radius;

  Point3f dir; // for capsule


  // for faces: four pos
  unsigned int flags; // for faces, capsules, spheres only

  Box3f bbox;

  float* GetRotMatrix() const; // rotation matrix for capsules
private:
  void UpdateBBox();
};

class BrfBody
{
public:
  BrfBody();

  static int tokenIndex(){return BODY;}
  char name[255];
  bool Load(FILE*f, int verbose=1);
  void Save(FILE*f) const;

  std::vector<BrfBodyPart> part;

  bool IsAnimable() const{return false;}

  Box3f bbox;
private:
  void UpdateBBox();
};

#endif // BRFBODY_H
