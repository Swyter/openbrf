#ifndef ASKTEXTURENAMEDIALOG_H
#define ASKTEXTURENAMEDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class askTexturenameDialog;
}

class AskTexturenameDialog : public QDialog {
    Q_OBJECT
public:
    AskTexturenameDialog(QWidget *parent , bool letAlsoAdd);
    ~AskTexturenameDialog();

    void setDef(QString s);
    void setLabel(QString s);
    void setBrowsable(QString s);
    QString getRes() const;
    bool alsoAdd();
protected:
    QString path;
    void changeEvent(QEvent *e);
    static bool lastAlsoAdd;

private:
    Ui::askTexturenameDialog *m_ui;

private slots:
    void on_pushButton_clicked();
};

#endif // ASKTEXTURENAMEDIALOG_H
