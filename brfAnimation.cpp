#include <vector>
#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;
#include "saveLoad.h"

#include "brfSkeleton.h"
#include "brfAnimation.h"

void BrfAnimation::SetSkeleton(BrfSkeleton *s){
  skel=s;
  if (s) bbox = s->bbox;
}

Box3f BrfAnimation::bbox;
BrfSkeleton* BrfAnimation::skel=NULL;
int  BrfAnimation::curFrame; // frame currently being shown

using namespace std;

// classes for saving loading bones and frames
template <class PointT>
class TmpCas{
public:
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
    LoadVector(f,cas);
    return true;
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
    vf[i].wasImplicit.resize(vb.size() + 1); // +1 for the translation
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
      if (!vf[ fi ].wasImplicit[bi]) vb[bi].cas.push_back( c );
    }
    TmpCas3 c;
    c.findex = vf[ fi ].index;
    c.rot = vf[ fi ].tra;
    if (!vf[ fi ].wasImplicit[nbones]) vt.push_back(c);
  }
}

int BrfAnimation::FirstIndex() const{
  if (frame.size()==0) return 0;
  return frame[0].index;
}
int BrfAnimation::LastIndex() const{
  if (frame.size()==0) return 0;
  return frame[ frame.size()-1 ].index;
}

int BrfAnimation::Break(vector<BrfAnimation> &vect, char* fn) const{

  int res=0;
  FILE *fin=fopen(fn,"rt");
  char fn2[1024];
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

  BrfAnimation ani;
  ani.flags=flags;
  ani.nbones=nbones;

  char oldst[2048];
  for (int i=0; i<n; i++){
    fscanf(fin," %s %u %d \n",aniName, &v00, &nparts);
    sprintf(oldst," %s %u  %d\n",aniName, v00, nparts);
    int v0; float v1, v2, v3, v4;
    int ia, ib; unsigned int flags;
    int cumulated = 0;
    for (int j=0; j<nparts; j++) {
      fscanf(fin,"  %f %s %d %d %u %d %f %f %f %f \n", &speed, aniSubName, &ia, &ib ,  &flags, &v0, &v1,&v2,&v3,&v4);
      sprintf(oldst,"%s  %f %s %d %d %u %d %f %f %f %f \n", oldst, speed, aniSubName, ia, ib ,  flags, v0, v1,v2,v3,v4);
      if (!strcmp(aniSubName,name) && (ia!=0 || ib!=1) ) {
        for (unsigned int h=0; h<frame.size(); h++) {
          if (frame[h].index>=ia && frame[h].index<=ib) {
            ani.frame.push_back(frame[h]);
            ani.frame[ ani.frame.size()-1 ].index += -ia + cumulated;
            cumulated+=ib-ia+1;
          }
        }
      }
    }
    if (ani.frame.size()) {
      sprintf(ani.name, "%s_%s",name, aniName);
      vect.push_back(ani);
      fprintf(fout," %s %u  1 \n",aniName, v00);
      fprintf(fout,"  %f %s %d %d %u %d %f %f %f %f \n", speed, ani.name,
              ani.FirstIndex(),ani.LastIndex(),  flags, v0, v1,v2,v3,v4);

      ani.frame.clear();
      res++;
    } else {
      fprintf(fout,oldst);
    }
  }
  fclose(fin);
  fclose(fout);
  return res;
}
int BrfAnimation::Break(vector<BrfAnimation> &vect) const{
  int res=0;
  BrfAnimation ani;
  ani.flags=flags;
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

bool BrfAnimation::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);

  vector< TmpBone4 > tmpBone4v;
  LoadVector(f,tmpBone4v);

  nbones = tmpBone4v.size();

  vector< TmpCas3 > tmpCas3v;
  LoadVector(f,tmpCas3v);

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

  /*
  FILE *ff = fopen("after.txt","wt");
  for (unsigned int i=0; i<tmpBone4v.size(); i++) {
    fprintf(ff,"(Bone %d) ",i); tmpBone4v[i].Export(ff);
  }
  fclose(ff);*/

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
}
