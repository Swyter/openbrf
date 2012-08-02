

#include <QtGui>
#include "brfdata.h"
#include "selector.h"

#include "tablemodel.h"

Selector::Selector(QWidget *parent)
   : QTabWidget(parent)
{
  for (int i=0; i<N_TOKEN; i++) {
    tab[i]=NULL;
    //tabIndex[i]=-1;
  }
  connect(this, SIGNAL(currentChanged(int)),
          this, SLOT(onChanged(int)) );

}


template<class BrfType>
void Selector::addBrfTab(const vector<BrfType>  &v){

  int ti = BrfType::tokenIndex();

  if (v.size()!=0) {
    TableModel *tableModel = new TableModel(NULL);
    for (unsigned int k=0; k<v.size(); k++) {
      tableModel->vec.push_back( QString( v[k].name ) );
    }


    if (tab[ti]) {
      delete tab[ti]; tab[ti]=NULL;
    }
    tab[ti] = new QListView;

    tab[ti]->setModel(tableModel);
    if (ti==MESH) tab[ti]->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(tab[ti]->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
          this, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)));


    //tabIndex[ti]=
    addTab(tab[ti], tr("%1 (%2)")
           .arg( tokenName[ti])
           .arg(v.size())
           );
  } else {
    //if (tabIndex[ti]>=0)
    //this->removeTab(tabIndex[ti]);
  }

}

void Selector::onChanged(int k){
  for(int ti=0; ti<N_TOKEN; ti++){
    if (tab[ti]) tab[ti]->clearSelection();
    if (this->currentWidget()==tab[ti]) emit tabChanged( ti );
  }

}

void Selector::setup(const BrfData &data){

  this->clear();
  addBrfTab<BrfMesh> (data.mesh);
  addBrfTab<BrfShader> (data.shader);
  addBrfTab<BrfTexture> (data.texture);
  addBrfTab<BrfMaterial> (data.material);
  addBrfTab<BrfSkeleton> (data.skeleton);
  addBrfTab<BrfAnimation> (data.animation);
  addBrfTab<BrfBody> (data.body);

}
