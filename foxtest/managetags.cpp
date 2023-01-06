#include "managetags.h"

FXIMPLEMENT(ManageTags,FXDialogBox,ManageTagsMap,ARRAYNUMBER(ManageTagsMap))

ManageTags::ManageTags(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE, 0, 0, 320, 260, 0,0,0,0, 4, 4)
{
    /*
        FXVerticalFrame* mainframe;
        FXHorizontalFrame* buttonframe;
        FXLabel* mainlabel;
        FXIcon* newicon;
        FXIcon* editicon;
        FXIcon* remicon;
     */ 
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    mainlabel = new FXLabel(mainframe, "Case Tags");
    taglist = new FXList(mainframe, this, ID_LISTSELECT, LIST_SINGLESELECT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    buttonframe = new FXHorizontalFrame(mainframe, LAYOUT_BOTTOM|LAYOUT_FILL_X);
    newicon = new FXPNGIcon(this->getApp(), bookmarknew);
    newbutton = new FXButton(buttonframe, "New Tag", newicon, this, ID_NEWTAG);
    editicon = new FXPNGIcon(this->getApp(), bookmark);
    editbutton = new FXButton(buttonframe, "Modify Tag", editicon, this, ID_EDITTAG);
    remicon = new FXPNGIcon(this->getApp(), bookmarkrem);
    rembutton = new FXButton(buttonframe, "Remove Tag", remicon, this, ID_REMTAG);
    /*
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    toolbar = new FXToolBar(mainframe, this, LAYOUT_TOP|LAYOUT_LEFT);
    vsplitter = new FXSplitter(mainframe, SPLITTER_NORMAL|LAYOUT_FILL);
    statusbar = new FXStatusBar(mainframe, LAYOUT_BOTTOM|LAYOUT_LEFT|LAYOUT_FILL_X);
    treelist = new FXTreeList(vsplitter, this, ID_TREESELECT, TREELIST_SHOWS_LINES|TREELIST_SINGLESELECT|TREELIST_ROOT_BOXES|TREELIST_SHOWS_BOXES);
    hsplitter = new FXSplitter(vsplitter, SPLITTER_VERTICAL);
    tablelist = new FXTable(hsplitter);
    plaintext = new FXText(hsplitter);
    openicon = new FXPNGIcon(this->getApp(), folderopen);
    openbutton = new FXButton(toolbar, "", openicon, this, ID_OPEN, BUTTON_TOOLBAR);
    managetagsicon = new FXPNGIcon(this->getApp(), managetags);
    managetagsbutton = new FXButton(toolbar, "", managetagsicon, this, ID_MANAGETAGS, BUTTON_TOOLBAR);
    previewicon = new FXPNGIcon(this->getApp(), reportpreview1);
    previewbutton = new FXButton(toolbar, "", previewicon, this, ID_PREVIEW, BUTTON_TOOLBAR);
    publishicon = new FXPNGIcon(this->getApp(), paperairplane2);
    publishbutton = new FXButton(toolbar, "", publishicon, this, ID_PUBLISH, BUTTON_TOOLBAR);
    abouticon = new FXPNGIcon(this->getApp(), helpcontents);
    aboutbutton = new FXButton(toolbar, "", abouticon, this, ID_ABOUT, BUTTON_TOOLBAR);
    statusbar->getStatusLine()->setNormalText("Open a Hive File to Begin");

    //rootitem = treelist->getFirstItem();

    //rootitem = new FXTreeItem("Root Item");

    //std::cout << "firstitem:" << rootitem;
    //FXTreeItem* mainitem = new FXTreeItem("Root Item");
    //treelist->setAnchorItem(rootitem);

    //treelist->appendItem(0, rootitem);

    //treelist->makeItemVisible(rootitem);
    //treelist->appendItem(0, mainitem);
    //treelist->appendItem(mainitem, new FXTreeItem("Test 2"));
    */
    rembutton->disable();
    editbutton->disable();
}

void ManageTags::SetTagList(std::vector<std::string>* tagslist)
{
    tags = tagslist;
    if(tags != NULL)
        UpdateList();
}

void ManageTags::UpdateList()
{
    taglist->clearItems();
    for(int i=0; i < tags->size(); i++)
        taglist->appendItem(new FXListItem(FXString(tags->at(i).c_str())));
}

long ManageTags::AddTag(FXObject*, FXSelector, void*)
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

long ManageTags::ModifyTag(FXObject*, FXSelector, void*)
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
    /*
    QString tmpstr = "";
    QString modtagname = "";
    modtagdialog->setTextValue(selectedtag);
    if(modtagdialog->exec())
        modtagname = modtagdialog->textValue();
    if(!modtagname.isEmpty())
    {
	ui->listWidget->currentItem()->setText(modtagname);
	tags->replace(tagindex, modtagname);
    }
    */
    return 1;
}

long ManageTags::RemoveTag(FXObject*, FXSelector, void*)
{
    return 1;
}

long ManageTags::ListSelection(FXObject*, FXSelector, void*)
{
    rembutton->enable();
    editbutton->enable();
    return 1;
}
