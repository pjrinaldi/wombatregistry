#ifndef WOMBATREGISTRY_H
#define WOMBATREGISTRY_H

// Copyright 2022-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

//#include "wombatinclude.h"
//#include "globals.h"
//#include "wombatfunctions.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QtEndian>
#include <QDateTime>
#include <QTimeZone>
#include "ui_wombatregistry.h"
#include "tagmanager.h"
#include "htmlviewer.h"
#include "aboutbox.h"
#include "libregf.h"
#include "cssstrings.h"
//#include "tagging.h"
//#include "reporting.h"

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL
#define NSEC_BTWN_1904_1970	(uint32_t) 2082844800U

namespace Ui
{
    class WombatRegistry;
}

class WombatRegistry : public QMainWindow
{
    Q_OBJECT

public:
    explicit WombatRegistry(QWidget* parent = 0);
    ~WombatRegistry();
    void LoadRegistryFile(void);
    //void LoadRegistryFile(QString regid, QString regname);

private slots:
    //void HideClicked();
    void OpenHive(void);
    void ManageTags(void);
    void PreviewReport(void);
    void ShowAbout(void);
    void KeySelected(void);
    void ValueSelected(void);
    void TagMenu(const QPoint &point);
    void SetTag(void);
    void CreateNewTag(void);
    void RemoveTag(void);
    void UpdateTagsMenu(void);
    //void DoubleClick(QTableWidgetItem* curitem);
    //void ImgHexMenu(const QPoint &point);
    void StatusUpdate(QString tmptext)
    {
        statuslabel->setText(tmptext);
    };
signals:
    //void TagCarved(QString ctitle, QString ctag);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::WombatRegistry* ui;
    QLabel* statuslabel;
    //HtmlViewer* htmlviewer;
    QString hivefilepath;
    QFile hivefile;
    void PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* curerr);
    QString DecryptRot13(QString encstr);
    QChar Rot13Char(QChar curchar);
    QString ConvertUnixTimeToString(uint32_t input);
    QString ConvertWindowsTimeToUnixTimeUTC(uint64_t input);
    //QString regfilepath;
    QMenu* tagmenu;
    //QTableWidgetItem* currow;
    QAction* newtagaction;
    QAction* remtagaction;
    QStringList registrytaglist;
    QString htmlentry;
    QString htmlvalue;
    QString hivename;
    QString reportstring;
    QByteArray reporttimezone;
    QStringList tags;
};

//Q_DECLARE_METATYPE(WombatRegistry*);

#endif // WOMBATREGISTRY_H
