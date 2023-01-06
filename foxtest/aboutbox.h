#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class AboutBox : public FXDialogBox
{
    FXDECLARE(AboutBox)

    private:
        FXVerticalFrame* mainframe;
	FXPNGImage* mainimage;
	FXImageFrame* imgframe;
        FXLabel* mainlabel;
	/*
        FXList* taglist;
        FXIcon* newicon;
        FXIcon* editicon;
        FXIcon* remicon;
        FXButton* newbutton;
        FXButton* editbutton;
        FXButton* rembutton;
	*/

        std::vector<std::string>* tags = NULL;

    protected:
        AboutBox() {}

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
        AboutBox(FXWindow* parent, const FXString& title);
	/*
        void SetTagList(std::vector<std::string>* tagslist);
        void UpdateList();
        long AddTag(FXObject*, FXSelector, void*);
        long ModifyTag(FXObject*, FXSelector, void*);
        long RemoveTag(FXObject*, FXSelector, void*);
        long ListSelection(FXObject*, FXSelector, void*);
	*/

};

/*
FXDEFMAP(AboutBox) AboutBoxMap[]={
    FXMAPFUNC(SEL_COMMAND, AboutBox::ID_NEWTAG, AboutBox::AddTag),
    FXMAPFUNC(SEL_COMMAND, AboutBox::ID_EDITTAG, AboutBox::ModifyTag),
    FXMAPFUNC(SEL_COMMAND, AboutBox::ID_REMTAG, AboutBox::RemoveTag),
    FXMAPFUNC(SEL_CLICKED, AboutBox::ID_LISTSELECT, AboutBox::ListSelection),
};
*/

#endif // ABOUTBOX_H
