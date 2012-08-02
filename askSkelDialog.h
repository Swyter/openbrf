#ifndef ASKSKELDIALOG_H
#define ASKSKELDIALOG_H

#include <QtGui/QDialog>
#include <vector>

class BrfSkeleton;
namespace Ui {
    class AskSkelDialog;
}

class AskSkelDialog : public QDialog {
    Q_OBJECT
public:
    AskSkelDialog(QWidget *parent, const std::vector<BrfSkeleton> &sv );
    ~AskSkelDialog();
    int getSkelFrom() const;
    int getSkelTo() const;
    bool asFrame() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::AskSkelDialog *m_ui;
};

#endif // ASKSKELDIALOG_H
