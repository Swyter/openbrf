
#include <vector>
#include <stdio.h>

#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

using namespace std;
using namespace vcg;

#include "BrfMesh.h"

#include "saveLoad.h"
typedef vcg::Point3f Pos;

int SameTri(Pos a0, Pos a1, Pos b0, Pos b1, Pos c0, Pos c1){  
   
  if  (a0==a1 && b0==b1 && c0==c1) return 1;
    //  ||(a0==b1 && b0==c1 && c0==a1) 
  //  ||(a0==c1 && b0==a1 && c0==b1)
  ;
  return 0;
  
}

void BrfMesh::UpdateBBox(){
  bbox.SetNull();
  for (unsigned int i=0; i<frame[0].pos.size(); i++) {
    bbox.Add(frame[0].pos[i]);
  }
}


void BrfMesh::FindSymmetry(vector<int> &output){
  int nfound=0,nself=0;
  output.resize(frame[0].pos.size());
  for (int i=0; i<frame[0].pos.size(); i++) {
    output[i]=i;
  }
  
  for (int i=0; i<frame[0].pos.size(); i++) {
    for (int j=0; j<frame[0].pos.size(); j++) {
      Point3f pi = frame[0].pos[i];
      Point3f pj = frame[0].pos[j];
      pj[0]*=-1;
      if ((pi-pj).Norm()<0.01){ 
        output[i]=j;
        if (i==j) nself++; else nfound++;
      }
    }  
  }
  printf("Found %d symmetry (and %d self) on %d",nfound,nself,frame[0].pos.size());
}

void BrfMesh::ApplySymmetry(const vector<int> &input){
  vector<Point3f> oldpos=frame[0].pos;
  for (int i=0; i<frame[0].pos.size(); i++) {
    for (int k=1; k<3; k++) { // for Y and Z, not X
      frame[0].pos[i][k] = oldpos[input[i]][k];
    }
  }
}

void BrfMesh::CopyPos(int nf, const BrfMesh &brf, const BrfMesh &newbrf){
  int nbad=0, ngood=0;
  
  for (int i=0; i<vert.size(); i++) {
    int found=false;
    for (int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[nf].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[0 ].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      if (1
      // && (posA-posB).SquaredNorm()<0.01
      && posA==posB
      //&& uvA==uvB
      ) 
      { 
         frame[nf].pos[vert[i].index] = newbrf.frame[0].pos[newbrf.vert[j].index];
         //frame[nf].myster[vert[i].index] = newbrf.frame[0].myster[brfnew.vert[j].index];
         found=true;
         break;
      }
    }
    if (!found) nbad++; else ngood++;
  }
  printf("NOt changed %d (bad) and changed %d good\n",nbad, ngood);
}

void BrfMesh::FixTextcoord(const BrfMesh &brf,BrfMesh &ref, int fi){
  int nbad=0, ngood=0, nundec=0;
  
  vector<bool> undec;
  undec.resize(vert.size(), false);
  
  vector<int> perm(vert.size(), -1);
  
  for (int i=0; i<vert.size(); i++) {
    int found=false;
    float score = 100000000.0;
    int bestj=-1;
    for (int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[fi].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[ 0].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      
      if (1
      //&& (posA-posB).SquaredNorm()<0.01
      && posA==posB
      //&& uvA==uvB
      ) 
      { 
         if (found) undec[i]=true;
         if (perm[j]!=-1) continue;
         found=true;
         float curscore = (ref.vert[i].ta - brf.vert[j].ta).SquaredNorm();
         if (curscore<score) {
            score=curscore;
         } else continue;
         bestj=j;
      }
    }
    
    if (found) perm[bestj]=i;
    if (!found) nbad++; else
      if (undec[i]) nundec++; else
        ngood++;
  }

  printf("Found %d bad, %d good, %d undex\n",nbad, ngood, nundec);
  
  // apply permut
  vector<BrfVert> tmp=vert;
  vector<bool> tmp2=undec;
  for (int i=0; i<vert.size(); i++) {
    vert[ i ] = tmp[ perm[i] ];
    ref.vert[ i ] = tmp[ perm[i] ];
    undec[i] = tmp2[ perm[i] ];    
  }
  
  vector<int> permInv(vert.size());
  for (int i=0; i<vert.size(); i++) {
    permInv[ perm[i] ] = i;
  }
  
  
  for (int i=0; i<face.size(); i++) 
  for (int j=0; j<3; j++) {
    face[i].index[j] = permInv[face[i].index[j]];
    ref.face[i].index[j] = permInv[ref.face[i].index[j]];
  }
  
  return;

  // avrg
  for (int i=0; i<ref.face.size(); i++) {
    int ndec=0, nundec=0;
    Point2f avga,avgb;
    avga=avgb=Point2f(0,0);
    for (int j=0; j<3; j++) {
      int k=ref.face[i].index[j];
      if (undec[k])  { nundec++; }
      else {
        ndec++;
        avga+=ref.vert[k].ta;
        avgb+=ref.vert[k].tb;
      }
    }
    avga/=ndec;
    avgb/=ndec;
    if (nundec>0 && nundec<3) for (int j=0; j<3; j++) {
      int k=ref.face[i].index[j];
      if (undec[k]) {
        ref.vert[k].ta=avga;
        ref.vert[k].tb=avgb;
      }
    }
    
  }
  
}

