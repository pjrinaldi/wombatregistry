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
        FXVerticalFrame* vframe;
        FXFont* header1;
        FXFont* header2;
        FXFont* header3;
        FXFont* regular;
        FXFont* monospace;
        FXLabel* clabel;
        FXLabel* hlabel;

    protected:
        Viewer() {}

    public:
        Viewer(FXWindow* parent, const FXString& title);
        void GenerateContents(std::vector<std::string> tags);
        void GenerateTaggedItems(std::vector<std::string>taggeditems);
};

#endif // VIEWER_H
