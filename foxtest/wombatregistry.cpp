#include <iostream>
#include <stdio.h>
#include "/usr/local/include/fox-1.7/fx.h"
#include "folder-open.h"

class WombatRegistry : public FXMainWindow
{
    FXDECLARE(WombatRegistry)

    private:
        FXVerticalFrame* mainframe;
        FXSplitter* hsplitter;
        FXSplitter* vsplitter;
        FXTreeList* treelist;
        FXToolBar* toolbar;
        FXText* plaintext;
        FXTable* tablelist;
        FXTreeItem* rootitem;
	FXIcon* openicon;
        FXButton* openbutton;

    protected:
        WombatRegistry() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_TREELIST = 1,
            ID_OPEN,
            //ID_CANVAS = FXMainWindow::ID_LAST,
            //ID_CLEAR,
            ID_LAST
        };
        WombatRegistry(FXApp* a);
        long onMouseDown(FXObject*, FXSelector, void*);
        virtual void create();

};

/*
class ScribbleWindow : public FXMainWindow
{
    FXDECLARE(ScribbleWindow)
    private:
    FXHorizontalFrame *contents;     // Content frame
    FXVerticalFrame   *buttonFrame;  // Button frame
    FXVerticalFrame   *canvasFrame;                                 // 
    FXCanvas          *canvas;       // Canvas to draw into
    int                mdflag;       // Mouse button down?
    int                dirty;        // Canvas has been painted?
    FXColor            drawColor;    // Color for the line

    protected:
        ScribbleWindow() {}

        long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        long onPaint(FXObject*,FXSelector,void*);
        long onMouseDown(FXObject*,FXSelector,void*);
        long onMouseUp(FXObject*,FXSelector,void*);
        long onMouseMove(FXObject*,FXSelector,void*);
        long onCmdClear(FXObject*,FXSelector,void*);
        long onUpdClear(FXObject*,FXSelector,void*);

    public:
    enum{
      ID_CANVAS=FXMainWindow::ID_LAST,
      ID_CLEAR,
      ID_LAST
    };

    public:
    ScribbleWindow(FXApp* a);
    virtual void create();
};

FXDEFMAP(ScribbleWindow) ScribbleWindowMap[]={
    //________Message_Type_____________________ID_______________Message_Handler_______
    FXMAPFUNC(SEL_PAINT,            ScribbleWindow::ID_CANVAS,ScribbleWindow::onPaint),
    FXMAPFUNC(SEL_LEFTBUTTONPRESS,  ScribbleWindow::ID_CANVAS,ScribbleWindow::onMouseDown),
    FXMAPFUNC(SEL_LEFTBUTTONRELEASE,ScribbleWindow::ID_CANVAS,ScribbleWindow::onMouseUp),
    FXMAPFUNC(SEL_MOTION,           ScribbleWindow::ID_CANVAS,ScribbleWindow::onMouseMove),
    FXMAPFUNC(SEL_COMMAND,          ScribbleWindow::ID_CLEAR, ScribbleWindow::onCmdClear),
    FXMAPFUNC(SEL_UPDATE,           ScribbleWindow::ID_CLEAR, ScribbleWindow::onUpdClear),
    };

FXIMPLEMENT(ScribbleWindow,FXMainWindow,ScribbleWindowMap,ARRAYNUMBER(ScribbleWindowMap))
*/

FXDEFMAP(WombatRegistry) WombatRegistryMap[]={
    FXMAPFUNC(SEL_CHANGED, WombatRegistry::ID_TREELIST, WombatRegistry::onMouseDown),
    FXMAPFUNC(SEL_LEFTBUTTONPRESS, WombatRegistry::ID_TREELIST, WombatRegistry::onMouseDown),
    //FXMAPFUNC(SEL_LEFTBUTTONPRESS, WombatRegistry::ID_CANVAS, WombatRegistry::onMouseDown),
};

FXIMPLEMENT(WombatRegistry,FXMainWindow,WombatRegistryMap,ARRAYNUMBER(WombatRegistryMap))

WombatRegistry::WombatRegistry(FXApp* a):FXMainWindow(a, "Wombat Registry Forensics", NULL, NULL, DECOR_ALL, 0, 0, 1024, 768)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    toolbar = new FXToolBar(mainframe, this, LAYOUT_TOP|LAYOUT_LEFT);
    vsplitter = new FXSplitter(mainframe, SPLITTER_NORMAL|LAYOUT_FILL);
    treelist = new FXTreeList(vsplitter, this, ID_TREELIST, TREELIST_SHOWS_LINES|TREELIST_SINGLESELECT|TREELIST_ROOT_BOXES|TREELIST_SHOWS_BOXES);
    hsplitter = new FXSplitter(vsplitter, SPLITTER_VERTICAL);
    tablelist = new FXTable(hsplitter);
    plaintext = new FXText(hsplitter);
    openicon = new FXPNGIcon(this->getApp(), folderopen);
    openbutton = new FXButton(toolbar, "Open", openicon, this, ID_OPEN, BUTTON_NORMAL);
    //toolbar->setHeight(150);
    //toolbar->layout();
    //openbutton = new FXButton(toolbar, "Open", NULL, this, 0, BUTTON_TOOLBAR);

    //rootitem = treelist->getFirstItem();
    rootitem = new FXTreeItem("Root Item");
    //std::cout << "firstitem:" << rootitem;
    //FXTreeItem* mainitem = new FXTreeItem("Root Item");
    //treelist->setAnchorItem(rootitem);
    treelist->appendItem(0, rootitem);
    //treelist->makeItemVisible(rootitem);
    //treelist->appendItem(0, mainitem);
    //treelist->appendItem(mainitem, new FXTreeItem("Test 2"));


}

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
    show();
}

long WombatRegistry::onMouseDown(FXObject*, FXSelector, void*)
{
    std::cout << "mouse down pressed." << std::endl;
    printf("hello there.");
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
