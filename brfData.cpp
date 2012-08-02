
#include "brfData.h"

#include "saveLoad.h"

#if (0)
 char * tokenName[N_TOKEN] = {
  "Mesh",
  "Texture",
  "Shader",
  "Material",
  "Skeleton",
  "Animation",
  "Collision",
};
#else
 char * tokenName[N_TOKEN] = {
  "&Mesh",
  "&Texture",
  "&Shader",
  "M&aterial",
  "S&keleton",
  "A&nimation",
  "&Collision",
};
#endif

BrfData::BrfData(FILE*f,int verbose){
  Load(f,verbose);
}

bool BrfData::Load(char*filename,int verbose){
  FILE *f = fopen(filename,"rb");
  if (!f) return false;
  return Load(f, verbose);
}

bool BrfData::Load(FILE*f,int verbose){
  printf("LOADING BRF...\n");

  mesh.clear();
  texture.clear();
  shader.clear();
  material.clear();
  skeleton.clear();
  animation.clear();
  body.clear();

  while (1) {
    char str[255];
    LoadString(f, str);
    printf("Read \"%s\"\n",str);
    if (!strcmp(str,"end")) break;
    int k;
    LoadInt(f, k);
    if (!strcmp(str,"mesh")) {
      for (int i=0; i<k; i++) {
        BrfMesh m(f, verbose);
        printf("  loading \"%s\"\n",m.name);
        mesh.push_back( m );
      }
    }
    else if (!strcmp(str,"texture")) {
      for (int i=0; i<k; i++) {
        BrfTexture s;
        s.Load(f, verbose);
        printf("  loading \"%s\"\n",s.name);
        texture.push_back( s );
      }
    }
    else if (!strcmp(str,"shader")) {
      for (int i=0; i<k; i++) {
        BrfShader s; 
        s.Load(f, verbose);
        printf("  loading \"%s\"\n",s.name);
        shader.push_back( s );
      }
    }
    else if (!strcmp(str,"material")) {
      for (int i=0; i<k; i++) {
        BrfMaterial s;
        s.Load(f, verbose);
        printf("  loading \"%s\"\n",s.name);
        material.push_back( s );
      }
    }
    else if (!strcmp(str,"skeleton")) {
      for (int i=0; i<k; i++) {
        BrfSkeleton s;
        s.Load(f, verbose);
        printf("  loading \"%s\"\n",s.name);
        skeleton.push_back( s );
      }
    }
    else if (!strcmp(str,"skeleton_anim")) {
      return true;
      for (int i=0; i<k; i++) {
        BrfAnimation s;
        s.Load(f, verbose);
        printf("  loading \"%s\"\n",s.name);
        animation.push_back( s );
        return true;
      }
    }
    else if (!strcmp(str,"body")) {
      for (int i=0; i<k; i++) {
        BrfBody s;
        s.Load(f, verbose);
        printf("  loading \"%s\"\n",s.name);
        body.push_back( s );
      }
    }
    else {
      printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      return false;
    }
  }
  return true;

}
