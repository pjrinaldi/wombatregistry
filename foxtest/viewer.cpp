#include "viewer.h"

FXIMPLEMENT(Viewer,FXDialogBox,NULL, 0)

Viewer::Viewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 600, 0,0,0,0, 0, 0)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    maintext = new FXText(mainframe, NULL, 0, TEXT_READONLY|TEXT_WORDWRAP);
    maintext->setVisibleRows(80);
    maintext->setVisibleColumns(80);
    /*
    mainimage = new FXPNGImage(this->getApp(), aboutwombat);
    imgframe = new FXImageFrame(mainframe, mainimage, FRAME_NONE);
    */
    FXString abouttext = "\n\nAbout Wombat Registry v0.4\n\n";
    abouttext += "License: GPLv2\n";
    abouttext += "Copyright: 2022-2023 Pasquale J. Rinaldi, Jr.\n";
    abouttext += "Email: pjrinaldi@gmail.com\n\n";
    abouttext += "This program incorporates Fox Toolkit v1.7.6.81 (LGPLv2),\n";
    abouttext += "some of the elementary icons (GPLv3),\n";
    abouttext += " and tango icons (public domain).\n";
    abouttext += " Registry parsing by libregf library (LGPLv3) from Joachim Metz\n\n";
    abouttext += "Wombat icon and about image designed by Lindsay Pargman\n.";
    abouttext += "Design Implementation ideas brainstormed with Mark Smith.\n\n";
    abouttext += "If I have forgotten to mention any library or anyone,\n";
    abouttext += " let me know and I will update this content accordingly.";
    maintext->setText(abouttext);
    /*
    mainlabel = new FXLabel(mainframe, abouttext, NULL, JUSTIFY_LEFT);
    this->setBackColor(FX::colorFromName("white"));
    mainframe->setBackColor(FX::colorFromName("white"));
    mainlabel->setBackColor(FX::colorFromName("white"));
    */

    /*
    taglist = new FXList(mainframe, this, ID_LISTSELECT, LIST_SINGLESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    buttonframe = new FXHorizontalFrame(mainframe, LAYOUT_BOTTOM|LAYOUT_FILL_X);
    newicon = new FXPNGIcon(this->getApp(), bookmarknew);
    newbutton = new FXButton(buttonframe, "New Tag", newicon, this, ID_NEWTAG);
    editicon = new FXPNGIcon(this->getApp(), bookmark);
    editbutton = new FXButton(buttonframe, "Modify Tag", editicon, this, ID_EDITTAG);
    remicon = new FXPNGIcon(this->getApp(), bookmarkrem);
    rembutton = new FXButton(buttonframe, "Remove Tag", remicon, this, ID_REMTAG);
    rembutton->disable();
    editbutton->disable();
    */
}

/*
void Viewer::SetTagList(std::vector<std::string>* tagslist)
{
    tags = tagslist;
    if(tags != NULL)
        UpdateList();
}

void Viewer::UpdateList()
{
    taglist->clearItems();
    for(int i=0; i < tags->size(); i++)
        taglist->appendItem(new FXListItem(FXString(tags->at(i).c_str())));
}

long Viewer::AddTag(FXObject*, FXSelector, void*)
{
    FXString tagstr = "";
    bool isset = FXInputDialog::getString(tagstr, this, "Enter Tag Name", "New Tag");
    if(isset)
    {
        tags->push_back(tagstr.text());
        UpdateList();
    }
    return 1;
}

long Viewer::ModifyTag(FXObject*, FXSelector, void*)
{
    FXint curitem = taglist->getCurrentItem();
    std::string curtext = tags->at(curitem);
    FXString modtagname = "";
    bool isset = FXInputDialog::getString(modtagname, this, "Modify Tag Name", "Modify Tag");
    if(isset)
    {
        taglist->getItem(curitem)->setText(modtagname);
        taglist->update();
        tags->at(curitem) = modtagname.text();
    }
    return 1;
}

long Viewer::RemoveTag(FXObject*, FXSelector, void*)
{
    int curitem = taglist->getCurrentItem();
    std::string curtext = tags->at(curitem);
    tags->erase(tags->begin() + curitem);
    rembutton->disable();
    editbutton->disable();
    UpdateList();
    return 1;
}

long Viewer::ListSelection(FXObject*, FXSelector, void*)
{
    rembutton->enable();
    editbutton->enable();
    return 1;
}
*/