void BrfMesh::SetName(char* st){
  sprintf(name,st);
}

void BrfMesh::DeleteSelected(){
  printf("  thereis %3dface, %3dpoin, %3dvert\n",face.size(), frame[0].pos.size(), vert.size());


  vector<int> remapv;
  remapv.resize(vert.size(),-1);
  

  vector<bool> survivesp;
  survivesp.resize(frame[0].pos.size(),false);
  
  int deletedV=0, deletedP=0, deletedF=0;
  
  vector<BrfVert> tvert = vert;
  vert.clear();
  
  vector<BrfFrame> tframe = frame;
  for (int j=0; j<frame.size(); j++) frame[j].myster.clear();
  
  for (int i=0; i<tvert.size(); i++) {
    if (!selected[i]) {
      vert.push_back(tvert[i]);
      for (int j=0; j<frame.size(); j++) frame[j].myster.push_back(tframe[j].myster[i]);
      remapv[i]=vert.size()-1;
      
      int j=tvert[i].index;
      survivesp[j]=true;
    } else deletedV++;
  }

  vector<int> remapp;
  remapp.resize(frame[0].pos.size(),-1);
  
  for (int i=0; i<frame.size(); i++) {
    int k=0;
    deletedP =0;
    for (int j=0; j<frame[i].pos.size(); j++) {
      if (survivesp[j]) {
        remapp[j]=k;
        frame[i].pos[k]=frame[i].pos[j];
        frame[i].myster[k]=frame[i].myster[j];
        k++;
      } else deletedP++;
    }
    frame[i].pos.resize(k);
  }
  
  for (int i=0; i<vert.size(); i++) {
   vert[i].index = remapp[vert[i].index];
  }
  
  vector<BrfFace> tface = face;
  face.clear();
  for (int i=0; i<tface.size(); i++) {
    BrfFace f;
    bool ok=true;
    for (int j=0; j<3; j++) {
      int k=remapv[ tface[i].index[j] ];
      if (k<0) ok=false;
      f.index[j]=k;
    }
    if (ok) face.push_back(f); else deletedF++;
  }
  printf("  deleted %3dface, %3dpoin, %3dvert\n",deletedF, deletedP, deletedV);
  printf("  dsurviv %3dface, %3dpoin, %3dvert\n",face.size(), frame[0].pos.size(), vert.size());
}

void BrfMesh::SelectAbsent(const BrfMesh& brf, int fi){
  int nbad=0, ngood=0;

  selected.clear();
  selected.resize(vert.size(),true);

  for (int i=0; i<vert.size(); i++) {
    int found=false;
    for (int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[fi].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[ 0].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      
      if (1
      && (posA-posB).SquaredNorm()<0.0001
      //&& posA==posB
      //&& uvA==uvB
      && (uvA-uvB).SquaredNorm()<0.0001
      ) 
      {
         selected[i]=false;
         found=true; 
         //break;
      }
    }
    if (!found) nbad++; else ngood++;
  }
  printf("Selected %d absent (%d where present)\n",nbad, ngood);
}



void BrfMesh::CopyTextcoord(const BrfMesh &brf, const BrfMesh &newbrf, int fi){
  int nbad=0, ngood=0;
  
  for (int i=0; i<vert.size(); i++) {
    int found=false;
    for (int j=0; j<brf.vert.size(); j++) {
      vcg::Point3f posA =     frame[fi].pos[     vert[i].index ];
      vcg::Point3f posB = brf.frame[ 0].pos[ brf.vert[j].index ];
      vcg::Point2f uvA =     vert[i].ta;
      vcg::Point2f uvB = brf.vert[j].ta;
      
      
      if (1
      && (posA-posB).SquaredNorm()<0.0001
      //&& posA==posB
      //&& uvA==uvB
      && (uvA-uvB).SquaredNorm()<0.0001     ) 
      { 
         vert[i].ta = newbrf.vert[j].ta;
         vert[i].tb = newbrf.vert[j].tb;
         //frame[0].pos[     vert[i].index ] =  newbrf.frame[0].pos[ newbrf.vert[j].index ];
         //printf("%d<->%d ",i,j); 
         //vert[i].col=0xFFFF0000; 
         found=true; 
         //break;
      }
    }
    if (!found) nbad++; else ngood++;
  }
  printf("Found %d bad and %d good\n",nbad, ngood);
}

bool BrfMesh::CheckAssert() const{
  
  bool ok = true;
  for (int i=0; i<frame.size(); i++) {
    if ( frame[i].myster.size() != vert.size() ) {
      printf("Check failed for frame %d! (%d != %d)\n",
      i,frame[i].myster.size() , vert.size());
      ok=false;
    }
  }
  return ok;
}

