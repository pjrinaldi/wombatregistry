#include "wombatregistry.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

WombatRegistry::WombatRegistry(QWidget* parent) : QMainWindow(parent), ui(new Ui::WombatRegistry)
{
    ui->setupUi(this);
    this->menuBar()->hide();
    statuslabel = new QLabel(this);
    this->statusBar()->addPermanentWidget(statuslabel, 0);
    StatusUpdate("Open a Hive to Begin");
    ui->tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tablewidget->setHorizontalHeaderLabels({"Tag", "Value Name", "Value Type"});
    connect(ui->treewidget, SIGNAL(itemSelectionChanged()), this, SLOT(KeySelected()), Qt::DirectConnection);
    connect(ui->tablewidget, SIGNAL(itemSelectionChanged()), this, SLOT(ValueSelected()), Qt::DirectConnection);
    connect(ui->actionOpenHive, SIGNAL(triggered()), this, SLOT(OpenHive()), Qt::DirectConnection);
    connect(ui->actionManageTags, SIGNAL(triggered()), this, SLOT(ManageTags()), Qt::DirectConnection);
    connect(ui->actionPreviewReport, SIGNAL(triggered()), this, SLOT(PreviewReport()), Qt::DirectConnection);
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(ShowAbout()), Qt::DirectConnection);
    // initialize temp directory for html code...
    QDir tmpdir;
    tmpdir.mkpath(QDir::tempPath() + "/wr");
    tmpdir.mkpath(QDir::tempPath() + "/wr/tagged");
    // initialize Preview Report HTML code
    prehtml = "<html><body style='" + ReturnCssString(0) + "'>\n";
    prehtml += "<div style='" + ReturnCssString(1) + "'><h1><span id='casename'>Registry Report</span></h1></div>\n";
    //reportstring = "<html><body style='" + ReturnCssString(0) + "'>\n";
    //reportstring += "<div style='" + ReturnCssString(1) + "'><h1><span id='casename'></span></h1></div>\n"; // figure out title of report
    //prehtml = reportstring;
    // OR DO I CARE ABOUT TIMEZONE AND JUST WANT TO LEAVE IT IN UTC...
    /*
    reportstring += "<div id='tz'><h4>Report Time Zone:&nbsp;" + reporttimezone + "</h4></div><br/>\n"; // create reporttimezone variable
    reportstring += "<div id='toc'><h2>Contents</h2>";
    reportstring += "<div id='elinks'>";
    reportstring += "<!--firstlink-->";
    reportstring += "<!--lastlink-->";
    reportstring += "</div><br/>";
    reportstring += "</div><br/><br/>";
    reportstring += "<h2>Tagged Items</h2>";
    reportstring += "<div id='tags'>";
    reportstring += "<!--firsttag-->";
    reportstring += "<!--lasttag--></div>";
    */
    psthtml = "</body></html>";
    //reportstring += "</body></html>";

    /*
    QString initialhtml = "";
    previewfile.setFileName(wombatvariable.tmpmntpath + "previewreport.html");
    previewfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if(previewfile.isOpen())
    {
        QTimeZone itz = QTimeZone(reporttimezone);
        //previewfile.write(initialhtml.toStdString().c_str());
        QString initialstr = "<html><body style='" + ReturnCssString(0) + "'>";
        initialstr = "<div style='" + ReturnCssString(1) + "'><h1>Case Title:&nbsp;<span id='casename'>" + wombatvariable.casename + "</span></h1></div>\n";
        initialstr += "<div id='tz'><h4>Report Time Zone:&nbsp;" + reporttimezone + "</h4><div><br/>\n";
        initialstr += "<div id='toc'><h2>Contents</h2>";
        initialstr += "<div id='elinks'>";
        initialstr += "<!--firstelink-->";
        initialstr += "<!--lastelink-->";
        initialstr += "</div><br/>";
        initialstr += "<div id='tlinks'>";
        initialstr += "<!--firsttlink-->";
        initialstr += "<!--lasttlink-->";
        initialstr += "</div><br/><br/>";
        initialstr += "</div><br/><br/>";
        initialstr += "<h2>Evidence Items</h2>";
        initialstr += "<div id='evidence'>";
        initialstr += "<!--firstevid-->";
        initialstr += "<!--lastevid-->";
        initialstr += "\n</div><br/><br/>";
        initialstr += "<h2>Tagged Items</h2>";
        initialstr += "<div id='tags'>";
        initialstr += "<!--firsttag-->";
        initialstr += "<!--lasttag--></div>";
        previewfile.write(initialstr.toStdString().c_str());
 
   }
    previewfile.close();

     */ 
    
    tags.clear();
    tagmenu = new QMenu(ui->tablewidget);
    UpdateTagsMenu();

    ui->tablewidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tablewidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TagMenu(const QPoint &)), Qt::DirectConnection);

    /*
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
	taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    for(int i=0; i < taglist.count(); i++)
    {
	QAction* tmpaction = new QAction(taglist.at(i), tagmenu);
	tmpaction->setIcon(QIcon(":/bar/addtotag"));
	tmpaction->setData(QVariant("t" + QString::number(i)));
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetTag()));
	tagmenu->addAction(tmpaction);
    }
    tagmenu->addSeparator();
    QAction* remtagaction = new QAction("Remove Tag", tagmenu);
    remtagaction->setIcon(QIcon(":/bar/tag-rem"));
    connect(remtagaction, SIGNAL(triggered()), this, SLOT(RemoveTag()));
    tagmenu->addAction(remtagaction);
    */

    /*
    QFile registryfile;
    registryfile.setFileName(wombatvariable.tmpmntpath + "registrytags");
    registryfile.open(QIODevice::ReadOnly | QIODevice::Text);
    registrytaglist.clear();
    if(registryfile.isOpen())
	registrytaglist = QString(registryfile.readLine()).split(",", Qt::SkipEmptyParts);
    registryfile.close();
    */
}

