#ifndef ASKTRANSFORMDIALOG_H
#define ASKTRANSFORMDIALOG_H

#include <QtGui/QDialog>


namespace Ui {
    class AskTransformDialog;
}

class AskTransformDialog : public QDialog {
    Q_OBJECT
public:
    AskTransformDialog(QWidget *parent = 0);
    ~AskTransformDialog();

    float* matrix;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskTransformDialog *ui;
private slots:
    void update();
    void reset();
signals:
    void changed();

};

#endif // ASKTRANSFORMDIALOG_H
