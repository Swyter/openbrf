#include <QtCore>

#include "brfData.h"

#include "iniData.h"

#define MAX_ERR 10

template <class T> int _findByName( const vector<T> &v, const QString &s){
  for (unsigned int i=0; i<v.size(); i++)
    if (QString::compare(v[i].name,s,Qt::CaseInsensitive)==0) return i;
  return -1;
}

template <class T> int _findByNameNoExt( const vector<T> &v, const QString &s){
  for (unsigned int i=0; i<v.size(); i++) {
    QString st = v[i].name;
    int k = st.lastIndexOf('.');
    if (k>0) st.truncate(k);
    QString s2 = s;
    k = s2.lastIndexOf('.');
    if (k>0) s2.truncate(k);

    if (QString::compare(st,s2,Qt::CaseInsensitive)==0) return i;
  }
  return -1;
}

template <class T> void _addNames( const vector<T> &v, std::map<QString, Pair> &m, int j){
  for (unsigned int i=0; i<v.size(); i++)
    m[v[i].name] = Pair(j,i);
}

template <class T> void _addNamesNoExt( const vector<T> &v, std::map<QString, Pair> &m, int j){
  for (unsigned int i=0; i<v.size(); i++) {
    QString st = v[i].name;
    int k = st.lastIndexOf('.');
    if (k>0) st.truncate(k);
    m[st] = Pair(j,i);
  }
}

QString IniData::linkShort(int i, int j, int kind) const{
  QString res;
  res = QString("<a href=\"#%1.%2.%3\">%4</a>")
        .arg(i).arg(j).arg(kind).arg(shortFileName(i));
  return res;
}

QString IniData::shortFileName(int i) const {
  return QString(filename[i]).replace(mabPath,"");
}


QString IniData::link(int i, int j, int kind) const {
  QString res;
  res = QString("%6 <a href=\"#%1.%2.%3\">%4</a> (in %5)")
        .arg(i).arg(j).arg(kind)
        .arg(file[i].GetName(j,kind)).arg(shortFileName(i)).arg(tokenFullName[kind]);
  return res;
}

void IniData::clearIndexing(){
  for (int i=0; i<N_TOKEN; i++) indexing[i].clear();
}

void IniData::checkFile(int i, int j, int kind, char* usedFile, QDir *d0, QDir *d1){

  if (errorList.size()>MAX_ERR) return;
  bool res = false;
  if (d0) if (d0->exists(usedFile)) res = true;
  if (!res) if (d1) if (d1->exists(usedFile)) res = true;

  if (!res)
    errorList.push_back(
      QString("<b>File-not-found:</b> can't find texture file for %1.")
      .arg(link(i,j,kind))
    );

}

void IniData::checkUses(int i, int j, int kind, char* usedName, int usedKind){

  if (usedKind==TEXTURE && QString(usedName)=="none") return;

  if (errorList.size()>MAX_ERR) return;
  Pair d = indexOf( usedName, usedKind );
  if (d.first==-1) {
    errorList.push_back(
      QString("<b>Missing:</b> %1 uses unknown %2 <u>%3</u>")
      .arg(link(i,j,kind)).arg(tokenFullName[usedKind]).arg(usedName)
    );
  } else
  if (d.first>i) {
    errorList.push_back(
      QString("<b>Ordering problem:</b> %1 uses %2, which appears later in <i>module.ini</i>")
      .arg(link(i,j,kind)).arg(link(d.first,d.second,usedKind))
    );
  }
}

template <class T>
bool IniData::checkDuplicated(std::vector<T> &v, int j){
  int kind = T::tokenIndex();
  for (unsigned int i=0; i<v.size(); i++) {
    if (errorList.size()>MAX_ERR) return false;
    Pair d = indexOf(v[i].name, kind );
    if (d.first==-1) {
      errorList.push_back("<b>Internal error:</b> this should never happen");
    } else
    if (d.first!=j || d.second!=(int)i) {
      errorList.push_back(
          QString("<b>Duplicate:</b> %1 was already defined in file %2")
          .arg(link(j,i,kind)).arg(linkShort(d.first,d.second,kind))
      );
    }
  }
  return true;
}

