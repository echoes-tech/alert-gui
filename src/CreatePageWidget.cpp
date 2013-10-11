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
    nameResourcePage = namePage;
    created_ = false;
    butAdd_ = 0;
    butModif_ = true;
    butSup_ = true;
    dial_ = true;
    nbResource_ = 0;
}

CreatePageWidget::~CreatePageWidget() { }

void    CreatePageWidget::update()
{
    if (!created_)
    {
        this->clear();
        bindWidget("resource-header", createHeaderTable());
        bindWidget("resource-table", createBodyTable());
        fillInTable();
        created_ = true;
    }
}

Wt::WContainerWidget    *CreatePageWidget::createHeaderTable()
{
    // gkr: Init header of table.
    Wt::WContainerWidget *headerTable = new Wt::WContainerWidget();
    headerTable->addStyleClass("widget-title");
    headerTable->setId("Header Table");
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
    + tr("Alert." + nameResourcePage + ".add-form")
    + "</h5>", headerTable);
    // gkr: For add Resource.
    Wt::WAnchor *headerButton = new Wt::WAnchor(headerTable);
    if (butAdd_ == 0)
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::popupWindow, this));
    else if (butAdd_ == 1)
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::inputForAdd, this));
    headerButton->setTextFormat(Wt::XHTMLUnsafeText);
    headerButton->setStyleClass("buttons btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus '></i></span>");

    return headerTable;
}


Wt::WContainerWidget    *CreatePageWidget::createBodyTable()
{
    Wt::WContainerWidget *resourceTable = new Wt::WContainerWidget();
    // gkr: Init body of table
    resourceTable->addStyleClass("widget-content nopadding");
    resourceTable->addStyleClass("DataTables_Table_0_wrapper");
    resourceTable->addStyleClass("dataTables_wrapper");

    mediaTable_ = new Wt::WTable(resourceTable);
    mediaTable_->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    mediaTable_->setHeaderCount(1,Wt::Horizontal);

    return resourceTable;
}

void    CreatePageWidget::fillInTable()
{
    addResourceInHeaderTable();

    std::vector<long long> ids = getIdsTable();    
    int columnTable(0);
    int rowBodyTable(1);

    for (std::vector<long long>::iterator j = ids.begin(); j != ids.end(); j++)
    {
        columnTable = 0;
        vector_type resourceRow =  getResourceRowTable(*j);
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
    if (butAdd_ == 1)
        addInputAdd(rowBodyTable);
}

void            CreatePageWidget::addInputAdd(int rowBodyTable)
{
    int columnTable(0);
    if (inputs_.size() > 0)
        inputs_.clear();
    for (int i = 0; i < nbResource_; ++i)
    {
        Wt::WLineEdit *input = new Wt::WLineEdit("");
        input->setValidator(editValidator(i));
        inputs_.push_back(input);
        mediaTable_->elementAt(rowBodyTable, columnTable++)->addWidget(input);
        mediaTable_->rowAt(rowBodyTable)->hide();
        input->hide();
        input->setWidth(Wt::WLength(35, Wt::WLength::Unit::Percentage));
    }

        Wt::WPushButton *modifButton = new Wt::WPushButton(mediaTable_->elementAt(rowBodyTable, columnTable));
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
        modifButton->clicked().connect(boost::bind(&CreatePageWidget::checkAdd, this));
        mediaTable_->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        mediaTable_->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);
        modifButton->hide();
        
        inputs_.push_back(modifButton);
}

void    CreatePageWidget::addResourceInHeaderTable()
{
    int columnTable(0);
    std::vector<std::string>    titleName = getHeaderTableTitle();
    for (std::vector<std::string>::iterator i = titleName.begin(); i != titleName.end(); i++)
    {
        new Wt::WText(tr("Alert." + nameResourcePage + "." + boost::lexical_cast<std::string>(*i)),
                mediaTable_->elementAt(0, columnTable++));        
    }
    if (butModif_)
        new Wt::WText(tr("Alert." + nameResourcePage + ".modif-button"),
                mediaTable_->elementAt(0, columnTable++));
    if (butSup_)
        new Wt::WText(tr("Alert." + nameResourcePage + ".delete-button"),
                mediaTable_->elementAt(0, columnTable)); 
}