void BrfMesh::Save(FILE*f,int verbose) const{
  CheckAssert();

  if (verbose) printf(" saving \"%s\"...\n",name);

  SaveString(f, name);
  
  SaveInt(f , flags); 
  
  SaveString(f, material); // material used
  
  SaveInt(f , frame[0].pos.size() ); // n vertices
    
  for (int j=0; j<frame[0].pos.size(); j++) {
    SavePoint(f,  frame[0].pos[j]);
  }
  
  SaveInt(f , 0); // hum... 

  SaveInt(f , frame.size()-1 ); // nframes!

  for (int i=1; i<frame.size(); i++) {
    
    SaveInt(f , frame[i].time);
    SaveInt(f , frame[i].pos.size()); // n vertices
    
    for (int j=0; j<frame[i].pos.size(); j++) {
      SavePoint(f,  frame[i].pos[j]);
    }
    
    //SaveInt(f, 0 ); // 0 mysterious things
    
    SaveInt(f, frame[i].myster.size() ); // vertex_key_ipo . keys[...].corner_normals.size().
    for (int j=0; j<frame[i].myster.size(); j++) {
      SavePoint(f,  frame[i].myster[j]);
    }
  }

    
  SaveInt(f , vert.size());
  
  for (int i=0; i<vert.size(); i++) { // facecorner.size
    SaveInt(f , vert[i].index); 
    
    //if (vert[i].index>150) SaveUint(f,0xFFFFFFFF); else SaveUint(f,0x000000FF);
    SaveUint(f , vert[i].col ); // color x vert! as 4 bytes AABBGGRR
//    printf("%x ",vert[i].col&0xFFFFFF);
    SavePoint(f,  vert[i].n ); 
    
    SavePoint(f,vert[i].ta );
    SavePoint(f,vert[i].tb );
  }
  
  SaveInt(f, face.size()); // 2 tri-faces (1 quad)
  
  for (int i=0; i<face.size(); i++) {
    for (int j=0; j<3; j++)
      SaveInt(f,  face[i].index[j] );
  }
  
  return;
}

/*
BrfMesh& BrfMesh::operator = (const BrfMesh &brf){
  flags=brf.flags;
  sprintf(name,"%s",brf.name);
  sprintf(material,"%s",brf.material);
  
  frame.resize(brf.frame.size() );
  
  Merge(brf);  
  return *this;
}*/

bool BrfMesh::SaveAsPly(int frameIndex, char* path) const{
  char filename[255];
  if (frame.size()==0) sprintf(filename,"%s%s.ply",path, name);
  else sprintf(filename,"%s\%s%02d.ply",path, name,frameIndex);
  FILE* f = fopen(filename,"wt");
  if (!f) { printf("Cannot save \"%s\"!\n",filename); return false;}
  printf("Saving \"%s\"...\n",filename);
  fprintf(f,
    "ply\n"
    "format ascii 1.0\n"
    "comment fromBRF (by Marco Tarini)\n"
    "element vertex %d\n"
    "property float x\n"
    "property float y\n"
    "property float z\n"
    "element face %d\n"
    "property list uchar int vertex_indices\n"
    "end_header\n", vert.size(), face.size()
  );
  for (int i=0; i<vert.size(); i++) {
    fprintf(f,"%f %f %f\n",
      frame[frameIndex].pos[ vert[i].index ].X(),
      frame[frameIndex].pos[ vert[i].index ].Y(),
      frame[frameIndex].pos[ vert[i].index ].Z()
    );
  }
  for (int i=0; i<face.size(); i++) {
    fprintf(f,"3 %d %d %d\n",
      face[i].index[2],
      face[i].index[1],
      face[i].index[0]
    );
  }
  fclose(f);
  return true;
}

void BrfMesh::AdaptToRes(const BrfMesh& ref){
  //int np = frame[0].pos.size();
  
  for (int i=0; i<frame.size(); i++) {
    for (int j=0; j< frame[i].pos.size(); j++)
    frame[i].pos[j]= ref.frame[0].pos[j+4];
//     frame[i].pos.push_back( ref.frame[0].pos[j]);
   // for (int j=frame[i].pos.size(); j< ref.frame[0].pos.size(); j++) 
   //  frame[i].pos.push_back( ref.frame[0].pos[j]);
  }
  vector<int> newv(ref.vert.size(), -1);
  
/*  for (int i=0; i<ref.vert.size(); i++) {
    if (ref.vert[i].index>=np) {
      newv[i]=vert.size();
      vert.push_back(vert[i]);
    }
 }
 */

/*
  for (int i=0; i<face.size(); i++) {
    if (face[i].index>refnp) {
      int v0= newv[ face[i].index[0] ];
      int v1= newv[ face[i].index[1] ];
      int v2= newv[ face[i].index[2] ];
      if (v0>=0) {
        face.push_back(vert[i]);
      }
    }
  }
*/
 
//  vert=ref.vert;
//  face=ref.face;
}