template <class T>
void IniData::searchAllNamesV(const QString &s, int t,const std::vector<T> &v, int j, QString &res) const{
  int kind = T::tokenIndex();
  if (t!=-1 && t!=kind) return;
  for (unsigned int i=0; i<v.size(); i++) {

    if (QString(v[i].name).contains(s,Qt::CaseInsensitive))
      res+=link(j,i,kind)+"<br>";

  }
}

void IniData::findErrors(){
  errorList = errorListOnLoad;//.clear();
  for (unsigned int i=0; i<file.size(); i++) {
    // check for dupilcates
    checkDuplicated(file[i].texture,i);
    checkDuplicated(file[i].shader,i);
    checkDuplicated(file[i].material,i);
    checkDuplicated(file[i].mesh,i);
    checkDuplicated(file[i].body,i);
    checkDuplicated(file[i].skeleton,i);
    checkDuplicated(file[i].animation,i);

    // check for mesh->material
    for (unsigned int j=0; j<file[i].mesh.size(); j++) {
      checkUses(i,j,MESH, file[i].mesh[j].material, MATERIAL );
    }

    // check for material->etc
    for (unsigned int j=0; j<file[i].material.size(); j++) {
      checkUses(i,j,MATERIAL, file[i].material[j].diffuseA, TEXTURE );
      checkUses(i,j,MATERIAL, file[i].material[j].diffuseB, TEXTURE );
      checkUses(i,j,MATERIAL, file[i].material[j].bump, TEXTURE );
      checkUses(i,j,MATERIAL, file[i].material[j].enviro, TEXTURE );
      checkUses(i,j,MATERIAL, file[i].material[j].spec, TEXTURE );
      checkUses(i,j,MATERIAL, file[i].material[j].shader, SHADER );
    }

    // check for texture->fileondisk
    QDir d0(this->mabPath); d0.cd("Textures");
    QDir d1(this->modPath); d1.cd("Textures");
    for (unsigned int j=0; j<file[i].texture.size(); j++) {
      if (QString(file[i].texture[j].name)!="waterbump") // waterbumb hack
      checkFile(i,j,TEXTURE, file[i].texture[j].name , &d0, &d1);
    }
  }
  if (errorList.size()>MAX_ERR) {
    errorList.push_back("Too many errors for now. Stopping here.");
  };

}

QString IniData::searchAllNames(const QString &s, bool cr, int to) const{
  QString res;
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].texture,i,res);
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].shader,i,res);
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].material,i,res);
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].mesh,i,res);
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].body,i,res);
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].skeleton,i,res);
  for (int i=0; i<(int)file.size(); i++) if (origin[i]==MODULE_RES || cr)
    searchAllNamesV(s,to,file[i].animation,i,res);
  if (res.isEmpty()) res+="<i>[0 results]</i>";

  return res;
}


