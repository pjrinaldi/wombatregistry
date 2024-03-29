#include "wombatregistry.h"

// Copyright 2013-2023 Pasquale J. Rinaldi, Jr.
// Distributed under the terms of the GNU General Public License version 2

FXIMPLEMENT(WombatRegistry,FXMainWindow,WombatRegistryMap,ARRAYNUMBER(WombatRegistryMap))

WombatRegistry::WombatRegistry(FXApp* a):FXMainWindow(a, "Wombat Registry Forensics", new FXICOIcon(a, wombat_32), new FXICOIcon(a, wombat_32), DECOR_ALL, 0, 0, 1024, 768)
{
    new FXToolTip(this->getApp(), TOOLTIP_PERMANENT);
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    toolbar = new FXToolBar(mainframe, this, LAYOUT_TOP|LAYOUT_LEFT);
    vsplitter = new FXSplitter(mainframe, SPLITTER_NORMAL|LAYOUT_FILL);
    statusbar = new FXStatusBar(mainframe, LAYOUT_BOTTOM|LAYOUT_LEFT|LAYOUT_FILL_X);
    treelist = new FXTreeList(vsplitter, this, ID_TREESELECT, TREELIST_SHOWS_LINES|TREELIST_SINGLESELECT|TREELIST_ROOT_BOXES|TREELIST_SHOWS_BOXES);
    treelist->setWidth(this->getWidth() / 4);
    hsplitter = new FXSplitter(vsplitter, SPLITTER_VERTICAL);
    tablelist = new FXTable(hsplitter, this, ID_TABLESELECT, TABLE_COL_SIZABLE|LAYOUT_FILL_X, LAYOUT_FILL_Y);
    plainfont = new FXFont(a, "monospace");
    plaintext = new FXText(hsplitter);
    plaintext->setFont(plainfont);
    plaintext->setEditable(false);
    tablelist->setHeight(this->getHeight() / 3);
    tablelist->setEditable(false);
    tablelist->setTableSize(4, 3);
    tablelist->setColumnText(0, "Tag");
    tablelist->setColumnText(1, "Value Name");
    tablelist->setColumnText(2, "Value Type");
    tablelist->setColumnHeaderHeight(tablelist->getColumnHeaderHeight() + 5);
    tablelist->setRowHeaderWidth(0);
    openicon = new FXPNGIcon(this->getApp(), folderopen);
    openbutton = new FXButton(toolbar, "", openicon, this, ID_OPEN, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    openbutton->setTipText("Open Registry File");
    closeicon = new FXPNGIcon(this->getApp(), closehive);
    closebutton = new FXButton(toolbar, "", closeicon, this, ID_CLOSE, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    closebutton->setTipText("Close Selected Hive");
    managetagsicon = new FXPNGIcon(this->getApp(), managetags);
    managetagsbutton = new FXButton(toolbar, "", managetagsicon, this, ID_MANAGETAGS, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    managetagsbutton->setTipText("Manage Tags");
    previewicon = new FXPNGIcon(this->getApp(), reportpreview1);
    previewbutton = new FXButton(toolbar, "", previewicon, this, ID_PREVIEW, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    previewbutton->setTipText("Preview Report");
    publishicon = new FXPNGIcon(this->getApp(), paperairplane2);
    publishbutton = new FXButton(toolbar, "", publishicon, this, ID_PUBLISH, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    publishbutton->setTipText("Publish Report");
    abouticon = new FXPNGIcon(this->getApp(), helpcontents);
    aboutbutton = new FXButton(toolbar, "", abouticon, this, ID_ABOUT, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    aboutbutton->setTipText("About Wombat Registry Forensics");
    statusbar->getStatusLine()->setNormalText("Open a Hive File to Begin");
    hives.clear();
    tags.clear();
    taggedlist.clear();
}

void WombatRegistry::create()
{
    FXMainWindow::create();
    show(PLACEMENT_SCREEN);
    if(this->getApp()->getArgc() == 2)
    {
        cmdhivepath = std::string(this->getApp()->getArgv()[1]);
        int ret = OpenHive(NULL, 0, NULL);
    }
}

long WombatRegistry::TagMenu(FXObject*, FXSelector, void* ptr)
{
    FXEvent* event = (FXEvent*)ptr;
    if(tablelist->getCurrentRow() > -1 && !tablelist->getItemText(tablelist->getCurrentRow(), 1).empty())
    {
        if(!event->moved)
        {
            FXMenuPane tagmenu(this, POPUP_SHRINKWRAP);
            new FXMenuCommand(&tagmenu, "Create New Tag", new FXPNGIcon(this->getApp(), bookmarknew), this, ID_NEWTAG);
            new FXMenuSeparator(&tagmenu);
            for(int i=0; i < tags.size(); i++)
            {
                new FXMenuCommand(&tagmenu, FXString(tags.at(i).c_str()), new FXPNGIcon(this->getApp(), bookmark), this, ID_SETTAG);
            }
            new FXMenuSeparator(&tagmenu);
            new FXMenuCommand(&tagmenu, "Remove Tag", new FXPNGIcon(this->getApp(), bookmarkrem), this, ID_REMTAG);
            tagmenu.forceRefresh();
            tagmenu.create();
            tagmenu.popup(nullptr, event->root_x, event->root_y);
            getApp()->runModalWhileShown(&tagmenu);
        }
    }
    return 1;
}

long WombatRegistry::CreateNewTag(FXObject*, FXSelector, void*)
{
    FXString tagstr = "";
    bool isset = FXInputDialog::getString(tagstr, this, "Enter Tag Name", "New Tag");
    if(isset)
    {
        tags.push_back(tagstr.text());
        tablelist->setItemText(tablelist->getCurrentRow(), 0, tagstr);
	if(tagstr.length() > 5)
	    tablelist->fitColumnsToContents(0);
    }
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    for(int i=0; i < taggedlist.no(); i++)
    {
        if(taggedlist.at(i).contains(idkeyvalue))
            taggedlist.erase(i);
    }
    taggedlist.append(tagstr + "|" + idkeyvalue + "|" + plaintext->getText());

    return 1;
}

long WombatRegistry::RemoveTag(FXObject*, FXSelector, void*)
{
    tablelist->setItemText(tablelist->getCurrentRow(), 0, "");
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    for(int i=0; i < taggedlist.no(); i++)
    {
        if(taggedlist.at(i).contains(idkeyvalue))
            taggedlist.erase(i);
    }

    return 1;
}

long WombatRegistry::KeySelected(FXObject* sender, FXSelector, void*)
{
    size_t namemax = 0;
    FXTreeItem* curitem = treelist->getCurrentItem();
    bool toplevel = false;
    std::vector<FXString> pathitems;
    pathitems.clear();
    pathitems.push_back(curitem->getText());
    FXTreeItem* parent;
    FXTreeItem* child;
    child = curitem;
    while(toplevel == false)
    {
	parent = child->getParent();
	if(parent == NULL)
        {
	    toplevel = true;
        }
	else
	{
	    pathitems.push_back(parent->getText());
	    child = parent;
	}
    }
    FXString rootstring = "";
    FXString hivepath = "";
    GetRootString(curitem, &rootstring);
    for(int i=0; i < hives.size(); i++)
    {
        if(rootstring.contains(FXString(hives.at(i).string().c_str())))
            hivepath = FXString(hives.at(i).string().c_str());
    }
    //std::cout << "hivepath: " << hivepath.text() << std::endl;
    FXString keypath = "";
    int found = pathitems.at(pathitems.size() - 1).find(" (");
    //std::cout << "found: " << found << std::endl;
    FXString rootstr = pathitems.at(pathitems.size() - 1).left(found).upper();
    //std::cout << "rootstr: " << rootstr.text() << std::endl;
    //keypath += "\\" + rootstr;
    for(int i=pathitems.size() - 2; i > -1; i--)
    {
	keypath += "\\" + pathitems.at(i);
    }
    StatusUpdate(rootstr + keypath);
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    libregf_file_open(regfile, hivepath.text(), LIBREGF_OPEN_READ, &regerr);
    //libregf_file_open(regfile, hivefilepath.c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_key_t* curkey = NULL;
    libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.text()), keypath.count(), &curkey, &regerr);
    // valid key, get values...
    int valuecount = 0;
    libregf_key_get_number_of_values(curkey, &valuecount, &regerr);
    //std::cout << "curkey: " << curkey << std::endl;
    //std::cout << "value count: " << valuecount << std::endl;
    tablelist->clearItems();
    plaintext->setText("");
    tablelist->setTableSize(valuecount, 3);
    tablelist->setColumnText(0, "Tag");
    tablelist->setColumnText(1, "Value Name");
    tablelist->setColumnText(2, "Value Type");
    FXString tagstr = "";
    if(valuecount == 0) // no values, so create empty key
    {
	tablelist->setTableSize(1, 3);
	tablelist->setColumnText(0, "Tag");
	tablelist->setColumnText(1, "Value Name");
	tablelist->setColumnText(2, "Value Type");
	FXString curtagvalue = keypath + "\\" + "(empty)";
	tablelist->setItemText(0, 1, "(empty)");
	tablelist->setItemText(0, 2, "0x00");
        for(int j=0; j < taggedlist.no(); j++)
        {
            if(taggedlist.at(j).contains(curtagvalue))
            {
                std::size_t found = taggedlist.at(j).find("|");
                tagstr = taggedlist.at(j).left(found);
            }
        }
        tablelist->setItemText(0, 0, tagstr);
    }
    for(int i=0; i < valuecount; i++)
    {
	libregf_value_t* curval = NULL;
	libregf_key_get_value(curkey, i, &curval, &regerr);
	size_t namesize = 0;
	libregf_value_get_utf8_name_size(curval, &namesize, &regerr);
	if(namemax < namesize)
	    namemax = namesize;
	uint8_t name[namesize];
	libregf_value_get_utf8_name(curval, name, namesize, &regerr);
	uint32_t type = 0;
	libregf_value_get_value_type(curval, &type, &regerr);
	FXString curtagvalue = keypath + "\\";
	if(namesize == 0)
	{
	    curtagvalue += "(unnamed)";
	    tablelist->setItemText(i, 1, "(unnamed)");
	    FXString typestr = FXString::value(type, 16);
	    tablelist->setItemText(i, 2, typestr);
	}
	else
	{
	    curtagvalue += reinterpret_cast<char*>(name);
	    FXString valuetypestr = "";
	    tablelist->setItemText(i, 1, reinterpret_cast<char*>(name));
            if(type == 0x00) // none
            {
            }
            else if(type == 0x01) // reg_sz
            {
                valuetypestr = "REG_SZ";
            }
            else if(type == 0x02) // reg_expand_sz
            {
                valuetypestr = "REG_EXPAND_SZ";
            }
            else if(type == 0x03) // reg_binary
            {
                valuetypestr = "REG_BINARY";
            }
            else if(type == 0x04) // reg_dword reg_dword_little_endian (4 bytes)
            {
                valuetypestr = "REG_DWORD";
            }
            else if(type == 0x05) // reg_dword_big_endian (4 bytes)
            {
                valuetypestr = "REG_DWORD_BIG_ENDIAN";
            }
            else if(type == 0x06) // reg_link
            {
                valuetypestr = "REG_LINK";
            }
            else if(type == 0x07) // reg_multi_sz
            {
                valuetypestr = "REG_MULTI_SZ";
            }
            else if(type == 0x08) // reg_resource_list
            {
                valuetypestr = "REG_RESOURCE_LIST";
            }
            else if(type == 0x09) // reg_full_resource_descriptor
            {
                valuetypestr = "REG_FULL_RESOURCE_DESCRIPTOR";
            }
            else if(type == 0x0a) // reg_resource_requirements_list
            {
                valuetypestr = "REG_RESOURCE_REQUIREMENTS_LIST";
            }
            else if(type == 0x0b) // reg_qword_little_endian (8 bytes)
            {
                valuetypestr = "REG_QWORD";
            }
            else
            {
            }
	    tablelist->setItemText(i, 2, valuetypestr);
	}
        for(int j=0; j < taggedlist.no(); j++)
        {
            if(taggedlist.at(j).contains(curtagvalue))
            {
                std::size_t found = taggedlist.at(j).find("|");
                tagstr = taggedlist.at(j).left(found);
                tablelist->setItemText(i, 0, tagstr); 
            }
        }
	libregf_value_free(&curval, &regerr);
    }
    libregf_key_free(&curkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
    AlignColumn(tablelist, 0, FXTableItem::LEFT);
    AlignColumn(tablelist, 1, FXTableItem::LEFT);
    AlignColumn(tablelist, 2, FXTableItem::LEFT);
    if(!tagstr.empty() && tagstr.length() > 5)
	tablelist->fitColumnsToContents(0);
    if(namemax > 12)
	tablelist->fitColumnsToContents(1);
    tablelist->setCurrentItem(0, 0);
    tablelist->selectRow(0, true);

    return 1;
}

void WombatRegistry::GetRootString(FXTreeItem* curitem, FXString* rootstring)
{
    if(curitem->getParent() == NULL)
	*rootstring = curitem->getText();
    else
        GetRootString(curitem->getParent(), rootstring);
}

FXString WombatRegistry::ConvertUnixTimeToString(uint32_t input)
{
    time_t crtimet = (time_t)input;
    struct tm* dt;
    dt = gmtime(&crtimet);
    char timestr[30];
    strftime(timestr, sizeof(timestr), "%m/%d/%Y %I:%M:%S %p", dt);

    return timestr;
}

FXString WombatRegistry::ConvertWindowsTimeToUnixTimeUTC(uint64_t input)
{
    uint64_t temp;
    temp = input / TICKS_PER_SECOND; //convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  //subtract number of seconds between epochs
    time_t crtimet = (time_t)temp;
    struct tm* dt;
    dt = gmtime(&crtimet);
    char timestr[30];
    strftime(timestr, sizeof(timestr), "%m/%d/%Y %I:%M:%S %p", dt);

    return timestr;
}

long WombatRegistry::ValueSelected(FXObject*, FXSelector, void*)
{
    if(tablelist->getCurrentRow() > -1)
    {
	tablelist->selectRow(tablelist->getCurrentRow());
	int valueindex = tablelist->getCurrentRow();
        if(!tablelist->getItemText(tablelist->getCurrentRow(), 1).empty())
        {
            FXString valuename = tablelist->getItemText(tablelist->getCurrentRow(), 1);
            FXString valuetype = tablelist->getItemText(tablelist->getCurrentRow(), 2);
            FXTreeItem* curitem = treelist->getCurrentItem();
            FXString rootstring = "";
            FXString hivepath = "";
            GetRootString(curitem, &rootstring);
            for(int i=0; i < hives.size(); i++)
            {
                if(rootstring.contains(FXString(hives.at(i).string().c_str())))
                    hivepath = FXString(hives.at(i).string().c_str());
            }
	    int found = statusbar->getStatusLine()->getNormalText().find_first_of("\\");
            FXString keypath = statusbar->getStatusLine()->getNormalText().after('\\');
            libregf_file_t* regfile = NULL;
            libregf_error_t* regerr = NULL;
            libregf_file_initialize(&regfile, &regerr);
            libregf_file_open(regfile, hivepath.text(), LIBREGF_OPEN_READ, &regerr);
            libregf_key_t* curkey = NULL;
            libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.text()), keypath.count(), &curkey, &regerr);
            libregf_value_t* curval = NULL;
            libregf_key_get_value(curkey, valueindex, &curval, &regerr);
            uint64_t lastwritetime = 0;
            libregf_key_get_last_written_time(curkey, &lastwritetime, &regerr);
            FXString valuedata = "Last Written Time:\t" + ConvertWindowsTimeToUnixTimeUTC(lastwritetime) + " UTC\n\n";
            valuedata += "Name:\t" + valuename + "\n\n";
            if(valuename.contains("(unnamed)") && !keypath.contains("USBSTOR"))
            {
                valuedata += "Content\n-------\n\n";
                valuedata += "Hex:\t0x" + FXString::value(valuetype.toInt(16), 16) + "\n";
                valuedata += "Integer:\t" + FXString::value(valuetype.toInt()) + "\n";
            }
            else
            {
                if(valuetype.contains("REG_SZ") || valuetype.contains("REG_EXPAND_SZ"))
                {
                    valuedata += "Content:\t";
                    size_t strsize = 0;
                    libregf_value_get_value_utf8_string_size(curval, &strsize, &regerr);
                    uint8_t valstr[strsize];
                    libregf_value_get_value_utf8_string(curval, valstr, strsize, &regerr);
                    valuedata += FXString(reinterpret_cast<char*>(valstr));
                }
                else if(valuetype.contains("REG_BINARY"))
                {
                    valuedata += "Content\n-------\n\n";
                    if(keypath.contains("UserAssist") && keypath.contains("{"))
                    {
                        valuedata += "ROT13 Decrypted Content:\t";
                        valuedata += DecryptRot13(valuename) + "\n";
                    }
                    else if(keypath.contains("SAM") && valuename.count() == 1 && valuename.contains("F"))
                    {
                        uint64_t tmp64 = 0;
                        size_t datasize = 0;
                        libregf_value_get_value_data_size(curval, &datasize, &regerr);
                        uint8_t data[datasize];
                        libregf_value_get_value_data(curval, data, datasize, &regerr);
                        valuedata += "Account Expiration:\t\t";
                        if(data[32] == 0xff)
                        {
                            valuedata += "No Expiration is Set\n";
                        }
                        else
                        {
                            tmp64 = (uint64_t)data[32] | (uint64_t)data[33] << 8 | (uint64_t)data[34] << 16 | (uint64_t)data[35] << 24 | (uint64_t)data[36] << 32 | (uint64_t)data[37] << 40 | (uint64_t)data[38] << 48 | (uint64_t)data[39] << 56;
                            valuedata += ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                        }
                        tmp64 = (uint64_t)data[8] | (uint64_t)data[9] << 8 | (uint64_t)data[10] << 16 | (uint64_t)data[11] << 24 | (uint64_t)data[12] << 32 | (uint64_t)data[13] << 40 | (uint64_t)data[14] << 48 | (uint64_t)data[15] << 56;
                        valuedata += "Last Logon Time:\t\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                        tmp64 = (uint64_t)data[40] | (uint64_t)data[41] << 8 | (uint64_t)data[42] << 16 | (uint64_t)data[43] << 24 | (uint64_t)data[44] << 32 | (uint64_t)data[45] << 40 | (uint64_t)data[46] << 48 | (uint64_t)data[47] << 56;
                        valuedata += "Last Failed Login:\t\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                        tmp64 = (uint64_t)data[24] | (uint64_t)data[25] << 8 | (uint64_t)data[26] << 16 | (uint64_t)data[27] << 24 | (uint64_t)data[28] << 32 | (uint64_t)data[29] << 40 | (uint64_t)data[30] << 48 | (uint64_t)data[31] << 56;
                        valuedata += "Last Time Password Changed:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                    }
                    else if(valuename.contains("ShutdownTime"))
                    {
                        size_t datasize = 0;
                        libregf_value_get_value_data_size(curval, &datasize, &regerr);
                        uint8_t data[datasize];
                        libregf_value_get_value_data(curval, data, datasize, &regerr);
                        uint64_t tmp64 = (uint64_t)data[0] | (uint64_t)data[1] << 8 | (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 | (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 | (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
                        valuedata += "Shutdown Time:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";

                    }
                    else if(valuename.contains("MRUListEx"))
                    {
                        size_t datasize = 0;
                        libregf_value_get_value_data_size(curval, &datasize, &regerr);
                        uint8_t data[datasize];
                        libregf_value_get_value_data(curval, data, datasize, &regerr);
                        valuedata += "Order:\t[";
                        for(int i=0; i < sizeof(data) / 4; i++)
                        {
                            uint32_t tmp32 = (uint32_t)data[i*4] | (uint32_t)data[i*4 + 1] << 8 | (uint32_t)data[i*4 + 2] << 16 | (uint32_t)data[i*4 + 3] << 24;
                            if(tmp32 < 0xFFFFFFFF)
                                valuedata += FXString::value(tmp32);
                            if(i < ((sizeof(data) / 4) - 2))
                                valuedata += ", ";
                        }
                        valuedata += "]\n";
                    }
		    else if(keypath.contains("CIDSizeMRU") || keypath.contains("LastVisitedPidlMRU") || keypath.contains("RecentDocs") || keypath.contains("WordWheelQuery") || keypath.contains("MountedDevices"))
		    {
			if(!valuename.contains("MRUListEx"))
			{
			    size_t datasize = 0;
			    libregf_value_get_value_data_size(curval, &datasize, &regerr);
			    uint8_t data[datasize];
			    libregf_value_get_value_data(curval, data, datasize, &regerr);
			    valuedata += "Name:\t";
			    for(int i=0; i < sizeof(data) / 2; i++)
			    {
				uint16_t tmp16 = (uint16_t)data[i*2] | (uint16_t)data[i*2+1] << 8;
				FXwchar tmpwc = FX::wc(&tmp16);
				if(tmp16 == 0x0000)
				    break;
				valuedata += tmpwc;
			    }
			}
		    }
		    else if(keypath.contains("OpenSavePidlMRU") && !valuename.contains("MRUListEx"))
		    {
			size_t datasize = 0;
			libregf_value_get_value_data_size(curval, &datasize, &regerr);
			uint8_t data[datasize];
			libregf_value_get_value_data(curval, data, datasize, &regerr);
			valuedata += "Name:\t";
			int asciioffset = 0;
			for(int i=0; i < sizeof(data) / 2; i++)
			{
			    uint16_t tmp16 = (uint16_t)data[i*2] | (uint16_t)data[i*2 + 1] << 8;
			    FXwchar tmpwc = FX::wc(&tmp16);
			    if(tmp16 == 0x0080)
			    {
				asciioffset = i*2 + 2;
				break;
			    }
			    else if(tmp16 == 0x8000)
			    {
				asciioffset = i*2 + 3;
				break;
			    }
                            else if(data[i*2] == 0x20 && data[i*2+1] < 0x0a)
                            {
                                asciioffset = i*2 + 3;
                                break;
                            }
                            else if(data[i*2] < 0x0a && data[i*2+1] == 0x20)
                            {
                                asciioffset = i*2 + 2;
                                break;
                            }
			}
			//std::cout << "ascii offset: " << asciioffset << std::endl;
			if(asciioffset > 0)
			{
			    for(int i=asciioffset; i < sizeof(data); i++)
			    {
				FXchar tmpc = FX::FXchar((char)data[i]);
				//std::cout << "char: " << tmpc << std::endl;
				if(data[i] == 0x00)
				    break;
				valuedata += tmpc;
			    }
			}
		    }
                }
                else if(valuetype.contains("REG_DWORD"))
                {
                    valuedata += "Content:\t";
                    uint32_t dwordvalue = 0;
                    libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                    if(valuename.lower().contains("date") || valuename.lower().contains("time"))
		    {
			valuedata += "\n--------\n";
                        valuedata += "DateTime:\t" + ConvertUnixTimeToString(dwordvalue) + "\n";
                        valuedata += "Value:\t\t" + FXString::value(dwordvalue);
		    }
                    else
                        valuedata += FXString::value(dwordvalue);
                }
                else if(valuetype.contains("REG_DWORD_BIG_ENDIAN"))
                {
                    valuedata += "Content:\t";
                    uint32_t dwordvalue = 0;
                    libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                    valuedata += FXString::value(dwordvalue);
                }
                else if(valuetype.contains("REG_MULTI_SZ"))
                {
                    valuedata += "Content\n";
                    valuedata += "-------\n";
                    libregf_multi_string_t* multistring = NULL;
                    libregf_value_get_value_multi_string(curval, &multistring, &regerr);
                    int strcnt = 0;
                    libregf_multi_string_get_number_of_strings(multistring, &strcnt, &regerr);
                    for(int i=0; i < strcnt; i++)
                    {
                        size_t strsize = 0;
                        libregf_multi_string_get_utf8_string_size(multistring, i, &strsize, &regerr);
                        uint8_t valstr[strsize];
                        libregf_multi_string_get_utf8_string(multistring, i, valstr, strsize, &regerr);
                        valuedata += FXString(reinterpret_cast<char*>(valstr));
                    }
                    libregf_multi_string_free(&multistring, &regerr);
                }
                else if(valuetype.contains("REG_QWORD"))
                {
                    valuedata += "Content:\t";
                    uint64_t qwordvalue = 0;
                    libregf_value_get_value_64bit(curval, &qwordvalue, &regerr);
                    if(valuename.lower().contains("date") || valuename.lower().contains("time"))
		    {
			valuedata += "\n--------\n";
                        valuedata += "DateTime:\t" + ConvertUnixTimeToString(qwordvalue) + "\n";
			valuedata += "DateTime:\t" + ConvertWindowsTimeToUnixTimeUTC(qwordvalue) + "\n";
                        valuedata += "Value:\t\t" + FXString::value(qwordvalue);
		    }
		    else
			valuedata += FXString::value(qwordvalue);
                }
                else if(keypath.contains("USBSTOR"))
                {
                    uint64_t tmp64 = 0;
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    if(keypath.contains("0003") || keypath.contains("000A"))
                    {
                        valuedata += "Name:\t";
                        for(int i=0; i < sizeof(data) / 2; i++)
                        {
                            uint16_t tmp16 = (uint16_t)data[i*2] | (uint16_t)data[i*2+1] << 8;
                            FXwchar tmpwc = FX::wc(&tmp16);
                            if(tmp16 == 0x0000)
                                break;
                            valuedata += tmpwc;
                        }
                    }
                    else if(keypath.contains("0064"))
                    {
                        tmp64 = (uint64_t)data[0] | (uint64_t)data[1] << 8 | (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 | (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 | (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
                        valuedata += "First Install:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                    }
                    else if(keypath.contains("0066"))
                    {
                        tmp64 = (uint64_t)data[0] | (uint64_t)data[1] << 8 | (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 | (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 | (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
                        valuedata += "Last Connected:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                    }
                    else if(keypath.contains("0067"))
                    {
                        tmp64 = (uint64_t)data[0] | (uint64_t)data[1] << 8 | (uint64_t)data[2] << 16 | (uint64_t)data[3] << 24 | (uint64_t)data[4] << 32 | (uint64_t)data[5] << 40 | (uint64_t)data[6] << 48 | (uint64_t)data[7] << 56;
                        valuedata += "Last Removal:\t" + ConvertWindowsTimeToUnixTimeUTC(tmp64) + " UTC\n";
                    }
                }
            }
            size_t datasize = 0;
            libregf_value_get_value_data_size(curval, &datasize, &regerr);
            uint8_t data[datasize];
            libregf_value_get_value_data(curval, data, datasize, &regerr);
            valuedata += "\n\nBinary Content\n--------------\n\n";
            if(datasize < 16)
            {
                valuedata += "0000\t";
                std::stringstream ss;
                ss << std::hex << std::setfill('0');
                //for(int i=0; i < datasize; i++)
                for(int i=0; i < 16; i++)
		    if(i < datasize)
			ss << std::setw(2) << ((uint)data[i]) << " ";
		    else
			ss << "   ";
                valuedata += FXString(ss.str().c_str()).upper();
                for(int i=0; i < datasize; i++)
                //for(int i=0; i < 16; i++)
                {
                    if(isprint(data[i]))
                        valuedata += FXchar(reinterpret_cast<unsigned char>(data[i]));
                    else
                        valuedata += ".";
                }
                valuedata += "\n";
            }
            else
            {
                int linecount = datasize / 16;
                int linerem = datasize % 16;
                if(linerem > 0)
                    linecount++;
                for(int i=0; i < linecount; i++)
                {
                    std::stringstream ss;
                    ss << std::hex << std::setfill('0') << std::setw(8) << i * 16 << "\t";
                    for(int j=0; j < 16; j++)
                    {
                        if(j+i*16 < datasize)
                            ss << std::setw(2) << ((uint)data[j+i*16]) << " ";
                        else
                            ss << "   ";
                    }
                    valuedata += FXString(ss.str().c_str()).upper();
                    for(int j=0; j < 16; j++)
                    {
                        if(j+i*16 < datasize)
                        {
                            if(isprint(data[j+i*16]))
                                valuedata += FXchar(reinterpret_cast<unsigned char>(data[j+i*16]));
                            else
                                valuedata += ".";
                        }
                    }
                    valuedata += "\n";
                }
            }
            plaintext->setText(valuedata);
            libregf_value_free(&curval, &regerr);
            libregf_key_free(&curkey, &regerr);
            libregf_file_close(regfile, &regerr);
            libregf_file_free(&regfile, &regerr);
            libregf_error_free(&regerr);
        }
    }
    return 1;
}

FXString WombatRegistry::DecryptRot13(FXString encstr)
{
    FXString decstr = "";
    int i = 0;
    int strlength = 0;
    strlength = encstr.count();
    decstr = encstr;
    for(i = 0; i < strlength; i++)
    {
        decstr[i] = Rot13Char(decstr.at(i));
    }
    return decstr;
}

FXchar WombatRegistry::Rot13Char(FXchar curchar)
{
    FXchar rot13char;
    if('0' <= curchar && curchar <= '4')
        rot13char = FXchar(curchar + 5);
    else if('5' <= curchar && curchar <= '9')
        rot13char = FXchar(curchar - 5);
    else if('A' <= curchar && curchar <= 'M')
        rot13char = FXchar(curchar + 13);
    else if('N' <= curchar && curchar <= 'Z')
        rot13char = FXchar(curchar - 13);
    else if('a' <= curchar && curchar <= 'm')
        rot13char = FXchar(curchar + 13);
    else if('n' <= curchar && curchar <= 'z')
        rot13char = FXchar(curchar - 13);
    else
        rot13char = curchar;

    return rot13char;
}

void WombatRegistry::AlignColumn(FXTable* curtable, int col, FXuint justify)
{
    for(int i=0; i < curtable->getNumRows(); i++)
        curtable->setItemJustify(i, col, justify);
}

long WombatRegistry::OpenTagManager(FXObject*, FXSelector, void*)
{
    ManageTags tagmanager(this, "Manage Tags");
    tagmanager.SetTagList(&tags, &taggedlist);
    tagmanager.execute(PLACEMENT_OWNER);
    for(int i=0; i < tablelist->getNumRows(); i++)
    {
        FXString curidkeyval = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(i, 1);
        FXString curtag = tablelist->getItemText(i, 0);
        for(int j=0; j < taggedlist.no(); j++)
        {
            std::size_t found = taggedlist.at(j).find("|");
            std::size_t rfound = taggedlist.at(j).rfind("|");
            FXString itemtag = taggedlist.at(j).mid(0, found);
            FXString itemhdr = taggedlist.at(j).mid(found+1, rfound - found - 1);
            if(FXString::compare(curidkeyval, itemhdr) == 0)
            {
                if(FXString::compare(curtag, itemtag) != 0)
                    tablelist->setItemText(i, 0, itemtag);
            }
        }
    }

    return 1;
}

long  WombatRegistry::OpenAboutBox(FXObject*, FXSelector, void*)
{
    AboutBox aboutbox(this, "About Wombat Registry Forensics");
    aboutbox.execute(PLACEMENT_OWNER);
    return 1;
}

long WombatRegistry::PreviewReport(FXObject*, FXSelector, void*)
{
    GenerateReport(taggedlist, tags);
    viewer = new Viewer(this, "Report Preview");
    viewer->SetText(reportstring);
    viewer->execute(PLACEMENT_OWNER);

    return 1;
}

long WombatRegistry::PublishReport(FXObject*, FXSelector, void*)
{
    FXString startpath = FXString(getenv("HOME")) + "/";
    FXString filename = FXFileDialog::getSaveFilename(this, "Publish Report", startpath, "Text Files (*.txt)\nHTML Files (*.html,*.htm)");
    if(!filename.empty())
    {
        FXFile* outfile = new FXFile(filename, FXIO::Writing, FXIO::OwnerReadWrite);
        FXString buf;
        if(filename.contains(".htm"))
        {
            buf = "<html><head><title>Wombat Registry Report</title></head>\n";
            buf += "<body style='font-color: #3a291a; background-color: #d6ceb5;'>\n";
            buf += "<h2>Wombat Registry Report</h2>\n";
            buf += "<div id='toc'><h3>Contents</h3>\n";
            for(int j=0; j < tags.size(); j++)
            {
                int tagcnt = 0;
                for(int i=0; i < taggedlist.no(); i++)
                {
                    //if(taggedlist.at(i).contains(tags.at(j).c_str()))
                    std::size_t found = taggedlist.at(i).find("|");
                    FXString itemtag = taggedlist.at(i).mid(0, found);
                    if(FXString::compare(itemtag, FXString(tags.at(j).c_str())) == 0)
                        tagcnt++;
                }
                buf += "<div><a href='#t" + FXString::value(j) + "'>" + FXString(tags.at(j).c_str()) + " (" + FXString::value(tagcnt) + ")</a></div>\n";
            }
            buf += "<h3>Tagged Items</h3>";
            for(int i=0; i < tags.size(); i++)
            {
                buf += "<div id='t" + FXString::value(i) + "'><h4>" + tags.at(i).c_str() + "<span style='font-size: 10px;'>&nbsp;&nbsp;<a href='#toc'>TOP</a></span></h4>\n";
                for(int j=0; j < taggedlist.no(); j++)
                {
                    std::size_t found = taggedlist.at(j).find("|");
                    std::size_t rfound = taggedlist.at(j).rfind("|");
                    FXString itemtag = taggedlist.at(j).mid(0, found);
                    FXString itemhdr = taggedlist.at(j).mid(found+1, rfound - found - 1);
                    FXString itemcon = taggedlist.at(j).mid(rfound+1, taggedlist.at(j).length() - rfound);
                    if(itemtag == tags.at(i).c_str())
                    {
                        buf += "<div style='border-bottom: 1px solid black;'>\n";
                        buf += "<div>Key:&nbsp;&nbsp;&nbsp;&nbsp;" + itemhdr + "</div>\n";
                        buf += "<div><pre>" + itemcon + "</pre></div>\n";
                        buf += "</div>\n";
                    }
                }
            }
            buf += "</body></html>";
        }
        else
        {
            GenerateReport(taggedlist, tags);
            buf = reportstring;
        }
        outfile->writeBlock(buf.text(), buf.length());
        outfile->close();
    }

    return 1;
}

long WombatRegistry::TableUp(FXObject*, FXSelector, void* ptr)
{
    int currow = tablelist->getCurrentRow();
    //std::cout << "currow: " << currow << " num rows: " << tablelist->getNumRows() << std::endl;
    switch(((FXEvent*)ptr)->code)
    {
        case KEY_Up:
            tablelist->setCurrentItem(currow - 1, 0, true);
	    tablelist->selectRow(currow - 1, true);
            if(currow - 1 > 0)
            {
            if(!tablelist->isItemVisible(currow, 0))
                tablelist->makePositionVisible(currow - 1, 0);
            }
            break;
        case KEY_Down:
            tablelist->setCurrentItem(currow + 1, 0, true);
	    tablelist->selectRow(currow + 1, true);
            if(currow + 1 < tablelist->getNumRows() - 1)
            {
            if(!tablelist->isItemVisible(currow, 0))
                tablelist->makePositionVisible(currow + 1, 0);
            }
            break;
    }

    return 1;
}

long WombatRegistry::CloseHive(FXObject*, FXSelector, void*)
{
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    int rootoffset = idkeyvalue.find("\\");
    FXTreeItem* curitem = treelist->getCurrentItem();
    bool toplevel = false;
    FXTreeItem* parent;
    FXTreeItem* child;
    child = curitem;
    while(toplevel == false)
    {
	parent = child->getParent();
	if(parent == NULL)
	    toplevel = true;
	else
	    child = parent;
    }
    for(int i=taggedlist.no() - 1; i >= 0; i--)
    {
        if(taggedlist.at(i).contains(idkeyvalue.left(rootoffset)))
	{
            taggedlist.erase(i);
	}
    }
    treelist->removeItem(child);
    plaintext->clearText();
    tablelist->clearItems();
    tablelist->setEditable(false);
    tablelist->setTableSize(4, 3);
    tablelist->setColumnText(0, "Tag");
    tablelist->setColumnText(1, "Value Name");
    tablelist->setColumnText(2, "Value Type");
    tablelist->setColumnHeaderHeight(tablelist->getColumnHeaderHeight() + 5);

    return 1;
}

long WombatRegistry::OpenHive(FXObject*, FXSelector, void*)
{
    FXString filename = "";
    if(cmdhivepath.empty())
    {
        if(prevhivepath.empty())
            prevhivepath = getenv("HOME") + std::string("/");
	else
	{
	    std::size_t found = prevhivepath.find_last_of("/");
	    prevhivepath = prevhivepath.substr(0, found+1);
	}
        filename = FXFileDialog::getOpenFilename(this, "Open Hive", prevhivepath.c_str());
    }
    else
    {
        filename = FXString(cmdhivepath.c_str());
	cmdhivepath = "";
    }

    if(!filename.empty())
    {
        hivefilepath = filename.text();
        prevhivepath = hivefilepath;
        hives.push_back(std::filesystem::canonical(hivefilepath));
        std::ifstream filebuffer(hivefilepath.c_str(), std::ios::in|std::ios::binary);
        filebufptr = &filebuffer;
        filebufptr->seekg(0);
        char* registryheader = new char[4];
        filebufptr->read(registryheader, 4);
        std::string regheadstr(registryheader);
        delete[] registryheader;
        if(regheadstr.find("regf") != std::string::npos) // win nt reg file
        {
            filebuffer.close();
            libregf_file_t* regfile = NULL;
            libregf_error_t* regerr = NULL;
            libregf_file_initialize(&regfile, &regerr);
            libregf_file_open(regfile, hivefilepath.c_str(), LIBREGF_OPEN_READ, &regerr);
            libregf_error_fprint(regerr, stderr);
            libregf_key_t* rootkey = NULL;
            libregf_file_get_root_key(regfile, &rootkey, &regerr);
            libregf_error_fprint(regerr, stderr);
            int rootsubkeycnt = 0;
            libregf_key_get_number_of_sub_keys(rootkey, &rootsubkeycnt, &regerr);
            libregf_error_fprint(regerr, stderr);
            std::size_t rfound = hivefilepath.rfind("/");
            std::string hivefilename = hivefilepath.substr(rfound+1);
            FXString rootitemstring(std::string(hivefilename + " (" + hivefilepath + ")").c_str());
            rootitem = new FXTreeItem(rootitemstring);
            treelist->appendItem(0, rootitem);
	    this->getApp()->beginWaitCursor();
	    PopulateChildKeys(rootkey, rootitem, regerr);
	    this->getApp()->endWaitCursor();
	    treelist->expandTree(rootitem);
	    libregf_key_free(&rootkey, &regerr);
	    libregf_file_close(regfile, &regerr);
	    libregf_file_free(&regfile, &regerr);
	    libregf_error_free(&regerr);
        }
        else
        {
            StatusUpdate("File not opened. Not a valid registry file.");
            //std::cout << "check failed..." << std::endl;
        }
    }

    return 1;
}

void WombatRegistry::PopulateChildKeys(libregf_key_t* curkey, FXTreeItem* curitem, libregf_error_t* regerr)
{
    int subkeycount = 0;
    libregf_key_get_number_of_sub_keys(curkey, &subkeycount, &regerr);
    if(subkeycount > 0)
    {
	for(int i=0; i < subkeycount; i++)
	{
	    libregf_key_t* cursubkey = NULL;
	    libregf_key_get_sub_key(curkey, i, &cursubkey, &regerr);
	    size_t namesize = 0;
	    libregf_key_get_utf8_name_size(cursubkey, &namesize, &regerr);
	    uint8_t name[namesize];
	    libregf_key_get_utf8_name(cursubkey, name, namesize, &regerr);
            FXString itemstring((reinterpret_cast<char*>(name)));
	    FXTreeItem* subitem = new FXTreeItem(itemstring);
	    treelist->appendItem(curitem, subitem);
	    int subsubkeycount = 0;
	    libregf_key_get_number_of_sub_keys(cursubkey, &subsubkeycount, &regerr);
	    if(subsubkeycount > 0)
	    {
		PopulateChildKeys(cursubkey, subitem, regerr);
	    }
	    libregf_key_free(&cursubkey, &regerr);
	}
    }
}


long WombatRegistry::SetTag(FXObject* sender, FXSelector, void*)
{
    FXString tagstr = ((FXMenuCommand*)sender)->getText();
    tablelist->setItemText(tablelist->getCurrentRow(), 0, tagstr);
    FXString idkeyvalue = statusbar->getStatusLine()->getText() + "\\" + tablelist->getItemText(tablelist->getCurrentRow(), 1);
    for(int i=0; i < taggedlist.no(); i++)
    {
        if(taggedlist.at(i).contains(idkeyvalue))
            taggedlist.erase(i);
    }
    taggedlist.append(tagstr + "|" + idkeyvalue + "|" + plaintext->getText());

    return 1;
}

int main(int argc, char* argv[])
{
    FXApp* wr = new FXApp("Registry", "Wombat");

    wr->init(argc, argv);

    new WombatRegistry(wr);

    wr->create();
    wr->run();

    return 0;
}

void WombatRegistry::GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags)
{
    reportstring.clear();
    // PLACE TITLE
    reportstring.append("Wombat Registry Report\n----------------------\n\n");
    // PLACE CONTENTS HEADER
    reportstring.append("Contents\n--------\n\n");
    // GENERATE TAGS AND THEIR COUNT
    for(int j=0; j < tags.size(); j++)
    {
        int tagcnt = 0;
        for(int i=0; i < taggedlist.no(); i++)
        {
            std::size_t found = taggedlist.at(i).find("|");
            FXString itemtag = taggedlist.at(i).mid(0, found);
            if(FXString::compare(itemtag, FXString(tags.at(j).c_str())) == 0)
                tagcnt++;
        }
        reportstring.append(FXString(tags.at(j).c_str()) + " (" + FXString::value(tagcnt) + ")\n");
    }
    reportstring.append("\n\n");
    // GENERATE TAG HEADER AND CONTENT
    for(int i=0; i < tags.size(); i++)
    {
        reportstring.append(FXString(tags.at(i).c_str()) + "\n");
        for(int j=0; j < tags.at(i).size(); j++)
            reportstring.append("-");
        reportstring.append("\n\n");
        for(int j=0; j < taggedlist.no(); j++)
        {
            std::size_t found = taggedlist.at(j).find("|");
            std::size_t rfound = taggedlist.at(j).rfind("|");
            FXString itemtag = taggedlist.at(j).mid(0, found);
            FXString itemhdr = taggedlist.at(j).mid(found+1, rfound - found - 1);
            FXString itemcon = taggedlist.at(j).mid(rfound+1, taggedlist.at(j).length() - rfound);
            if(itemtag == tags.at(i).c_str())
            {
                reportstring.append("Key:\t" + itemhdr + "\n");
                reportstring.append(itemcon + "\n");
                for(int k=0; k < 80; k++)
                    reportstring.append("-");
                reportstring.append("\n\n");
            }
        }
    }
}
