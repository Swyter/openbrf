#include <vector>
#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;

#include "brfBody.h"

#include "saveLoad.h"


BrfBody::BrfBody()
{
}

float* BrfBodyPart::GetRotMatrix() const{
  vcg::Point3f dx, dy, dz(1,1.1,1.3);
  dx = (dir-center)/2.0;
  dy = dz ^ dx;
  dz = dx ^ dy;
  dy.Normalize();
  dz.Normalize();
  static float res[16];
  for (int i=0; i<4; i++) {
    res[i]    = (i==3)?0:dx[i];
    res[i+4]  = (i==3)?0:dy[i];
    res[i+8]  = (i==3)?0:dz[i];
    res[i+12] = (i==3)?1:0;
  }
  return res;
}

const char* BrfBodyPart::name() const{
  switch(type) {
  case MANIFOLD: return "manifold";
  case CAPSULE: return "cylinder";
  case SPHERE: return "sphere";
  case FACE:return "polygon";
  default: return "unknown";
  }

}

static void _addHypotesis(Point3f &a, Point3f b, Point3f c){
  float b0 = a.normalized()*b;
  float c0 = a.normalized()*c;
  if (fabs(b0)>fabs(c0)){
    a+=(b0>0)?b:-b;
  } else {
    a+=(c0>0)?c:-c;
  }
}

Point3f  BrfBodyPart::Baricenter() const{
  Point3f res(0,0,0);
  for (unsigned int i=0; i<pos.size(); i++) res+=pos[i];
  if (pos.size()>0) res/=pos.size();
  return res;
}

void BrfBodyPart::GuessFromManyfold(){
  switch(type) {
    case SPHERE: {
      center = Baricenter();
      radius = 0;
      for (unsigned int i=0; i<pos.size(); i++){
        float dist = (center-pos[i]).SquaredNorm();
        if (radius<dist) radius = dist;
      }
      radius = (float)sqrt(radius);
      face.clear();
      pos.clear();
      break;
    }
    case FACE: {
      // keep only one face
      face.resize(1);
      std::vector<Point3f> v = pos;
      pos.resize(face[0].size());
      for (int i=0; i<(int)face[0].size(); i++) {
        pos[i]=v[ face[0][i] ];
        face[0][i]=i;
      }
      break;
    }
    case CAPSULE: {
      // guess an axis looking at quadrilaterals
      Point3f ip0(0,0,0), ip1(0,0,0); // axis hipotesis
      if (face.size()==0) break;
      bool first=true;

      for (unsigned int i=0; i<face.size(); i++) if (face[i].size()==4) {
        int a = face[i][0];
        int b = face[i][1];
        int c = face[i][2];
        //int d = face[i][3];
        Point3f ipA = (pos[a]-pos[b]).Normalize();
        Point3f ipB = (pos[b]-pos[c]).Normalize();
        if (first) {
          ip0 = ipA;
          ip1 = ipB;
        } else {
          _addHypotesis(ip0, ipA, ipB);
          _addHypotesis(ip1, ipA, ipB);
        }
      }
      Point3f axis = (ip0.SquaredNorm()>ip1.SquaredNorm())?ip0:ip1;
      axis.Normalize();
      Point3f ce = Baricenter();
      radius = 0;
      float min=0; float max=0;
      for (unsigned int i=0; i<pos.size(); i++){
        float d = (pos[i]-ce)*axis;
        if (d<min) min = d;
        if (d>max) max = d;
        float dist = ((ce+axis*d)-pos[i]).SquaredNorm();
        if (radius<dist) radius = dist;
      }
      radius=(float)sqrt(radius);
      center = ce+axis*max;
      dir = ce+axis*min;
      face.clear();
      pos.clear();
      break;
    }
    default:
      break;
  }
  flags=0;
  this->ori=-1;
}

static bool fscanln(FILE*f, char *ln){
  int i=0;
  while (1) {
    fread(&ln[i],1,1,f);
    if (ln[i]=='\n') { ln[i]=0; return true;}
    i++;
  }
}

