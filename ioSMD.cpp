#include "brfData.h"

#include "ioSMD.h"

#include "vcg\math\quaternion.h"

// used to export/import skeletons, rigged meshes, animations...
// return 0 on success. +... on error. -... on warnings

static float* matrix2euler(Matrix44f m){
  static float res[3];
  float f[16]={1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1};
  Matrix44f inv(f);
  m = inv*m*inv;
  //m.ToEulerAngles(res[0], res[1], res[2]);

  static vcg::Quaternionf q;
  q.FromMatrix(m);
  q.ToEulerAngles( res[0], res[1], res[2]);
  return res;
}

static Matrix44f euler2matrix(float* eul){
  Matrix44f m;
  m.FromEulerAngles(eul[0], eul[1], eul[2]);
  float f[16]={1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1};
  Matrix44f inv(f);
  m = inv*m*inv;
  return m;
}

static int lastErr;
char *expectedErr, *foundErr;
int versionErr;

static bool expect(FILE* f, char* what){
  static char str[255];
  fscanf(f, "%s", str);
  if (strcmp(str,what)){
    expectedErr = what;
    foundErr = str;
    lastErr = 4;
    return false;
  }
  return true;
}

static bool fscanln(FILE*f, char *ln){
  int i=0;
  while (1) {
    fread(&ln[i],1,1,f);
    if (ln[i]=='\n') { ln[i]=0; return true;}
    i++;
  }
}

static bool ioSMD_ImportTriangles(FILE*f, BrfMesh &m ){
  if (!expect(f,"triangles")) return false;
  int pi=0;
  m.frame.resize(1);
  m.frame[0].time=0;

  while (1){
    char matName[255];
    fscanf(f,"%s\n", matName);
    if (strcmp(matName,"end")==0) break;
    for (int w=0; w<3; w++) {
      int bi;
      Point3f p;
      Point3f n;
      Point2f t;
      char line[512];
      BrfRigging r;
      int nr=0;
      fscanln(f, line);
      int nread =
      sscanf(line,"%d %f %f %f %f %f %f %f %f %d %d %f %d %f %d %f %d %f",
        &bi,
        &(p[0]),&(p[2]),&(p[1]),
        &(n[0]),&(n[2]),&(n[1]),
        &(t[0]),&(t[1]),
        &nr,
        &(r.boneIndex[0]), &(r.boneWeight[0]),
        &(r.boneIndex[1]), &(r.boneWeight[1]),
        &(r.boneIndex[2]), &(r.boneWeight[2]),
        &(r.boneIndex[3]), &(r.boneWeight[3])
      );
      assert ( nread==9 || nread == 9+1+nr*2);
      for (int k = nr; k<4; k++) {
        r.boneIndex[k]=-1; r.boneWeight[k]=0;
      }
      float sumW = 0;
      for (int k = 0; k<4; k++) sumW += r.boneWeight[k];
      if (sumW<0.999999) {
        if (nr<4) {
          r.boneIndex[nr] = bi;
          r.boneWeight[nr] = 1-sumW;
        }
      }

      t[1]=1-t[1];
      BrfVert v;
      v.index=pi;
      v.ta = v.tb = t;
      v.__norm = n;
      v.col=0xffffffff; // white

      m.vert.push_back(v);
      m.frame[0].pos.push_back(p);
      m.frame[0].norm.push_back(n);

      m.rigging.push_back(r);
      pi++;
    }
    m.face.push_back( BrfFace( pi-3, pi-2, pi-1 ) );
  }
  return true;
}

static void ioSMD_ExportTriangles(FILE*f,const BrfMesh &m , int fi){
  fprintf(f,"triangles\n");
  assert(m.rigging.size()==m.frame[fi].pos.size());
  for (unsigned int i=0; i<m.face.size(); i++){
    fprintf(f,"material_name\n");
    for (int w=0; w<3; w++){
      int vi = m.face[i].index[w]; // vertex index
      int pi = m.vert[vi].index; // position index
      fprintf(f,"%d\t%f %f %f\t%f %f %f\t%f %f",
        m.rigging[pi].boneIndex[0],
        m.frame[fi].pos[pi][0],
        m.frame[fi].pos[pi][2],
        m.frame[fi].pos[pi][1],
        m.frame[fi].norm[vi][0],
        m.frame[fi].norm[vi][2],
        m.frame[fi].norm[vi][1],
        m.vert[vi].ta[0],
        1.0f-m.vert[vi].ta[1]
      );
      int nrig=0;
      for (int j=0; j<4; j++) if (m.rigging[pi].boneIndex[j]!=-1) nrig++;
      if (nrig>1) {
        fprintf(f,"\t%d",nrig); // number of links except 1st one
        for (int j=0; j<nrig; j++) {
          fprintf(f," %d %f", m.rigging[pi].boneIndex[j], m.rigging[pi].boneWeight[j] );
        }
      }
      fprintf(f,"\n");

    }
  }

  fprintf(f,"end\n");
}

static bool ioSMD_ImportBoneStruct(FILE*f,BrfSkeleton &s ){
  int v=-1;
  if (!expect(f,"version")) return false;
  fscanf(f, "%d\n",&v);
  if (!expect(f,"nodes")) return false;
  if (v!=1) { versionErr = v; lastErr=3; return false;}
  while (1) {
    int a, b;
    char st[255];
    int res= fscanf(f,"%d \"%s %d\n",&a, st, &b);
    if (res<3) {
      if (!expect(f,"end")) return false;
      break;
    }
    assert(st[strlen(st)-1]=='"');
    st[strlen(st)-1]=0;
    if (a<=(int)s.bone.size()) s.bone.resize(a+1);
    s.bone[a].attach=b;
    sprintf(s.bone[a].name,"%s",st);
  }
  s.BuildTree();
  return true;
}

