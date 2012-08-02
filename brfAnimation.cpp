#include <vector>
#include <vcg/space/box3.h>
#include <vcg/math/quaternion.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "saveLoad.h"

#include "brfSkeleton.h"
#include "brfAnimation.h"


Matrix44f BrfAnimationFrame::getRotationMatrix(int i) const{
  vcg::Matrix44f res;
  vcg::Quaternionf qua = rot[i];
  qua.ToMatrix(res);
  //res.transposeInPlace();
  return res;
  /*

  float dva[] = {-1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1};
  vcg::Matrix44f swapYZ(dva);
  float dvb[] = {-1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1};
  vcg::Matrix44f invertXY(dvb);

  vcg::Point4f p = rot[i];
  vcg::Quaternionf qua(p[1],p[2],p[3],p[0]);
  qua.Normalize();
  qua.Invert();
  vcg::Matrix44f mat;

  qua.ToMatrix(mat);

  mat=invertXY*swapYZ*mat*swapYZ;
  //mat=invertXY*mat;

  mat = mat.transpose();
  return mat;
  */
}

void BrfAnimationFrame::setRotationMatrix(Matrix44f mat, int i){
  vcg::Quaternionf qua;
  qua.FromMatrix(mat);
  rot[i]=qua;
  return;
  /*
  float dva[] = {-1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1};
  vcg::Matrix44f swapYZ(dva);
  float dvb[] = {-1,0,0,0, 0,-1,0,0, 0,0,1,0, 0,0,0,1};
  vcg::Matrix44f invertXY(dvb);

  //mat = mat.transpose();
  mat=swapYZ*invertXY*mat*swapYZ;

  vcg::Quaternionf qua; qua.FromMatrix(mat);
  qua.Normalize();
  qua.Invert();
  rot[i]= Point4f(qua[3],qua[0],qua[1],qua[2]);
  */
}

Box3f BrfAnimation::bbox;

using namespace std;

// classes for saving loading bones and frames
template <class PointT>
class TmpCas{
public:
  static unsigned int SizeOnDisk(){
    return sizeof(PointT)+4;
  }
  void Save(FILE *f)const{
    SaveInt(f,findex);
    SavePoint(f,rot);
  }
  bool Load(FILE *f){
    LoadInt(f,findex);
    LoadPoint(f,rot);
    return true;
  }

  int findex; // frame index
  PointT rot;
};
typedef TmpCas< Point3f > TmpCas3;
typedef TmpCas< Point4f > TmpCas4;

class TmpBone4{
public:
  void Save(FILE *f)const{
    SaveVector(f,cas);
  }
  bool Load(FILE *f){
    return LoadVector(f,cas);
  }
  static bool Skip(FILE *f){
    SkipVectorF<TmpCas4>(f);
    return true;
  }
  void Adjust(){
    for (unsigned int i=0; i<cas.size(); i++)
      cas[i].rot = BrfSkeleton::adjustCoordSyst(cas[i].rot);
      /* cas[i].rot=vcg::Point4f(
        cas[i].rot[3],
       -cas[i].rot[0],
        cas[i].rot[1],
        cas[i].rot[2]
     );*/
  }
  void AdjustInv(){
    Adjust(); // it's its own inverse
     /*cas[i].rot=vcg::Point4f(
       -cas[i].rot[1],
        cas[i].rot[2],
        cas[i].rot[3],
        cas[i].rot[0]
     );*/
  }

  void AdjustRoot(){
    for (unsigned int i=0; i<cas.size(); i++)
      cas[i].rot = BrfSkeleton::adjustCoordSystHalf(cas[i].rot);
//    for (unsigned int i=0; i<cas.size(); i++) {
//      Quaternionf q = Quaternionf(0,0,1,1).Normalize();
//      cas[i].rot.Import( ((Quaternionf)cas[i].rot)*q );
//    }
  }
  void AdjustRootInv(){    
    AdjustRoot(); // it's its own inverse
    //for (unsigned int i=0; i<cas.size(); i++)
    // cas[i].rot = BrfSkeleton::adjustCoordSystHalfInv(cas[i].rot);
  }

  /*void Export(FILE* f){
    for (unsigned int i=0; i<cas.size(); i++) {
      fprintf(f,"%d,",cas[i].findex);
      if ((i+1)%10==0) fprintf(f,"\n     ");
    }
    fprintf(f,"\n");
  }*/
  vector< TmpCas4 > cas;
};