WombatRegistry::~WombatRegistry()
{
    delete ui;
    QDir tmpdir(QDir::tempPath() + "/wr");
    tmpdir.removeRecursively();
}

void WombatRegistry::OpenHive()
{
    QFileDialog openhivedialog(this, tr("Open Registry Hive"), QDir::homePath());
    openhivedialog.setLabelText(QFileDialog::Accept, "Open");
    //openhivedialog.setOption(QFileDialog::DontUseNativeDialog, true);
    if(openhivedialog.exec())
    {
        hivefilepath = openhivedialog.selectedFiles().first();
        hivefile.setFileName(hivefilepath);
        if(!hivefile.isOpen())
            hivefile.open(QIODevice::ReadOnly);
        if(hivefile.isOpen())
        {
            hivefile.seek(0);
            uint32_t hiveheader = qFromBigEndian<uint32_t>(hivefile.read(4));
            //qDebug() << "hiveheader:" << QString::number(hiveheader, 16);
            if(hiveheader == 0x72656766) // valid "regf" header
            {
                LoadRegistryFile();
                hivename = hivefilepath.split("/").last().split(".").first();
                if(!hivename.isEmpty())
                {
                    this->setWindowTitle(QString("Wombat Registry - ") + hivename);
                }
                StatusUpdate("Hive: " + openhivedialog.selectedFiles().first() + " successfully opened.");
            }
                                        
        }
        // open file, read header and verify it is a registry file to process.
    }
}

