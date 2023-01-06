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
    tablelist = new FXTable(hsplitter);
    plaintext = new FXText(hsplitter);
    tablelist->setHeight(this->getHeight() / 3);
    tablelist->setVisibleColumns(3);
    tablelist->setEditable(false);
    tablelist->setColumnText(0, "Tag");
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
    treelist->setCurrentItem(rootitem);
    return 1;
}

long WombatRegistry::OpenTagManager(FXObject*, FXSelector, void*)
{
    ManageTags tagmanager(this, "Manage Tags");
    tagmanager.SetTagList(&tags);
    //tagmanager.setX(getX()+80);
    //tagmanager.setY(getY()+80);
    tagmanager.execute(PLACEMENT_OWNER);
    return 1;
}

long WombatRegistry::OpenHive(FXObject*, FXSelector, void*)
{
    if(prevhivepath.empty())
        prevhivepath = getenv("HOME");
    //if(prevhivepath.isEmpty())
    //	prevhivepath = QDir::homePath();
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
        //else if(hfssigstr.find("H+") != std::string::npos) // HFS+
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
            //rootitem->setText(rootitemstring);
            rootitem = new FXTreeItem(rootitemstring);
            //treelist->setAnchorItem(rootitem);
            treelist->appendItem(0, rootitem);
        }
        else
            std::cout << "check failed..." << std::endl;
    }
    /*
        std::size_t lfound = fileinfovector.at(i).find("|");
        filename = fileinfovector.at(i).substr(0, lfound);
	std::size_t rfound = fileinfovector.at(i).rfind("|");
        mntptstr = fileinfovector.at(i).substr(lfound+1, rfound - lfound-1);
	devicestr = fileinfovector.at(i).substr(rfound+1);

    QTreeWidgetItem* rootitem = new QTreeWidgetItem(ui->treewidget);
    rootitem->setText(0, hivefilepath.split("/").last().toUpper() + " (" + hivefilepath + ")");
    //rootitem->setText(0, hivefilepath.split("/").last().toUpper());
    ui->treewidget->addTopLevelItem(rootitem);
    PopulateChildKeys(rootkey, rootitem, regerr);
    ui->treewidget->expandItem(rootitem);
    libregf_key_free(&rootkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
     */ 
    //rootitem = treelist->getFirstItem();

    //rootitem = new FXTreeItem("Root Item");

    //std::cout << "firstitem:" << rootitem;
    //FXTreeItem* mainitem = new FXTreeItem("Root Item");
    //treelist->setAnchorItem(rootitem);

    //treelist->appendItem(0, rootitem);

    /*
        hivefile.setFileName(hivefilepath);
        if(!hivefile.isOpen())
            hivefile.open(QIODevice::ReadOnly);
        if(hivefile.isOpen())
        {
            hivefile.seek(0);
            uint32_t hiveheader = qFromBigEndian<uint32_t>(hivefile.read(4));
            if(hiveheader == 0x72656766) // valid "regf" header
            {
                LoadRegistryFile();
                StatusUpdate("Hive: " + openhivedialog.selectedFiles().first() + " successfully opened.");
            }
	    hivefile.close();
        }
     */ 
    //std::cout << "file name selected:" << filename.text() << std::endl;
    return 1;
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
