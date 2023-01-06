#include "wombatregistry.h"

FXIMPLEMENT(WombatRegistry,FXMainWindow,WombatRegistryMap,ARRAYNUMBER(WombatRegistryMap))

WombatRegistry::WombatRegistry(FXApp* a):FXMainWindow(a, "Wombat Registry Forensics", new FXICOIcon(a, wombat_32), new FXICOIcon(a, wombat_32), DECOR_ALL, 0, 0, 1024, 768)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    toolbar = new FXToolBar(mainframe, this, LAYOUT_TOP|LAYOUT_LEFT);
    vsplitter = new FXSplitter(mainframe, SPLITTER_NORMAL|LAYOUT_FILL);
    statusbar = new FXStatusBar(mainframe, LAYOUT_BOTTOM|LAYOUT_LEFT|LAYOUT_FILL_X);
    treelist = new FXTreeList(vsplitter, this, ID_TREESELECT, TREELIST_SHOWS_LINES|TREELIST_SINGLESELECT|TREELIST_ROOT_BOXES|TREELIST_SHOWS_BOXES);
    treelist->setWidth(this->getWidth() / 4);
    hsplitter = new FXSplitter(vsplitter, SPLITTER_VERTICAL);
    tablelist = new FXTable(hsplitter, this, ID_TABLESELECT, LAYOUT_FILL_X, LAYOUT_FILL_Y);
    plaintext = new FXText(hsplitter);
    tablelist->setHeight(this->getHeight() / 3);
    //tablelist->setVisibleColumns(3);
    tablelist->setEditable(false);
    tablelist->setTableSize(4, 3);
    tablelist->setColumnText(0, "Tag");
    tablelist->setColumnText(1, "Value Name");
    tablelist->setColumnText(2, "Value Type");
    tablelist->setColumnHeaderHeight(tablelist->getColumnHeaderHeight() + 5);
    //tablelist->fitColumnsToContents(1, 1);
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
    hives.clear();
    tags.clear();


}


/*
void ReadContent(std::ifstream* rawcontent, int8_t* tmpbuf, uint64_t offset, uint64_t size)
{
    rawcontent->seekg(offset);
    rawcontent->read((char*)tmpbuf, size);
}

void ReturnUint32(uint32_t* tmp32, uint8_t* tmp8)
{
    *tmp32 = (uint32_t)tmp8[0] | (uint32_t)tmp8[1] << 8 | (uint32_t)tmp8[2] << 16 | (uint32_t)tmp8[3] << 24;
}
 */



/*
// Construct a ScribbleWindow
  ScribbleWindow::ScribbleWindow(FXApp *a):FXMainWindow(a,"ScribbleApplication",NULL,NULL,DECOR_ALL,0,0,800,600){

  contents=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,0,0,0,0);

  // LEFT pane to contain the canvas
  canvasFrame=new FXVerticalFrame(contents,
                                  FRAME_SUNKEN|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);

  // Label above the canvas
  new FXLabel(canvasFrame,"CanvasFrame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

  // Horizontal divider line
  new FXHorizontalSeparator(canvasFrame,SEPARATOR_GROOVE|LAYOUT_FILL_X);

  // Drawing canvas
  canvas=new FXCanvas(canvasFrame,this,ID_CANVAS,
                      FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT);

  // RIGHT pane for the buttons
  buttonFrame=new FXVerticalFrame(contents,
                                  FRAME_SUNKEN|LAYOUT_FILL_Y|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);

  // Label above the buttons
  new FXLabel(buttonFrame,"ButtonFrame",NULL,JUSTIFY_CENTER_X|LAYOUT_FILL_X);

  // Horizontal divider line
  new FXHorizontalSeparator(buttonFrame,SEPARATOR_RIDGE|LAYOUT_FILL_X);

  // Button to clear
  new FXButton(buttonFrame,"&Clear",NULL,this,ID_CLEAR,
               FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  // Exit button
  new FXButton(buttonFrame,"&Exit",NULL,getApp(),FXApp::ID_QUIT,
               FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,5,5);

  // Initialize private variables
  drawColor=FXRGB(255,0,0);
  mdflag=0;
  dirty=0;
  }
*/

