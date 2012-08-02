#include <stdio.h>
#include <strings.h>

#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

void SaveString(FILE *f, const char *st){
  int x=strlen(st);
  fwrite(&x, 4, 1,  f);
  fwrite(st, 1, x, f);
}

void SaveInt(FILE *f, int x){
  fwrite(&x, 4, 1,  f);
}

void SaveUint(FILE *f, unsigned int x){
  fwrite(&x, 4, 1,  f);
}

void SaveFloat(FILE *f, float x){
  fwrite(&x, 4, 1,  f);
}

void SavePoint(FILE *f, vcg::Point3f p) {
  SaveFloat(f,  p.X() ); 
  SaveFloat(f,  p.Z() );
  SaveFloat(f,  p.Y() ); 
}

void SavePoint(FILE *f, vcg::Point2f p) {
  SaveFloat(f,  p.X() ); 
  SaveFloat(f,  p.Y() ); 
}

void LoadString(FILE *f, char *st){
  int x;
  fread(&x, 4, 1,  f);
  
  fread(st, 1, x, f);
  st[x]=0;
  
 //printf("\"%s\"...\n",st);
}

void LoadInt(FILE *f, int &i){
  fread(&i, 4, 1,  f);
  //printf("%d ",i);
}

void LoadFloat(FILE *f, float &x){
  fread(&x, 4, 1,  f);
}

void LoadUint(FILE *f, unsigned int &x){
  fread(&x, 4, 1,  f);
  //printf("%ud ",x);
}

void LoadPoint(FILE *f, vcg::Point3f &p){
  LoadFloat(f,  p.X() ); 
  LoadFloat(f,  p.Z() );
  LoadFloat(f,  p.Y() ); 
  
  //printf("(%f,%f,%f) ",p[0],p[1],p[2]);
}

void LoadPoint(FILE *f, vcg::Point2f &p){
  LoadFloat(f,  p.X() ); 
  LoadFloat(f,  p.Y() ); 
  //printf("(%f,%f) ",p[0],p[1]);
}
