#ifndef WOMBATREGISTRY_H
#define WOMBATREGISTRY_H

// Copyright 2022-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

//#include "wombatinclude.h"
//#include "globals.h"
//#include "wombatfunctions.h"
//#include "ui_registryviewer.h"
#include "ui_wombatregistry.h"
#include "libregf.h"
//#include "tagging.h"
//#include "reporting.h"

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
    void LoadRegistryFile(QString regid, QString regname);

private slots:
    void HideClicked();
    void KeySelected(void);
    void ValueSelected(void);
    void TagMenu(const QPoint &point);
    void SetTag(void);
    //void CreateNewTag(void);
    void RemoveTag(void);
    //void DoubleClick(QTableWidgetItem* curitem);
    //void ImgHexMenu(const QPoint &point);
signals:
    //void TagCarved(QString ctitle, QString ctag);
protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::WombatRegistry* ui;
    void PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* curerr);
    QString DecryptRot13(QString encstr);
    QChar Rot13Char(QChar curchar);
    QString regfilepath;
    QMenu* tagmenu;
    QStringList registrytaglist;
    QString htmlentry;
    QString htmlvalue;
};

//Q_DECLARE_METATYPE(WombatRegistry*);

#endif // WOMBATREGISTRY_H
