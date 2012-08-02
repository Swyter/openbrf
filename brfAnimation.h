#ifndef BRFANIMATION_H
#define BRFANIMATION_H

#include "brfToken.h"

class BrfSkeleton;

class BrfAnimationFrame
{
public:
  int index;
  std::vector<vcg::Point4f> rot;
  Point3f tra;
  std::vector< bool > wasImplicit;
};

class BrfAnimation
{
public:
  BrfAnimation();
  static int tokenIndex(){return ANIMATION;}
  char name[255];
  unsigned int flags;
  int nbones; // in all frames

  std::vector<BrfAnimationFrame> frame;

  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;

  void Export(char *f);

  // to dysplay the animation...
  bool IsAnimable() const{return true;}

  static Box3f bbox;
  static void SetSkeleton(BrfSkeleton* sk);
  static int curFrame; // frame currently being shown
  //bool capped; // if true, ATTENTION, animation was capped at MAXFRAMES

  int Break(std::vector<BrfAnimation> &res) const;
  int Break(std::vector<BrfAnimation> &res, char* aniFile) const;


private:
  static BrfSkeleton* skel;
  void EnlongFrames(int nframes);
  int FirstIndex() const;
  int LastIndex() const;

};

#endif // BRFANIMATION_H
