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


extern char * tokenBrfName[N_TOKEN];

#endif // BRFTOKEN_H