bool BrfBody::ImportOBJ(char *fn){

  BrfBodyPart dump, curr;
  dump.flags=0;
  dump.ori=-1;
  dump.type = BrfBodyPart::MANIFOLD;

  int startV = 0; // starting v

  // to do: read all v and f fields, looking for "o" (objects)
  FILE* f = fopen(fn,"rt");
  if (!f) return false;

  std::string s;
  char line[255];
  bool reading = false;
  while (fscanln(f, line)) {
    if (line[1]==' ') {
      char c=line[0];
      switch (c){
        case 'v':{
          Point3f p;
          sscanf(line, "v %f %f %f",&(p[0]),&(p[1]),&(p[2]));
          if (reading) {
            curr.pos.push_back(p);
          } else {
            dump.pos.push_back(p);
          }
          break;
        }
        case 'f':{
          static char buf[20][20];
          int res = sscanf(line, "f %s %s %s %s %s %s %s %s",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7]);
          std::vector<int> fac;
          for (int i=0; i<res; i++){
            int d=0;
            sscanf(buf[i],"%d",&d);
            fac.push_back(d+startV-1);
          }
          if (reading) {
            curr.face.push_back(fac);
          } else {
            dump.face.push_back(fac);
          }
          break;
        }
        case 'o':
          char str[255];
          sscanf(line, "o %s", str);
          /*if (reading) {
            if (curr.face.size()>0) part.push_back(curr);
            curr.face.clear();
            curr.pos.clear();
            curr.InferTypeFromString(name);
          }*/
          //reading =
          break;
      }
    }
  }


  //for (unsigned int i=0; i<part.size(); i++) part[i].GuessFromManyfold();
  if (dump.face.size()) part.push_back(dump);
  UpdateBBox();
  return true;
}

void BrfBodyPart::UpdateBBox(){
  bbox.SetNull();
  vcg::Point3f r(radius, radius, radius);
  if (type==MANIFOLD || type==FACE) {
    for (unsigned int i=0; i<pos.size(); i++) {
      bbox.Add(pos[i]);
    }
  }
  if (type==SPHERE) {
    bbox.Add(center+r);
    bbox.Add(center-r);
  }
  if (type==CAPSULE) {
    bbox.Add(center+r);
    bbox.Add(center-r);
    bbox.Add(dir+r);
    bbox.Add(dir-r);
  }
}

void BrfBody::UpdateBBox(){
  bbox.SetNull();
  for (unsigned int i=0; i<part.size(); i++) {
    bbox.Add(part[i].bbox);
  }
}



bool BrfBodyPart::Load(FILE*f, char* _firstWord, int verbose ){
  char firstWord[255];
  static FILE* fff = fopen("flags.txt","wt");
  if (!_firstWord) LoadString(f,firstWord);
  else sprintf(firstWord,_firstWord);

  //static FILE* ftmp=NULL; //if (!ftmp) ftmp=fopen("debug.txt","wt");

  if (!strcmp(firstWord,"manifold")) {
    type=MANIFOLD;
    LoadVector(f, pos);// # points

    int k;
    LoadInt(f,k); // # faces
    for (int i=0; i<k; i++) {
      int h;
      LoadInt(f,ori); // orientation? -1 or 1 apparently.
      assert(ori==1 || ori==-1);
      LoadInt(f,h);
      assert(h==0);
      LoadInt(f,h); // # verts
      std::vector<int> v;
      for (int j=0; j<h; j++) {
        int pp;
        LoadInt(f,pp);
        v.push_back(pp);
      }
      face.push_back(v);
    }
  } else if (!strcmp(firstWord,"capsule")) {
    type=CAPSULE;
    LoadFloat(f,radius);
    LoadPoint(f,center);
    LoadPoint(f,dir);
    LoadUint(f,flags);
    fprintf(fff,"Flags cil: %u\n",flags);
    //fprintf(ftmp,"%f %f %f %f %f %f %f\n",radius,center[0],center[2],center[1],dir[0],dir[2],dir[1]);
    //fflush(ftmp);
  } else if (!strcmp(firstWord,"sphere")) {
    type=SPHERE;
    LoadFloat(f,radius);
    LoadPoint(f,center);
    LoadUint(f,flags);
    fprintf(fff,"Flags sphere: %u\n",flags);
  } else if (!strcmp(firstWord,"face")) {
    type=FACE;
    LoadVector(f,pos);

    int k = pos.size();

    // default face
    std::vector<int> aface;
    for (int i=0; i<k; i++) aface.push_back(i);
    face.push_back(aface);

    LoadUint(f,flags);
    fprintf(fff,"Flags face: %u\n",flags);

  } else {
    printf("Unknown body (collision mesh) type `%s`\n",firstWord);
    assert(0);
  }

  UpdateBBox();
  return true;
}


void BrfBodyPart::Save(FILE *f) const {
  switch(type) {
  case MANIFOLD:
      SaveString(f,"manifold");
      SaveVector(f,pos);

      SaveUint(f,face.size());
      for (unsigned int i=0; i<face.size(); i++) {
        SaveInt(f,ori);
        SaveInt(f,0);
        SaveUint(f,face[i].size());
        for (unsigned int j=0; j<face[i].size(); j++) {
          SaveInt(f,face[i][j]);
        }
      }
      break;
  case CAPSULE:
      SaveString(f,"capsule");
      SaveFloat(f,radius);
      SavePoint(f,center);
      SavePoint(f,dir);
      SaveUint(f,flags);
      break;
  case SPHERE:
      SaveString(f,"sphere");
      SaveFloat(f,radius);
      SavePoint(f,center);
      SaveUint(f,flags);
      break;
  case FACE:
      SaveString(f,"face");
      assert(face.size()==1);
      SaveVector(f,pos);
      SaveUint(f,flags);
      break;
  default: assert(0);
  }
}

