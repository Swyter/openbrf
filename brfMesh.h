#ifndef BRF_MESH
#define BRF_MESH
// a BRF mesh....

#include "brfToken.h"

class BrfVert{
public:
  BrfVert();
  int index;
  unsigned int col;
  Point3f __norm; // use normal inside frame instead
  Point2f ta,tb; // texture
  
  BrfVert operator + (const int i) const {BrfVert res = *this; res.index += i; return res;}
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;


};



class BrfFace{
public:
  int index[3];
  
  BrfFace(){}
  BrfFace(int i, int j, int k) {index[0]=i; index[1]=j; index[2]=k; }
  BrfFace operator + (const int i) const {BrfFace res = *this; 
    res.index[0] += i; res.index[1] += i; res.index[2] += i; return res;}
  
  void Flip() { int tmp=index[1]; index[1]=index[2]; index[2]=tmp;}
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
};

class BrfRigging{
public:
  BrfRigging();
  // rigging
  int boneIndex[4];
  float boneWeight[4];
  void SetColorGl() const;
};

class BrfFrame{
public:
  int time;
  vector<Point3f> pos;
  vector<Point3f> norm;
   // must be as many as the number of vertices!!!
  bool Load(FILE*f, int verbose);
  void Save(FILE*f) const;
  void LoadV1(FILE*f, int verbose);
  
  BrfFrame Average(BrfFrame& b, float t);
  BrfFrame Average(BrfFrame& b, float t, const vector<bool> &sel);
  
  Point3f MinPos();
  Point3f MaxPos();

};

class BrfMesh{
private:


public:
  int GetFirstSelected(int after=-1) const;
  
  // adds a rope from avg selected pos to To.AvgSelPos
  void AddRope(const BrfMesh &to, int nseg, float width);
  
  vector<BrfRigging> rigging; // one per pos

  void DiminishAni(float t);
  void DiminishAniSelected(float t);
  BrfMesh(){}
  BrfMesh(FILE *f, int verbose=1){ Load(f,verbose);}
  int flags;
  
  char name[255];
  char material[255];
  static int tokenIndex(){return MESH;}
  vector<bool> selected; 
  
  vector<BrfFrame> frame;
  
  vector<BrfVert> vert;
  
  vector<BrfFace> face;

  Box3f bbox;

  bool isRigged; // for convenience
  void UpdateBBox();
 
  // sanity check
  bool CheckAssert() const;
  
  bool Load(FILE*f,int verbose=1);
  void Save(FILE*f) const;
  bool SaveAsPly(int nframe=0, char* path="") const;
  
  void Bend(int frame, float range); // bends as if on a cylinder
  
  void CopyTimesFrom(const BrfMesh &brf);
  void Average(const BrfMesh &brf);
  void Merge(const BrfMesh &brf);
  void MergeMirror(const BrfMesh &brf);
  void AddFrame(const BrfMesh &brf);
  
  void CollapseBetweenFrames(int fi, int fj);
  void DuplicateFrames(const BrfMesh &brf);
  void DuplicateFrames(int i);
  void CopySelectedFrom(const BrfMesh &brf);
  void FollowSelected(const BrfMesh &brf, int baseframe=0);
  void FollowSelectedSmart(const BrfMesh &brf, int baseframe=0);
  
  void KeepSelctedFixed(int asInFrame, double howmuch);
  void CopyTextcoord(const BrfMesh &origbrf, const BrfMesh &newbrf, int nframe=0);
  void CopyPos(int x, const BrfMesh &origbrf, const BrfMesh &newbrf);
  
  void PropagateDeformations(int nframe, const BrfMesh &brf);
  
  void SelectRed(const BrfMesh &brf);
  void SelectRand();
  void PaintAll(int r, int g, int b);
  
  float GetTopPos(int frame, int axis=1) const;
    
  void Translate(Point3f p);
  void Scale(float f);
  void TranslateSelected(Point3f p);
  void CycleFrame(int i);
  void Flip();
  void Hasten(float timemult);
  void FixTextcoord(const BrfMesh &newbrf, BrfMesh &refbrf, int nframe=0);
  void FixPosOrder(const BrfMesh &refbrf);
  void InvertPosOrder();
  
  Point3f GetASelectedPos(int frame=0) const;
  Point3f GetAvgSelectedPos(int framei) const;
  
  void FindRefPoints();
  vector<int> refpoint;
  
  void FindSymmetry(vector<int> &output);
  void ApplySymmetry(const vector<int> &output);
  
  void SelectAbsent(const BrfMesh& ref, int nframe=0);
  void DeleteSelected();
  void ClearSelection();
  void CopySelection(const BrfMesh& ref);
  
  void SetName(char* st);
  void AdaptToRes(const BrfMesh& ref);
  //BrfMesh& operator = (const BrfMesh &brf);
  
  static void AlignToTop(BrfMesh& a, BrfMesh& b);
  BrfMesh SingleFrame(int i) const; // returns a BrfMesh consisting only of frame i

  void Render() const;
};

#endif
