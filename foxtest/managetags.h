#ifndef MANAGETAGS_H
#define MANAGETAGS_H

#include "/usr/local/include/fox-1.7/fx.h"

class ManageTags : public FXDialogBox
{
    FXDECLARE(ManageTags)

    private:
        FXVerticalFrame* mainframe;
        FXHorizontalFrame* buttonframe;
        FXLabel* mainlabel;
        FXList* taglist;
        FXIcon* newicon;
        FXIcon* editicon;
        FXIcon* remicon;
        FXButton* newbutton;
        FXButton* editbutton;
        FXButton* rembutton;

    protected:
        ManageTags() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_TREELIST = 1,
            ID_LISTSELECT = 100,
            ID_NEWTAG = 101,
            ID_EDITTAG = 102,
            ID_REMTAG = 103,
            ID_LAST
        };
        ManageTags(FXWindow* parent, const FXString& title);
        //long OpenHive(FXObject*, FXSelector, void*);
        //long KeySelected(FXObject*, FXSelector, void*);

};

FXDEFMAP(ManageTags) ManageTagsMap[]={
    //FXMAPFUNC(SEL_CLICKED, WombatRegistry::ID_TREESELECT, WombatRegistry::KeySelected),
    //FXMAPFUNC(SEL_COMMAND, WombatRegistry::ID_OPEN, WombatRegistry::OpenHive),
    //FXMAPFUNC(SEL_LEFTBUTTONPRESS, WombatRegistry::ID_TREELIST, WombatRegistry::onMouseDown),
    //FXMAPFUNC(SEL_LEFTBUTTONPRESS, WombatRegistry::ID_CANVAS, WombatRegistry::onMouseDown),
};

#endif // MANAGETAGS_H
