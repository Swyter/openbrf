
#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void TableModel::updateChanges(){
  int t=vec.size();
  emit(this->dataChanged(createIndex(0,0),createIndex(1,t+100)));
  emit(layoutChanged());
}

void TableModel::clear()
{
  int t=vec.size();
  vec.clear();
  this->dataChanged(createIndex(0,0),createIndex(1,t));
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return vec.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
  if (role==Qt::DisplayRole)
    return vec[ index.row() ];
  return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  return tr("HEADER");

  //  return QVariant();
}




