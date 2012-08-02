
#include "brfData.h"

#include "saveLoad.h"


 char * tokenBrfName[N_TOKEN] = {
  "mesh",
  "texture",
  "shader",
  "material",
  "skeleton",
  "skeleton_anim",
  "body",
};

char * tokenTabName[N_TOKEN] = {
  "&Mesh",
  "Te&xture",
  "&Shader",
  "Mat&erial",
  "S&keleton",
  "&Animation",
  "&Collision",
};


char * tokenFullName[N_TOKEN] = {
  "Mesh",
  "Texture",
  "Shader",
  "Material",
  "Skeleton",
  "Animation",
  "Collision Object",
};


BrfData::BrfData(FILE*f,int verbose, int stopAt){
  Load(f,verbose,stopAt);
}
BrfData::BrfData(char*f,int verbose, int stopAt){
  Load(f,verbose,stopAt);
}

int BrfData::GetFirstUnusedLetter() const{
  vector<bool> used(255,false);

  for (unsigned int i=0; i<mesh.size(); i++) {
    char let = mesh[i].name[4];
    used[let]=true;
  }
  for (char i='A'; i<'Z'; i++) if (!used[i]) return i-'A';
  return -1;
}

BrfMesh BrfData::GetCompleteSkin(int pos) const{
  BrfMesh res;
  bool first = true;
  for (unsigned int i=0; i<mesh.size(); i++) {
    char let = mesh[i].name[4];
    if (let==char(pos+'A')) {
      BrfMesh a = mesh[i];
      a.KeepOnlyFrame(0);
      if (first) res=a; else res.Merge(a);
      first=false;
    }
  }
  return res;
}

template <class T>
static void mergeVec(vector<T> &a, const vector<T> &b){
  for (unsigned int i=0; i<b.size(); i++) a.push_back(b[i]);
}

void  BrfData::Merge(const BrfData& b){
  mergeVec(mesh, b.mesh);
  mergeVec(texture, b.texture);
  mergeVec(shader, b.shader);
  mergeVec(material, b.material);
  mergeVec(skeleton, b.skeleton);
  mergeVec(animation, b.animation);
  mergeVec(body, b.body);
}


bool BrfData::Load(char*filename,int verbose, int stopAt){
  FILE *f = fopen(filename,"rb");
  if (!f) return false;
  return Load(f, verbose, stopAt);
}



template<class BrfType> void BrfData::SaveAll(FILE *f, const vector<BrfType> &v) const{
  if (v.size()==0) return;
  SaveString(f,tokenBrfName[ BrfType::tokenIndex() ]);
  SaveInt(f, v.size());
  for (unsigned int i=0; i<v.size(); i++) {
     v[i].Save(f);
  }
}

int BrfData::getOneSkeleton(int nbones, int after){

  for (unsigned int i=0; i<skeleton.size(); i++){
    if ((int)skeleton[i].bone.size()==nbones) { if (!after)return i; after--; }
  }
  return -1;
}

bool BrfData::Save(const char*fn) const{

  FILE *f = fopen(fn,"wb");
  if (!f) return false;
  SaveAll(f,shader);
  SaveAll(f,texture);
  SaveAll(f,material);
  SaveAll(f,mesh);
  SaveAll(f,skeleton);
  SaveAll(f,animation);
  SaveAll(f,body);

  SaveString(f,"end");
  fclose(f);

  return true;
}


int BrfData::FirstToken() const{
  if(mesh.size()) return MESH;
  if(material.size()) return MATERIAL;
  if(shader.size()) return SHADER;
  if(texture.size()) return TEXTURE;
  if(body.size()) return BODY;
  if(skeleton.size()) return SKELETON;
  if(animation.size()) return ANIMATION;
  return -1;
}

void BrfData::Clear(){
  mesh.clear();
  texture.clear();
  shader.clear();
  material.clear();
  skeleton.clear();
  animation.clear();
  body.clear();
}

bool BrfData::LoadMat(FILE *f){
  Clear();
  while (1) {
    char str[255];
    LoadString(f, str);

    if (!strcmp(str,"end")) break;

    if (!strcmp(str,"mesh")) LoadVector(f,mesh); //SkipVectorV<BrfMesh>(f);
    else if (!strcmp(str,"texture")) LoadVector(f,texture);
    else if (!strcmp(str,"shader")) LoadVector(f,shader);
    else if (!strcmp(str,"material")) {
      LoadVector(f,material);
      return true;
    }
    else if (!strcmp(str,"skeleton")) LoadVector(f,skeleton);
    else if (!strcmp(str,"skeleton_anim")) LoadVector(f,animation);
    else if (!strcmp(str,"body")) LoadVector(f,body);
    else {
      printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      return false;
    }


  }
  return true;
}

void  BrfData::LoadVersion(FILE*f){
  LoadInt(f,versionA);
  LoadInt(f,versionB);
}

bool BrfData::Load(FILE*f,int verbose, int stopAt){

  Clear();

  while (1) {
    char str[255];
    LoadString(f, str);
    if (verbose>1) printf("Read \"%s\"\n",str);
    if (!strcmp(str,"end")) break;
    else if (!strcmp(str,"rfver")) LoadVersion(f);
    else if (!strcmp(str,"mesh")) LoadVector(f,mesh);
    else if (!strcmp(str,"texture")) LoadVector(f,texture);
    else if (!strcmp(str,"shader")) LoadVector(f,shader);
    else if (!strcmp(str,"material")) LoadVector(f,material);
    else if (!strcmp(str,"skeleton")) LoadVector(f,skeleton);
    else if (!strcmp(str,"skeleton_anim")) LoadVector(f,animation);
    else if (!strcmp(str,"body")) LoadVector(f,body);
    else {
      printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      fclose(f);
      return false;
    }

  }
  fclose(f);

  return true;

}


bool BrfData::LoadFast(char*filename){
  FILE *f = fopen(filename,"rb");
  if (!f) return false;
  while (1) {
    char str[255];
    LoadString(f, str);
    if (!strcmp(str,"end")) break;
    else if (!strcmp(str,"shader")) SkipVector(f,shader);
    else if (!strcmp(str,"texture")) LoadVector(f,texture);
    else if (!strcmp(str,"material")) SkipVector(f,material);
    else if (!strcmp(str,"mesh")) SkipVector(f,mesh);
    else if (!strcmp(str,"skeleton")) SkipVector(f,skeleton);
    else if (!strcmp(str,"skeleton_anim")) SkipVector(f,animation);
    else if (!strcmp(str,"body")) SkipVector(f,body);
    else {
      printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      fclose(f);
      return false;
    }

  }
  fclose(f);
  return true;
}