BrfFrame BrfFrame::Average(BrfFrame& b, float t){
  BrfFrame res=*this;
  for (int i=0; i<pos.size(); i++) {
    res.pos[i]=pos[i]*(t) + b.pos[i]*(1.0f-t);
  }
  return res;
}

BrfFrame BrfFrame::Average(BrfFrame& b, float t, const vector<bool> &sel){
  BrfFrame res=*this;
  for (int i=0; i<pos.size(); i++) {
     if (sel[i]) res.pos[i]=pos[i]*(t) + b.pos[i]*(1.0f-t);
  }
  return res;
}




  
void BrfMesh::DiminishAni(float t){
  BrfFrame orig=frame[3];
  for (int i=2; i<frame.size(); i++) {
    int j=i+1;
    frame[i]=frame[i].Average((j==frame.size())?orig:frame[j],t);
  }
}

void BrfMesh::DiminishAniSelected(float t){
  vector<BrfFrame> orig=frame;
  for (int i=2; i<frame.size(); i++) {
    int j1=i+1; if (j1>=frame.size()) j1=2;
    int j2=i-1; if (j1<2) j1+=frame.size()-2;
    BrfFrame b = orig[j1];
    b=b.Average(orig[j2],0.5,selected);
    for (int k=0; k<frame[i].pos.size(); k++) {
     if (selected[k]) frame[i].pos[k]=b.pos[k];
    }
    frame[i].Average(orig[i],t,selected);
  }
}


void BrfMesh::KeepSelctedFixed(int asInFrame, double howmuch){
  for (int i=2; i<frame.size(); i++) if (i!=asInFrame) {
    frame[i]=frame[i].Average(frame[asInFrame],howmuch,selected);
  }
}


int BrfMesh::GetFirstSelected(int after) const{
  for (int i=after+1; i<selected.size(); i++) if (selected[i]) return i;
  return -1;
}

Point3f BrfMesh::GetASelectedPos(int framei) const{
  return frame[framei].pos[ vert[ GetFirstSelected()] .index ];
}

Point3f BrfMesh::GetAvgSelectedPos(int framei) const{
  Point3f res=Point3f(0,0,0);
  int n=0;
  for (int i=0; i<frame[framei].pos.size(); i++) {
    if (selected[i]) {
      res+=frame[framei].pos[i];
      n++;
    }
  }
//  printf("(sel=%d on %d, vert=%d)\n",n,selected.size(), frame[framei].pos.size());
  return res/n;
}


void BrfMesh::FollowSelected(const BrfMesh &brf, int bf){
  int sel=brf.GetFirstSelected(); // first selected point
  if (sel<0) return;
  for (int j=0; j<frame.size(); j++) {
    vcg::Point3f d = brf.frame[j].pos[sel] - brf.frame[bf].pos[sel];
    for (int i=0; i<frame[j].pos.size(); i++){
      frame[j].pos[i]+=d;
    }
  }
  
}

void BrfMesh::CopySelectedFrom(const BrfMesh &brf){
  for (int j=0; j<frame.size(); j++) {
    for (int i=0; i<frame[j].pos.size(); i++){
      if (selected[i])
       frame[j].pos[i]=brf.frame[j].pos[i];
    }
  }
}



void BrfMesh::Hasten(float timemult){
  for (int j=0; j<frame.size(); j++) {
     int t= frame[j].time;
     if (t>=100) //t=100+int((t-100)/timemult);
     t=140-int((140-t)/timemult);
     frame[j].time=t;
  }
}

void BrfMesh::Flip(){
  for (int j=0; j<frame.size(); j++) {
    for (int i=0; i<frame[j].pos.size(); i++){
      frame[j].pos[i].X()*=-1;
    }
    for (int i=0; i<frame[j].myster.size(); i++){
      frame[j].myster[i].X()*=-1;
    }
  }
  
  for (int i=0; i<face.size(); i++) face[i].Flip();
}

class CoordSist{
public:
  Point3f base;
  Point3f x,y,z;
  Point3f operator() (Point3f p) {
    return x*p[0] + y*p[1] + z*p[2] + base;
  };
  CoordSist Inverse() {
    CoordSist tmp;
    tmp.x=Point3f(x[0],y[0],z[0]);
    tmp.y=Point3f(x[1],y[1],z[1]);
    tmp.z=Point3f(x[2],y[2],z[2]);
    tmp.base=Point3f(0,0,0);
    tmp.base = tmp(-base);
    return tmp;
  };
  CoordSist(){};
  CoordSist(Point3f a, Point3f b, Point3f c){
    base = a;
    b-=a;
    c-=a;
    x=b;
    y=b^c;
    z=x^y;
    x.Normalize();
    z.Normalize();
    y.Normalize();
  };
};

