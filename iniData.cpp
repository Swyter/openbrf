#include <QtCore>

#include "brfData.h"

#include "iniData.h"

IniData::IniData(BrfData &_currentBrf): currentBrf(_currentBrf)
{
  filename.clear();
  file.clear();

  modPath.clear();;
  mabPath.clear();
  updated = false;
}

template <class T> int _findByName( const vector<T> &v, const QString &s){
  for (unsigned int i=0; i<v.size(); i++)
    if (QString::compare(v[i].name,s,Qt::CaseInsensitive)==0) return i;
  return -1;
}

QString IniData::mat2tex(const QString &n){
  int j = _findByName( currentBrf.material, n);
  if (j>=0) return currentBrf.material[j].diffuseA;
  for (unsigned int i=0; i<filename.size(); i++)
  for (unsigned int j=0; j<file[i].material.size(); j++) {
    BrfMaterial &m(file[i].material[j]);
    if (QString(m.name)==n) {
      return m.diffuseA;
    }
  }
  return QString();
}

BrfMaterial* IniData::findMaterial(const QString &name,Pair startFrom) {
  int j = _findByName( currentBrf.material, name);
  if (j>=0) return &(currentBrf.material[j]);

  Pair p = indexOf(name, MATERIAL, startFrom);
  if (p.first>=0) return &file[p.first].material[p.second];
  return NULL;
}

int IniData::findFile(const QString &fn) {
  for (unsigned int i=0; i<filename.size(); i++) {
    if (QString::compare(fn,filename[i],Qt::CaseInsensitive)==0) return i;
  }
  return -1;
}


Pair IniData::indexOf(const QString &name, int kind, Pair startFrom){
  for (unsigned int i=0; i<filename.size(); i++) {
    int k;
    switch (kind) {
      default:
      case MATERIAL: k= _findByName( file[i].material, name ); break;
      case TEXTURE: k= _findByName( file[i].texture, name ); break;
      case SHADER: k= _findByName( file[i].shader, name ); break;
    }
    if (k>=0) return Pair(i,k);
  }
  return Pair(-1,-1);
}

void IniData::setPath(QString _mabPath, QString _modPath){
  if (
   QString::compare(mabPath,_mabPath,Qt::CaseInsensitive)==0 &&
   QString::compare(modPath,_modPath,Qt::CaseInsensitive)==0
  ) return;
  updated=false;
  mabPath=_mabPath;
  modPath=_modPath;
}

bool IniData::load(){
  if (updated) return false;
  QFile f(modPath+"/module.ini");
  if (!f.open( QIODevice::ReadOnly| QIODevice::Text )) return false;
  char st[255];
  for (unsigned int i=0; i<file.size(); i++) file[i].Clear();
  file.clear();
  filename.clear();

  while (f.readLine(st,254)>-1)  {

    QString s(st);
    s = s.trimmed(); // removal of spaces
    if (s[0]=='#') continue; // skip all comments
    char com1[512], com2[512];
    if (sscanf(s.toAscii().data(),"%s = %s",com1, com2)==2) {
      bool loadRes = QString(com1)=="load_resource";
      bool loadMod = QString(com1)=="load_mod_resource";
      if (loadRes || loadMod) {
        addBrfFile(com2,(loadMod)?MODULE_RES:COMMON_RES);
      }

    }
  }
  addBrfFile("core_materials",CORE_RES);
  addBrfFile("core_textures",CORE_RES);
  addBrfFile("core_shaders",CORE_RES);
  addBrfFile("core_pictures",CORE_RES);
  addBrfFile("core_ui_meshes",CORE_RES);
  updated = true;
  updateLists();
  return true;
}

bool IniData::addBrfFile(const char* name, Origin origin){
  QString brfFn;
  if (origin == MODULE_RES) {
    brfFn = modPath + "/resource/" +name +".brf";
  }
  else {
    brfFn = mabPath + "/CommonRes/" + name +".brf";
  }
  file.push_back(BrfData());
  BrfData &d(file[file.size()-1]);
  printf("Loading \"%s\"...\n",brfFn.toAscii().data());
  if (!d.LoadFast(brfFn.toAscii().data())) {
    // ERROR!!
    file.pop_back();
    printf("  - failed!\n");
    return false;
  }
  filename.push_back(brfFn);
  return true;
}

template <class T>
void _updateList(QStringList &l, const vector<T> &d){
  for (unsigned int i=0; i<d.size(); i++) {
    l.append(d[i].name);
  }
}

template <class T>
void _updateListNoExt(QStringList &l, const vector<T> &d){
  for (unsigned int i=0; i<d.size(); i++) {
    QString s(d[i].name);
    s.truncate( s.lastIndexOf("."));
    l.append(s);
  }
}

void IniData::updateLists(){
  for (int i=0; i<N_TOKEN; i++)
    namelist[i].clear();
  for (unsigned int i=0; i<file.size(); i++) {
    _updateListNoExt( namelist[TEXTURE], file[i].texture);
    _updateList( namelist[MATERIAL], file[i].material);
    _updateList( namelist[MESH], file[i].mesh);
    _updateList( namelist[BODY], file[i].body);
    _updateList( namelist[SHADER], file[i].shader);
    _updateList( namelist[ANIMATION], file[i].animation);
    _updateList( namelist[SKELETON], file[i].skeleton);
  }
  namelist[TEXTURE].append("none");
  namelist[SHADER].append("prova");
}
/*
QStringList& IniData::nameList(int kind) const{
  return namelist[kind];
}*/




//Pair IniData::indexOf(const QString &name, int kind){
//}