void WombatRegistry::ManageTags()
{
    TagManager* tagmanager = new TagManager(this);
    //connect(tagmanage, SIGNAL(HideManagerWindow()), this, SLOT(HideTagManager()), Qt::DirectConnection);
    tagmanager->SetTagList(&tags);
    tagmanager->exec();
    UpdateTagsMenu();
    UpdatePreviewLinks();
    /*
    tagmanage->setWindowIcon(QIcon(":/bar/managetags"));
    connect(tagmanage, SIGNAL(ReadBookmarks()), this, SLOT(ReadBookmarks()), Qt::DirectConnection);

void WombatForensics::HideTagManager()
{
    treemenu->clear();
    ReadBookmarks();
    treemenu->addAction(ui->actionView_File);
    treemenu->addAction(ui->actionView_Properties);
    treemenu->addAction(viewmenu->menuAction());
    treemenu->addSeparator();
    treemenu->addAction(ui->actionCheck);
    treemenu->addAction(remcheckedaction);
    treemenu->addSeparator();
    treemenu->addMenu(bookmarkmenu);
    treemenu->addAction(remtagaction);
    treemenu->addSeparator();
    treemenu->addMenu(tagcheckedmenu);
    treemenu->addAction(remtagaction1);
    treemenu->addSeparator();
    treemenu->addAction(ui->actionDigDeeper);
    treemenu->addAction(ui->actionExport);
    treemenu->addAction(ui->actionExportForensicImage);
}
     */ 

}

void WombatRegistry::UpdatePreviewLinks()
{
    // POSSIBLY REBUILD THE MAIN PAGE EVERY TIME, RATHER THAN FIND AND REPLACE...
    QString curcontent = "";
    curcontent += "<div id='toc'><h2>Contents</h2>";
    for(int i=0; i < tags.count(); i++)
    {
        curcontent += "<span id='t" + QString::number(i) + "'><a href='#t" + QString::number(i) + "'>" + tags.at(i) + "</a></span><br/>\n";
    }
    curcontent += "<h2>Tagged Items</h2>";
    for(int i=0; i < tags.count(); i++)
    {
        curcontent += "<div id='t" + QString::number(i) + "'><h3>" + tags.at(i) + "</h3><br/><br/><table><tr>";
        for(int j=0; j < taggeditems.count(); j++)
        {
            if(taggeditems.at(j).split("|", Qt::SkipEmptyParts).at(0) == tags.at(i))
                curcontent += "<td>" + taggeditems.at(j).split("|", Qt::SkipEmptyParts).at(1) + "</td>";
                /*
                 *
                htmlentry = "";
                htmlentry += "<td style='" + ReturnCssString(11) + "' id='" + this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text() + "'>";
                htmlentry += "<table style='" + ReturnCssString(2) + "' width='300px'><tr style='" + ReturnCssString(3) + "'><th style='" + ReturnCssString(6) + "' colspan='2'>" + ui->tableWidget->selectedItems().first()->text() + "</th></tr>";
                htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Path:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->label->text() + "</span></td></tr>";
                htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Last Modified:</td><td style='" + ReturnCssString(14) + "'>" + ConvertWindowsTimeToUnixTime(lastwritetime) + "</td></tr>";
                htmlentry += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>ID:</td><td style='" + ReturnCssString(14) + "'>" + this->windowTitle().mid(16) + "</td></tr>";
                htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='" + ReturnCssString(7) + "'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./registry/" + this->windowTitle().mid(16) + "." + ui->label->text().replace("\\", "-") + "-" + ui->tableWidget->selectedItems().first()->text() + "'>Link</a></td></tr>";
                htmlentry += "</table></td>";
                htmlvalue = "<html><body style='" + ReturnCssString(0) + "'>";
                QFile initfile(":/html/artifactprephtml");
                initfile.open(QIODevice::ReadOnly);
                if(initfile.isOpen())
                    htmlvalue = initfile.readAll();
                initfile.close();
                htmlvalue += "<div style='" + ReturnCssString(1) + "'>Registry Analysis</div><br/>";
                htmlvalue += "<pre>";
                htmlvalue += valuedata;
                htmlvalue += "</pre>";
                htmlvalue += "</table></body></html>";
                */ 
        }
        curcontent += "</tr></table></div><br/>\n";
        //curcontent += "<div id='t" + QString::number(tagid) + "'><h3>" + tagname + "</h3><br/><br/><table><tr><!--firstfile--><!--lastfile--></tr></table></div><br/>\n";
    }
    //reportstring += "<h2>Tagged Items</h2>";
    reportstring = prehtml + curcontent + psthtml;

    // OLD METHOD TO FIND AND REPLACE
    /*
    QString origstr = "";
    QString linkstr = "";
    QStringList beginsplit = reportstring.split("<!--firstlink-->", Qt::SkipEmptyParts);
    QString precontent = beginsplit.first();
    precontent += "<!--firstlink-->";
    QString curcontent = beginsplit.last().split("<!--lastlink-->").first();
    QString postcontent = beginsplit.last().split("<!--lastlink-->").last();
    postcontent = "<!--lastlink-->" + postcontent;
    QStringList linklist = curcontent.split("\n", Qt::SkipEmptyParts);
    linkstr = "";
    for(int i=0; i < tags.count(); i++)
    {
        linkstr += "<span id='t" + QString::number(i) + "'><a href='#t" + QString::number(i) + "'>" + tags.at(i) + "</a></span><br/>\n";
    }
    reportstring = precontent + linkstr + postcontent;
    */

    /*
    if(tagid == linklist.count())
        linkstr += "<span id='l" + QString::number(tagid) + "'><a href='#t" + QString::number(tagid) + "'>" + tagname + "</a></span><br/>\n";
    curcontent += linkstr;
    isignals->ActivateReload();
     */ 
}

