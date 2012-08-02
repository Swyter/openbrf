#ifndef BRFANIMATION_H
#define BRFANIMATION_H

#include "brfToken.h"

class BrfSkeleton;

class BrfAnimationFrame
{
public:
  int index;
  std::vector<vcg::Point4f> rot;
  Matrix44f getRotationMatrix(int i) const;
  void setRotationMatrix(Matrix44f m, int i);
  Point3f tra;
  std::vector< bool > wasImplicit;

  bool Reskeletonize(const BrfSkeleton& from, const BrfSkeleton& to);

};

class BrfAnimation
{
public:
  BrfAnimation();
  static int tokenIndex(){return ANIMATION;}
  char name[255];
  int nbones; // in all frames

  std::vector<BrfAnimationFrame> frame;

  bool Load(FILE*f,int verbose=1);
  bool Skip(FILE*f);
  void Save(FILE*f) const;

  void Export(const wchar_t *f);

  // to dysplay the animation...
  bool IsAnimable() const{return true;}

  static Box3f bbox;

  int Break(std::vector<BrfAnimation> &res) const;
  int Break(std::vector<BrfAnimation> &res, const wchar_t* aniFile, wchar_t *fn2) const;

  bool SaveSMD(FILE *f) const;
  bool LoadSMD(FILE *f);

  int FirstIndex() const;
  int LastIndex() const;

  bool AutoAssingTimesIfZero();
  bool Reskeletonize(const BrfSkeleton& from, const BrfSkeleton& to);

  void ShiftIndexInterval(int i);

  void GetTimings(std::vector<int> &v);
  void SetTimings(const std::vector<int> &v);

private:
  void EnlongFrames(int nframes);

};

#endif // BRFANIMATION_H
