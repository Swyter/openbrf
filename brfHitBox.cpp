
#include <vector>
#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>
using namespace vcg;

#include "brfBody.h"
#include "brfSkeleton.h"

#include <QFile>
#include <QXmlStreamReader>

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class Reader : public QXmlStreamReader
{
public:
    //void XbelReader(QTreeWidget *treeWidget);

 };

void BrfSkeleton::LoadHitBox(char * filename){
  Reader reader;
  QFile file(filename);
  reader.setDevice(&file);

}