void TmpBone2BrfFrame(const vector<TmpBone4> &vb, const vector<TmpCas3> &vt,
                      vector<BrfAnimationFrame> &vf){

  int MAXFRAMES=0;

  for (unsigned int i=0; i<vb.size(); i++)
  for (unsigned int j=0; j<vb[i].cas.size(); j++) {
      int fi= vb[i].cas[j].findex +1;
      if (fi>MAXFRAMES) MAXFRAMES=fi;
  }
  for (unsigned int j=0; j<vt.size(); j++){
    int fi= vt[j].findex +1;
    if (fi>MAXFRAMES) MAXFRAMES=fi;
  }

  vector<int> present(MAXFRAMES, 0);

  // find and count frames that are acutally used
  int nf=0;
  for (unsigned int i=0; i<vb.size(); i++) {
    int ndup=1;
    for (unsigned int j=0; j<vb[i].cas.size(); j++){
      int fi= vb[i].cas[j].findex;
      if (j>0) {
        if (fi==vb[i].cas[j-1].findex) ndup++; else ndup=1;
      }
      if (present[fi]<ndup) {
        nf++;
        present[ fi ] = ndup;
      }
    }
  }
  // last round for translation
  {
    int ndup=1;
    for (unsigned int j=0; j<vt.size(); j++){
      int fi= vt[j].findex;
      if (j>0) {
        if (fi==vt[j-1].findex) ndup++; else ndup=1;
      }
      if (present[fi]<ndup) {
        nf++;
        present[ fi ] = ndup;
      }
    }
  }

  // allocate and fill frames
  vf.resize(nf);
  for (int i=0; i<nf; i++) {
    vf[i].rot.resize(vb.size());
    vf[i].wasImplicit.resize(vb.size() + 1, false); // +1 for the translation
  }

  for (unsigned int bi=0; bi<vb.size(); bi++) {
    int j=0, // pos in current vb
        m=0; // pos in vf
    for (int fi=0; fi<MAXFRAMES; fi++)
    for (int dupl=0; dupl<present[fi]; dupl++) // often zero times
    {
      int fi2;

      while ( (fi2= vb[bi].cas[j].findex) < fi) j++;
      if (dupl>0) { if (fi2==fi) { j++; fi2= vb[bi].cas[j].findex;} }

      vf[ m ].index = fi;
      if  (fi2==fi) {
        vf[m].rot[bi]=vb[bi].cas[j].rot;
        vf[m].wasImplicit[bi] = false;
      } else
      {
        // copy from prev
        assert (m>0);
        vf[m].rot[bi]=vf[m-1].rot[bi];
        vf[m].wasImplicit[bi] = true;
      }
      m++;
    }
    assert(m==nf);
  }
  {
    // last round for translation
    int j=0, // pos in current vb
        m=0; // pos in vf
    for (int fi=0; fi<MAXFRAMES; fi++)
    for (int dupl=0; dupl<present[fi]; dupl++) // often zero times
    {
      int fi2;
      while ( (fi2= vt[j].findex) < fi) j++;
      if (dupl>0) { if (fi2==fi) { j++; fi2= vt[j].findex;} }

      vf[ m ].index = fi;
      if  (fi2==fi) {
        vf[m].tra=vt[j].rot;
        vf[m].wasImplicit[vb.size()] = false;
      } else
      {
        // copy from prev
        assert (m>0);
        vf[m].tra=vf[m-1].tra;
        vf[m].wasImplicit[vb.size()] = true;
      }
      m++;
    }
    assert(m==nf);
  }
}

void BrfFrame2TmpBone(const vector<BrfAnimationFrame> &vf,
                      vector<TmpBone4> &vb, vector<TmpCas3> &vt){
  if (vf.size()==0) return;
  int nbones=vf[0].rot.size();
  vb.resize(nbones);
  for (unsigned int fi=0; fi<vf.size(); fi++) {
    int k = vf[fi].rot.size();
    assert(k==nbones);
    nbones=k;
    for (int bi=0; bi<nbones; bi++){
      TmpCas4 c;
      c.findex = vf[ fi ].index;
      c.rot = vf[ fi ].rot[bi];
      if ((!vf[ fi ].wasImplicit[bi]) || vb[bi].cas.size()==0) vb[bi].cas.push_back( c );
    }
    TmpCas3 c;
    c.findex = vf[ fi ].index;
    c.rot = vf[ fi ].tra;
    if ((!vf[ fi ].wasImplicit[nbones]) || vt.size()==0 ) vt.push_back(c);
  }
}

