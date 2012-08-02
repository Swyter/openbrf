#ifndef SELECTOR_H
#define SELECTOR_H

#include <QTabWidget>
#include <QItemSelection>
#include "BrfToken.h"

class BrfData;
class QListView;

class Selector : public QTabWidget
{
  Q_OBJECT

public:
  Selector(QWidget *parent=0);
  void setup(const BrfData &data);

private slots:
  void onChanged();

signals:
  //void selectionChanged (const QItemSelection &selected, const QItemSelection &now);
  void setSelection(const QModelIndexList & newSel, int newToken);

private:
  template<class BrfType> void addBrfTab(const std::vector<BrfType> &x);
  QListView * tab[N_TOKEN];
  //int tabIndex[N_TOKEN];
};

#endif // SELECTOR_H