void    CreatePageWidget::addButtons(long long id, int rowTable, int columnTable)
{
    if (butModif_)
    {
        Wt::WPushButton *modifButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
        modifButton->setAttributeValue("class","btn btn-inverse");
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
        if (dial_)
            modifButton->clicked().connect(boost::bind(&CreatePageWidget::popupForModif, this, id));
        else
            modifButton->clicked().connect(boost::bind(&CreatePageWidget::inputForModif, this, id, rowTable, columnTable));
        mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
        columnTable++;
    }
    if (butSup_)
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


void    CreatePageWidget::inputForAdd()
{
    std::cout << "COMBIEN DE COUT : " << mediaTable_->rowCount() << std::endl;
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
        ((Wt::WInteractWidget*)(*j))->enterPressed().connect(boost::bind(&CreatePageWidget::checkAdd, this));
    }
}

void    CreatePageWidget::checkAdd()
{
    if (checkInput())
    {
        inputs_.pop_back();
        addResource(inputs_);
        inputs_.clear();
    }
}

void    CreatePageWidget::checkModif(long long id)
{
    if (checkInput())
    {
        modifResource(inputs_, id);
        inputs_.clear();
    }    
}

bool    CreatePageWidget::checkInput()
{
    for (std::vector<Wt::WInteractWidget*>::const_iterator j = inputs_.begin(); 
            j != inputs_.end(); 
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt9WLineEditE");
        if (nameRessouce.compare(typeid(*widgetAdd).name()) == 0)
        {
            if (((Wt::WLineEdit*)(*j))->validate() == Wt::WValidator::Invalid
                    || ((Wt::WLineEdit*)(*j))->text().empty())
            {
                ((Wt::WLineEdit*)(*j))->setFocus();
                return false;
            }
        }
    }
    created_ = false;
    return true;
}

void    CreatePageWidget::inputForModif(long long id, int rowTable, int columnTable)
{
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
            mediaTable_->elementAt(rowTable, column)->clear();
            mediaTable_->elementAt(rowTable, column++)->addWidget(input);
            input->setToolTip(nameRessouce);
            input->setValidator(editValidator(cpt));
            input->enterPressed().connect(boost::bind(&CreatePageWidget::checkModif, this, id));
            input->setFocus();
            input->setWidth(Wt::WLength(35, Wt::WLength::Unit::Percentage));
            inputs_.push_back(input);
        }
        cpt++;
    }
    mediaTable_->elementAt(rowTable, columnTable)->clear();
    Wt::WPushButton *valideBut = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    valideBut->setTextFormat(Wt::XHTMLUnsafeText);
    valideBut->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    valideBut->clicked().connect(boost::bind(&CreatePageWidget::checkModif, this, id));
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
}

void    CreatePageWidget::setButtonModif(bool check)
{
    butModif_ = check;
}

void    CreatePageWidget::setButtonSup(bool check)
{
    butSup_ = check;
}

void    CreatePageWidget::setTypeButtonAdd(int check)
{
    butAdd_ = check;
}

void    CreatePageWidget::setResouceNumberAdd(int nbResource)
{
    nbResource_ = nbResource;
}

void    CreatePageWidget::setLocalTable(bool background)
{
    dial_ = background;
}

// POPUP : ----------------------------------------------------

