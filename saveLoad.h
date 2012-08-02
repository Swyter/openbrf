
// small helper functions for saving/loading stuff into a brf file

void SaveString(FILE *f, const char *st);
void SaveInt(FILE *f, int i);
void SaveFloat(FILE *f, float f);
void SaveUint(FILE *f, unsigned int x);
void SavePoint(FILE *f, Point3f p);
void SavePoint(FILE *f, Point2f p);

void LoadString(FILE *f, char *st);
void LoadInt(FILE *f, int &i);
void LoadFloat(FILE *f, float &f);
void LoadUint(FILE *f, unsigned int &x);
void LoadPoint(FILE *f, Point3f &p);
void LoadPoint(FILE *f, Point2f &p);