void BrfMesh::FollowSelectedSmart(const BrfMesh &brf, int bf){
  int p0=brf.GetFirstSelected(); 
  int p1=brf.GetFirstSelected(p0); 
  int p2=brf.GetFirstSelected(p1);
  if (p0<0 || p1<0 || p1<1 ) return;
  
  CoordSist direct( brf.frame[bf].pos[p0], brf.frame[bf].pos[p1], brf.frame[bf].pos[p2]);
  CoordSist inverse=direct.Inverse();
  
  for (int j=0; j<frame.size(); j++) {
    if (j==bf) continue;
    
    CoordSist local( brf.frame[j].pos[p0], brf.frame[j].pos[p1], brf.frame[j].pos[p2]);
    
    for (int i=0; i<frame[j].pos.size(); i++){
      frame[j].pos[i] = local( inverse ( frame[j].pos[i] ) );
    }
  }
  
}

void BrfMesh::PropagateDeformations(int bf, const BrfMesh &ori){
  int p0=GetFirstSelected(); 
  int p1=GetFirstSelected(p0); 
  int p2=GetFirstSelected(p1);
  if (p0<0 || p1<0 || p1<1 ) return;

  CoordSist direct( ori.frame[bf].pos[p0], ori.frame[bf].pos[p1], ori.frame[bf].pos[p2]);
  CoordSist inverse=direct.Inverse();
  
  for (int j=1; j<frame.size(); j++) {
    if (j==bf) continue;
    
    CoordSist local( frame[j].pos[p0], frame[j].pos[p1], frame[j].pos[p2]);
    
    for (int i=0; i<frame[j].pos.size(); i++){
      if (frame[bf].pos[i] != ori.frame[bf].pos[i] )
      frame[j].pos[i] = local( inverse ( frame[bf].pos[i] ) );
    }
  }
}


void BrfMesh::ClearSelection(){
  selected.clear();
  selected.resize(frame[0].pos.size(),false);
}

void BrfMesh::CopySelection(const BrfMesh& ref){
  ClearSelection();
  for (int i=0; i<ref.selected.size(); i++) if (ref.selected[i]) selected[i]=true;
//  selected = ref.selected;
}

void BrfMesh::SelectRed(const BrfMesh &brf){
  selected.clear();
  selected.resize(frame[0].pos.size(),false);
  int count=0;
  for (int i=0; i<vert.size(); i++) {
    if ( brf.vert[i].col == 0xFFFF0000 ) { 
      int j=vert[i].index; 
      if (!selected[j]) count++;
      selected[j]=true; 
    }
  }
  printf("Selected %d points\n",count);
}

void BrfMesh::SelectRand(){
  selected.clear();
  selected.resize(frame[0].pos.size(),false);
  int count=0;
  for (int i=0; i<42; i++) rand();
  for (int i=0; i<3; i++) {
    int j=rand()%(frame[0].pos.size()); 
    if (!selected[j]) count++;
    selected[j]=true; 
  }
  printf("Selected %d random points\n",count);
}

BrfMesh BrfMesh::SingleFrame(int i) const {
  BrfMesh res = *this;
  res.frame.clear();
  res.frame.push_back(frame[i]);
  return res;
}


void BrfMesh::FindRefPoints(){
  for (int i=0; i<vert.size(); i++) {
    if ( vert[i].col == 0xFF800080 ) refpoint.push_back(vert[i].index);
  }

  printf("Found %d ref points\n",refpoint.size());
}

Point3f OnCylinder(Point3f p, float range){
  float angle = p[2]/range;
  Point3f sol;
  sol[0] = cos(angle)*(range+p[0]);
  sol[2] = sin(angle)*(range+p[0]);
  sol[1] = p[1];
  return sol;
}

Point3f BrfFrame::MinPos(){
  Point3f res(0,0,0);
  if (pos.size()) res=pos[0];
  for (int i=1; i<pos.size(); i++) 
    for (int k=0; k<3; k++) if (pos[i][k]<res[k]) res[k]=pos[i][k];
  return res;
}
Point3f BrfFrame::MaxPos(){
  Point3f res(0,0,0);
  if (pos.size()) res=pos[0];
  for (int i=1; i<pos.size(); i++) 
    for (int k=0; k<3; k++) if (pos[i][k]>res[k]) res[k]=pos[i][k];
  return res;
}

// bends as if on a cylinder
void BrfMesh::Bend(int j, float range){
  float max=GetTopPos(j,2);
  float dx = OnCylinder( Point3f(0,0,max*10/16) , range)[0];
  for (int i=0; i<frame[j].pos.size(); i++) {
    frame[j].pos[i] = OnCylinder( frame[j].pos[i] , range);
    frame[j].pos[i][0] -=dx;
  }
}

void BrfFrame::Load(FILE*f, int verbose)
{
    LoadInt(f , time);
    if (verbose>2) printf("    [t:%03d ",time);
    //system("PAUSE");

    int k;
    LoadInt(f , k  ); // n vertices
    if (verbose>2) printf("npos p:%d ",k);
    //system("PAUSE");
  
    pos.resize(k);
    
    for (int j=0; j<pos.size(); j++) {
      LoadPoint(f,  pos[j]);
    }
    
    LoadInt(f , k  ); // n vertices
    if (verbose>2) printf("n:%d ]\n",k);
    myster.resize(k);
    for (int j=0; j<myster.size(); j++) {
      LoadPoint(f,  myster[j]);
    }


}