IniData::IniData(BrfData &_currentBrf): currentBrf(_currentBrf)
{
  filename.clear();
  file.clear();
  origin.clear();

  modPath.clear();;
  mabPath.clear();
  updated = false;
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
  if (indexing[kind].empty()) {
    for (unsigned int i=0; i<filename.size(); i++) {
      switch (kind) {
        default:
        case MATERIAL:  _addNames( file[i].material, indexing[kind],i ); break;
        case TEXTURE: _addNamesNoExt( file[i].texture, indexing[kind],i ); break;
        case SHADER: _addNames( file[i].shader, indexing[kind],i ); break;
        case MESH: _addNames( file[i].mesh, indexing[kind],i ); break;
        case ANIMATION: _addNames( file[i].animation, indexing[kind],i ); break;
        case SKELETON: _addNames( file[i].skeleton, indexing[kind],i ); break;
        case BODY: _addNames( file[i].body, indexing[kind],i ); break;
      }
    }
  }
  QString st = name;
  if (kind==TEXTURE)  {
    int k=st.lastIndexOf('.');
    if (k>0) st.truncate(k);
  }
  map<QString,Pair>::iterator p=(indexing[kind]).find(st);
  if (p==indexing[kind].end()) return Pair(-1,-1);
  else return p->second;
  /*
  for (unsigned int i=0; i<filename.size(); i++) {
    int k;
    switch (kind) {
      default:
      case MATERIAL: k= _findByName( file[i].material, name ); break;
      case TEXTURE: k= _findByNameNoExt( file[i].texture, name ); break;
      case SHADER: k= _findByName( file[i].shader, name ); break;
      case MESH: k= _findByName( file[i].mesh, name ); break;
      case ANIMATION: k= _findByName( file[i].animation, name ); break;
      case SKELETON: k= _findByName( file[i].skeleton, name ); break;
      case BODY: k= _findByName( file[i].body, name ); break;
    }
    if (k>=0) return Pair(i,k);
  }
  return Pair(-1,-1);
  */
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

QString IniData::name() const{
  return modPath;
}

bool IniData::load(bool faster){
  if (updated) return false;
  errorListOnLoad.clear();
  QFile f(modPath+"/module.ini");
  if (!f.open( QIODevice::ReadOnly| QIODevice::Text )) return false;
  char st[255];
  for (unsigned int i=0; i<file.size(); i++) file[i].Clear();
  file.clear();
  filename.clear();
  origin.clear();

  addBrfFile("core_shaders",CORE_RES,faster);
  addBrfFile("core_textures",CORE_RES,faster);
  addBrfFile("core_materials",CORE_RES,faster);
  addBrfFile("core_pictures",CORE_RES,faster);
  addBrfFile("core_ui_meshes",CORE_RES,faster);

  bool res=true;
  while (f.readLine(st,254)>-1)  {

    QString s = QString("%1").arg(st);
    s = s.trimmed(); // removal of spaces
    if (s[0]=='#') continue; // skip all comments
    char com1[512], com2[512];
    if (sscanf(s.toAscii().data(),"%s = %s",com1, com2)==2) {
      bool loadRes = QString(com1)=="load_resource";
      bool loadMod = ((QString(com1)=="load_mod_resource") || (QString(com1)=="load_module_resource"));
      if (loadRes || loadMod) {
        if (!addBrfFile(com2,(loadMod)?MODULE_RES:COMMON_RES,faster)) res=false;
      }

    }
  }
  updated = true;
  updateLists();
  return res;
}

int IniData::nRefObjects() const{
  int res=0;
  for (unsigned int i=0; i<file.size(); i++) {
    res+=file[i].texture.size();
    res+=file[i].material.size();
    res+=file[i].shader.size();
  }
  return res;
}

int IniData::nObjects() const{
  int res=0;
  for (unsigned int i=0; i<file.size(); i++) {
    res+=file[i].texture.size();
    res+=file[i].material.size();
    res+=file[i].body.size();
    res+=file[i].mesh.size();
    res+=file[i].shader.size();
    res+=file[i].animation.size();
    res+=file[i].skeleton.size();
  }
  return res;
}

bool IniData::addBrfFile(const char* name, Origin ori, bool faster){
  QString brfFn, brfPath;
  if (ori == MODULE_RES) {
    brfFn = modPath + "/Resource/" +name +".brf";
    brfPath = modPath + "/Resource";
  }
  else {
    brfFn = mabPath + "/CommonRes/" + name +".brf";
    brfPath = mabPath + "/CommonRes";
  }
  file.push_back(BrfData());
  filename.push_back(brfFn);
  origin.push_back(ori);
  BrfData &d(file[file.size()-1]);
  printf("Loading \"%s\"...\n",brfFn.toAscii().data());
  if (!d.LoadFast(brfFn.toAscii().data(),faster)) {
    // ERROR!!
    if (!QDir(brfPath).exists( QString("%1.brf").arg(name)))
    errorListOnLoad.push_back(QString("<b>File-Not-Found:</b> could not read brf file <u>%1</u>, listed in module.ini file")
       .arg(shortFileName(file.size()-1)));
    else
    errorListOnLoad.push_back(QString("<b>File-Format Error:</b> could not read brf file <u>%1</u>")
       .arg(shortFileName(file.size()-1)));

    //file.pop_back();
    //filename.pop_back(brfFn);
    return false;
  }
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
    //_updateList( namelist[MESH], file[i].mesh);
    //_updateList( namelist[BODY], file[i].body);
    _updateList( namelist[SHADER], file[i].shader);
    //_updateList( namelist[ANIMATION], file[i].animation);
    //_updateList( namelist[SKELETON], file[i].skeleton);
  }
  namelist[TEXTURE].append("none");
  namelist[SHADER].append("prova");
  this->clearIndexing();
}
/*
QStringList& IniData::nameList(int kind) const{
  return namelist[kind];
}*/




//Pair IniData::indexOf(const QString &name, int kind){
//}


