#ifndef BRFTOKEN_H
#define BRFTOKEN_H

typedef enum{
  MESH,
  TEXTURE,
  SHADER,
  MATERIAL,
  SKELETON,
  ANIMATION,
  BODY,
  N_TOKEN,
  NONE
} TokenEnum;

// used to identify an object (a mesh, or a texture, or...) in a iniFile
class ObjCoord{
public:
  ObjCoord(int _fi,int _oi, int  _t):fi(_fi),oi(_oi),t(_t){}
  ObjCoord():fi(0),oi(0),t(NONE){}
  static ObjCoord Invalid(){return ObjCoord(-1,-1,NONE);}
  bool isValid() const {return fi!=-1;}
  int fi; // file index
  int oi; // object index inside that file
  int  t; // token index
};

typedef enum{
  TXTFILE_ACTIONS,
  TXTFILE_SKIN,
  TXTFILE_ITEM,
  TXTFILE_MESHES,
  TXTFILE_ICONS,
  TXTFILE_PROP,
  TXTFILE_PARTICLE,
  TXTFILE_TABLEAU,
  TXTFILE_FLORA_KINDS,
  TXTFILE_GROUND_SPECS,
  TXTFILE_SKYBOXES,
  TXTFILE_SCENES,
  N_TXTFILES,
  TXTFILE_CORE,
  TXTFILE_NONE,
} FileTxtEnum;

inline static unsigned int bitMask(int txt){return 1<<txt;}
extern char * txtFileName[N_TXTFILES];


extern char * tokenBrfName[N_TOKEN];

#endif // BRFTOKEN_H