bool BrfAnimationFrame::Reskeletonize(const BrfSkeleton& from, const BrfSkeleton& to){


  float tmp[16]={-1,0,0,0, 0,0,1,0, 0,1,0,0, 0,0,0,1};
  Matrix44f t(tmp); // t goes from BRFedit style to OpenBrf style!
  /*
  Matrix44f t,tt,t0,t1;
  t0.SetIdentity();
  t0.SetColumn(0,from.bone[1].t);
  t0.SetColumn(1,from.bone[3].t);
  t0.SetColumn(2,from.bone[4].t);
  t0=Invert(t0);
  t1.SetIdentity();
  t1.SetColumn(0,to.bone[1].t);
  t1.SetColumn(1,to.bone[3].t);
  t1.SetColumn(2,to.bone[4].t);
  t1.transposeInPlace();
  t0.transposeInPlace();
  t = t0*t1;
  tt = t; tt.transposeInPlace();*/

  for (unsigned int i=0; i<rot.size(); i++) {
    Matrix44f m = this->getRotationMatrix(i);
    //m.transposeInPlace();
    /*
    Matrix44f debug;
    Quaternionf q0, q1, q2, q3, q4;
    q0 = this->rot[i];
    q1.FromMatrix(t);
    q3.FromMatrix(t*m*t);
    q4.FromMatrix(m);
    q2 = q1*q4*q1;

    Matrix44f m2, m3;
    q2.ToMatrix(m2); m2 -= t*m*t;
    q3.ToMatrix(m3); m3 -= t*m*t;
    */

    Matrix44f a = to.getRotationMatrix(i);
    Matrix44f b = from.getRotationMatrix(i);
    Matrix44f at = a;
    Matrix44f bt = b;
    b.transposeInPlace();
    a.transposeInPlace();

    //m = t*m * b*t * at ;
    m = m * b * at ;


    this->setRotationMatrix( m , i );
  }

  return true;
}

bool BrfAnimation::Reskeletonize(const BrfSkeleton& from, const BrfSkeleton& to){
  for (unsigned int i=0; i<frame.size(); i++)
    if (!frame[i].Reskeletonize(from, to)) return false;
  return true;
}

int BrfAnimation::FirstIndex() const{
  if (frame.size()==0) return 0;
  return frame[0].index;
}
int BrfAnimation::LastIndex() const{
  if (frame.size()==0) return 0;
  return frame[ frame.size()-1 ].index;
}

void BrfAnimation::ShiftIndexInterval(int d){
  for (unsigned int i=0; i<frame.size(); i++) frame[i].index+=d;
}

int BrfAnimation::Break(vector<BrfAnimation> &vect, char* fn, char *fn2) const{

  int res=0;
  FILE *fin=fopen(fn,"rt");
  //static char fn2[1024];
  sprintf(fn2,"%s [after splitting %s].txt",fn,name);
  FILE *fout=fopen(fn2,"wt");
  if (!fin) return -1;
  if (!fout) return -2;
  int n=0;
  fscanf(fin,"%d ",&n);
  fprintf(fout,"%d \n",n);

  char aniName[255];
  char aniSubName[255];
  unsigned int v00;
  int nparts;
  float speed;


  char* formatHeader = " %s %u  %d\n";
  char* formatAni = "  %f %s %d %d %u %u %f %f %f  %f \n";

  for (int i=0; i<n; i++){
    fscanf (fin, formatHeader,aniName, &v00, &nparts);
    fprintf(fout,formatHeader,aniName,  v00,  nparts);
    unsigned int v0; float v1, v2, v3, v4;
    int ia, ib; unsigned int flags;

    for (int j=0; j<nparts; j++) {

      BrfAnimation ani;
      //ani.flags=flags;
      ani.nbones=nbones;
      fscanf   (fin, formatAni,&speed,aniSubName,&ia,&ib,&flags,&v0,&v1,&v2,&v3,&v4);
      if (!strcmp(aniSubName,name) && (ia!=0 || ib!=1) ) {
        if (nparts>1) sprintf(ani.name, "%s_%s_%d",name, aniName,j+1);
        else sprintf(ani.name, "%s_%s",name, aniName);
        for (unsigned int h=0; h<frame.size(); h++) {
          if (frame[h].index>=ia && frame[h].index<=ib) {
            ani.frame.push_back(frame[h]);
            //ani.frame[ ani.frame.size()-1 ].index -= ia;
          }
        }
      }
      if (ani.frame.size()>0) {
        vect.push_back(ani);
        fprintf(fout,formatAni, speed, ani.name , ia, ib, flags, v0, v1, v2, v3, v4);
        /*ani.FirstIndex(),ani.LastIndex(),*/
        res++;
      } else
      fprintf(fout,formatAni, speed,aniSubName, ia, ib, flags, v0, v1, v2, v3, v4);

    }
  }
  fclose(fin);
  fclose(fout);
  return res;
}
int BrfAnimation::Break(vector<BrfAnimation> &vect) const{
  int res=0;
  BrfAnimation ani;
  //ani.flags=flags;
  ani.nbones=nbones;
  int start=-1;
  for (unsigned int i=0; i<frame.size(); i++) {

    if (start==-1) start = frame[i].index;

    if (( i>0) && (frame[i].index>frame[i-1].index+11)) {
      res++;
      sprintf(ani.name,"%s_%d_%d",name, start, frame[i-1].index);
      vect.push_back(ani);
      ani.frame.clear();
      start = frame[i].index;
    }

    ani.frame.push_back(frame[i]);
    ani.frame[ ani.frame.size()-1 ].index -=start;
  }
  return res;
}

