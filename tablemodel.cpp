
#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractListModel(parent)
{
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