void BrfBody::Save(FILE*f) const{
  SaveString(f,name);
  if (part.size()==1) {
    part[0].Save(f);
  } else {
    SaveString(f,"composite");
    SaveVector(f,part);
  }
}

bool BrfBodyPart::ExportOBJ(FILE* f, int i, int &vc) const{
  fprintf(f,"o %s_%d\n",name(),i);
  switch (type) {
  default:
    break;
  case MANIFOLD:
  case FACE:
    for (unsigned int i=0; i<pos.size(); i++) {
      fprintf(f,"v %f %f %f\n",pos[i].X(),pos[i].Y(),pos[i].Z());
    }
    for (unsigned int i=0; i<face.size(); i++) {
      fprintf(f,"f ");
      for (unsigned int j=0; j<face[i].size(); j++)
      fprintf(f,"%d// ",vc+face[i][face[i].size()-1-j]);
      fprintf(f,"\n");
    }
    vc+=pos.size();
    break;
  case CAPSULE:{
    vcg::Point3f a = this->center;
    vcg::Point3f b = this->dir;

    vcg::Point3f dx, dy, dz(1,1.1,1.3);
    dx = (a-b)/2.0;
    dy = dz ^ dx;
    dz = dx ^ dy;
    dy.Normalize();
    dz.Normalize();
    int N=10;
    for (int i=0; i<N; i++) {
      float ca = (float)cos(M_PI*2*i/N)*radius;
      float sa = (float)sin(M_PI*2*i/N)*radius;
      vcg::Point3f p;
      p = a+dy*sa+dz*ca;
      fprintf(f,"v %f %f %f\n",p.X(), p.Y(), p.Z());
      p = b+dy*sa+dz*ca;
      fprintf(f,"v %f %f %f\n",p.X(), p.Y(), p.Z());
    }
    fprintf(f,"v %f %f %f\n",a.X(), a.Y(), a.Z());
    fprintf(f,"v %f %f %f\n",b.X(), b.Y(), b.Z());

    for (int h=0; h<N; h++) {
      int i = h*2;
      int j = ((h+1)%N)*2;
      fprintf(f,"f %d// %d// %d//\n", vc+N*2, vc+j, vc+i );
      fprintf(f,"f %d// %d// %d// %d//\n",  vc+j, vc+j+1, vc+i+1, vc+i );
      fprintf(f,"f %d// %d// %d//\n", vc+N*2+1, vc+i+1, vc+j+1 );
    }
    vc+=2*N+2;
    break;
  }
  case SPHERE:{
    int N=10;
    int M=5;

    Point3f p;
    for (int j=0; j<M; j++)
    for (int i=0; i<N; i++) {
      p.FromPolar(radius,M_PI*2*i/N,M_PI*(j+1)/(M+1)-M_PI/2); p+= center;
      fprintf(f,"v %f %f %f\n",p.X(), p.Y(), p.Z());
    }
    p.FromPolar(radius,0,-M_PI/2); p+= center;
    fprintf(f,"v %f %f %f\n",p.X(), p.Y(), p.Z());
    p.FromPolar(radius,0,+M_PI/2); p+= center;
    fprintf(f,"v %f %f %f\n",p.X(), p.Y(), p.Z());

    for (int i=0; i<N; i++) {
      int ip = (i+1)%N;
      fprintf(f,"f %d// %d// %d//\n", vc+N*M, vc+i, vc+ip );
      for (int j=0; j<M-1; j++)
        fprintf(f,"f %d// %d// %d// %d//\n",  vc+ip+j*N, vc+i+j*N, vc+i+j*N+N, vc+ip+j*N+N );
      fprintf(f,"f %d// %d// %d//\n", vc+N*M+1, vc+ip+(M-1)*N, vc+i+(M-1)*N );
    }
    vc+=N*M+2;
    break;
  }
  }
  return true;
}

bool BrfBody::ExportOBJ(char* fn) const {
  FILE* f =fopen(fn,"wb");
  if (!f) return false;
  fprintf(f,
    "# export of a body (Mount and Blade collision object)\n"
    "# by OpenBrf (Marco Tarini)\n");
  int vc=1; // vertex count
  for (unsigned int i=0; i<part.size(); i++)
    part[i].ExportOBJ(f,i,vc);
  fclose(f);
  return true;
}
bool BrfBody::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);

  char str[255];
  LoadString(f, str);

  if (!strcmp(str,"composite")) {
    LoadVector(f, part);
  } else {
    BrfBodyPart b;
    b.Load(f,str,verbose);
    part.push_back(b);
  }
  UpdateBBox();

  return true;
}
