#ifndef ASKFLAGSDIALOG_H
#define ASKFLAGSDIALOG_H

#include <QtGui/QDialog>

class QCheckBox;

namespace Ui {
    class AskFlagsDialog;
}

class AskFlagsDialog : public QDialog {
    Q_OBJECT
public:
    AskFlagsDialog(QWidget *parent, unsigned int ones, unsigned int zeros, QStringList l);
    ~AskFlagsDialog();
    unsigned int toOne() const;  // result: flags that must be set to 1
    unsigned int toZero() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskFlagsDialog *m_ui;
    QCheckBox* cb[32];

private slots:
    void on_pushButton_clicked();
};

#endif // ASKFLAGSDIALOG_H
