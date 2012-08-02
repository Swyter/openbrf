/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

 #ifndef BRFBODY_H
#define BRFBODY_H

// Body: collision objects...

#include "brfToken.h"

class BrfBodyPart{
public:
  bool Load(FILE*f,char* firstWord=NULL, int verbose=0);
  static bool Skip(FILE*f,char* firstWord=NULL);
  void Save(FILE*f) const;
  typedef enum {MANIFOLD, FACE, CAPSULE, SPHERE, N_TYPE } Type;
  Type type;
  static char* typeName[N_TYPE];

  void Merge(const BrfBodyPart &brf);

  // for manyfolds
  std::vector<vcg::Point3f> pos;
  std::vector< std::vector<int> > face;
  int ori ; // +1 or -1


  // for capsule / spheres
  Point3f center;
  float radius;

  Point3f dir; // for capsule


  // for faces: four pos (implicit)

  unsigned int flags; // for faces, capsules, spheres only

  Box3f bbox;


  const char* name() const;

  float* GetRotMatrix() const; // rotation matrix for capsules

  bool ExportOBJ(FILE *f, int i, int &vc) const;
  bool ImportOBJ(FILE *f);

  void GuessFromManyfold(); // guess sphere, ect parameter from manufold data

  void UpdateBBox();
  void InferTypeFromString(char* str);
  void Flip();
  void Transform(float * m);
  void MakeQuadDominant();

private:
  Point3f Baricenter() const;
};

class BrfBody
{
public:
  BrfBody();

  static int tokenIndex(){return BODY;}
  char name[255];

  bool Load(FILE*f, int verbose=0);
  bool Skip(FILE*f);
  void Save(FILE*f) const;

  void Flip();
  void Transform(float * m);

  bool Merge(const BrfBody &brf);

  std::vector<BrfBodyPart> part;

  bool IsAnimable() const{return false;}

  Box3f bbox;

  bool ExportOBJ(const wchar_t *fn) const;
  bool ImportOBJ(const wchar_t *fn);
  void MakeQuadDominant();

  void UpdateBBox();
private:
};

#endif // BRFBODY_H