void WombatRegistry::PreviewReport()
{
    HtmlViewer* htmlviewer = new HtmlViewer();
    //qDebug() << "reportstring:" << reportstring;
    htmlviewer->SetSource(&reportstring);
    htmlviewer->show();
}

void WombatRegistry::ShowAbout()
{
    AboutBox* aboutbox = new AboutBox();
    aboutbox->exec();
}

/*
void WombatRegistry::HideClicked()
{
    //this->close();
}
*/

void WombatRegistry::CreateNewTag()
{
    QString tagname = "";
    QInputDialog* newtagdialog = new QInputDialog(this);
    newtagdialog->setCancelButtonText("Cancel");
    newtagdialog->setInputMode(QInputDialog::TextInput);
    newtagdialog->setLabelText("Enter Tag Name");
    newtagdialog->setOkButtonText("Create Tag");
    newtagdialog->setTextEchoMode(QLineEdit::Normal);
    newtagdialog->setWindowTitle("New Tag");
    if(newtagdialog->exec())
        tagname = newtagdialog->textValue();
    if(!tagname.isEmpty())
    {
	tags.append(tagname);
        UpdateTagsMenu();
    }
    ui->tablewidget->selectedItems().first()->setText(tagname);
}

void WombatRegistry::UpdateTagsMenu()
{
    tagmenu->clear();
    newtagaction = new QAction("New Tag", tagmenu);
    newtagaction->setIcon(QIcon(":/bar/newtag"));
    connect(newtagaction, SIGNAL(triggered()), this, SLOT(CreateNewTag()));
    tagmenu->addAction(newtagaction);
    tagmenu->addSeparator();
    for(int i=0; i < tags.count(); i++)
    {
	QAction* tmpaction = new QAction(tags.at(i), tagmenu);
	tmpaction->setIcon(QIcon(":/bar/tag"));
	tmpaction->setData(QVariant("t" + QString::number(i)));
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetTag()));
	tagmenu->addAction(tmpaction);
    }
    tagmenu->addSeparator();
    remtagaction = new QAction("Remove Tag", tagmenu);
    remtagaction->setIcon(QIcon(":/bar/removetag"));
    connect(remtagaction, SIGNAL(triggered()), this, SLOT(RemoveTag()));
    tagmenu->addAction(remtagaction);
}

