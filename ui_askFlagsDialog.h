/********************************************************************************
** Form generated from reading ui file 'askFlagsDialog.ui'
**
** Created: Thu Sep 16 22:03:42 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ASKFLAGSDIALOG_H
#define UI_ASKFLAGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AskFlagsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QWidget *widget;
    QWidget *widget_2;
    QWidget *widget_3;
    QWidget *widget_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton;

    void setupUi(QDialog *AskFlagsDialog)
    {
        if (AskFlagsDialog->objectName().isEmpty())
            AskFlagsDialog->setObjectName(QString::fromUtf8("AskFlagsDialog"));
        AskFlagsDialog->resize(562, 299);
        buttonBox = new QDialogButtonBox(AskFlagsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(200, 260, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        groupBox = new QGroupBox(AskFlagsDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 541, 241));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(193, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        widget = new QWidget(groupBox);
        widget->setObjectName(QString::fromUtf8("widget"));

        horizontalLayout->addWidget(widget);

        widget_2 = new QWidget(groupBox);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));

        horizontalLayout->addWidget(widget_2);

        widget_3 = new QWidget(groupBox);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));

        horizontalLayout->addWidget(widget_3);

        widget_4 = new QWidget(groupBox);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));

        horizontalLayout->addWidget(widget_4);

        horizontalSpacer_2 = new QSpacerItem(192, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton = new QPushButton(AskFlagsDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(11, 263, 51, 23));

        retranslateUi(AskFlagsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AskFlagsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AskFlagsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AskFlagsDialog);
    } // setupUi

    void retranslateUi(QDialog *AskFlagsDialog)
    {
        AskFlagsDialog->setWindowTitle(QApplication::translate("AskFlagsDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("AskFlagsDialog", "Flags", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("AskFlagsDialog", "show all", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(AskFlagsDialog);
    } // retranslateUi

};

namespace Ui {
    class AskFlagsDialog: public Ui_AskFlagsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASKFLAGSDIALOG_H
