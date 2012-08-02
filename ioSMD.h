#ifndef IOSMD_H
#define IOSMD_H

/*static*/ class ioSMD
{
public:

  static int Export(const char*filename, const BrfMesh &m , const BrfSkeleton &s, int fi);
  static int Import(const char*filename, BrfMesh &m , BrfSkeleton &s);
  static int Export(const char*filename, const BrfAnimation &a, const BrfSkeleton &s);
  static int Import(const char*filename, BrfAnimation &a, BrfSkeleton &s);
  static char* LastErrorString();
  static char* LastWarningString();
  static bool Warning();

private:


};

#endif // IOSMD_H
