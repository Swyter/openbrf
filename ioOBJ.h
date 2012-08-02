#ifndef IOOBJ_H
#define IOOBJ_H

class IoOBJ
{
public:
  static bool wasMultpileMat();

  static bool open(QFile&f, QString fn);
  static bool writeMesh(QFile &f, const BrfMesh& m, int nframe);
  static void reset();
  static void subdivideLast(const BrfMesh& m, std::vector<BrfMesh> &v);
};

#endif // IOOBJ_H
