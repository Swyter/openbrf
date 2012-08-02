#include <vcg/space/point4.h>

// small helper functions for saving/loading stuff into a brf file

void SaveString(FILE *f, const char *st);
void SaveStringNotempty(FILE *f, const char *st, const char *ifnot);
void SaveInt(FILE *f, int i);
void SaveFloat(FILE *f, float f);
void SaveUint(FILE *f, unsigned int x);
void SavePoint(FILE *f, Point4f p);
void SavePoint(FILE *f, Point3f p);
void SavePoint(FILE *f, Point2f p);




void LoadString(FILE *f, char *st);
bool LoadStringMaybe(FILE *f, char *st, const char *ifnot); // if it does not look like a string, uses ifnot
void LoadInt(FILE *f, int &i);
void LoadFloat(FILE *f, float &f);
void LoadUint(FILE *f, unsigned int &x);
void LoadPoint(FILE *f, Point4f &p);
void LoadPoint(FILE *f, Point3f &p);
void LoadPoint(FILE *f, Point2f &p);

void LoadVector(FILE *f,std::vector<int> &);
void LoadVector(FILE *f,std::vector<Point3f> &);
void SaveVector(FILE *f,const std::vector<int> &);
void SaveVector(FILE *f,const std::vector<Point3f> &);

template<class T> void SaveVector(FILE *f,const std::vector< std::vector<T> > &v){
  SaveUint(f,v.size());
  for (unsigned int i=0; i<v.size(); i++) SaveVector(f,v[i]);
}
template<class T> void SaveVector(FILE *f,const std::vector<T> &v){
  SaveUint(f,v.size());
  for (unsigned int i=0; i<v.size(); i++) v[i].Save(f);
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


template<class T>
void Skip(FILE *f){
  fseek(f,sizeof(T), SEEK_CUR);
}

void SkipString(FILE *f);

void Skip(FILE *f, int k);

template<class T> void SkipVector(FILE *f, std::vector<T> &v){
  unsigned int k;
  LoadUint(f,k);
  v.resize(k);
  for (unsigned int i=0; i<k; i++) v[i].Skip(f);
}

// skip of a vector, Recursive: when objects in vector have a skip member
template<class T> void SkipVectorR(FILE *f){
  unsigned int k;
  LoadUint(f,k);
  for (unsigned int i=0; i<k; i++) T::Skip(f);

}

// direct skip of a vector, when objects are fixed size
template<class T> void SkipVectorF(FILE *f){
  unsigned int k;
  LoadUint(f,k);
  fseek(f,k*T::SizeOnDisk(), SEEK_CUR);
}

// direct skip of a vector of with BASE type
template<class T> void SkipVectorB(FILE *f){
  unsigned int k;
  LoadUint(f,k);
  fseek(f,k*sizeof(T), SEEK_CUR);
}