static void ioSMD_ExportBoneStruct(FILE*f,const BrfSkeleton &s ){
  fprintf(f,"version 1\n"); // header

  fprintf(f,"nodes\n");
  for (unsigned int i=0; i<s.bone.size(); i++)
    fprintf(f,"%d \"%s\" %d\n", i, s.bone[i].name, s.bone[i].attach);
  fprintf(f,"end\n");
}

template <class T>
static void ioSMD_ExportPose(FILE* f, const BrfSkeleton &s,  const T& pose, int time){
  fprintf(f,"time %d\n",time);
  for (unsigned int i=0; i<s.bone.size(); i++) {
    float* euler=matrix2euler( pose.getRotationMatrix(i) );
    fprintf(f,"%d\t\%f %f %f\t%f %f %f\n",
            i,
            s.bone[i].t[0], s.bone[i].t[2], s.bone[i].t[1],
            euler[0], euler[1],euler[2]);
  }
}

template <class T>
static bool ioSMD_ImportPose(FILE* f, BrfSkeleton &s,  T& pose, int &time){
  if (!expect(f,"time")) return false;

  fscanf(f,"%d",&time);
  //for (static int i=0;i<s.bone.size(); i++)
  //  s.bone[i].
  while (1) {
    int i;
    int res = fscanf(f,"%d",&i);
    if (res==0) break; // opefully it is an "end"
    assert(i<(int)s.bone.size());
    float r[3];
    vcg::Point3f t;
    fscanf(f,"%f %f %f %f %f %f", &(t[0]),&(t[2]),&(t[1]), r+0, r+1, r+2);
    s.bone[i].t = t;
    pose.setRotationMatrix( euler2matrix(r) , i );
  }
  return true;
}

int ioSMD::Export(const char*filename, const BrfMesh &m , const BrfSkeleton &s, int fi){
  FILE* f=fopen(filename,"wt");
  lastErr = 0;
  if (!f) return(lastErr=2);

  ioSMD_ExportBoneStruct(f,s);

  fprintf(f,"skeleton\n");
  ioSMD_ExportPose(f,s,s,0); // one pose: skeleton pose
  fprintf(f,"end\n");
  ioSMD_ExportTriangles(f,m,fi);
  fclose(f);

  return lastErr;
}


int ioSMD::Export(const char*filename, const BrfAnimation &a, const BrfSkeleton &s){

  FILE* f=fopen(filename,"wt");
  lastErr = 0;
  if (!f) return(lastErr=2);

  ioSMD_ExportBoneStruct(f,s);


  fprintf(f,"skeleton\n");
  ioSMD_ExportPose(f,s,s,0); // one pose: skeleton pose
  for (unsigned int i=0; i<a.frame.size(); i++) {
    BrfSkeleton s0 = s;
    s0.bone[ s0.root ].t+=a.frame[i].tra;

    ioSMD_ExportPose(f,s0,a.frame[i], a.frame[i].index+1); // other poses (at index+1, to leave 0 for basic pose)
  }
  fprintf(f,"end\n");

  fclose(f);

  return lastErr;
}


int ioSMD::Import(const char*filename, BrfMesh &m , BrfSkeleton &s){
  lastErr = 0;
  FILE* f=fopen(filename,"rt");
  if (!f) return(lastErr=1);

  if (!ioSMD_ImportBoneStruct(f,s)) return lastErr;

  if (!expect(f,"skeleton")) return false;
  int time;
  if (!ioSMD_ImportPose(f,s,s,time)) return lastErr;
  if (!expect(f,"end")) return false;

  if (!ioSMD_ImportTriangles(f,m)) return false;

  m.UnifyPos();
  m.UnifyVert();
  m.AfterLoad();
  m.material[0]=0;
  m.flags=0;
  m.maxBone=s.bone.size();

  m.isRigged = true;

  fclose(f);
  return lastErr;
}

int ioSMD::Import(const char*filename, BrfAnimation &a, BrfSkeleton &s){

  lastErr = 0;
  FILE* f=fopen(filename,"rt");
  if (!f) return(lastErr=1);

  if (!ioSMD_ImportBoneStruct(f,s)) return lastErr;

  if (!expect(f,"skeleton")) return false;
  int time;
  if (!ioSMD_ImportPose(f,s,s,time)) return lastErr; // initial pose
  a.nbones = s.bone.size();
  BrfAnimationFrame af;
  af.rot.resize(a.nbones);
  af.tra = vcg::Point3f(0,0,0);
  while (1) {
    BrfSkeleton s0 = s;
    if (!ioSMD_ImportPose(f,s0,af,af.index)) break;
    af.tra = s0.bone[ s.root ].t -s.bone[ s.root ].t;
    af.index--;
    a.frame.push_back(af);
  }
  expectedErr="end";
  if (strcmp(foundErr,expectedErr)==0) lastErr=0; else return false;

  fclose(f);
  return lastErr;
}


char* ioSMD::LastErrorString(){
  static char res[255];
  switch(lastErr) {
  case 1: return "File not found"; break;
  case 2: return "Cannot open file for writing"; break;
  case 3: sprintf(res,"Version %d no supported?",versionErr); return res; break;
  case 4: sprintf(res,"Expected '%s' found '%s'",expectedErr, foundErr); return res; break;
  case 0: return "(no error)"; break;
  default: return "undocumented error"; break;
  }
}