void WombatRegistry::SetTag()
{
    QString curtag = "";
    //QString regstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    //regstring += statuslabel->text() + "\\";
    //regstring += ui->tablewidget->selectedItems().at(1)->text() + "|";
    //if(!ui->tablewidget->selectedItems().first()->text().isEmpty())
    //    curtag = regstring + ui->tablewidget->selectedItems().first()->text();
    //regstring += tagaction->iconText();
    QString idkeyvalue = statuslabel->text() + "\\" + ui->tablewidget->selectedItems().at(1)->text();
    QString htmlvalue = ui->plaintext->toPlainText();
    //qDebug() << "regstring:" << regstring;
    //qDebug() << "curtag:" << curtag;
    //qDebug() << "idkeyvalue:" << idkeyvalue;
    //qDebug() << "htmlvalue:" << htmlvalue;

    /*
    regstring += this->windowTitle().mid(16) + "|"; // file id
    regstring += ui->label->text() + "\\"; // key
    regstring += ui->tableWidget->selectedItems().first()->text() + "|";
    if(!ui->tableWidget->selectedItems().last()->text().isEmpty())
	curtag = regstring + ui->tableWidget->selectedItems().last()->text();
    regstring += tagaction->iconText();
    QString idkeyvalue = this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text();
    */
    //if(!ui->tablewidget->selectedItems().first()->text().isEmpty())
	//curtag = ui->tablewidget->selectedItems().first()->text();
    ui->tablewidget->selectedItems().first()->setText(tagaction->iconText());
    //qDebug() << "curtag to remove:" << curtag;
    /*
    if(!curtag.isEmpty())
	RemTag("registry", curtag);
    AddTag("registry", regstring); // add htmlentry and htmlvalue to this function...
    RemoveFileItem(idkeyvalue);
    RemoveArtifactFile("registry", idkeyvalue);
    AddFileItem(tagaction->iconText(), htmlentry);
    CreateArtifactFile("registry", idkeyvalue, htmlvalue);
    // ADD TO PREVIEW REPORT
    //RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
    //AddFileItem(tagname, filestr);
    //CreateArtifactFile("registry", curtag, htmlvalue);
    */
}

void WombatRegistry::RemoveTag()
{
    //QAction* tagaction = qobject_cast<QAction*>(sender());
    ui->tablewidget->selectedItems().first()->setText("");
    /*
    //qDebug() << "remove tag";
    QString regstring = "";
    regstring += this->windowTitle().mid(16) + "|"; // file id
    regstring += ui->label->text() + "\\"; // key
    regstring += ui->tableWidget->selectedItems().first()->text() + "|";
    regstring += tagaction->iconText() + ",";
    QString idkeyvalue = this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text();
    RemTag("registry", idkeyvalue + "|" + ui->tableWidget->selectedItems().last()->text());
    // REMOVE FROM PREVIEW REPORT
    RemoveFileItem(idkeyvalue);
    RemoveArtifactFile("registry", idkeyvalue);
    //RemoveFileItem(selectedindex.sibling(selectedindex.row(), 11).data().toString());
    */
}

