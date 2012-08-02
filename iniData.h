#ifndef INIDATA_H
#define INIDATA_H

#include <map>
typedef QPair<int,int> Pair;

class QDir;

class IniData
{

public:
  BrfData &currentBrf;
  typedef enum {MODULE_RES, COMMON_RES, CORE_RES} Origin;
  IniData(BrfData &currentBrf);
  std::vector<BrfData> file;
  std::vector<QString> filename; // full path included
  std::vector<Origin> origin;


  QString mat2tex(const QString &s);
  bool load(bool faster);

  QString modPath;
  QString mabPath;
  QString name() const;

  bool updated;
  int nRefObjects() const;
  int nObjects() const;

  bool addBrfFile(const char* name, Origin origin, bool faster);
  void setPath(QString mabPath, QString modPath);

  QStringList namelist[N_TOKEN];
  void updateLists();

  int findFile(const QString &fn); // returns index of a given file

  Pair indexOf(const QString &name, int kind, Pair startFrom=Pair(0,0) ); // returns: index of file, of object inside file
  BrfMaterial* findMaterial(const QString &name, Pair startFrom=Pair(0,0) );

  QStringList errorList; // list all error strings
  void findErrors();
  QString searchAllNames(const QString &s,bool commonResToo, int token) const;
private:
  QString link(int i, int j, int kind) const; // given an object j of kind kind in file i, returns a strig link
  QString linkShort(int i, int j, int kind) const;
  QString shortFileName(int i) const;
  template<class T> bool checkDuplicated(std::vector<T> &v, int fi);
  void checkUses(int i, int j, int kind, char* usedName, int usedKind);
  void checkFile(int i, int j, int kind, char* fileName, QDir* d0, QDir* d1);
  std::map<QString, Pair> indexing[N_TOKEN];
  void clearIndexing();
  QStringList errorListOnLoad; // list all error strings on load
  template<class T>  void searchAllNamesV(const QString &s, int t, const std::vector<T> &v, int i, QString &res) const;

};

#endif // INIDATA_H
