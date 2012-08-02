/* OpenBRF -- by marco tarini. Provided under GNU General Public License */


#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <vector>

//! [0]
class TableModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    TableModel(QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    std::vector<QString> vec;
    std::vector<int> vecUsed;
    void clear();
    int size() const {return (int)vec.size();}
    QModelIndex pleaseCreateIndex(int a, int b){
      return createIndex(a,b);
    }
    void updateChanges();
private:
};
//! [0]

#endif
