/* 
 * Gui CreatePageWidget.c
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 23/09/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "CreatePageWidget.h"

CreatePageWidget::CreatePageWidget(std::string namePage)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + namePage + ".Management.template"))
{
    this->addStyleClass("template");
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(namePage,false);
    
    this->nameResourcePage = namePage;
    this->dial_ = true;
    this->butSup_ = true;
    this->created_ = false;
    this->butModif_ = true;
    this->nbAff_ = 5;
    this->butAdd_ = 0;
    this->nbResource_ = 0;
}

CreatePageWidget::~CreatePageWidget() { }

// INIT Elements. ------------------------------------------------------

void    CreatePageWidget::update()
{
    if (!this->created_)
    {
        this->clear();
        this->bindWidget("resource-header", createHeaderTable());
        this->bindWidget("resource-table", createBodyTable());
        this->bindWidget("resource-footer", createFooterTable());
        this->created_ = true;
    }
    resourceBeAff();
}

// gkr: Init header.
Wt::WContainerWidget    *CreatePageWidget::createHeaderTable()
{
    Wt::WContainerWidget *headerTable = new Wt::WContainerWidget();
    headerTable->addStyleClass("widget-title");
    headerTable->setId("Header Table");
    // gkr: Logo and title header.
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
    + tr("Alert." + this->nameResourcePage + ".add-form")
    + "</h5>", headerTable);
    
    if (this->dial_) // gkr: whether class is not a dialog(popup).
        headerTable->addWidget(getComboBox());

    Wt::WAnchor *headerButton = new Wt::WAnchor(headerTable);
    if (this->butAdd_ == 0) //gkr: popup is created  in popupWindow.
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::popupWindow, this));
    else if (this->butAdd_ == 1) // gkr: Input is created in createBodyTable, showInputForAdd() is there just for show this.
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::showInputForAdd, this));

    headerButton->setTextFormat(Wt::XHTMLUnsafeText);
    headerButton->setStyleClass("buttons btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus '></i></span>");
    return headerTable;
}

Wt::WContainerWidget    *CreatePageWidget::createBodyTable()
{
    Wt::WContainerWidget *resourceTable = new Wt::WContainerWidget();
    // gkr: Init body of table
    resourceTable->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper");

    mediaTable_ = new Wt::WTable(resourceTable);
    mediaTable_->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    mediaTable_->setHeaderCount(1,Wt::Horizontal);
    
    addResourceInHeaderTable();
    fillInTable();
    
    return resourceTable;
}

Wt::WContainerWidget    *CreatePageWidget::createFooterTable()
{
    Wt::WContainerWidget *footerTable = new Wt::WContainerWidget();
    footerTable->addStyleClass("fg-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix");
    // revoir ui-toolbar quand css finit
    new Wt::WText("search : ", footerTable); //xml

    Wt::WLineEdit *search = new Wt::WLineEdit(footerTable);
    search->resize(Wt::WLength(100), Wt::WLength(15));
    search->enterPressed().connect(boost::bind(&CreatePageWidget::searchName, this, search));
    return footerTable;
}

// Add Resouce For Elements -----------------------------------------------------------------

void    CreatePageWidget::addResourceInHeaderTable()
{
    int                         columnTable(0);
    std::vector<std::string>    titleName = getHeaderTableTitle();

    for (std::vector<std::string>::iterator i = titleName.begin(); i != titleName.end(); i++)
    {
        new Wt::WText(tr("Alert." + this->nameResourcePage + "." + boost::lexical_cast<std::string>(*i)),
                mediaTable_->elementAt(0, columnTable++));        
    }
    if (this->butModif_) // gkr: whether button Modif is true.
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".modif-button"),
                mediaTable_->elementAt(0, columnTable++));
    if (this->butSup_) // gkr: whether button Sup(Delete) is true.
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".delete-button"),
                mediaTable_->elementAt(0, columnTable)); 
}

void    CreatePageWidget::fillInTable()
{
    std::vector<long long>      ids = getIdsTable();    
    int                         columnTable(0);
    int                         rowBodyTable(1);

    for (std::vector<long long>::iterator j = ids.begin(); j != ids.end(); j++)
    {
        columnTable = 0;
        vector_type resourceRow = getResourceRowTable(*j);
        for (vector_type::iterator k = resourceRow.begin(); k != resourceRow.end(); k++)
        {
            Wt::WInteractWidget *widgetAdd = *k;
            std::string nameRessouce("N2Wt5WTextE");
            if (nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
            {
                nameRessouce = boost::lexical_cast<std::string>(((Wt::WText*)(widgetAdd))->text());
                std::string newName = nameRessouce;
                if (nameRessouce.size() > SIZE_NAME_RESOURCE)
                {
                    newName.resize(SIZE_NAME_RESOURCE);
                    newName.resize(newName.size() + 3, '.');
                }
                Wt::WText *newColumn = new Wt::WText(newName, mediaTable_->elementAt(rowBodyTable, columnTable));  
                newColumn->setToolTip(nameRessouce);
                columnTable++;
            }
            else
            {
                mediaTable_->elementAt(rowBodyTable, columnTable)->addWidget(widgetAdd);
                mediaTable_->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);
                mediaTable_->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
                columnTable++;
            }
        }
        addButtons(*j, rowBodyTable, columnTable);
        rowBodyTable++;
    }
    if (!this->dial_)
        addInputForAffix(rowBodyTable);
}

void    CreatePageWidget::addButtons(long long id, int rowTable, int columnTable)
{
    if (this->butModif_)
    {
        Wt::WPushButton *modifButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
        modifButton->setAttributeValue("class","btn btn-inverse");
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
        if (this->dial_)
            modifButton->clicked().connect(boost::bind(&CreatePageWidget::popupForModif, this, id));
        else
            modifButton->clicked().connect(boost::bind(&CreatePageWidget::inputForModif, this, id, rowTable, columnTable));
        mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
        columnTable++;
    }
    if (this->butSup_)
    {
        Wt::WPushButton *delButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
        delButton->setAttributeValue("class","btn btn-danger");
        delButton->setTextFormat(Wt::XHTMLUnsafeText);
        delButton->setText("<span class='input-group-btn'><i class='icon-remove icon-white'></i></span>");
        delButton->clicked().connect(boost::bind(&CreatePageWidget::deleteResource, this, id));
        mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
        mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    }
}

void            CreatePageWidget::addInputForAffix(int rowBodyTable)
{
    std::cout << "Add input For affix" << std::endl;
    std::vector<Wt::WText*> errorMessage;
    int columnTable(0);
    if (inputs_.size() > 0)
        inputs_.clear();
    for (int i = 0; i < this->nbResource_; ++i)
    {
        std::cout << "Add Resource" << std::endl;
        Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-info"));
        error->hide();
        errorMessage.push_back(error);

        Wt::WLineEdit *input = new Wt::WLineEdit("");
        input->enterPressed().connect(boost::bind(&CreatePageWidget::checkAdd, this, errorMessage));
        input->setWidth(Wt::WLength(35, Wt::WLength::Unit::Percentage));
        input->setValidator(editValidator(i));
        input->hide();

        mediaTable_->elementAt(rowBodyTable, columnTable)->addWidget(input);
        mediaTable_->elementAt(rowBodyTable, columnTable++)->addWidget(error);

        inputs_.push_back(input);
    }
    Wt::WPushButton *acceptButton = new Wt::WPushButton(mediaTable_->elementAt(rowBodyTable, columnTable));

    acceptButton->setTextFormat(Wt::XHTMLUnsafeText);
    acceptButton->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    acceptButton->clicked().connect(boost::bind(&CreatePageWidget::checkAdd, this, errorMessage));
    acceptButton->hide();

    mediaTable_->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    inputs_.push_back(acceptButton);
}

void    CreatePageWidget::inputForModif(long long id, int rowTable, int columnTable)
{
    std::cout << "inputForModif" << std::endl;
    std::vector<Wt::WText*> errorMessage;
    int column(0);
    int cpt(0);
    vector_type resourceTable = getResourceRowTable(id);
    for (vector_type::iterator j = resourceTable.begin(); j != resourceTable.end(); j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt5WTextE");
        std::cout << "name : " << nameRessouce.compare(typeid(*widgetAdd).name()) << std::endl;
        if (nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
        {
            std::cout << "Ca rentre les input" << std::endl;
            nameRessouce = boost::lexical_cast<std::string>(((Wt::WText*)(widgetAdd))->text());
            std::string newName = nameRessouce;
            if (nameRessouce.size() > SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE)
            {
                newName.resize(SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE);
                newName.resize(newName.size() + 3, '.');
            }
            
            Wt::WLineEdit *input = new Wt::WLineEdit(newName);             
            input->setToolTip(nameRessouce);
            input->setValidator(editValidator(cpt));
            input->enterPressed().connect(boost::bind(&CreatePageWidget::checkModif, this, id, errorMessage));
            input->setFocus();
            input->setWidth(Wt::WLength(35, Wt::WLength::Unit::Percentage));
            inputs_.push_back(input);
            
            Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-info"));
            error->hide();
            errorMessage.push_back(error);

            mediaTable_->elementAt(rowTable, column)->clear();
            mediaTable_->elementAt(rowTable, column)->addWidget(input);            
            mediaTable_->elementAt(rowTable, column++)->addWidget(error);
            
        }
        cpt++;
    }
    mediaTable_->elementAt(rowTable, columnTable)->clear();
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    Wt::WPushButton *valideBut = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    valideBut->setTextFormat(Wt::XHTMLUnsafeText);
    valideBut->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    valideBut->clicked().connect(boost::bind(&CreatePageWidget::checkModif, this, id, errorMessage));
}

// Aff Input ----------------------------------------------------------------------

void    CreatePageWidget::showInputForAdd()
{
    mediaTable_->rowAt(mediaTable_->rowCount() - 1)->show();
    for (std::vector<Wt::WInteractWidget*>::const_iterator j = inputs_.begin(); 
            j != inputs_.end(); 
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt9WLineEditE");
        if (inputs_.begin() == j && nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
            ((Wt::WLineEdit*)(*j))->setFocus();
        ((Wt::WInteractWidget*)(*j))->show();
    }
}

// Check input ----------------------------------------------

void    CreatePageWidget::checkAdd(std::vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs_, errorMessage) == 0)
    {
        this->created_ = false;
        inputs_.pop_back();
        addResource(inputs_);
        inputs_.clear();
    }
//    else
//        this->created_ = false;
}

void    CreatePageWidget::checkModif(long long id, std::vector<Wt::WText*> errorMessage)
{
    std::cout << "Checkmodif" << std::endl;
    if (checkInput(inputs_, errorMessage) == 0)
    {
    std::cout << "Checkmodif OK" << std::endl;
        modifResource(inputs_, id);
        inputs_.clear();
    }    
    else
    {
        std::cout << "Checkmodif BAD" << std::endl;
        this->created_ = false;
    }
}

int    CreatePageWidget::checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage)
{
    int check(0);

    std::vector<Wt::WText*>::const_iterator i = errorMessage.begin(); 
    for (std::vector<Wt::WInteractWidget*>::const_iterator j = inputName.begin(); 
            j != inputName.end(); 
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt9WLineEditE");
        if (nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
        {
            if (((Wt::WLineEdit*)(*j))->validate() == Wt::WValidator::Invalid
                    || ((Wt::WLineEdit*)(*j))->text().empty())
            {
                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-error");
                ((Wt::WLineEdit*)(*j))->setWidth(Wt::WLength(20, Wt::WLength::Unit::Percentage));
                ((Wt::WLineEdit*)(*j))->setFocus();
                ((Wt::WText*)(*i))->show();
                check = 1;
            }
            else
            {
                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-success");
                ((Wt::WText*)(*i))->hide();
            }
        }
        i++;
    }
    return check;
}

// Function useful ----------------------------------------------

Wt::WComboBox   *CreatePageWidget::getComboBox()
{
    Wt::WComboBox *comboBox = new Wt::WComboBox();
    comboBox->addItem("5");
    comboBox->addItem("10");
    comboBox->addItem("20");
    comboBox->addItem("40");
    comboBox->addItem("all");
    comboBox->setStyleClass("dataTables_length");
    comboBox->setWidth(Wt::WLength(61));
    comboBox->setMargin(50, Wt::Right);
    comboBox->changed().connect(std::bind([=] () {
        if (comboBox->currentText().toUTF8().compare("all") == 0)
            this->nbAff_ = 0;
        else
            this->nbAff_ = atoi(comboBox->currentText().toUTF8().c_str());
        update();
    }));
    return comboBox;
}

void    CreatePageWidget::resourceBeAff()
{
    int nb(0);
    int nbMax = mediaTable_->rowCount();
    if (this->nbAff_ == 0)
        this->nbAff_ = nbMax;
    while ((nb + 1) < nbMax)
    {
        mediaTable_->rowAt(nb + 1)->hide();
        nb++;
    }
    for (nb = 0; nb < this->nbAff_ && (nb + 1) < nbMax; nb++)
    {
        mediaTable_->rowAt(nb + 1)->show();
    }
    
}

void     CreatePageWidget::searchName(Wt::WLineEdit *arg)
{
    std::cout << "Your search : " << arg->text() << std::endl;
}

// Set attribut for option. -------------------------------------

void    CreatePageWidget::setButtonModif(bool check)
{
    this->butModif_ = check;
}

void    CreatePageWidget::setButtonSup(bool check)
{
    this->butSup_ = check;
}

void    CreatePageWidget::setTypeButtonAdd(int check)
{
    this->butAdd_ = check;
}

void    CreatePageWidget::setResourceNumberAdd(int nbResource)
{
    this->nbResource_ = nbResource;
}

void    CreatePageWidget::setLocalTable(bool background)
{
    this->dial_ = background;
}

// POPUP : ----------------------------------------------------

void    CreatePageWidget::popupWindow()
{
    Wt::WLineEdit                       *input;
    std::vector<Wt::WInteractWidget*> inputName;
    std::vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogAdd_ = new Wt::WDialog(tr("Alert." + this->nameResourcePage + ".add-" + this->nameResourcePage));
//    dialogAdd_->setId("DialogPupop");
    std::vector<std::string> titleHeader = getHeaderTableTitle();
    std::vector<std::string>::iterator i = titleHeader.begin();
    for (int j(0); j < this->nbResource_; j++)
    {
        if (i == titleHeader.end())
        {
            std::cout << "Error : Too many title for header table" << std::endl;
            return;
        }
        new Wt::WText(*i + " : <br />", dialogAdd_->contents());
        
        // recois dialog ,inputName et errorMessage
        
        input = new Wt::WLineEdit(dialogAdd_->contents());
        input->setValidator(editValidator(j));
        input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
        input->setWidth(Wt::WLength(20, Wt::WLength::Unit::Percentage));
        if (inputName.size() == 0)
            input->setFocus();
        inputName.push_back(input);
        
        Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-info"), dialogAdd_->contents());
        error->hide();
        errorMessage.push_back(error);
        
        new Wt::WText("<br />", dialogAdd_->contents());

        //
        
        i++;
    }

    popupComplete(dialogAdd_);
    
    dialogAdd_->finished().connect(std::bind([=] () {
        popupCheck(inputName, errorMessage, dialogAdd_, -1);
        return;
    }));
    dialogAdd_->show();
    this->created_ = false;
}

void    CreatePageWidget::popupForModif(long long id) 
{
    std::vector<Wt::WInteractWidget*> inputName;
    std::vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif_ = new Wt::WDialog(tr("Alert." + this->nameResourcePage + ".add-" + this->nameResourcePage));

    vector_type resourceTable = getResourceRowTable(id);
    std::vector<std::string> titleHeader = getHeaderTableTitle(); 
    std::vector<std::string>::iterator i = titleHeader.begin();
    
    int cpt(0);
    for (vector_type::iterator j = resourceTable.begin(); j != resourceTable.end(); j++)
    {
        if (i == titleHeader.end())
        {
            std::cout << "Error : Too many title for header table (popupforModif" << std::endl;
        }
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt5WTextE");
        if (nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
        {
            new Wt::WText(*i + " : <br />", dialogModif_->contents());
            nameRessouce = boost::lexical_cast<std::string>(((Wt::WText*)(widgetAdd))->text());
            std::string newName = nameRessouce;
            if (nameRessouce.size() > SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE)
            {
                newName.resize(SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE);
                newName.resize(newName.size() + 3, '.');
            }
            
            // Recois : dialog, inputName, errorMessage et newName
            
            Wt::WLineEdit *input = new Wt::WLineEdit(newName, dialogModif_->contents());
            input->setValidator(editValidator(cpt));
            input->enterPressed().connect(dialogModif_, &Wt::WDialog::accept);
            input->setWidth(Wt::WLength(20, Wt::WLength::Unit::Percentage));

            if (inputName.size() == 0)
                input->setFocus();
            input->setToolTip(nameRessouce);

            inputName.push_back(input);
            
            Wt::WText *error2 = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-info"), dialogModif_->contents());
            error2->hide();
            errorMessage.push_back(error2);
            new Wt::WText("<br />", dialogModif_->contents());

            //
        }
        i++;
        cpt++;
    }
    
    popupComplete(dialogModif_);

    dialogModif_->finished().connect(std::bind([=] () {
        popupCheck(inputName, errorMessage, dialogModif_, id);
        return;
    }));
    dialogModif_->show();
    this->created_ = false;
}

void    CreatePageWidget::popupCheck(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id)
{
    int check(0);
    if (dialog->result() == Wt::WDialog::Rejected)
        return;
    else
        check = checkInput(inputName, errorMessage);
    if (check == 0)
        id >= 0 ? modifResource(inputName, id) : addResource(inputName);
    else if (check == 1)
        dialog->show();
    return;
}

void    CreatePageWidget::popupComplete(Wt::WDialog *dialog)
{
    dialog->setResizable(true);
    dialog->setClosable(true);
    dialog->setWidth(Wt::WLength(90, Wt::WLength::Unit::Percentage));
    dialog->setHeight(Wt::WLength(70, Wt::WLength::Unit::Percentage));

    Wt::WPushButton *ok = new Wt::WPushButton("Ok", dialog->footer());
    ok->clicked().connect(dialog, &Wt::WDialog::accept);    
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul = new Wt::WPushButton("Annuler", dialog->footer()); //xml
    annul->clicked().connect(dialog, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");

    new Wt::WText("\n", dialog->contents());

    Wt::WTabWidget *tabW = new Wt::WTabWidget();
    popupAddTables(tabW);
    if (tabW->count() > 0)
    {
        dialog->contents()->addWidget(tabW);
        tabW->setHeight(250);
    }    
}

