/********************************************************************************
** Form generated from reading UI file 'drxcc.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRXCC_H
#define UI_DRXCC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_drxCCClass
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QComboBox *comboBox;
    QLineEdit *lineEdit_callback;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_getNotifyRoutine;
    QTreeWidget *treeWidget_callbackList;
    QPushButton *pushButton_derefCallback;
    QWidget *tab_3;
    QPushButton *pushButton;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QRadioButton *radioButton_enablePProcess;
    QRadioButton *radioButton_disablePProcess;
    QSpinBox *spinBox_pid_pProcess;
    QLabel *label;
    QPushButton *pushButton_setPProcess;
    QGroupBox *groupBox_2;
    QSpinBox *spinBox_pid;
    QLabel *label_2;
    QPushButton *pushButton_godShell;

    void setupUi(QMainWindow *drxCCClass)
    {
        if (drxCCClass->objectName().isEmpty())
            drxCCClass->setObjectName(QStringLiteral("drxCCClass"));
        drxCCClass->resize(526, 338);
        centralWidget = new QWidget(drxCCClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        comboBox = new QComboBox(tab);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        gridLayout_2->addWidget(comboBox, 0, 0, 1, 1);

        lineEdit_callback = new QLineEdit(tab);
        lineEdit_callback->setObjectName(QStringLiteral("lineEdit_callback"));

        gridLayout_2->addWidget(lineEdit_callback, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 2, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 1, 1, 2);

        pushButton_getNotifyRoutine = new QPushButton(tab);
        pushButton_getNotifyRoutine->setObjectName(QStringLiteral("pushButton_getNotifyRoutine"));

        gridLayout_2->addWidget(pushButton_getNotifyRoutine, 0, 3, 1, 1);

        treeWidget_callbackList = new QTreeWidget(tab);
        treeWidget_callbackList->setObjectName(QStringLiteral("treeWidget_callbackList"));
        treeWidget_callbackList->setUniformRowHeights(true);
        treeWidget_callbackList->setAnimated(true);
        treeWidget_callbackList->header()->setDefaultSectionSize(200);

        gridLayout_2->addWidget(treeWidget_callbackList, 1, 0, 1, 4);

        pushButton_derefCallback = new QPushButton(tab);
        pushButton_derefCallback->setObjectName(QStringLiteral("pushButton_derefCallback"));

        gridLayout_2->addWidget(pushButton_derefCallback, 2, 3, 1, 1);

        tabWidget->addTab(tab, QString());
        treeWidget_callbackList->raise();
        comboBox->raise();
        lineEdit_callback->raise();
        pushButton_getNotifyRoutine->raise();
        pushButton_derefCallback->raise();
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        pushButton = new QPushButton(tab_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(10, 10, 111, 23));
        tabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(150, 10, 261, 81));
        radioButton_enablePProcess = new QRadioButton(groupBox);
        radioButton_enablePProcess->setObjectName(QStringLiteral("radioButton_enablePProcess"));
        radioButton_enablePProcess->setGeometry(QRect(10, 20, 81, 21));
        radioButton_disablePProcess = new QRadioButton(groupBox);
        radioButton_disablePProcess->setObjectName(QStringLiteral("radioButton_disablePProcess"));
        radioButton_disablePProcess->setGeometry(QRect(70, 20, 81, 21));
        radioButton_disablePProcess->setChecked(true);
        spinBox_pid_pProcess = new QSpinBox(groupBox);
        spinBox_pid_pProcess->setObjectName(QStringLiteral("spinBox_pid_pProcess"));
        spinBox_pid_pProcess->setGeometry(QRect(170, 20, 81, 22));
        spinBox_pid_pProcess->setMaximum(9999);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 20, 21, 21));
        pushButton_setPProcess = new QPushButton(groupBox);
        pushButton_setPProcess->setObjectName(QStringLiteral("pushButton_setPProcess"));
        pushButton_setPProcess->setGeometry(QRect(10, 50, 101, 23));
        groupBox_2 = new QGroupBox(tab_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 121, 80));
        spinBox_pid = new QSpinBox(groupBox_2);
        spinBox_pid->setObjectName(QStringLiteral("spinBox_pid"));
        spinBox_pid->setGeometry(QRect(30, 20, 71, 22));
        spinBox_pid->setMaximum(9999);
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 20, 31, 21));
        pushButton_godShell = new QPushButton(groupBox_2);
        pushButton_godShell->setObjectName(QStringLiteral("pushButton_godShell"));
        pushButton_godShell->setGeometry(QRect(10, 50, 91, 21));
        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        drxCCClass->setCentralWidget(centralWidget);

        retranslateUi(drxCCClass);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(drxCCClass);
    } // setupUi

    void retranslateUi(QMainWindow *drxCCClass)
    {
        drxCCClass->setWindowTitle(QApplication::translate("drxCCClass", "drxCC - und3ath Kernel Exploiter | v0.2 | Private Build", 0));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("drxCCClass", "ProcessNotifyRoutine", 0)
         << QApplication::translate("drxCCClass", "ThreadNotifyRoutine", 0)
         << QApplication::translate("drxCCClass", "ImageNotifyRoutine", 0)
         << QApplication::translate("drxCCClass", "RegisterNotifyRoutine", 0)
         << QApplication::translate("drxCCClass", "ObjectNotifyRoutine", 0)
        );
        pushButton_getNotifyRoutine->setText(QApplication::translate("drxCCClass", "Get Routines", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_callbackList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("drxCCClass", "Information", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("drxCCClass", "PTR", 0));
        pushButton_derefCallback->setText(QApplication::translate("drxCCClass", "Dereference Callback !", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("drxCCClass", "Notify Routines", 0));
        pushButton->setText(QApplication::translate("drxCCClass", "Get Minifilters", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("drxCCClass", "Minifilter", 0));
        groupBox->setTitle(QApplication::translate("drxCCClass", "Protect Process", 0));
        radioButton_enablePProcess->setText(QApplication::translate("drxCCClass", "Enable", 0));
        radioButton_disablePProcess->setText(QApplication::translate("drxCCClass", "Disable", 0));
        label->setText(QApplication::translate("drxCCClass", "PID", 0));
        pushButton_setPProcess->setText(QApplication::translate("drxCCClass", "Set", 0));
        groupBox_2->setTitle(QApplication::translate("drxCCClass", "Nt-Authority\\System", 0));
        label_2->setText(QApplication::translate("drxCCClass", "PID", 0));
        pushButton_godShell->setText(QApplication::translate("drxCCClass", "Set", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("drxCCClass", "Drx Sys", 0));
    } // retranslateUi

};

namespace Ui {
    class drxCCClass: public Ui_drxCCClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRXCC_H
