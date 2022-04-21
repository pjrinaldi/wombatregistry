/********************************************************************************
** Form generated from reading UI file 'wombatregistry.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WOMBATREGISTRY_H
#define UI_WOMBATREGISTRY_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WombatRegistry
{
public:
    QAction *actionOpenHive;
    QAction *actionManageTags;
    QAction *actionAbout;
    QAction *actionPreviewReport;
    QAction *actionPublish;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter_2;
    QTreeWidget *treewidget;
    QSplitter *splitter;
    QTableWidget *tablewidget;
    QPlainTextEdit *plaintext;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *WombatRegistry)
    {
        if (WombatRegistry->objectName().isEmpty())
            WombatRegistry->setObjectName(QString::fromUtf8("WombatRegistry"));
        WombatRegistry->resize(1022, 712);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/wombat"), QSize(), QIcon::Normal, QIcon::Off);
        WombatRegistry->setWindowIcon(icon);
        actionOpenHive = new QAction(WombatRegistry);
        actionOpenHive->setObjectName(QString::fromUtf8("actionOpenHive"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/bar/openfolder"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenHive->setIcon(icon1);
        actionManageTags = new QAction(WombatRegistry);
        actionManageTags->setObjectName(QString::fromUtf8("actionManageTags"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/bar/managetags"), QSize(), QIcon::Normal, QIcon::Off);
        actionManageTags->setIcon(icon2);
        actionAbout = new QAction(WombatRegistry);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/help"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon3);
        actionPreviewReport = new QAction(WombatRegistry);
        actionPreviewReport->setObjectName(QString::fromUtf8("actionPreviewReport"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/bar/preview"), QSize(), QIcon::Normal, QIcon::Off);
        actionPreviewReport->setIcon(icon4);
        actionPublish = new QAction(WombatRegistry);
        actionPublish->setObjectName(QString::fromUtf8("actionPublish"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/bar/publish"), QSize(), QIcon::Normal, QIcon::Off);
        actionPublish->setIcon(icon5);
        centralwidget = new QWidget(WombatRegistry);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        splitter_2 = new QSplitter(centralwidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        treewidget = new QTreeWidget(splitter_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treewidget->setHeaderItem(__qtreewidgetitem);
        treewidget->setObjectName(QString::fromUtf8("treewidget"));
        treewidget->setSelectionBehavior(QAbstractItemView::SelectItems);
        splitter_2->addWidget(treewidget);
        treewidget->header()->setVisible(false);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        tablewidget = new QTableWidget(splitter);
        if (tablewidget->columnCount() < 3)
            tablewidget->setColumnCount(3);
        tablewidget->setObjectName(QString::fromUtf8("tablewidget"));
        tablewidget->setAlternatingRowColors(true);
        tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        tablewidget->setColumnCount(3);
        splitter->addWidget(tablewidget);
        tablewidget->horizontalHeader()->setCascadingSectionResizes(true);
        tablewidget->horizontalHeader()->setStretchLastSection(true);
        tablewidget->verticalHeader()->setVisible(false);
        plaintext = new QPlainTextEdit(splitter);
        plaintext->setObjectName(QString::fromUtf8("plaintext"));
        QFont font;
        font.setFamily(QString::fromUtf8("Source Code Pro"));
        plaintext->setFont(font);
        plaintext->setReadOnly(true);
        splitter->addWidget(plaintext);
        splitter_2->addWidget(splitter);

        verticalLayout->addWidget(splitter_2);

        WombatRegistry->setCentralWidget(centralwidget);
        menubar = new QMenuBar(WombatRegistry);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1022, 22));
        WombatRegistry->setMenuBar(menubar);
        statusbar = new QStatusBar(WombatRegistry);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        WombatRegistry->setStatusBar(statusbar);
        toolBar = new QToolBar(WombatRegistry);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        WombatRegistry->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actionOpenHive);
        toolBar->addAction(actionManageTags);
        toolBar->addAction(actionPreviewReport);
        toolBar->addAction(actionPublish);
        toolBar->addAction(actionAbout);

        retranslateUi(WombatRegistry);

        QMetaObject::connectSlotsByName(WombatRegistry);
    } // setupUi

    void retranslateUi(QMainWindow *WombatRegistry)
    {
        WombatRegistry->setWindowTitle(QCoreApplication::translate("WombatRegistry", "Wombat Registry", nullptr));
        actionOpenHive->setText(QCoreApplication::translate("WombatRegistry", "Open Hive", nullptr));
#if QT_CONFIG(shortcut)
        actionOpenHive->setShortcut(QCoreApplication::translate("WombatRegistry", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionManageTags->setText(QCoreApplication::translate("WombatRegistry", "Manage Tags", nullptr));
#if QT_CONFIG(tooltip)
        actionManageTags->setToolTip(QCoreApplication::translate("WombatRegistry", "Manage Tags", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionManageTags->setShortcut(QCoreApplication::translate("WombatRegistry", "Ctrl+M", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("WombatRegistry", "About", nullptr));
#if QT_CONFIG(tooltip)
        actionAbout->setToolTip(QCoreApplication::translate("WombatRegistry", "About", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionAbout->setShortcut(QCoreApplication::translate("WombatRegistry", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPreviewReport->setText(QCoreApplication::translate("WombatRegistry", "Preview Report", nullptr));
#if QT_CONFIG(tooltip)
        actionPreviewReport->setToolTip(QCoreApplication::translate("WombatRegistry", "Preview Report", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionPreviewReport->setShortcut(QCoreApplication::translate("WombatRegistry", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPublish->setText(QCoreApplication::translate("WombatRegistry", "Publish Report", nullptr));
#if QT_CONFIG(tooltip)
        actionPublish->setToolTip(QCoreApplication::translate("WombatRegistry", "Publish Report", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionPublish->setShortcut(QCoreApplication::translate("WombatRegistry", "Ctrl+G", nullptr));
#endif // QT_CONFIG(shortcut)
        toolBar->setWindowTitle(QCoreApplication::translate("WombatRegistry", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WombatRegistry: public Ui_WombatRegistry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WOMBATREGISTRY_H
