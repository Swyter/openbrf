/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef ASKUNREFTEXTUREDIALOG_H
#define ASKUNREFTEXTUREDIALOG_H

#include <QDialog>

namespace Ui {
    class AskUnrefTextureDialog;
}

class AskUnrefTextureDialog : public QDialog {
    Q_OBJECT
public:
    AskUnrefTextureDialog(QWidget *parent = 0);
    ~AskUnrefTextureDialog();
    void addFile(QString f);

private slots:
    void refresh();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskUnrefTextureDialog *ui;
};

#endif // ASKUNREFTEXTUREDIALOG_H