void BrfFrame::LoadV1(FILE*f, int verbose)
{

    
}

void BrfMesh::InvertPosOrder(){
  vector<Point3f> p=frame[0].pos;
  int N = p.size()-1;
  for (int i=0; i<frame[0].pos.size(); i++) frame[0].pos[i]=p[N-i];
}

void BrfMesh::FixPosOrder(const BrfMesh &b){
  vector<int> best(frame[0].pos.size(),0);
  vector<float> bestscore(frame[0].pos.size(),99999999.0);
  for (int i=0; i<vert.size(); i++) 
    for (int j=0; j<b.vert.size(); j++) {
    if ((vert[i].ta == b.vert[j].ta)  && (vert[i].tb == b.vert[j].tb) ) {
      int i2=vert[i].index;
      int j2=b.vert[j].index;
      float score=(frame[0].pos[i2]-b.frame[0].pos[j2]).Norm();
      if (score<bestscore[i2]) {
        bestscore[i2]=score;
        best[i2]=j2;
      }
    }
  }
  
  vector<Point3f> p=frame[0].pos;
  for (int i=0; i<frame[0].pos.size(); i++) frame[0].pos[i]=p[best[i]];
}

void BrfMesh::Load(FILE*f, int verbose){
  LoadString(f, name);
  if (verbose>0) printf("loading \"%s\"...\n",name);
  LoadInt(f , flags); 
  if (verbose>1) printf("  flags: 0x%04x",flags);
  LoadString(f, material); // material used
  if (verbose>1) printf("  mat: \"%s\"",material);
  
  frame.resize(1);
  
  int k;
  //system("PAUSE");
  LoadInt(f , k  ); // n vertices
  if (verbose>1) printf("  Npos:%d ",k);
  frame[0].pos.resize(k);
  
  for (int j=0; j<frame[0].pos.size(); j++) {
    LoadPoint(f,  frame[0].pos[j]);
    //if (j<5) printf("(%f-%f-%f)",frame[0].pos[j][0],frame[0].pos[j][1],frame[0].pos[j][2]);
  }
  
  LoadInt(f , k ); 
  if (verbose>1) printf("  records:%d\n",k);
  //system("PAUSE");
  
  int n_skel=k;
  int tot=0;
  float totf=0;
  for (int h=0; h<n_skel; h++) {
    int k;
    LoadInt(f , k  ); // n... something...
    if (verbose>2) printf("    index(?):%02d  ",k);
    LoadInt(f , k  ); // n... something...
    if (verbose>2) printf("pairs:%d ",k);
    tot+=k;
//    system("PAUSE");  
    
     
    for (int j=0; j<k; j++) {
       if (verbose>3) if (j%5==0) printf("\n      ");
//      LoadPoint(f,  pos[j]);
//      if (verbose>2) printf("%6.3f%6.3f%6.3f  ",pos[j][0],pos[j][1],pos[j][2]);
       float b;
       unsigned int k;
       LoadUint(f,k);
       LoadFloat(f,b);
       if (verbose>3) printf("(%03d %5.3f) ",k,b);
       totf+=b;
    }
    if (verbose>2) printf("\n");
  }
  if (verbose>2) if (n_skel>0) printf("    (total pair: %d -- sum w: %f)\n",tot,totf);

  LoadInt(f , k  ); // nframes!
  if (verbose>1) if (k>0) printf("  frames:%d...\n",k);
//  system("PAUSE");
  
  frame.resize(k+1);
  
  for (int i=1; i<frame.size(); i++) 
    frame[i].Load(f,verbose);


  

  LoadInt(f , k  );
  vert.resize(k);
  
  if (verbose>1) printf("  verts: %d ",k);
  for (int i=0; i<vert.size(); i++) {
    LoadInt(f , vert[i].index); 

    LoadUint(f , vert[i].col ); // color x vert! as 4 bytes AABBGGRR
    //printf("%x ",vert[i].col&0xFFFFFF);

    LoadPoint(f,  vert[i].n ); 
    
    LoadPoint(f,vert[i].ta );
//    if (i<5) printf("(%f-%f)",vert[i].ta[0],vert[i].ta[1]);
    LoadPoint(f,vert[i].tb );
  }

  LoadInt(f , k  );
  if (verbose>1) printf("faces: %d\n",k);
  face.resize(k);
  
  for (int i=0; i<face.size(); i++) {
    for (int j=0; j<3; j++)
      LoadInt(f,  face[i].index[j] );
  }
  
  // let's make the structure so that M&B is happy
  frame[0].myster.resize(vert.size());
  for (int i=0; i<vert.size(); i++) {
    frame[0].myster[i]=vert[i].n;
  }
  
  if (verbose>0) printf("  [%dv %dFx%dp %df]\n", vert.size(),frame.size(),frame[0].pos.size(), face.size());

  UpdateBBox();
}


