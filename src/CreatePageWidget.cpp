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
    this->nbAffBegin_ = 1;
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
    paginatePage();
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
    if (this->dial_) //gkr: popup is created  in popupWindow.
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::popupWindow, this));
    else if (!this->dial_) // gkr: Input is created in createBodyTable, showInputForAdd() is there just for show this.
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
    if (resources_.size() > 0)
        resources_.clear();
    fillInTable();
    
    return resourceTable;
}

Wt::WContainerWidget    *CreatePageWidget::createFooterTable()
{
    if (butPaginate_.size() > 0)
        butPaginate_.clear();
    Wt::WContainerWidget *footerTable = new Wt::WContainerWidget();
    footerTable->addStyleClass("fg-toolbar ui-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix");
    // revoir ui-toolbar quand css finit
    new Wt::WText("search : ", footerTable); //xml

    Wt::WLineEdit *search = new Wt::WLineEdit(footerTable);
    search->resize(Wt::WLength(100), Wt::WLength(15));
    search->enterPressed().connect(boost::bind(&CreatePageWidget::searchName, this, search));

    Wt::WNavigationBar *navBar = new Wt::WNavigationBar(footerTable);
//    test->setWidth(Wt::WLength(170));
    navBar->addStyleClass("dataTables_paginate fg-buttonset ui-buttonset fg-buttonset-multi ui-buttonset-multi paging_full_numbers ");
    initPaginatePage(navBar);
    
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
        resources_.push_back(std::pair<int, Wt::WObject*>(0, mediaTable_->rowAt(rowBodyTable)));
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
    std::vector<Wt::WText*> errorMessage;
    int columnTable(0);
    if (inputs_.size() > 0)
        inputs_.clear();
    for (int i = 0; i < this->nbResource_; ++i)
    {
        Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-info"));
        error->hide();
        errorMessage.push_back(error);

        Wt::WLineEdit *input = new Wt::WLineEdit("");
//        input->enterPressed().connect(boost::bind(&CreatePageWidget::checkAdd, this, errorMessage));
        input->setWidth(Wt::WLength(150));
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
    std::vector<Wt::WText*> errorMessage;
    vector_type inputs;
    int column(0);
    int cpt(0);
    vector_type resourceTable = getResourceRowTable(id);
    for (vector_type::iterator j = resourceTable.begin(); j != resourceTable.end(); j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt5WTextE");
        if (nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
        {
            nameRessouce = boost::lexical_cast<std::string>(((Wt::WText*)(widgetAdd))->text());
            std::string newName = nameRessouce;
            if (nameRessouce.size() > SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE)
            {
                newName.resize(SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE);
                newName.resize(newName.size() + 3, '.');
            }
            
            Wt::WLineEdit *input = new Wt::WLineEdit(newName);             
            inputs.push_back(input);
            input->setToolTip(nameRessouce);
            input->setValidator(editValidator(cpt));
            if (inputs.size() == 0)
                input->setFocus();
            input->setWidth(Wt::WLength(150));
            
            Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-info"));
            error->hide();
            errorMessage.push_back(error);

            mediaTable_->elementAt(rowTable, column)->clear();
            mediaTable_->elementAt(rowTable, column)->addWidget(input);            
            mediaTable_->elementAt(rowTable, column++)->addWidget(error);
            
//            input->enterPressed().connect(boost::bind(&CreatePageWidget::checkModif, this, inputs, id, errorMessage));
        }
        cpt++;
    }
    mediaTable_->elementAt(rowTable, columnTable)->clear();
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    Wt::WPushButton *valideBut = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    valideBut->setTextFormat(Wt::XHTMLUnsafeText);
    valideBut->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    valideBut->clicked().connect(boost::bind(&CreatePageWidget::checkModif, this, inputs, id, errorMessage));
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
}

void    CreatePageWidget::checkModif(vector_type inputs, long long id, std::vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs, errorMessage) == 0)
    {
        this->created_ = false;
        modifResource(inputs, id);
        inputs.clear();
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
                ((Wt::WLineEdit*)(*j))->setWidth(Wt::WLength(150));
                ((Wt::WLineEdit*)(*j))->setFocus();
                ((Wt::WText*)(*i))->show();
                check = 1;
            }
            else
            {
                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-success");
                if (((Wt::WText*)(*i))->isHidden() == false)
                    ((Wt::WText*)(*i))->hide();
            }
        }
        i++;
    }
    return check;
}

// Function useful ----------------------------------------------

void    CreatePageWidget::initPaginatePage(Wt::WNavigationBar *navBar)
{
//    int nbRow = resources_.size();
    int nbRow = sizeAff();
    if (nbAff_ == 0)
        nbAff_ = nbRow;
    if (nbRow > this->nbAff_)
    {
        Wt::WPushButton *butPaginate = new Wt::WPushButton("prev"); // XML
        butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, -1));
        butPaginate->addStyleClass("fg-button ui-button ui-state-default");
        navBar->addWidget(butPaginate);

        for (int rst(1); rst <= ((nbRow / this->nbAff_) + ((nbRow % this->nbAff_) > 0 ? 1 : 0)); rst++)
        {
            butPaginate = new Wt::WPushButton(boost::lexical_cast<std::string>(rst));
            butPaginate->addStyleClass("fg-button ui-button ui-state-default");
            butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, rst));
            navBar->addWidget(butPaginate);
            butPaginate_.push_back(butPaginate);
        }
        butPaginate = new Wt::WPushButton("next");  //XML
        butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, -2));
        butPaginate->addStyleClass("fg-button ui-button ui-state-default");
        navBar->addWidget(butPaginate);
    }
}

