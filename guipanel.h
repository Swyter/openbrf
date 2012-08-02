#ifndef GUIPANEL_H
#define GUIPANEL_H

#include <QtGui/QWidget>
#include <QModelIndexList>

class BrfData;

namespace Ui {
    class GuiPanel;
}

class GuiPanel : public QWidget {
    Q_OBJECT
public:
    GuiPanel(QWidget *parent = 0);
    ~GuiPanel();
    BrfData* data;
    BrfData* reference;

protected:
    void changeEvent(QEvent *e);
    enum{MAXSEL=500};


public:
    Ui::GuiPanel *ui;
public slots:
    void setSelection(const QModelIndexList &, int k);
};

#endif // GUIPANEL_H
