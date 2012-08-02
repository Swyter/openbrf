#ifndef BRF_MESH
#define BRF_MESH
// a BRF mesh....

#include "brfToken.h"

namespace vcg {
template <class T> class Matrix44;
}

class BrfSkeleton;

class BrfVert{
public:
  BrfVert();
  int index;
  unsigned int col;
  Point3f __norm; // use normal inside frame instead
  Point3f tang; // tangent dir...
  unsigned char ti; // texture index
  Point2f ta,tb; // texture
  
  BrfVert operator + (const int i) const {BrfVert res = *this; res.index += i; return res;}
  bool Load(FILE*f);
  void Save(FILE*f) const;

  static unsigned int SizeOnDisk();
};



class BrfFace{
public:
  int index[3];
  
  BrfFace(){}
  BrfFace(int i, int j, int k) {index[0]=i; index[1]=j; index[2]=k; }
  BrfFace operator + (const int i) const {BrfFace res = *this; 
    res.index[0] += i; res.index[1] += i; res.index[2] += i; return res;}
  
  void Flip() { int tmp=index[1]; index[1]=index[2]; index[2]=tmp;}
  bool Load(FILE*f);
  void Save(FILE*f) const;

  static unsigned int SizeOnDisk();

};

class BrfRigging{
public:
  BrfRigging();
  // rigging
  int boneIndex[4];
  float boneWeight[4];
  void SetColorGl() const;
  bool operator < (const BrfRigging &b) const;
  bool operator == (const BrfRigging &b) const;
  int FirstEmpty() const;
  int LeastIndex() const; // index with the smallest weight
  float WeightOf(int i) const;
  void Normalize();
  void Add(int index, float w);
};

class BrfFrame{
public:
  int time;
  vector<Point3f> pos;
  vector<Point3f> norm;
  vector<Point3f> tang;
   // must be as many as the number of vertices!!!
  bool Load(FILE*f);
  void Save(FILE*f) const;
  static bool Skip(FILE*f);
  
  BrfFrame Average(BrfFrame& b, float t);
  BrfFrame Average(BrfFrame& b, float t, const vector<bool> &sel);
  
  Point3f MinPos();
  Point3f MaxPos();
  void Apply(Matrix44<float> m);
  int FindClosestPoint(Point3f to, float* maxdist)const;

  void MakeSlim(float ratioX, float ratioZ, const BrfSkeleton* s);
};

class BrfMesh{
private:


public:
  void Apply(Matrix44<float> m);
  int GetFirstSelected(int after=-1) const;
  
  // adds a rope from avg selected pos to To.AvgSelPos
  void AddRope(const BrfMesh &to, int nseg, float width);
  
  vector<BrfRigging> rigging; // one per pos

  void DiminishAni(float t);
  void DiminishAniSelected(float t);

  void Unskeletonize(const BrfSkeleton& from);
  void Reskeletonize(const BrfSkeleton& from, const BrfSkeleton& to);
  void ReskeletonizeHuman(const BrfSkeleton& from, const BrfSkeleton& to, float bonusArm = 0);
  void TransferRigging(const std::vector<BrfMesh>& from, int nf, int nfb);
  void NormalizeRigging();
  void DiscardRigging();
  bool CopyModification(const BrfMesh& mod);


  BrfMesh(){}
  BrfMesh(FILE *f){ Load(f);}
  unsigned int flags;
  
  char name[255];
  char material[255];
  static int tokenIndex(){return MESH;}

  void KeepOnlyFrame(int i);

  vector<bool> selected; 
  
  vector<BrfFrame> frame;
  
  vector<BrfVert> vert;
  
  vector<BrfFace> face;

  Box3f bbox;
  int maxBone; // if rigged, what is the max bone index

  bool SaveSMD(FILE *f) const;
  bool LoadSMD(FILE *f) const;

  bool SaveOBJ(char* f, int frame) const;
  bool LoadOBJ(char* f);

  bool isRigged; // for convenience
  void UpdateBBox();
  void SetUniformRig(int nbone);
  void SplitFaces(const std::vector<int> &matIndex);
  void RemoveUnreferenced();
  void ColorAll(unsigned int newcol);
 
  void AdjustNormDuplicates(); // copys normals
  // sanity check
  bool CheckAssert() const;
  
  bool Load(FILE*f);
  void Save(FILE*f) const;
  bool Skip(FILE* f);

  bool SaveAsPly(int nframe=0, char* path="") const;
  bool HasVertexAni() const;
  
  void Flip();
  void PaintAll(int r, int g, int b);

  void Bend(int frame, float range); // bends as if on a cylinder

  bool IsAnimable() const;
  void ComputeNormals();
  void UnifyPos();
  void UnifyVert(bool careForNormals, float crease=0);
  void UnifyVertNormPerPos(bool careForNormals, float crease=0);
  void DivideVert();
  void AfterLoad();
  bool hasVertexColor;

  bool Merge(const BrfMesh &brf);
  bool AddFrameDirect(const BrfMesh &brf);
  bool AddFrameMatchVert(const BrfMesh &brf, int k);
  bool AddFrameMatchTc(const BrfMesh &brf, int k);
  bool AddFrameMatchPosOrDie(const BrfMesh &brf, int k);

  void Scale(float f);
  void Scale(float xNeg, float xPos, float yPos, float yNeg, float zPos, float zNeg);
  void TowardZero(float x,float y, float z);

  void Transform(float * m);
  void Translate(Point3f p);

  void RemoveBackfacingFaces();
  void AddBackfacingFaces();

private:
  void CopyTimesFrom(const BrfMesh &brf);
  void Average(const BrfMesh &brf);
  void MergeMirror(const BrfMesh &brf);
  void UpdateMaxBone();
  
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

  
  float GetTopPos(int frame, int axis=1) const;
    
  //void Scale(float f);
  void TranslateSelected(Point3f p);
  void CycleFrame(int i);

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



};

#endif
