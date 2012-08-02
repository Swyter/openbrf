#ifndef BRFSKELETON_H
#define BRFSKELETON_H

#include "brfToken.h"
#include <vector>

class BrfBone
{
public:
  BrfBone();
  BrfBone(FILE *f, int verbose=1){
    Load(f,verbose);
  }
  char name[255];

  Point3f x,y,z,t;

  // 56 bytes of data.... 3x4 matrix, 12 floats + int + int?
  int attach,b;
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
  void Export(FILE*f);

  float* fullMatrix() const;
  std::vector<int> next;
};

class BrfSkeleton
{      
public:
  BrfSkeleton();
  static int tokenIndex(){return SKELETON;}
  char name[255];
  unsigned int flags;
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
  void Export(char *f);

  std::vector<BrfBone> bone;
  int root;
  void BuildTree();
  bool IsAnimable() const{return false;}
  vcg::Box3f bbox;
};

#endif // BRFSKELETON_H
