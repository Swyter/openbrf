#include <stdio.h>
#include <strings.h>
#include <vector>

#include <vcg/space/point4.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

using namespace vcg;

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

void SavePoint(FILE *f, vcg::Point4f p) {
  SaveFloat(f,  p.X() );
  SaveFloat(f,  p.Y() );
  SaveFloat(f,  p.Z() );
  SaveFloat(f,  p.W() );
}


void SaveVector(FILE *f,const std::vector<int> & v){
  SaveUint(f,v.size());
  for (unsigned int i=0; i<v.size(); i++) SaveInt(f,v[i]);
}

void SaveVector(FILE *f,const std::vector<Point3f> &v){
  SaveUint(f,v.size());
  for (unsigned int i=0; i<v.size(); i++) SavePoint(f,v[i]);
}



void LoadString(FILE *f, char *st){
  int x;
  fread(&x, 4, 1,  f);
  assert(x<256);

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

void LoadPoint(FILE *f, vcg::Point4f &p){
  LoadFloat(f,  p.X() );
  LoadFloat(f,  p.Y() );
  LoadFloat(f,  p.Z() );
  LoadFloat(f,  p.W() );
}


void LoadPoint(FILE *f, vcg::Point2f &p){
  LoadFloat(f,  p.X() ); 
  LoadFloat(f,  p.Y() ); 
  //printf("(%f,%f) ",p[0],p[1]);
}

void LoadVector(FILE *f,std::vector<int> &v){
  unsigned int k;
  LoadUint(f,k);
  v.resize(k);
  for (unsigned int i=0; i<v.size(); i++) LoadInt(f,v[i]);
}

void LoadVector(FILE *f,std::vector<Point3f> &v){
  unsigned int k;
  LoadUint(f,k);
  v.resize(k);
  for (unsigned int i=0; i<v.size(); i++) LoadPoint(f,v[i]);
}

template<class T> void LoadVector(FILE *f,std::vector< std::vector<T> > &v){
  unsigned int k;
  LoadUint(f,k);
  v.resize(k);
  for (unsigned int i=0; i<v.size(); i++) LoadVector(f,v[i]);
}

template<class T> void LoadVector(FILE *f,std::vector<T, std::allocator<T> > &v){
  unsigned int k;
  LoadUint(f,k);
  v.resize(k);
  for (unsigned int i=0; i<v.size(); i++) v[i].Load(f);
}


