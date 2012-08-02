#ifndef BRF_DATA
#define BRF_DATA

#include <vector>
#include <stdio.h>

#include <vcg/space/box3.h>
#include <vcg/space/point4.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
#include <vcg/math/matrix44.h>

using namespace std;
using namespace vcg;

#include "BrfToken.h"
#include "BrfMesh.h"
#include "BrfTexture.h"
#include "BrfShader.h"
#include "BrfMaterial.h"
#include "BrfSkeleton.h"
#include "BrfAnimation.h"
#include "BrfBody.h"

#include <vector>
using namespace std;


class BrfData {
public:
  BrfData();
  BrfData(char*f,int verbose=1, int stopAt=N_TOKEN);
  BrfData(FILE*f,int verbose=1, int stopAt=N_TOKEN);
  vector<BrfMesh> mesh;
  vector<BrfTexture> texture;
  vector<BrfShader> shader;
  vector<BrfMaterial> material;
  vector<BrfSkeleton> skeleton;
  vector<BrfAnimation> animation;
  vector<BrfBody> body;
  bool Load(FILE*f,int verbose=1, int stopAt=N_TOKEN);
  bool Load(char*filename,int verbose=1, int stopAt=N_TOKEN);
  bool LoadFast(char*filename, bool ultrafast); // skips most data
  bool LoadMat(FILE *f);
  void Clear();
  int FirstToken() const;

  bool Save(const char* f) const;
  void  Merge(const BrfData& b);
  const char* GetName(int i, int token) const;

  int GetFirstUnusedLetter() const; // return first unused alphabet letter in meshes
  BrfMesh GetCompleteSkin(int i) const; // returns a mesh composed of all skin pieces

  int getOneSkeleton(int nbones, int after);
  int Find(char* name, int token);
  void ForgetTextureLocations();
  int version;
private:
  //template<class BrfType> bool LoadAll(FILE *f, vector<BrfType> &v, int k);
  template<class BrfType> void SaveAll(FILE *f, const vector<BrfType> &v) const;
  int lastLoaded;
  void LoadVersion(FILE *f);

};

#endif