void BrfMesh::CopyTimesFrom(const BrfMesh &b){
  if (frame.size()!=b.frame.size()) {
    printf("WARNING: different number of frames %d!=%d\n",frame.size(),b.frame.size());
  }
  for (int j=0; j<frame.size(); j++) {
    frame[j].time=b.frame[j].time;
  }
}

void BrfMesh::Average(const BrfMesh &brf){

  //int nvert = vert.size();
  for (int j=0; j<frame.size(); j++) {
    for (int i=0; i<brf.frame[j].pos.size(); i++){
      frame[j].pos[i] = (frame[j].pos[i] + brf.frame[j].pos[i])/2.0;
    }
    for (int i=0; i<brf.frame[j].myster.size(); i++){
      frame[j].myster[i] = (frame[j].myster[i] + brf.frame[j].myster[i])/2.0;
    }
  }
}
  
void BrfMesh::AddFrame(const BrfMesh &b)
{ 
  frame.push_back(b.frame[0]);
  int i=frame.size()-1;
  frame[i].time = frame[i-1].time +10;
}

class Rope{
public:
  vector<Point3f> pos;
  Point3f width;
  float lenght;
  
  void Fall(){
    for (int i=1; i<pos.size()-1; i++) {
      pos[i][1]-=0.01;
    }
  }
  
  void SetMinLenght(Point3f a, Point3f b){
    float tmp =(a-b).Norm();
    if (lenght<tmp) lenght=tmp;
  }
  
  void Resist(){
    vector<Point3f> post=pos;
    float k=lenght / (pos.size()-1);
    for (int i=0; i<pos.size()-1; i++) {
      Point3f v=pos[i]-pos[i+1];
      float l=v.Norm();
      v=-v.Normalize()*((k-l)*(0.5));
      post[i]-=v;
      post[i+1]+=v;
    }
    for (int i=1; i<pos.size()-1; i++) {
      pos[i]=post[i];
    }
  }
  
  void Simulation(int n){
    for (int i=0; i<n; i++) {
      Fall();
      Resist();
    }
  }
  
  void Init(int n){
    pos.resize(n);
    lenght=0;
  }

  void SetPos(Point3f a, Point3f b, float w){
    for (int i=0; i<pos.size(); i++) {
      float t=float(i)/(pos.size()-1);
      pos[i]=a*t+b*(1-t);
    } 
    Point3f tmp = (a-b);  tmp[1]=0;
    tmp=tmp.Normalize() * w;
    width = Point3f( tmp[2], tmp[1], -tmp[0]);
  }

  void AddTo(BrfFrame &f){
    for (int i=0; i<pos.size(); i++) {
      f.pos.push_back(pos[i]+width);
      f.pos.push_back(pos[i]-width);
      f.myster.push_back( Point3f(0,1,0) );
      f.myster.push_back( Point3f(0,1,0) );
    }
  }
  
  void AddTo(BrfMesh &b){
    int base = b.vert.size();
    
    for (int i=0; i<pos.size(); i++) {
      for (int k=0; k<2; k++) {
        BrfVert v;
        v.index=b.frame[0].pos.size()+i*2+k;
        v.n = Point3f(0,1,0);
        v.col = 0xffffffff;
        v.ta = v.tb = Point2f(
          479+4*k,
          512-(343+i/float(pos.size()-1) * 32)
        )/512.0;
        b.vert.push_back( v );
      }
    }
    
    for (int i=0; i<pos.size()-1; i++) {
      int i0=base+0, i1=base+1, i2=base+3, i3=base+2;
      b.face.push_back( BrfFace(i0,i1,i2) );
      b.face.push_back( BrfFace(i2,i3,i0) );
      b.face.push_back( BrfFace(i2,i1,i0) );
      b.face.push_back( BrfFace(i0,i3,i2) );
      base+=2;
    }
  }
};

void BrfMesh::AddRope(const BrfMesh &to, int nseg, float width){
  BrfMesh from=*this;
  Rope rope;
  rope.Init(nseg);
  
  for (int i=1; i<frame.size(); i++) { 
    rope.SetMinLenght(from.GetAvgSelectedPos(i), to.GetAvgSelectedPos(i));
  }
  
  rope.AddTo(*this);
  for (int i=0; i<frame.size(); i++) {
    rope.SetPos(from.GetAvgSelectedPos(i), to.GetAvgSelectedPos(i), width);
    rope.Simulation(100);
    rope.AddTo(frame[i]);
  }
}

void BrfMesh::Merge(const BrfMesh &b)
{
  int npos = frame[0].pos.size();
  int nvert = vert.size();
  
  for (int j=0; j<frame.size(); j++) {
    for (int i=0; i<b.frame[j].pos.size(); i++){
      frame[j].pos.push_back( b.frame[j].pos[i]);
    }
    for (int i=0; i<b.frame[j].myster.size(); i++){
      frame[j].myster.push_back( b.frame[j].myster[i]);
    }
  }
    
  for (int i=0; i<b.vert.size(); i++) {
    vert.push_back(b.vert[i] + npos);
  }

  for (int i=0; i<b.face.size(); i++) {
    face.push_back(b.face[i] + nvert);
  }
  
  for (int i=0; i<b.selected.size(); i++) {
    selected.push_back(b.selected[i]);
  }
  
}

