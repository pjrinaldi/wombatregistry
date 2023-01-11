#include "viewer.h"

FXIMPLEMENT(Viewer,FXDialogBox, NULL, 0)

Viewer::Viewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 300, 0,0,0,0, 0, 0)
{
    /*
    header1 = new FXFont(this->getApp(), "Roman", 18, FXFont::Bold);
    header2 = new FXFont(this->getApp(), "Roman", 14, FXFont::Bold);
    header3 = new FXFont(this->getApp(), "Roman", 12, FXFont::Bold);
    regular = new FXFont(this->getApp(), "Roman", 10);
    monospace = new FXFont(this->getApp(), "monospace");
    */
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    imgview = new FXImageView(vframe, NULL, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //report = new FXImage(this->getApp(), nullptr, IMAGE_DITHER|IMAGE_SHMI|IMAGE_SHMP, 800, 600);
    //report->create();
    //report->detach();
    //canvas = new FXCanvas(vframe, this, ID_CANVAS, LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_FILL_ROW|LAYOUT_FILL_COLUMN);
    /*
    //vframe2 = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 10, 10, 10, 10);
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 10, 10, 10, 10);
    //vbar = new FXScrollBar(vframe2, this, ID_SCROLLV);
    //hbar = new FXScrollBar(vframe2, this, ID_SCROLLH, SCROLLBAR_HORIZONTAL);
    //sarea = new FXScrollArea(vframe, 0, 0, 0, 0, 0);
    this->setBackColor(FX::colorFromName("white"));
    vframe->setBackColor(FX::colorFromName("white"));
    hlabel = new FXLabel(vframe, "Wombat Registry Report");
    hlabel->setFont(header1);
    hlabel->setBackColor(FX::colorFromName("white"));
    clabel = new FXLabel(vframe, "Contents");
    clabel->setFont(header2);
    clabel->setBackColor(FX::colorFromName("white"));
    */
    // GENERATE PDF
    pdf = HPDF_New(NULL, NULL);
    page = HPDF_AddPage(pdf);
    height = HPDF_Page_GetHeight(page);
    width = HPDF_Page_GetWidth(page);
    defaultfont = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, defaultfont, 24);
    tw = HPDF_Page_TextWidth(page, "Wombat Registry Report");
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, (width - tw) / 2, height - 50);
    HPDF_Page_ShowText(page, "Wombat Registry Report");
    HPDF_Page_EndText(page);

    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 60, height - 80);
    HPDF_Page_SetFontAndSize(page, defaultfont, 16);
    HPDF_Page_ShowText(page, "Contents");
    HPDF_Page_EndText(page);

    HPDF_SaveToFile(pdf, "tmp.pdf");
    HPDF_Free(pdf);

    // RENDER PDF TO AN IMAGE
    pdfdoc = poppler::document::load_from_file("tmp.pdf");
    pdfpage = pdfdoc->create_page(0);
    pdfrender.set_render_hint(poppler::page_renderer::antialiasing, true);
    pdfrender.set_render_hint(poppler::page_renderer::text_antialiasing, true);
    pdfimage = pdfrender.render_page(pdfpage, 72.0, 72.0, -1, -1, -1, -1, poppler::rotate_0);
    pdfimage.save("tmp.png", "PNG");
    // Load Image
    FXImage* img = new FXPNGImage(this->getApp(), NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    FXFileStream stream;
    this->getApp()->beginWaitCursor();
    stream.open("tmp.png", FXStreamLoad);
    img->loadPixels(stream);
    stream.close();
    img->create();
    imgview->setImage(img);
    this->getApp()->endWaitCursor();
    
}

void Viewer::GenerateContents(std::vector<std::string> tags)
{
    /*
    for(int i=0; i < tags.size(); i++)
    {
        FXLabel* tmplabel = new FXLabel(vframe, tags.at(i).c_str());
        tmplabel->setBackColor(FX::colorFromName("white"));
    }
    */
}

void Viewer::GenerateTaggedItems(std::vector<std::string> tags, FXArray<FXString> taggedlist)
{
    /*
    FXLabel* tagitemheader = new FXLabel(vframe, "Tagged Items");
    tagitemheader->setFont(header2);
    tagitemheader->setBackColor(FX::colorFromName("white"));
    for(int i=0; i < tags.size(); i++)
    {
        FXLabel* curtaghdr = new FXLabel(vframe, tags.at(i).c_str());
        //FXint labelwidth = curtaghdr->labelWidth(tags.at(i).c_str());
        curtaghdr->setBackColor(FX::colorFromName("white"));
        curtaghdr->setFont(header3);
        for(int j=0; j < taggedlist.no(); j++)
        {
            //std::cout << "tagged item:" << taggedlist.at(j).text() << std::endl;
            std::size_t found = taggedlist.at(j).find("|");
            std::size_t rfound = taggedlist.at(j).rfind("|");
            std::cout << "found: " << found << " rfound: " << rfound << std::endl;
            FXString itemtag = taggedlist.at(j).mid(0, found);
            FXString itemhdr = taggedlist.at(j).mid(found+1, rfound - found - 1);
            FXString itemcon = taggedlist.at(j).mid(rfound+1, taggedlist.at(j).length() - rfound);
            if(itemtag == tags.at(i).c_str())
            {
                std::cout << "taggedlist length:" << taggedlist.at(j).length() << std::endl;
                std::cout << "itemtag:" << itemtag.text() << std::endl;
                std::cout << "itemhdr:" << itemhdr.text() << std::endl;
                std::cout << "itemcon:" << itemcon.text() << std::endl;
                FXLabel* tagitemlabel = new FXLabel(vframe, itemhdr);
                vframe->setWidth(tagitemlabel->getWidth());
                tagitemlabel->setFont(regular);
                tagitemlabel->setBackColor(FX::colorFromName("white"));
    //plaintext = new FXText(hsplitter);
    //plaintext->setFont(plainfont);
    //plaintext->setEditable(false);
                FXSplitter* hsplitter = new FXSplitter(vframe, SPLITTER_VERTICAL);
                FXText* itemcontent = new FXText(hsplitter);
                itemcontent->setFont(monospace);
                itemcontent->setEditable(false);
                //FXLabel* tagitemcontent = new FXLabel(vframe, itemcon);
                //tagitemcontent->setFont(monospace);
                //tagitemcontent->setBackColor(FX::colorFromName("white"));
            }
        }
    }
    */

    /*  
    curcontent += "<h2>Tagged Items</h2>";
    for(int i=0; i < tags.count(); i++)
    {
        curcontent += "<div id='t" + QString::number(i) + "'><h3>" + tags.at(i) + "</h3><br/><table>";
        for(int j=0; j < taggeditems.count(); j++)
        {
            if(taggeditems.at(j).split("|", Qt::SkipEmptyParts).at(0) == tags.at(i))
            {
                curcontent += "<tr><td style='" + ReturnCssString(11) + "'>" + taggeditems.at(j).split("|").at(1);
                curcontent += "<div><pre>";
                curcontent += taggeditems.at(j).split("|").at(2).toUtf8();
                curcontent += "</pre></div></td></tr>";
            }
        }
        curcontent += "</table></div><br/>\n";
    }
    reportstring = prehtml + curcontent + psthtml;

     */ 
}
