#ifndef WOMBATREGISTRY_H
#define WOMBATREGISTRY_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <filesystem>
#include <byteswap.h>
#include "/usr/local/include/fox-1.7/fx.h"
#include "libregf.h"
#include "icons.h"
#include "managetags.h"
#include "aboutbox.h"

class WombatRegistry : public FXMainWindow
{
    FXDECLARE(WombatRegistry)

    private:
        FXVerticalFrame* mainframe;
        FXSplitter* hsplitter;
        FXSplitter* vsplitter;
        FXTreeList* treelist;
        FXToolBar* toolbar;
        FXText* plaintext;
        FXTable* tablelist;
        FXTreeItem* rootitem;
	FXIcon* openicon;
        FXButton* openbutton;
	FXIcon* managetagsicon;
	FXButton* managetagsbutton;
	FXIcon* previewicon;
	FXButton* previewbutton;
	FXIcon* publishicon;
	FXButton* publishbutton;
	FXIcon* abouticon;
	FXButton* aboutbutton;
        FXStatusBar* statusbar;
        std::string prevhivepath;
        std::string hivefilepath;
        std::vector<std::filesystem::path> hives;
        std::vector<std::string> tags;
        std::ifstream* filebufptr;

        //__builtin_bswapX
    protected:
        WombatRegistry() {}

        //long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_TREELIST = 1,
            ID_OPEN = 100,
            ID_TREESELECT = 101,
	    ID_MANAGETAGS = 102,
	    ID_PREVIEW = 103,
	    ID_PUBLISH = 104,
	    ID_ABOUT = 105,
	    ID_TABLESELECT = 106,
            ID_LAST
        };
        WombatRegistry(FXApp* a);
        long OpenHive(FXObject*, FXSelector, void*);
        long OpenTagManager(FXObject*, FXSelector, void*);
	long OpenAboutBox(FXObject*, FXSelector, void*);
        long KeySelected(FXObject*, FXSelector, void*);
	void PopulateChildKeys(libregf_key_t* curkey, FXTreeItem* curitem, libregf_error_t* regerr);
        virtual void create();

};

FXDEFMAP(WombatRegistry) WombatRegistryMap[]={
    FXMAPFUNC(SEL_CLICKED, WombatRegistry::ID_TREESELECT, WombatRegistry::KeySelected),
    FXMAPFUNC(SEL_COMMAND, WombatRegistry::ID_OPEN, WombatRegistry::OpenHive),
    FXMAPFUNC(SEL_COMMAND, WombatRegistry::ID_MANAGETAGS, WombatRegistry::OpenTagManager),
    FXMAPFUNC(SEL_COMMAND, WombatRegistry::ID_ABOUT, WombatRegistry::OpenAboutBox),
    //FXMAPFUNC(SEL_LEFTBUTTONPRESS, WombatRegistry::ID_TREELIST, WombatRegistry::onMouseDown),
    //FXMAPFUNC(SEL_LEFTBUTTONPRESS, WombatRegistry::ID_CANVAS, WombatRegistry::onMouseDown),
};

#endif // WOMBATREGISTRY_H
