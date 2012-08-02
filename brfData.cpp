
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

char * tokenPlurName[N_TOKEN] = {
  "Meshes",
  "Textures",
  "Shaders",
  "Materials",
  "Skeletons",
  "Animations",
  "Collision Objects",
};


BrfData::BrfData(){
  version = 0;
  //globalVersion = 0;
}

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

template <class T>
static int myfind(const vector<T> &b, char* name){
  for (unsigned int i=0; i<b.size(); i++) if (strcmp(b[i].name,name)==0) return i;
  return -1;
}

int BrfData::Find(char* name, int token){
  switch (token) {
    case MESH: return myfind(mesh,name);
    case MATERIAL: return myfind(material,name);
    case SHADER: return myfind(shader,name);
    case TEXTURE: return myfind(texture,name);
    case BODY: return myfind(body,name);
    case SKELETON: return myfind(skeleton,name);
    case ANIMATION: return myfind(animation,name);
  }
  return -1;
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

int globVersion;

bool BrfData::Save(const char*fn) const{
  globVersion = version;

  FILE *f = fopen(fn,"wb");
  if (!f) return false;
  if (globVersion==1) { SaveString(f, "rfver "); SaveInt(f,1); }
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


const char* BrfData::GetName(int i, int token) const{
  switch (token) {
    case MESH: return mesh[i].name;
    case MATERIAL: return material[i].name;
    case SHADER: return shader[i].name;
    case TEXTURE: return texture[i].name;
    case BODY: return body[i].name;
    case SKELETON: return skeleton[i].name;
    case ANIMATION: return animation[i].name;
  }
  assert(0);
  return NULL;
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

void BrfData::ForgetTextureLocations(){
  for (unsigned int i=0; i<material.size(); i++) material[i].location=BrfMaterial::UNKNOWN;
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

void  BrfData::LoadVersion(FILE*f){
  LoadInt(f,version);
  globVersion=version;

}


bool BrfData::Load(FILE*f,int verbose, int stopAt){

  Clear();

  version = 0;
  globVersion = version;
  while (1) {
    char str[255];
    if (!LoadString(f, str)) return false;
    if (verbose>1) printf("Read \"%s\"\n",str);
    if (!strcmp(str,"end")) break;
    else if (!strcmp(str,"rfver ")) LoadVersion(f);
    else if (!strcmp(str,"mesh"))  {if (!LoadVector(f,mesh)) return false;}
    else if (!strcmp(str,"texture")) {if (!LoadVector(f,texture)) return false;}
    else if (!strcmp(str,"shader")) {if (!LoadVector(f,shader)) return false;}
    else if (!strcmp(str,"material")) {if (!LoadVector(f,material)) return false;}
    else if (!strcmp(str,"skeleton")) {if (!LoadVector(f,skeleton)) return false;}
    else if (!strcmp(str,"skeleton_anim")) {if (!LoadVector(f,animation)) return false;}
    else if (!strcmp(str,"body")) {if (!LoadVector(f,body)) return false; }
    else {
      //printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      fclose(f);
      return false;
    }

  }
  fclose(f);
  return true;
}


bool BrfData::LoadFast(char*filename, bool faster){
  FILE *f = fopen(filename,"rb");
  if (!f) return false;

  version = 0;
  globVersion = version;

  while (1) {
    char str[255];
    if (!LoadString(f, str)) return false;
    if (!strcmp(str,"end")) break;
    else if (!strcmp(str,"rfver ")) LoadVersion(f);
    else if (!strcmp(str,"shader")) {if (!SkipVector(f,shader)) return false;}
    else if (!strcmp(str,"texture")) {if (!LoadVector(f,texture)) return false; }
    else if (!strcmp(str,"material")) {if (!SkipVector(f,material)) return false; }
    else if (!strcmp(str,"mesh")) { if (faster) break; if (!SkipVector(f,mesh)) return false;}
    else if (!strcmp(str,"skeleton")) {if (!SkipVector(f,skeleton)) return false;}
    else if (!strcmp(str,"skeleton_anim")) { if (faster) break; if (!SkipVector(f,animation)) return false;}
    else if (!strcmp(str,"body")) { if (faster) break; if (!SkipVector(f,body)) return false;}
    else {
      //printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      fclose(f);
      return false;
    }

  }
  fclose(f);
  return true;
}
