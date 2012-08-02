/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef ASKTRANSFORMDIALOG_H
#define ASKTRANSFORMDIALOG_H

#include <QtGui/QDialog>


namespace Ui {
    class AskTransformDialog;
}

class AskTransformDialog : public QDialog {
    Q_OBJECT
public:
		AskTransformDialog(QWidget *parent = 0, bool multObj=false);
    ~AskTransformDialog();

    float* matrix;
		bool* applyToAll;

		void setApplyToAllLoc(bool *loc);
		void setSensitivityOne(double sens);
		void setSensitivityAll(double sens);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskTransformDialog *ui;
		double sensitivityOne, sensitivityAll;
private slots:
    void update();
    void reset();
		void updateSensitivity();
signals:
    void changed();

};

#endif // ASKTRANSFORMDIALOG_H