void WombatRegistry::ValueSelected(void)
{
    if(ui->tablewidget->selectedItems().count() > 0)
    {
        //QTimeZone tmpzone = QTimeZone(reporttimezone);
	/*
        QString filestr = "<td class='fitem' id='" + curindex.sibling(curindex.row(), 11).data().toString() + "'>";
        filestr += "<table width='300px'><tr><th colspan='2'>" + curindex.sibling(curindex.row(), 0).data().toString() + "</th></tr>";
        filestr += "<tr class='odd vtop'><td class='pvalue'>File Path:</td><td class='property'><span style='word-wrap:break-word;'>" + curindex.sibling(curindex.row(), 1).data().toString() + "</span></td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>File Size:</td><td class='property'>" + curindex.sibling(curindex.row(), 2).data().toString() + " bytes</td></tr>";
        if(!curindex.sibling(curindex.row(), 3).data().toString().isEmpty())
            filestr += "<tr class='odd'><td class='pvalue'>Created:</td><td class='property'>" + QDateTime::fromSecsSinceEpoch(QDateTime::fromString(curindex.sibling(curindex.row(), 3).data().toString(), "MM/dd/yyyy hh:mm:ss AP").toSecsSinceEpoch(), tmpzone).toString("MM/dd/yyyy hh:mm:ss AP") + "</td></tr>";
        filestr += "<tr class='even'><td class='pvalue'>ID:</td><td class='property'>" + curindex.sibling(curindex.row(), 11).data().toString() + "</td></tr>";
        filestr += "<tr class='odd'><td class='pvalue'>&nbsp;</td><td class='lvalue'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='javascript:void(0)' onclick='ShowContent(\"./files/" + curindex.sibling(curindex.row(), 11).data().toString() + "\")'>Link</a></td></tr>";
        filestr += "</table></td>";
        RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
        AddFileItem(tagname, filestr);
	*/

/*
	htmlentry = "";
	htmlentry += "<td style='" + ReturnCssString(11) + "' id='" + this->windowTitle().mid(16) + "|" + ui->label->text() + "\\" + ui->tableWidget->selectedItems().first()->text() + "'>";
	htmlentry += "<table style='" + ReturnCssString(2) + "' width='300px'><tr style='" + ReturnCssString(3) + "'><th style='" + ReturnCssString(6) + "' colspan='2'>" + ui->tableWidget->selectedItems().first()->text() + "</th></tr>";
	htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Path:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->label->text() + "</span></td></tr>";
*/
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
/*
        htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>Last Modified:</td><td style='" + ReturnCssString(14) + "'>" + ConvertWindowsTimeToUnixTime(lastwritetime) + "</td></tr>";
	htmlentry += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(13) + "'>ID:</td><td style='" + ReturnCssString(14) + "'>" + this->windowTitle().mid(16) + "</td></tr>";
        htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='" + ReturnCssString(7) + "'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./registry/" + this->windowTitle().mid(16) + "." + ui->label->text().replace("\\", "-") + "-" + ui->tableWidget->selectedItems().first()->text() + "'>Link</a></td></tr>";
	htmlentry += "</table></td>";
        */
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
	            //QString filenamestring = QString::fromStdString(QByteArray(info2content.mid(curpos + 3, 260).toStdString().c_str(), -1).toStdString());
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
                /*
                else if(keypath.contains("SAM") && ui->tableWidget->selectedItems().first()->text().count() == 1 && ui->tableWidget->selectedItems().first()->text().startsWith("V"))
                {
                    size_t datasize = 0;
                    libregf_value_get_value_data_size(curval, &datasize, &regerr);
                    uint8_t data[datasize];
                    libregf_value_get_value_data(curval, data, datasize, &regerr);
                    QByteArray varray = QByteArray::fromRawData((char*)data, datasize);
                    valuedata += "Machine SID Location:\t" + varray.right(12).toHex();
                }
                */
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
	ui->plaintext->setPlainText(valuedata);

        /*
    	htmlvalue = "<html><body style='" + ReturnCssString(0) + "'>";
	QFile initfile(":/html/artifactprephtml");
	initfile.open(QIODevice::ReadOnly);
	if(initfile.isOpen())
	    htmlvalue = initfile.readAll();
	initfile.close();
        */
/*	htmlvalue += "<div style='" + ReturnCssString(1) + "'>Registry Analysis</div><br/>";
	htmlvalue += "<pre>";
	htmlvalue += valuedata;
	htmlvalue += "</pre>";
	htmlvalue += "</table></body></html>";
*/
        libregf_value_free(&curval, &regerr);
        libregf_key_free(&curkey, &regerr);
        libregf_file_close(regfile, &regerr);
        libregf_file_free(&regfile, &regerr);
        libregf_error_free(&regerr);
    }
}