void    CreatePageWidget::popupWindow()
{
    Wt::WLineEdit                       *input;
    std::vector<Wt::WInteractWidget*> inputName;

    //gkr: Init dialog popup
    Wt::WDialog *dialogAdd_ = new Wt::WDialog(tr("Alert." + nameResourcePage + ".add-" + nameResourcePage));
    dialogAdd_->setResizable(true);
    dialogAdd_->setClosable(true);
    dialogAdd_->resize(Wt::WLength(90, Wt::WLength::Unit::Percentage), Wt::WLength(65, Wt::WLength::Unit::Percentage));
    
    std::vector<std::string> titleHeader = getHeaderTableTitle();
    std::vector<std::string>::iterator i = titleHeader.begin();
    for (int j(0); j < nbResource_; j++)
    {
        if (i == titleHeader.end())
        {
            std::cout << "Error : Too many title for header table" << std::endl;
        }
        new Wt::WText(*i + " : <br />", dialogAdd_->contents());
        input = new Wt::WLineEdit(dialogAdd_->contents());
        new Wt::WText("<br />", dialogAdd_->contents());
        input->setValidator(editValidator(j));
        input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
        input->setFocus();
        input->setWidth(Wt::WLength(20, Wt::WLength::Unit::Percentage));
        inputName.push_back(input);
        i++;
    }

    Wt::WText    *error = popupComplete(dialogAdd_);
    
    dialogAdd_->finished().connect(std::bind([=] () {
        int check = popupCheck(inputName, dialogAdd_);
        if (check == 0)
            addResource(inputName);
        else if (check == 1)
        {
            error->show();
            dialogAdd_->show();
        }
        return;
    }));
    dialogAdd_->show();
    created_ = false;
}

void    CreatePageWidget::popupForModif(long long id) 
{

    std::vector<Wt::WInteractWidget*> inputName;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif_ = new Wt::WDialog(tr("Alert." + nameResourcePage + ".add-" + nameResourcePage));
    dialogModif_->setResizable(false);
    dialogModif_->setClosable(true);
    dialogModif_->resize(Wt::WLength(90, Wt::WLength::Unit::Percentage), Wt::WLength(50, Wt::WLength::Unit::Percentage));

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
            Wt::WLineEdit *input = new Wt::WLineEdit(newName, dialogModif_->contents());
            new Wt::WText("<br />", dialogModif_->contents());
            input->setToolTip(nameRessouce);
            input->setValidator(editValidator(cpt));
            input->enterPressed().connect(dialogModif_, &Wt::WDialog::accept);
            input->setFocus();
            input->setWidth(Wt::WLength(20, Wt::WLength::Unit::Percentage));
            inputName.push_back(input);
        }
        i++;
        cpt++;
    }
    
    
    Wt::WText   *error = popupComplete(dialogModif_);

    dialogModif_->finished().connect(std::bind([=] () {
        int check = popupCheck(inputName, dialogModif_);
        if (check == 0)
            modifResource(inputName, id);
        else if (check == 1)
        {
            error->show();
            dialogModif_->show();
        }
        return;
    }));
    dialogModif_->show();
    created_ = false;
}

int    CreatePageWidget::popupCheck(std::vector<Wt::WInteractWidget*> inputName, Wt::WDialog *dialog)
{
    int check = 2;
    if (dialog->result() == Wt::WDialog::Rejected)
    {
        return check;
    }
    else
    {
        check = 0;
        for (std::vector<Wt::WInteractWidget*>::const_iterator j = inputName.begin(); 
                j != inputName.end(); 
                j++)
        {
            if (((Wt::WLineEdit*)(*j))->validate() == Wt::WValidator::Invalid
                    || ((Wt::WLineEdit*)(*j))->text().empty())
                    // This is cast in LineEdit for use validate(), because in connect we can't write in variable 'j'.
                    // Il a était caster en LineEdit pour utiliser validate(), parce qu'a l'utilisation de connecte on ne peut pas modifier la variable 'j'.
            {
                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-error");
                ((Wt::WLineEdit*)(*j))->setWidth(Wt::WLength(20, Wt::WLength::Unit::Percentage));
                ((Wt::WLineEdit*)(*j))->setFocus();
check = 1;
            }
            else
                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-success");
        }
    }
    return check; 
}

Wt::WText    *CreatePageWidget::popupComplete(Wt::WDialog *dialog)
{
    Wt::WText *error =  new Wt::WText(tr("Alert." + nameResourcePage + ".invalid-name-info"), dialog->contents());
    error->hide();
    
    new Wt::WText("\n", dialog->contents()); 
    
    Wt::WTabWidget *tabW = new Wt::WTabWidget(dialog->contents());
    popupAddTables(tabW); 
        
    Wt::WPushButton *ok = new Wt::WPushButton("Ok", dialog->contents());
    ok->clicked().connect(dialog, &Wt::WDialog::accept);    
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul = new Wt::WPushButton("Annuler", dialog->contents()); //xml
    annul->clicked().connect(dialog, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");
    
    return error;
}