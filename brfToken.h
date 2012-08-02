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

typedef enum{
  NONE_FL = 0,
  MESH_FL = 1<<MESH,
  TEXTURE_FL = 1<<TEXTURE,
  SHADER_FL = 1<<SHADER,
  MATERIAL_FL = 1<<MATERIAL,
  SKELETON_FL = 1<<SKELETON,
  ANIMATION_FL = 1<<ANIMATION,
  BODY_FL = 1<<BODY,
  ALL_FL = (1<<N_TOKEN) - 1,
} TokenFlag;

extern char * tokenTabName[N_TOKEN];
extern char * tokenBrfName[N_TOKEN];
extern char * tokenFullName[N_TOKEN];

#endif // BRFTOKEN_H