void WombatRegistry::KeySelected(void)
{
    int itemindex = 0;
    QTreeWidgetItem* curitem = ui->treewidget->selectedItems().first();
    bool toplevel = false;
    QStringList pathitems;
    pathitems.clear();
    pathitems.append(curitem->text(itemindex));
    QTreeWidgetItem* parent;
    QTreeWidgetItem* child;
    child = curitem;
    while(toplevel == false)
    {
	parent = child->parent();
	if(parent == nullptr)
	    toplevel = true;
	else
	{
	    pathitems.append(parent->text(itemindex));
	    child = parent;
	}
    }
    // build path
    QString keypath = "";
    QChar sepchar = QChar(92);
    for(int i = pathitems.count() - 2; i > -1; i--)
    {
	keypath += "/" + pathitems.at(i);
    }
    keypath.replace("/", sepchar);
    // attempt to open by path...
    StatusUpdate(keypath);
    //ui->label->setText(keypath);
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    libregf_file_open(regfile, hivefilepath.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_key_t* curkey = NULL;
    libregf_file_get_key_by_utf8_path(regfile, (uint8_t*)(keypath.toUtf8().data()), keypath.toUtf8().size(), &curkey, &regerr);
    // valid key, get values...
    int valuecount = 0;
    libregf_key_get_number_of_values(curkey, &valuecount, &regerr);
    ui->tablewidget->clear();
    ui->plaintext->setPlainText("");
    ui->tablewidget->setRowCount(valuecount);
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
	QString curtagvalue = this->windowTitle().mid(16) + "|" + keypath + "\\";
	if(namesize == 0)
	{
	    curtagvalue += "(unnamed)";
	    ui->tablewidget->setHorizontalHeaderLabels({"Tag", "Value Name", "Value"});
	    ui->tablewidget->setItem(i, 1, new QTableWidgetItem("(unnamed)"));
	    ui->tablewidget->setItem(i, 2, new QTableWidgetItem(QString::number(type, 16)));
	    //ui->tablewidget->setItem(i, 2, new QTableWidgetItem(""));
	}
	else
	{
	    curtagvalue += QString::fromUtf8(reinterpret_cast<char*>(name));
            QString valuetypestr = "";
	    ui->tablewidget->setHorizontalHeaderLabels({"Tag", "Value Name", "Value Type"});
	    ui->tablewidget->setItem(i, 1, new QTableWidgetItem(QString::fromUtf8(reinterpret_cast<char*>(name))));
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
	    ui->tablewidget->setItem(i, 2, new QTableWidgetItem(valuetypestr));
	    //ui->tablewidget->setItem(i, 2, new QTableWidgetItem(""));
	}
	//qDebug() << "curtagvalue:" << curtagvalue;
	QString tagstr = "";
	for(int j=0; j < registrytaglist.count(); j++)
	{
	    //qDebug() << "registry tag list at(" << j << ") :" << registrytaglist.at(j);
	    if(registrytaglist.at(j).contains(curtagvalue))
		tagstr = registrytaglist.at(j).split("|", Qt::SkipEmptyParts).last();
	}
	//qDebug() << "tagstr:" << tagstr;
	ui->tablewidget->setItem(i, 0, new QTableWidgetItem(tagstr));
        ui->tablewidget->resizeColumnToContents(0);
        ui->tablewidget->setCurrentCell(0, 0);
	libregf_value_free(&curval, &regerr);
    }
    libregf_key_free(&curkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
}
void WombatRegistry::closeEvent(QCloseEvent* e)
{
    e->accept();
}

void WombatRegistry::LoadRegistryFile(void)
{
    //qDebug() << "regfilepath:" << regfilepath;
    libregf_file_t* regfile = NULL;
    libregf_error_t* regerr = NULL;
    libregf_file_initialize(&regfile, &regerr);
    libregf_file_open(regfile, hivefilepath.toStdString().c_str(), LIBREGF_OPEN_READ, &regerr);
    libregf_error_fprint(regerr, stderr);
    libregf_key_t* rootkey = NULL;
    libregf_file_get_root_key(regfile, &rootkey, &regerr);
    libregf_error_fprint(regerr, stderr);
    int rootsubkeycnt = 0;
    libregf_key_get_number_of_sub_keys(rootkey, &rootsubkeycnt, &regerr);
    libregf_error_fprint(regerr, stderr);
        QTreeWidgetItem* rootitem = new QTreeWidgetItem(ui->treewidget);
    rootitem->setText(0, hivefilepath.split("/").last().toUpper());
    ui->treewidget->addTopLevelItem(rootitem);
    PopulateChildKeys(rootkey, rootitem, regerr);
    ui->treewidget->expandItem(rootitem);
    libregf_key_free(&rootkey, &regerr);
    libregf_file_close(regfile, &regerr);
    libregf_file_free(&regfile, &regerr);
    libregf_error_free(&regerr);
}

void WombatRegistry::PopulateChildKeys(libregf_key_t* curkey, QTreeWidgetItem* curitem, libregf_error_t* regerr)
{
    //qDebug() << "populate:" << hivefilepath;
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
	    QTreeWidgetItem* subitem = new QTreeWidgetItem(curitem);
	    subitem->setText(0, QString::fromUtf8(reinterpret_cast<char*>(name)));
	    curitem->addChild(subitem);
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

QString WombatRegistry::DecryptRot13(QString encstr)
{
    QString decstr = "";
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

QChar WombatRegistry::Rot13Char(QChar curchar)
{
    QChar rot13char;
    if('0' <= curchar && curchar <= '4')
        rot13char = QChar(curchar.unicode() + 5);
    else if('5' <= curchar && curchar <= '9')
        rot13char = QChar(curchar.unicode() - 5);
    else if('A' <= curchar && curchar <= 'M')
        rot13char = QChar(curchar.unicode() + 13);
    else if('N' <= curchar && curchar <= 'Z')
        rot13char = QChar(curchar.unicode() - 13);
    else if('a' <= curchar && curchar <= 'm')
        rot13char = QChar(curchar.unicode() + 13);
    else if('n' <= curchar && curchar <= 'z')
        rot13char = QChar(curchar.unicode() - 13);
    else
        rot13char = curchar;
    return rot13char;
}

QString WombatRegistry::ConvertUnixTimeToString(uint32_t input)
{
    //QTimeZone tmpzone = QTimeZone(reporttimezone);
    time_t crtimet = (time_t)input;
    QString timestr = QDateTime::fromSecsSinceEpoch(crtimet, QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");

    return timestr;
}

QString WombatRegistry::ConvertWindowsTimeToUnixTimeUTC(uint64_t input)
{
    //QTimeZone tmpzone = QTimeZone(reporttimezone);
    uint64_t temp;
    temp = input / TICKS_PER_SECOND; //convert from 100ns intervals to seconds;
    temp = temp - EPOCH_DIFFERENCE;  //subtract number of seconds between epochs
    time_t crtimet = (time_t)temp;
    QString timestr = "";
    timestr = QDateTime::fromSecsSinceEpoch(crtimet, QTimeZone::utc()).toString("MM/dd/yyyy hh:mm:ss AP");

    return timestr;
}
/*
void WombatRegistry::DoubleClick(QTableWidgetItem* curitem)
{
    qDebug() << "Double click...";
}
*/

void WombatRegistry::TagMenu(const QPoint &pt)
{
    QTableWidgetItem* currow = ui->tablewidget->itemAt(pt);
    if(ui->tablewidget->item(currow->row(), 0)->text().isEmpty())
	remtagaction->setEnabled(false);
    else
	remtagaction->setEnabled(true);
    tagmenu->exec(ui->tablewidget->mapToGlobal(pt));
    
    // when i need the current value for the right click, i can use a class variable defined in .h so i can access it in the SetTag and CreateNewTag right click menu options...
    //QTableWidgetItem* curitem = ui->tableWidget->itemAt(pt);
    //qDebug() << "cur item:" << ui->tableWidget->item(curitem->row(), 0)->text();
}

/*
void WombatForensics::TreeContextMenu(const QPoint &pt)
{
    QModelIndex index = ui->dirTreeView->indexAt(pt);
    if(index.isValid())
    {
        actionitem = static_cast<TreeNode*>(index.internalPointer());
        if(!actionitem->IsChecked())
        {
            ui->actionCheck->setText("Check Selected");
            ui->actionCheck->setIcon(QIcon(":/echeck"));
        }
        else
        {
            ui->actionCheck->setText("UnCheck Selected");
            ui->actionCheck->setIcon(QIcon(":/remcheck"));
        }
        treemenu->exec(ui->dirTreeView->mapToGlobal(pt));
    }
}
*/