void WombatRegistry::create()
{
    FXMainWindow::create();
    show(PLACEMENT_SCREEN);
}

/*
long WombatRegistry::onMouseDown(FXObject*, FXSelector, void*)
{
    std::cout << "mouse down pressed." << std::endl;
    printf("hello there.");
    return 1;
}
*/

long WombatRegistry::KeySelected(FXObject* sender, FXSelector, void*)
{
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
	    toplevel = true;
	else
	{
	    pathitems.push_back(parent->getText());
	    child = parent;
	}
    }
    FXString keypath = "";
    for(int i=pathitems.size() - 2; i > -1; i--)
    {
	keypath += "\\" + pathitems.at(i);
    }
    StatusUpdate(keypath);
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    libregf_file_open(regfile, hivefilepath.c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_key_t* curkey = NULL;
    libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.text()), keypath.count(), &curkey, &regerr);
    // valid key, get values...
    int valuecount = 0;
    libregf_key_get_number_of_values(curkey, &valuecount, &regerr);
    tablelist->clearItems();
    plaintext->setText("");
    tablelist->setTableSize(valuecount, 3);
    tablelist->setColumnText(0, "Tag");
    tablelist->setColumnText(1, "Value Name");
    tablelist->setColumnText(2, "Value Type");
    if(valuecount == 0) // no values, so create empty key
    {
	tablelist->setTableSize(1, 3);
	tablelist->setColumnText(0, "Tag");
	tablelist->setColumnText(1, "Value Name");
	tablelist->setColumnText(2, "Value Type");
	FXString curtagvalue = keypath + "\\" + "(empty)";
	tablelist->setItemText(0, 1, "(empty)");
	tablelist->setItemText(0, 2, "0x00");
	/*
	for(int j=0; j < taggeditems.size(); j++)
	{
	    if(taggeditems.at(j).contains(curtagvalue))
		tagstr = taggeditems.at(j)
	}
	tablelist->setItemText(0, 0, tagstr);
	tablelist->fitColumnsToContents(0, 3);
	*/
    }
    for(int i=0; i < valuecount; i++)
    {
	libregf_value_t* curval = NULL;
	libregf_key_get_value(curkey, i, &curval, &regerr);
	size_t namesize = 0;
	libregf_value_get_utf8_name_size(curval, &namesize, &regerr);
	uint8_t name[namesize];
	libregf_value_get_utf8_name(curval, name, namesize, &regerr);
	uint32_t type = 0;
	libregf_value_get_value_type(curval, &type, &regerr);
	FXString curtagvalue = keypath + "\\";
	if(namesize == 0)
	{
	    curtagvalue += "(unnamed)";
	    tablelist->setItemText(i, 1, "(unnamed)");
	    std::stringstream ss;
	    ss << std::hex << type;
	    tablelist->setItemText(i, 2, "0x" + FXString(ss.str().c_str()));
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
	/*
	QString tagstr = "";
        for(int j=0; j < taggeditems.count(); j++)
        {
            if(taggeditems.at(j).contains(curtagvalue))
                tagstr = taggeditems.at(j).split("|", Qt::SkipEmptyParts).first();
        }
	ui->tablewidget->setItem(i, 0, new QTableWidgetItem(tagstr));
        ui->tablewidget->resizeColumnToContents(0);
        ui->tablewidget->setCurrentCell(0, 0);
	*/
	libregf_value_free(&curval, &regerr);
    }
    libregf_key_free(&curkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);

    return 1;
}
long WombatRegistry::ValueSelected(FXObject*, FXSelector, void*)
{
    if(tablelist->getCurrentRow() > -1)
    {
	tablelist->selectRow(tablelist->getCurrentRow());
	//std::cout << "current row:" << tablelist->getCurrentRow() << std::endl;
    }
    /*
    if(ui->tablewidget->selectedItems().count() > 0)
    {
	QTreeWidgetItem* curitem = ui->treewidget->selectedItems().first();
	int rootindex = GetRootIndex(curitem);
	hivefilepath = hives.at(rootindex);
	int valueindex = ui->tablewidget->selectedItems().at(1)->row();
	QString keypath = statuslabel->text();
	libregf_file_t* regfile = NULL;
	libregf_error_t* regerr = NULL;
	libregf_file_initialize(&regfile, &regerr);
	libregf_file_open(regfile, hivefilepath.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
	libregf_key_t* curkey = NULL;
	libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.toUtf8().data()), keypath.toUtf8().size(), &curkey, &regerr);
	libregf_value_t* curval = NULL;
	libregf_key_get_value(curkey, valueindex, &curval, &regerr);
        uint64_t lastwritetime = 0;
        libregf_key_get_last_written_time(curkey, &lastwritetime, &regerr);
        QString valuedata = "Last Written Time:\t" + ConvertWindowsTimeToUnixTimeUTC(lastwritetime) + " UTC\n\n";
	valuedata += "Name:\t" + ui->tablewidget->selectedItems().at(1)->text() + "\n\n";
	if(ui->tablewidget->selectedItems().at(1)->text().contains("(unnamed)"))
	{
	    valuedata += "Content\n-------\n\n";
	    valuedata += "Hex:\t0x" + ui->tablewidget->selectedItems().at(1)->text() + "\n";
	    valuedata += "Integer:\t" + QString::number(ui->tablewidget->selectedItems().at(1)->text().toInt(nullptr, 16)) + "\n";
	}
	else
	{
            QString valuetype = ui->tablewidget->selectedItems().at(2)->text();
            if(valuetype.contains("REG_SZ") || valuetype.contains("REG_EXPAND_SZ"))
            {
                valuedata += "Content:\t";
                size_t strsize = 0;
                libregf_value_get_value_utf8_string_size(curval, &strsize, &regerr);
                uint8_t valstr[strsize];
                libregf_value_get_value_utf8_string(curval, valstr, strsize, &regerr);
                valuedata += QString::fromUtf8(reinterpret_cast<char*>(valstr));
            }
            else if(valuetype.contains("REG_BINARY"))
            {
                valuedata += "Content\n-------\n\n";
                if(keypath.contains("UserAssist") && (keypath.contains("{750") || keypath.contains("{F4E") || keypath.contains("{5E6")))
                {
                    valuedata += "ROT13 Decrypted Content:\t";
                    valuedata += DecryptRot13(ui->tablewidget->selectedItems().at(1)->text()) + "\n";
                }
                else if(keypath.contains("SAM") && ui->tablewidget->selectedItems().at(1)->text().count() == 1 && ui->tablewidget->selectedItems().at(1)->text().startsWith("F"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray farray = QByteArray::fromRawData((char*)data, datasize);
                    valuedata += "Account Expiration:\t\t";
                    if(farray.mid(32,1).toHex() == "ff")
                    {
                        valuedata += "No Expiration is Set\n";
                    }
                    else
                        valuedata += ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(32, 8))) + " UTC\n";
                    valuedata += "Last Logon Time:\t\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(8, 8))) + " UTC\n";
                    valuedata += "Last Failed Login:\t\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(40, 8))) + " UTC\n";
                    valuedata += "Last Time Password Changed:\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(farray.mid(24, 8))) + " UTC";
                }
                else if(ui->tablewidget->selectedItems().at(1)->text().startsWith("ShutdownTime"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray valarray = QByteArray::fromRawData((char*)data, datasize);
                    valuedata += "Shutdown Time:\t" + ConvertWindowsTimeToUnixTimeUTC(qFromLittleEndian<uint64_t>(valarray)) + " UTC";
                }
                else if(keypath.contains("RecentDocs"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray valarray = QByteArray::fromRawData((char*)data, datasize);
                    if(ui->tablewidget->selectedItems().at(1)->text().startsWith("MRUListEx"))
                    {
                        valuedata += "Order:\t[";
                        for(int j=0; j < valarray.count() / 4; j++)
                        {
                            if(qFromLittleEndian<uint32_t>(valarray.mid(j*4, 4)) < 0xFFFF)
                                valuedata += QString::number(qFromLittleEndian<uint32_t>(valarray.mid(j*4, 4)));
                            if(j < ((valarray.count() / 4) - 2))
                                valuedata += ", ";
                        }
                        valuedata += "]";
                    }
                    else
                    {
                        valuedata += "Name:\t";
                        for(int j=0; j < valarray.count(); j++)
                        {
                            valuedata += QString(QChar(qFromLittleEndian<uint16_t>(valarray.mid(j*2, 2))));
                            if(qFromLittleEndian<uint16_t>(valarray.mid(j*2, 2)) == 0x0000)
                                break;
                        }
                    }
                }
            }
            else if(valuetype.contains("REG_DWORD"))
            {
                valuedata += "Content:\t";
                uint32_t dwordvalue = 0;
                libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                if(ui->tablewidget->selectedItems().at(1)->text().toLower().contains("date"))
                    valuedata += ConvertUnixTimeToString(dwordvalue);
                else
                    valuedata += QString::number(dwordvalue);
            }
            else if(valuetype.contains("REG_DWORD_BIG_ENDIAN"))
            {
                valuedata += "Content:\t";
                uint32_t dwordvalue = 0;
                libregf_value_get_value_32bit(curval, &dwordvalue, &regerr);
                valuedata += QString::number(qFromBigEndian<uint32_t>(dwordvalue));
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
                    valuedata += QString::fromUtf8(reinterpret_cast<char*>(valstr)) + "\n";
                }
                libregf_multi_string_free(&multistring, &regerr);
            }
            else if(valuetype.contains("REG_QWORD"))
            {
                valuedata += "Content:\t";
                uint64_t qwordvalue = 0;
                libregf_value_get_value_64bit(curval, &qwordvalue, &regerr);
                valuedata += QString::number(qwordvalue);
            }
	}
        size_t datasize = 0;
        libregf_value_get_value_data_size(curval, &datasize, &regerr);
        uint8_t data[datasize];
        libregf_value_get_value_data(curval, data, datasize, &regerr);
        QByteArray dataarray = QByteArray::fromRawData((char*)data, datasize);
        valuedata += "\n\nBinary Content\n--------------\n\n";
        if(datasize < 16)
        {
            valuedata += QString::number(0, 16).rightJustified(8, '0') + "\t";
            for(int i=0; i < datasize; i++)
                valuedata += QString("%1").arg(data[i], 2, 16, QChar('0')).toUpper() + " ";
            for(int i=0; i < datasize; i++)
            {
                if(!QChar(dataarray.at(i)).isPrint())
                    valuedata += ".";
                else
                    valuedata += QString("%1").arg(dataarray.at(i));
            }
            valuedata += "\n";
        }
        else
        {
            int linecount = datasize / 16;
            //int remainder = datasize % 16;
            for(int i=0; i < linecount; i++)
            {
                valuedata += QString::number(i * 16, 16).rightJustified(8, '0') + "\t";
                for(int j=0; j < 16; j++)
                {
                    valuedata += QString("%1").arg(data[j+i*16], 2, 16, QChar('0')).toUpper() + " ";
                }
                for(int j=0; j < 16; j++)
                {
                    if(!QChar(dataarray.at(j+i*16)).isPrint())
                    {
                        valuedata += ".";
                    }
                    else
                        valuedata += QString("%1").arg(dataarray.at(j+i*16));
                }
                valuedata += "\n";
            }
        }
	ui->plaintext->setPlainText(valuedata);

        libregf_value_free(&curval, &regerr);
        libregf_key_free(&curkey, &regerr);
        libregf_file_close(regfile, &regerr);
        libregf_file_free(&regfile, &regerr);
        libregf_error_free(&regerr);
    }
    */
    return 1;
}

