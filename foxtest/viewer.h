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
        // need to use FXDC and call onpaint where the dc is used...
        // build the image elsewhere and then push it in the on paint...y
        FXImage* report;
        FXVerticalFrame* vframe;
        /*
        FXScrollBar* vbar;
        FXScrollBar* hbar;
        //FXScrollArea* sarea;
        FXVerticalFrame* vframe;
        //FXVerticalFrame* vframe2;
        */
        FXFont* header1;
        FXFont* header2;
        FXFont* header3;
        FXFont* regular;
        FXFont* monospace;
        /*
        FXLabel* clabel;
        FXLabel* hlabel;
        */

    protected:
        Viewer() {}

    public:
        enum
        {
            ID_CANVAS,
            //ID_SCROLLV,
            //ID_SCROLLH,
            ID_LAST
        };
        long PaintPreview(FXObject*, FXSelector, void*);
        Viewer(FXWindow* parent, const FXString& title);
        void GenerateContents(std::vector<std::string> tags);
        void GenerateTaggedItems(std::vector<std::string> tags, FXArray<FXString> taggedlist);
};

FXDEFMAP(Viewer) ViewerMap[]={
    FXMAPFUNC(SEL_PAINT, Viewer::ID_CANVAS, Viewer::PaintPreview),
};

#endif // VIEWER_H