void    CreatePageWidget::paginatePage()
{
    int rst(0);
    vector_type::iterator i;
    for (i = butPaginate_.begin(); i != butPaginate_.end(); i++)
        ((Wt::WPushButton*)(*i))->hide();
    int nbRow = sizeAff();
    if (this->nbAff_ == 0)
        nbAff_ = nbRow;
    if (nbRow > this->nbAff_)
    {
        rst = (nbRow / this->nbAff_) + ((nbRow % this->nbAff_) > 0 ? 1 : 0);
    }
    for (i = butPaginate_.begin(); rst > 0 && i != butPaginate_.end(); i++, rst--)
    {
        ((Wt::WPushButton*)(*i))->show();
    }
}

void    CreatePageWidget::switchPage(int rst)
{
    if (rst == -1)
        this->nbAffBegin_ = (this->nbAffBegin_ - this->nbAff_) >= 1 ? (this->nbAffBegin_ - this->nbAff_) : 1;
    if (rst == -2)
        this->nbAffBegin_ = (this->nbAffBegin_ + this->nbAff_) <= sizeAff() ? (this->nbAffBegin_ + this->nbAff_) : this->nbAffBegin_;
    if (rst > 0)
    {
        this->nbAffBegin_ = (((this->nbAff_ * rst) - this->nbAff_) + 1);
    }
    update();
}

Wt::WComboBox   *CreatePageWidget::getComboBox()
{
    this->nbAff_ = 5;
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
        this->nbAffBegin_ = 1;
        update();
    }));
    return comboBox;
}

void    CreatePageWidget::resourceBeAff()
{
    int nb(0);
    int nbRow = sizeAff();

    if (this->nbAffBegin_ > nbRow)
        this->nbAffBegin_ -= this->nbAff_;
    if (this->nbAff_ == 0)
    {
        this->nbAff_ = nbRow;
        this->nbAffBegin_ = 1;
    }
    nb = 1;
    for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
    {
        Wt::WTableRow *tableRow = (Wt::WTableRow *)it->second;
        if ((int)it->first == 0)
        {
            if (nb >= this->nbAffBegin_ && nb <= (this->nbAff_ + this->nbAffBegin_ - 1))
            {
                tableRow->show();
            }
            else
            {
                tableRow->hide();
            }
            nb++;
        }
        else
            tableRow->hide();
    }
}

void     CreatePageWidget::searchName(Wt::WLineEdit *arg)
{
    int cpt(0);
    bool check;
    if (arg->text().empty())
        for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
            it->first = 0;
    else
        for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
        {
            check = false;
            Wt::WTableRow *tableRow = (Wt::WTableRow *)it->second;
            for (int j(0); j < this->nbResource_; j++)
            {
                Wt::WText *text = (Wt::WText*)tableRow->elementAt(j)->widget(0);
                std::string compareType("PN2Wt5WTextE");
                if (compareType.compare(typeid(text).name()) == 0)
                {
                    std::string argSearch = boost::lexical_cast<std::string>(arg->text());
                    std::transform(argSearch.begin(), argSearch.end(), argSearch.begin(), ::tolower);
                    std::string argInTable = boost::lexical_cast<std::string>(text->text());
                    std::transform(argInTable.begin(), argInTable.end(), argInTable.begin(), ::tolower);
                    if (boost::contains(argInTable, argSearch) == true)
                        check = true;
                }
            }
            if (check == true)
                it->first = 0;
            else
                it->first = 1;
            cpt++;
        }
    update();
}

int     CreatePageWidget::sizeAff()
{
    int cpt(0);
    
    for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
    {
        if (it->first == 0)
            cpt++;
    }
    return cpt;
}

Wt::WDialog    *CreatePageWidget::deleteResource(long long id)
{
    Wt::WDialog *box = new Wt::WDialog("Attention");

    box->contents()->addWidget(new Wt::WText(tr("Alert." + this->nameResourcePage + ".delete-message"))); // a voir xml
    
    buttonInDialogFooter(box);
    this->created_ = false;
    return box;
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
        input->setWidth(Wt::WLength(150));
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
            input->setWidth(Wt::WLength(150));

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
    {
        closePopup();
        return;
    }
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

    buttonInDialogFooter(dialog);

    new Wt::WText("\n", dialog->contents());

    Wt::WTabWidget *tabW = new Wt::WTabWidget();
    popupAddTables(tabW);
    if (tabW->count() > 0)
    {
        dialog->contents()->addWidget(tabW);
        dialog->resize(Wt::WLength(90, Wt::WLength::Unit::Percentage),
                Wt::WLength(70, Wt::WLength::Unit::Percentage));
        tabW->setHeight(250);
    }
}

void    CreatePageWidget::buttonInDialogFooter(Wt::WDialog *dialog)
{
        Wt::WPushButton *ok = new Wt::WPushButton("Ok", dialog->footer());
    ok->clicked().connect(dialog, &Wt::WDialog::accept);    
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul = new Wt::WPushButton("Annuler", dialog->footer()); //xml
    annul->clicked().connect(dialog, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");
}
