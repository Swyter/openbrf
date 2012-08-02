#ifndef INIDATA_H
#define INIDATA_H

typedef QPair<int,int> Pair;
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
  bool load();

  QString modPath;
  QString mabPath;
  bool updated;

  bool addBrfFile(const char* name, Origin origin);
  void setPath(QString mabPath, QString modPath);

  QStringList namelist[N_TOKEN];
  void updateLists();

  int findFile(const QString &fn); // returns index of a given file

  Pair indexOf(const QString &name, int kind, Pair startFrom=Pair(0,0) ); // returns: index of file, of object inside file
  BrfMaterial* findMaterial(const QString &name, Pair startFrom=Pair(0,0) );

};

#endif // INIDATA_H