long WombatRegistry::OpenTagManager(FXObject*, FXSelector, void*)
{
    ManageTags tagmanager(this, "Manage Tags");
    tagmanager.SetTagList(&tags);
    tagmanager.execute(PLACEMENT_OWNER);
    return 1;
}

long  WombatRegistry::OpenAboutBox(FXObject*, FXSelector, void*)
{
    AboutBox aboutbox(this, "About Wombat Registry Forensics");
    aboutbox.execute(PLACEMENT_OWNER);
    return 1;
}

long WombatRegistry::OpenHive(FXObject*, FXSelector, void*)
{
    if(prevhivepath.empty())
        prevhivepath = getenv("HOME");
    FXString filename = FXFileDialog::getOpenFilename(this, "Open Hive", prevhivepath.c_str());
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
            //std::cout << "it's a registry file, begin parsing..." << std::endl;
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
	    PopulateChildKeys(rootkey, rootitem, regerr);
	    treelist->expandTree(rootitem);
	    libregf_key_free(&rootkey, &regerr);
	    libregf_file_close(regfile, &regerr);
	    libregf_file_free(&regfile, &regerr);
	    libregf_error_free(&regerr);
        }
        else
            std::cout << "check failed..." << std::endl;
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

/*
// Create and initialize
  void ScribbleWindow::create(){
    // Create the windows
    FXMainWindow::create();
    // Make the main window appear
    show();
    }

// Mouse button was pressed somewhere
   long ScribbleWindow::onMouseDown(FXObject*,FXSelector,void*){

     // While the mouse is down, we'll draw lines
     mdflag=1;
     return 1;
     }

   // The mouse has moved, draw a line
   long ScribbleWindow::onMouseMove(FXObject*, FXSelector,void* ptr){
     FXEvent *ev=(FXEvent*)ptr;
     if(mdflag){

       // Get DC for the canvas
       FXDCWindow dc(canvas);

       // Set foreground color
       dc.setForeground(drawColor);

       // Draw line
       dc.drawLine(ev->last_x, ev->last_y,ev->win_x, ev->win_y);

       // We have drawn something, sonow the canvas is dirty
       dirty=1;
       }
     return 1;
     }

   // The mouse button was released again
   long ScribbleWindow::onMouseUp(FXObject*,FXSelector,void*ptr){
     FXEvent *ev=(FXEvent*) ptr;
     if(mdflag){
       FXDCWindow dc(canvas);
       dc.setForeground(drawColor);
       dc.drawLine(ev->last_x, ev->last_y,ev->win_x, ev->win_y);

       // We have drawn something, sonow the canvas is dirty
       dirty=1;

       // Mouse no longer down
       mdflag=0;
       }
     return 1;
     }

   // Paint the canvas
   long ScribbleWindow::onPaint(FXObject*,FXSelector,void*ptr){
     FXEvent *ev=(FXEvent*)ptr;
     FXDCWindow dc(canvas,ev);
     dc.setForeground(canvas->getBackColor());
     dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
     return 1;
     }

// Handle the clear message
  long ScribbleWindow::onCmdClear(FXObject*,FXSelector,void*){
    FXDCWindow dc(canvas);
    dc.setForeground(canvas->getBackColor());
    dc.fillRectangle(0,0,canvas->getWidth(),canvas->getHeight());
    dirty=0;
    return 1;
    }


  // Update the clear button
  long ScribbleWindow::onUpdClear(FXObject* sender,FXSelector,void*){
    if(dirty)
      sender->handle(this,FXSEL(SEL_COMMAND,ID_ENABLE),NULL);
    else
      sender->handle(this,FXSEL(SEL_COMMAND,ID_DISABLE),NULL);
    return 1;
    }

// Here we begin
  int main(int argc,char *argv[]){

    // Make application
    FXApp* application=new FXApp("Scribble","Test");

    // Start app
    application->init(argc,argv);
    // Scribble window
    new ScribbleWindow(application);

    // Create the application's windows
    application->create();

    // Run the application
    application->run();

    return 0;
    }
*/

int main(int argc, char* argv[])
{
    FXApp* wr = new FXApp("Registry", "Wombat");

    wr->init(argc, argv);

    new WombatRegistry(wr);

    wr->create();
    wr->run();

    return 0;
}
