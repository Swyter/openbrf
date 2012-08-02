#ifndef BRFSKELETON_H
#define BRFSKELETON_H

#include "brfToken.h"
#include <vcg/space/box3.h>
#include <vector>

namespace vcg{
  template<typename T> class Matrix44;
}

class BrfBone
{
public:
  BrfBone();
  BrfBone(FILE *f, int verbose=1){
    Load(f,verbose);
  }
  char name[255];

  vcg::Point3f x,y,z,t;

  // 56 bytes of data.... 3x4 matrix, 12 floats + int + int?
  int attach,b;
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
  void Export(FILE*f);

  vcg::Matrix44<float> getRotationMatrix() const;

  std::vector<int> next;
};

class BrfAnimationFrame;

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

  bool SaveSMD(FILE *f) const;
  bool LoadSMD(FILE *f);
  std::vector<vcg::Matrix44<float> >  GetBoneMatrices(const BrfAnimationFrame &fr) const;
  std::vector<vcg::Matrix44<float> >  GetBoneMatrices() const;
private:
  void SetBoneMatrices(const BrfAnimationFrame &fr, int boneIndex,
                       std::vector<vcg::Matrix44<float> > &boneMatrV, const vcg::Matrix44<float>  &curr) const;
  void SetBoneMatrices(int boneIndex,
                       std::vector<vcg::Matrix44<float> > &boneMatrV, const vcg::Matrix44<float>  &curr) const;
};

#endif // BRFSKELETON_H
