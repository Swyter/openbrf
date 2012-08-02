#ifndef BRF_DATA
#define BRF_DATA

#include <vector>
#include <stdio.h>

#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

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
  BrfData(){}
  BrfData(FILE*f,int verbose=1);
  vector<BrfMesh> mesh;
  vector<BrfTexture> texture;
  vector<BrfShader> shader;
  vector<BrfMaterial> material;
  vector<BrfSkeleton> skeleton;
  vector<BrfAnimation> animation;
  vector<BrfBody> body;
  bool Load(FILE*f,int verbose=1);
  bool Load(char*filename,int verbose=1);
};

#endif
