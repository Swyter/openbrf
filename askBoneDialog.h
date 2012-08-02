/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef ASKBONEDIALOG_H
#define ASKBONEDIALOG_H

#include <QtGui/QDialog>
#include <vector>

namespace Ui {
    class AskBoneDialog;
}

class BrfSkeleton;
class AskBoneDialog : public QDialog {
    Q_OBJECT
public:
    AskBoneDialog(QWidget *parent, const std::vector<BrfSkeleton> &skel);
    ~AskBoneDialog();
    void setSkeleton(const BrfSkeleton &s);
    int getSkel() const;
    int getBone() const;
    bool pieceAtOrigin() const;
    void sayNotRigged(bool say);

protected:
    void changeEvent(QEvent *e);
    //const std::vector<BrfSkeleton> &sv;
    std::vector<BrfSkeleton> sv;

protected slots:
    void selectSkel(int i);

private:
    Ui::AskBoneDialog *ui;
};

#endif // ASKBONEDIALOG_H
