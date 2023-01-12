#ifndef VIEWER_H
#define VIEWER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "hpdf.h"

#include "/usr/include/poppler/cpp/poppler-document.h"
#include "/usr/include/poppler/cpp/poppler-page.h"
#include "/usr/include/poppler/cpp/poppler-page-renderer.h"
#include "/usr/include/poppler/cpp/poppler-image.h"

#include "/usr/local/include/fox-1.7/fx.h"

class Viewer : public FXDialogBox
{
    FXDECLARE(Viewer)

    private:
        // need to use FXDC and call onpaint where the dc is used...
        // build the image elsewhere and then push it in the on paint...y
        //FXImage* report;
        FXVerticalFrame* vframe;
        FXImageView* imgview;
        /*
        FXScrollBar* vbar;
        FXScrollBar* hbar;
        //FXScrollArea* sarea;
        FXVerticalFrame* vframe;
        //FXVerticalFrame* vframe2;
        */
        /*
        FXFont* header1;
        FXFont* header2;
        FXFont* header3;
        FXFont* regular;
        FXFont* monospace;
        */
        HPDF_Doc pdf;
        HPDF_Page page;
        HPDF_Font defaultfont;
        HPDF_Font monofont;
        HPDF_REAL tw;
        HPDF_REAL height;
        HPDF_REAL width;
        HPDF_UINT i;

        poppler::document* pdfdoc;
        poppler::page* pdfpage;
        poppler::image pdfimage;
        poppler::page_renderer pdfrender;
        /*
        FXLabel* clabel;
        FXLabel* hlabel;
        */

    protected:
        Viewer() {}

    public:
        Viewer(FXWindow* parent, const FXString& title);
        //void GenerateContents(std::vector<std::string> tags);
        void GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags);
        //void GenerateTaggedItems(std::vector<std::string> tags, FXArray<FXString> taggedlist);
};

#endif // VIEWER_H