bool BrfAnimation::Skip(FILE*f){
  if (!LoadString(f, name)) return false;
  SkipVectorR<TmpBone4>(f);
  SkipVectorF<TmpCas3>(f);
  return true;
}

bool BrfAnimation::Load(FILE*f, int verbose){
  if (!LoadString(f, name)) return false;
  if (verbose>0) printf("loading \"%s\"...\n",name);

  vector< TmpBone4 > tmpBone4v;
  if (!LoadVector(f,tmpBone4v)) return false; // vector of [vector of bone rotations]

  for (unsigned int i=0; i<tmpBone4v.size(); i++) {
    tmpBone4v[i].Adjust();
    if (i==0) tmpBone4v[i].AdjustRoot(); // assuming bone 0 is the root
  }
  nbones = tmpBone4v.size();

  vector< TmpCas3 > tmpCas3v;
  if (!LoadVector(f,tmpCas3v)) return false; // vector of translations

  TmpBone2BrfFrame(tmpBone4v, tmpCas3v, frame);

  /*FILE *ff = fopen("before.txt","wt");
  for (unsigned int i=0; i<tmpBone4v.size(); i++) {
    fprintf(ff,"(Bone %d) ",i); tmpBone4v[i].Export(ff);
  }
  fclose(ff);*/

  //Export("tmpAni.txt");
  return true;
}

void BrfAnimation::Save(FILE *f) const{
  SaveString(f,name);
  vector< TmpBone4 > tmpBone4v;
  vector< TmpCas3 > tmpCas3v;

  BrfFrame2TmpBone(frame , tmpBone4v, tmpCas3v);

  for (unsigned int i=0; i<tmpBone4v.size(); i++) {
    if (i==0) tmpBone4v[i].AdjustRootInv(); // assuming bone 0 is the root
    tmpBone4v[i].AdjustInv();
  }

  SaveVector(f,tmpBone4v);
  SaveVector(f,tmpCas3v);
  //Export("tmpAni.txt");

}

void BrfAnimation::Export(char* fn){
  FILE* f = fopen(fn,"wt");
  fprintf(f,"%s -- %d bones  %u frames...\n",name, nbones,frame.size());
  for (unsigned int j=0; j<frame.size(); j++) {
    for (int i=0; i<nbones; i++) {

      fprintf(f," %6.3f %6.3f %6.3f %6.3f (%f)\n",
             frame[j].rot[i].X(),frame[j].rot[i].Y(),frame[j].rot[i].Z(),frame[j].rot[i].W(),frame[j].rot[i].Norm());
    }
    fprintf(f," %6.3f %6.3f %6.3f\n",frame[j].tra.X(),frame[j].tra.Y(),frame[j].tra.Z());
    fprintf(f,"\n");
  }
  fclose(f);
}

BrfAnimation::BrfAnimation()
{
  float h = 1;
  bbox.Add( vcg::Point3f(h,2*h,h));
  bbox.Add(-vcg::Point3f(h,0,h));
}

bool BrfAnimation::AutoAssingTimesIfZero(){
  if (frame.size()<=1) return false;
  for (unsigned int i=0; i<frame.size(); i++) {
    if (frame[i].index!=0) return false;
  }
  for (unsigned int i=0; i<frame.size(); i++)
    frame[i].index=i;
  return true;
}
