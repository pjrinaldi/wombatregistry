#ifndef VIEWER_H
#define VIEWER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class Viewer : public FXDialogBox
{
    FXDECLARE(Viewer)

    private:
        //FXSplitter* vsplitter;
        FXVerticalFrame* vframe;
        //FXCanvas* canvas;
        //FXText* maintext;
        FXFont* header1;
        FXFont* header2;
        FXFont* header3;
        FXFont* regular;
        FXLabel* clabel;
        FXLabel* hlabel;
        /*
	FXPNGImage* mainimage;
	FXImageFrame* imgframe;
        FXLabel* mainlabel;
        */
	/*
        FXList* taglist;
        FXIcon* newicon;
        FXIcon* editicon;
        FXIcon* remicon;
        FXButton* newbutton;
        FXButton* editbutton;
        FXButton* rembutton;
	*/

        //std::vector<std::string>* tags = NULL;

    protected:
        Viewer() {}

    public:
        enum
        {
            ID_CANVAS = 100,
            /*
            ID_TREELIST = 1,
            ID_LISTSELECT = 100,
            ID_NEWTAG = 101,
            ID_EDITTAG = 102,
            ID_REMTAG = 103,
            */
            ID_LAST
        };
        Viewer(FXWindow* parent, const FXString& title);
        void AppendTitle();
        void AppendContents();
        /*
        void AppendHeader1(FXString h1text);
        void AppendHeader2(FXString h2text);
        void AppendHeader3(FXString h3text);
        void AppendRegular(FXString regtext);
        */
        void ClearText();
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
FXDEFMAP(Viewer) ViewerMap[]={
    FXMAPFUNC(SEL_COMMAND, Viewer::ID_NEWTAG, Viewer::AddTag),
    FXMAPFUNC(SEL_COMMAND, Viewer::ID_EDITTAG, Viewer::ModifyTag),
    FXMAPFUNC(SEL_COMMAND, Viewer::ID_REMTAG, Viewer::RemoveTag),
    FXMAPFUNC(SEL_CLICKED, Viewer::ID_LISTSELECT, Viewer::ListSelection),
};
*/

#endif // VIEWER_H
