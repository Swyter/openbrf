#ifndef IOMB_H
#define IOMB_H

class IoMB
{
public:
  static bool Export(const char*filename, const BrfMesh &m , const BrfSkeleton &s, int fi);
  static bool Import(const char*filename, std::vector<BrfMesh> &m , BrfSkeleton &s, int want);
  static bool Export(const char*filename, const BrfAnimation &a, const BrfSkeleton &s);
  static bool Import(const char*filename, BrfAnimation &a, BrfSkeleton &s);
  static char* LastErrorString();
};

#endif // IOMB_H
