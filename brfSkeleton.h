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
  bool Save(FILE*f,int verbose=1) const;
  std::vector<BrfBone> bone;
  void Export(char *f);

  int root;
  void BuildTree();
  void Render(int bone, int lvl) const;
  void Render() const;
  vcg::Box3f bbox;
};

#endif // BRFSKELETON_H