Point3f Mirror(Point3f p) {
  return Point3f(-p[0],p[1],p[2]);
}

BrfFace Mirror(BrfFace f) {
  BrfFace r;
  r.index[0] = f.index[2];
  r.index[1] = f.index[1];
  r.index[2] = f.index[0];
  return r;
}

float BrfMesh::GetTopPos(int j, int axis) const{
  float min=-100;
  for (int i=0; i<frame[j].pos.size(); i++){
    if (min< frame[j].pos[i][axis] ) min = frame[j].pos[i][axis];
  }
  return min;
}

void AddRope(const BrfMesh &from, const BrfMesh &to, int nseg, double lenghtProp){
}


void BrfMesh::AlignToTop(BrfMesh& a, BrfMesh& b){
   
  for (int j=0; j<a.frame.size(); j++) {
      float dy = a.GetTopPos(j) - b.GetTopPos(j);
      if (dy<0) {
        for (int i=0; i<a.frame[j].pos.size(); i++)
          a.frame[j].pos[i][1]-=dy;
      } else {
        for (int i=0; i<b.frame[j].pos.size(); i++)
          b.frame[j].pos[i][1]+=dy;
      }
  }
}

void BrfMesh::MergeMirror(const BrfMesh &bb)
{
  BrfMesh b = bb;
  AlignToTop(*this,b);
  
  printf("MErging %d fotograms\n",frame.size() );
  int npos = frame[0].pos.size();
  int nvert = vert.size();
  
  for (int j=0; j<frame.size(); j++) {
    
    float dy = 0 ;// GetTopPos(j) - b.GetTopPos(j);
    
    for (int i=0; i<b.frame[j].pos.size(); i++){
      Point3f p = Mirror( b.frame[j].pos[i] );
      p[1]+=dy;
      
      // merge points near the junction
      if (fabs(p[0])<0.02) {
        p[0]=0.0;
        p = (p + frame[j].pos[i])/2.0;
        frame[j].pos[i] = p;
      } else {
        // hack to fix tail
        if (selected[i]) {
          float y,z;
          y = (p[1] + frame[j].pos[i][1])/2.0;
          z = (p[2] + frame[j].pos[i][2])/2.0;
          frame[j].pos[i][1] = p[1] = y;
          frame[j].pos[i][2] = p[2] = z;
        }
      }
      
      frame[j].pos.push_back( p );
    }
    for (int i=0; i<b.frame[j].myster.size(); i++){
      frame[j].myster.push_back( b.frame[j].myster[i]);
    }
  }
  for (int i=0; i<b.vert.size(); i++) {
    vert.push_back(b.vert[i] + npos);
  }

  for (int i=0; i<b.face.size(); i++) {
    face.push_back(Mirror( b.face[i] + nvert ));
  }
}

void BrfMesh::DuplicateFrames(const BrfMesh &b)
{
  DuplicateFrames(b.frame.size());
}

void BrfMesh::DuplicateFrames(int nf)
{
  frame.resize(nf);
  
  int time =0;
  for (int j=1; j<frame.size(); j++) {
    frame[j]=frame[0];
    frame[j].time=time;
    time+=10;
  }
}

void BrfMesh::PaintAll(int r, int g, int b){
  for (int i=0; i<vert.size(); i++) vert[i].col=0xFFFFFFFF;
}


void BrfMesh::Translate(Point3f p){
  for (int j=0; j<frame.size(); j++) 
    for (int i=0; i<frame[j].pos.size(); i++){
      //if (selected[i] || !selectedOnly) 
      frame[j].pos[i]+=p;
  }
}

void BrfMesh::CollapseBetweenFrames(int fi,int fj){
  Point3f point = frame[0].MinPos();
  if (fj>frame.size()-1) fj=frame.size()-1;
  for (int j=fi; j<=fj; j++) 
    for (int i=0; i<frame[j].pos.size(); i++){
      //if (selected[i] || !selectedOnly) 
      frame[j].pos[i]=point;
  }
}



void BrfMesh::Scale(float p){
  for (int j=0; j<frame.size(); j++) 
    for (int i=0; i<frame[j].pos.size(); i++){
      //if (selected[i] || !selectedOnly) 
      frame[j].pos[i]*=p;
  }
}

void BrfMesh::TranslateSelected(Point3f p){
  for (int j=0; j<frame.size(); j++) {
    for (int i=0; i<frame[j].pos.size(); i++){
      if (selected[i]) frame[j].pos[i]+=p;
    }
  }
}

void BrfMesh::CycleFrame(int i){
  //int n=frame.size() - 2;
  for (int k=0; k<i; k++) {
    BrfFrame tmp = frame[3];
    int j;
    for (j=3; j<frame.size()-1; j++) 
      frame[j]=frame[j+1];
    frame[j]=tmp;
    frame[2]=frame[j];
  }
}
