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

CreatePageWidget::CreatePageWidget(Echoes::Dbo::Session *session, std::string apiUrl, std::string namePage)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + namePage + ".Management.template"))
{
    this->addStyleClass("template");
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(namePage, false);

    this->nameResourcePage = namePage;
    this->butSup_ = true;
    this->butModif_ = true;
    this->mainPage_ = true; // Dialog/True
    this->created_ = false;
    this->nbAff_ = 5;
    this->nbAffBegin_ = 1;
    this->nbResource_ = 1;
    this->update_ = true;
    this->nameResourcePageSpec_ = "";
    this->session_ = session;
    setApiUrl(apiUrl);
}

CreatePageWidget::~CreatePageWidget() { }

// INIT Elements. ------------------------------------------------------

void
CreatePageWidget::update()
{
    if (this->update_ == true)
    {
        std::vector<std::string> titleName = getTitlesTableText();
        setResourceNumberAdd(titleName.size() > 0 ? titleName.size() : 1);
        if (this->nameResourcePageSpec_.empty() == true)
            this->nameResourcePageSpec_ = this->nameResourcePage;
        if (!this->created_ && checkResource())
        {
            this->clear();
            this->bindWidget("resource-header", createHeaderTable());
            this->bindWidget("resource-table", createBodyTable());
            this->bindWidget("resource-footer", createFooterTable());
            this->created_ = true;
        }
        if (!checkResource())
        {
            this->clear();
            Wt::WTable *table = new Wt::WTable();
            new Wt::WText(tr("Alert." + this->nameResourcePage + ".nothing-" + this->nameResourcePageSpec_), table->elementAt(0, 0));
            new Wt::WText(tr("Alert." + this->nameResourcePage + ".nothing-" + this->nameResourcePageSpec_ + "-text"), table->elementAt(1, 0));
            Wt::WPushButton *headerButton =
                    new Wt::WPushButton(tr("Alert." + this->nameResourcePage + ".nothing-button")
                    , table->elementAt(2, 0));

            this->bindWidget("resource-header", new Wt::WText());
            this->bindWidget("resource-table", table);
            this->bindWidget("resource-footer", new Wt::WText());

            table->addStyleClass("table table-striped table-hover data-table dataTable test");

            headerButton->clicked().connect(boost::bind(&CreatePageWidget::popupWindow, this));
            headerButton->setStyleClass("buttons btn");

            this->created_ = true;
        }
        else
        {
            resourceBeAff();
            paginatePage();
        }
    }
}

bool
CreatePageWidget::checkResource()
{
    if (getIdsTable().size() > 0)
        return true;
    else
        return false;
}

// gkr: Init header.

Wt::WContainerWidget *
CreatePageWidget::createHeaderTable()
{
    Wt::WContainerWidget *headerTable = new Wt::WContainerWidget();
    headerTable->addStyleClass("widget-title header-pers");
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
            + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePageSpec_)
            + "</h5>", headerTable);

    if (this->mainPage_) // gkr: whether class is not a dialog(popup).
        headerTable->addWidget(getComboBox());

    Wt::WAnchor *headerButton = new Wt::WAnchor(headerTable);
    if (this->mainPage_) //gkr: popup is created  in popupWindow.
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::popupWindow, this));
    else if (!this->mainPage_) // gkr: Input is created in createBodyTable, showInputForAdd() is there just for show this.
        headerButton->clicked().connect(boost::bind(&CreatePageWidget::showInputForAdd, this));

    //    headerButton->setTextFormat(Wt::XHTMLUnsafeText);
    headerButton->setStyleClass("button-add btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    return headerTable;
}

Wt::WContainerWidget *
CreatePageWidget::createBodyTable()
{
    Wt::WContainerWidget *resourceTable = new Wt::WContainerWidget();
    // gkr: Init body of table
    resourceTable->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");

    mediaTable_ = new Wt::WTable(resourceTable);
    mediaTable_->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    mediaTable_->setHeaderCount(1, Wt::Horizontal);
    addResourceInHeaderTable();
    if (resources_.size() > 0)
        resources_.clear();
    fillInTable();

    return resourceTable;
}

Wt::WContainerWidget *
CreatePageWidget::createFooterTable()
{
    if (butPaginate_.size() > 0)
        butPaginate_.clear();
    if (butPaginateExt_.size() > 0)
        butPaginateExt_.clear();
    Wt::WContainerWidget *footerTable = new Wt::WContainerWidget();
    footerTable->addStyleClass("fg-toolbar ui-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix footer-pers");
    // revoir ui-toolbar quand css finit  
    new Wt::WText(tr("Alert." + this->nameResourcePage + ".search-bar")
            , footerTable);

    Wt::WLineEdit *search = new Wt::WLineEdit(footerTable);
    search->resize(Wt::WLength(100), Wt::WLength(15));
    search->enterPressed().connect(boost::bind(&CreatePageWidget::searchName, this, search));

    Wt::WNavigationBar *navBar = new Wt::WNavigationBar(footerTable);
    navBar->addStyleClass("paginate-edit");
    initPaginatePage(navBar);

    return footerTable;
}

// Add Resouce For Elements -----------------------------------------------------------------

void
CreatePageWidget::addResourceInHeaderTable()
{
    int columnTable(0);
    std::vector<std::string> titleName = getTitlesTableText();
    std::vector<std::string> titleWidget = getTitlesTableWidget();
    std::vector<std::string>::iterator it;
    mediaTable_->elementAt(0, 0)->setAttributeValue("style", "border-left:0;");
    for (it = titleName.begin(); it != titleName.end(); it++)
    {
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + boost::lexical_cast<std::string>(*it)),
                mediaTable_->elementAt(0, columnTable));
        mediaTable_->elementAt(0, columnTable++)->addStyleClass(boost::lexical_cast<std::string>(*it), true);
    }
    for (it = titleWidget.begin(); it != titleWidget.end(); it++)
    {
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + boost::lexical_cast<std::string>(*it)),
                mediaTable_->elementAt(0, columnTable));
        mediaTable_->elementAt(0, columnTable++)->addStyleClass(boost::lexical_cast<std::string>(*it), true);
    }
    if (this->butModif_) // gkr: whether button Modif is true.
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".modif-button"),
            mediaTable_->elementAt(0, columnTable++));
    if (this->butSup_) // gkr: whether button Sup(Delete) is true.
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".delete-button"),
            mediaTable_->elementAt(0, columnTable));
}

void
CreatePageWidget::fillInTable()
{
    std::vector<long long> ids = getIdsTable();
    int columnTable(0);
    int rowBodyTable(1);

    for (std::vector<long long>::iterator j = ids.begin(); j != ids.end(); j++)
    {
        columnTable = 0;
        mediaTable_->elementAt(rowBodyTable, columnTable)->setAttributeValue("style", "border-left:0;");
        vector_type resourceRow = getResourceRowTable(*j);
        for (vector_type::iterator k = resourceRow.begin(); k != resourceRow.end(); k++)
        {
            Wt::WInteractWidget *widgetAdd = *k;
            std::string nameRessouce("N2Wt5WTextE");
            if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
            {
                nameRessouce = ((Wt::WText*)(widgetAdd))->text().toUTF8();
                std::string newName = nameRessouce;
                if (newName.find('|') != std::string::npos)
                {
                    std::vector<std::string> dataline;
                    boost::split(dataline, newName, boost::is_any_of("|"));
                    newName.clear();
                    nameRessouce.clear();
                    for (std::vector<std::string>::iterator it = dataline.begin();
                            it != dataline.end(); it++)
                    {
                        std::string resizeString = (*it);
                        if (((std::string)(*it)).find('<') == std::string::npos)
                        {
                            nameRessouce += (*it) + "\n";
                            if (resizeString.size() > (unsigned int) (SIZE_NAME_RESOURCE / this->nbResource_))
                            {
                                resizeString.resize(SIZE_NAME_RESOURCE / this->nbResource_);
                                resizeString.resize(resizeString.size() + 3, '.');
                            }
                        }
                        newName += resizeString;
                        resizeString.clear();
                    }
                }
                else
                {
                    if (nameRessouce.size() > (unsigned int) (SIZE_NAME_RESOURCE / this->nbResource_))
                    {
                        newName.resize(SIZE_NAME_RESOURCE / this->nbResource_);
                        newName.resize(newName.size() + 3, '.');
                    }
                }
                Wt::WText *newColumn = new Wt::WText(newName, mediaTable_->elementAt(rowBodyTable, columnTable));
                newColumn->setTextFormat(Wt::TextFormat::XHTMLUnsafeText);
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
    if (!this->mainPage_)
        addInputForAffix(rowBodyTable);
}

void
CreatePageWidget::addButtons(long long id, int rowTable, int columnTable)
{
    if (this->butModif_)
    {
        Wt::WPushButton *modifButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
        modifButton->setAttributeValue("class", "btn btn-inverse");
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
        if (this->mainPage_)
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
        delButton->setAttributeValue("class", "btn btn-danger");
        delButton->setTextFormat(Wt::XHTMLUnsafeText);
        delButton->setText("<span class='input-group-btn'><i class='icon-remove icon-white'></i></span>");
        delButton->clicked().connect(boost::bind(&CreatePageWidget::deleteResource, this, id));
        mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
        mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    }
}

void
CreatePageWidget::addInputForAffix(int rowBodyTable)
{
    std::vector<Wt::WText*> errorMessage;
    int columnTable(0);
    if (inputs_.size() > 0)
        inputs_.clear();
    std::vector<std::string> titleHeader = getTitlesTableText();
    std::vector<std::string>::iterator it = titleHeader.begin();

    for (int i = 0; i < this->nbResource_; ++i)
    {
        if (it == titleHeader.end())
        {
            std::cout << "Error : Too many title for header table" << std::endl;
            return;
        }
        Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                + boost::lexical_cast<std::string>(*it)));
        error->hide();
        errorMessage.push_back(error);

        Wt::WLineEdit *input = new Wt::WLineEdit("");
        input->setWidth(Wt::WLength(150));
        input->setValidator(editValidator(i));
        input->hide();

        mediaTable_->elementAt(rowBodyTable, columnTable)->addWidget(input);
        mediaTable_->elementAt(rowBodyTable, columnTable++)->addWidget(error);

        inputs_.push_back(input);
        it++;
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

void
CreatePageWidget::inputForModif(long long id, int rowTable, int columnTable)
{
    std::vector<Wt::WText*> errorMessage;
    vector_type inputs;
    int column(0);
    int cpt(0);
    std::vector<std::string> titleHeader = getTitlesTableText();
    std::vector<std::string>::iterator it = titleHeader.begin();

    vector_type resourceTable = getResourceRowTable(id);
    vector_type::iterator j;
    for (j = resourceTable.begin(); j != resourceTable.end() && it != titleHeader.end(); j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt5WTextE");
        if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
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

            Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                    + boost::lexical_cast<std::string>(*it)));
            error->hide();
            errorMessage.push_back(error);

            mediaTable_->elementAt(rowTable, column)->clear();
            mediaTable_->elementAt(rowTable, column)->addWidget(input);
            mediaTable_->elementAt(rowTable, column++)->addWidget(error);

            it++;
            cpt++;
        }
    }
    if (it == titleHeader.end() && j != resourceTable.end())
        std::cout << "Warning : Too many title for header table" << std::endl;
    mediaTable_->elementAt(rowTable, columnTable)->clear();
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    Wt::WPushButton *valideBut = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    valideBut->setTextFormat(Wt::XHTMLUnsafeText);
    valideBut->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    valideBut->clicked().connect(boost::bind(&CreatePageWidget::checkModif, this, inputs, id, errorMessage));
}

// Aff Input ----------------------------------------------------------------------

void
CreatePageWidget::showInputForAdd()
{
    mediaTable_->rowAt(mediaTable_->rowCount() - 1)->show();
    for (std::vector<Wt::WInteractWidget*>::const_iterator j = inputs_.begin();
            j != inputs_.end();
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt9WLineEditE");
        if (inputs_.begin() == j && nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
            ((Wt::WLineEdit*)(*j))->setFocus();
        ((Wt::WInteractWidget*)(*j))->show();
    }
}

// Check input ----------------------------------------------

void
CreatePageWidget::checkAdd(std::vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs_, errorMessage) == 0)
    {
        this->created_ = false;
        inputs_.pop_back();
        addResource(inputs_);
        inputs_.clear();
    }
}

void
CreatePageWidget::checkModif(vector_type inputs, long long id, std::vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs, errorMessage) == 0)
    {
        this->created_ = false;
        modifResource(inputs, id);
        inputs.clear();
    }
}

int
CreatePageWidget::checkName(std::string inputText, std::vector<long long> ids)
{
    for (std::vector<long long>::iterator j = ids.begin(); j != ids.end(); j++)
    {
        vector_type resourceRow = getResourceRowTable(*j);
        for (vector_type::iterator k = resourceRow.begin(); k != resourceRow.end(); k++)
        {
            Wt::WInteractWidget *widgetAdd = *k;
            std::string nameRessouce("N2Wt5WTextE");
            if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
            {
                if (inputText.compare(((Wt::WText*)(*k))->text().toUTF8()) == 0)
                    return 1;
            }
        }
    }
    return 0;
}

int
CreatePageWidget::checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage)
{
    std::vector<long long> ids = getIdsTable();
    int check(0);

    std::vector<Wt::WText*>::const_iterator i = errorMessage.begin();
    for (std::vector<Wt::WInteractWidget*>::const_iterator j = inputName.begin();
            j != inputName.end();
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt9WLineEditE");
        if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
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
                //            else if (checkName(((Wt::WLineEdit*)(*j))->text().toUTF8(), ids) == 1)
                //            {
                //                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-error");
                //                ((Wt::WLineEdit*)(*j))->setWidth(Wt::WLength(150));
                //                ((Wt::WLineEdit*)(*j))->setFocus();
                //                ((Wt::WText*)(*i))->setText(tr("Alert."
                //                        + this->nameResourcePage + ".invalid-name-twice"));
                //                ((Wt::WText*)(*i))->show();
                //                check = 1;
                //                
                //            }
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

void
CreatePageWidget::builtPaginate(Wt::WNavigationBar *navBar)
{
    Wt::WPushButton *butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-first"));
    butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, -1));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);

    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-prev"));
    butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, -2));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);


    for (int cpt(0); cpt < ((sizeAff() / this->nbAff_) + ((sizeAff() % this->nbAff_) > 0 ? 1 : 0)); cpt++)
    {
        butPaginate = new Wt::WPushButton(boost::lexical_cast<std::string>(cpt + 1));
        butPaginate->setStyleClass("fg-button ui-button ui-state-default btn");
        navBar->addWidget(butPaginate);
        butPaginate_.push_back(butPaginate);
    }


    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-next"));
    butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, -3));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);

    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-last"));
    butPaginate->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, -4));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);
}

void
CreatePageWidget::initPaginatePage(Wt::WNavigationBar *navBar)
{
    int nbRow = sizeAff();
    if (nbAff_ == 0)
        nbAff_ = nbRow;
    if (nbRow > this->nbAff_)
    {
        builtPaginate(navBar);
    }
}

void
CreatePageWidget::paginatePage()
{
    int nbRow = sizeAff();
    int cpt(0);
    bool check = false;
    if (nbRow > this->nbAff_)
        check = true;

    for (vector_type::iterator it = butPaginateExt_.begin(); it != butPaginateExt_.end(); it++)
    {
        if (check)
        {
            if (cpt < 2 && this->nbAffBegin_ > 1)
                ((Wt::WPushButton*)(*it))->setDisabled(false);
            else if (cpt > 1 &&
                    (nbRow + 1) > (this->nbAffBegin_ + this->nbAff_))
                ((Wt::WPushButton*)(*it))->setDisabled(false);
            else
                ((Wt::WPushButton*)(*it))->setDisabled(true);
            ((Wt::WPushButton*)(*it))->show();
        }
        else
            ((Wt::WPushButton*)(*it))->hide();
        cpt++;
    }
    cpt = 0;
    int rst = (this->nbAffBegin_ / this->nbAff_);
    for (vector_type::iterator i = butPaginate_.begin(); i != butPaginate_.end(); i++)
    {
        if (check)
        {
            ((Wt::WPushButton*)(*i))->setText(boost::lexical_cast<std::string>(cpt + 1));
            ((Wt::WPushButton*)(*i))->setAttributeValue("style",
                    "background-image: linear-gradient(#ffffff, #ffffff 25%, #e6e6e6)");
            ((Wt::WPushButton*)(*i))->clicked().connect(boost::bind(&CreatePageWidget::switchPage, this, cpt));
            if (cpt == rst)
                ((Wt::WPushButton*)(*i))->setAttributeValue("style",
                    "background-image: linear-gradient(#ffffff, #ffffff 25%, #CFCFCF)");
            ((Wt::WPushButton*)(*i))->hide();
            if (((nbRow / this->nbAff_) + ((nbRow % this->nbAff_) > 0 ? 1 : 0)) <= 5)
            {
                if (cpt < ((nbRow / this->nbAff_) + ((nbRow % this->nbAff_) > 0 ? 1 : 0)))
                    ((Wt::WPushButton*)(*i))->show();
            }
            else
            {
                if (rst <= 2 && (cpt - 2) <= 2)
                {
                    if ((cpt - 2) == 2)
                        ((Wt::WPushButton*)(*i))->setText("...");
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if (((cpt + 3) == rst || (cpt + 4) == rst)
                        && (cpt + 4) >= (int) (butPaginate_.size() - 1))
                {
                    if ((cpt + 4) == (int) (butPaginate_.size() - 1))
                        ((Wt::WPushButton*)(*i))->setText("...");
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if ((cpt + 2) == rst)
                {
                    if ((cpt + 3) < (int) (butPaginate_.size() - 1) && cpt > 0)
                        ((Wt::WPushButton*)(*i))->setText("...");
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if ((cpt - 2) > 2 && (cpt - 2) == rst)
                {
                    if ((cpt + 1) < (int) (butPaginate_.size()))
                        ((Wt::WPushButton*)(*i))->setText("...");
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if (cpt + 1 != rst
                        && cpt - 1 != rst
                        && cpt != rst)
                {
                    /* Hide */
                }
                else
                    ((Wt::WPushButton*)(*i))->show();
            }
        }
        else
            ((Wt::WPushButton*)(*i))->hide();
        cpt++;
    }
}

void
CreatePageWidget::switchPage(int rst)
{
    int nbRow = sizeAff();
    if (rst == -4)
        this->nbAffBegin_ = (nbRow - (nbRow % this->nbAff_)) + 1;
    else if (rst == -3)
        this->nbAffBegin_ = (this->nbAffBegin_ + this->nbAff_) <= nbRow ? (this->nbAffBegin_ + this->nbAff_) : this->nbAffBegin_;
    else if (rst == -2)
        this->nbAffBegin_ = (this->nbAffBegin_ - this->nbAff_) >= 1 ? (this->nbAffBegin_ - this->nbAff_) : 1;
    else if (rst == -1)
        this->nbAffBegin_ = 1;
    else
        this->nbAffBegin_ = (this->nbAff_ * rst) + 1;
    update();
}

Wt::WComboBox *
CreatePageWidget::getComboBox()
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
    comboBox->changed().connect(std::bind([ = ] (){
        if (comboBox->currentText().toUTF8().compare("all") == 0)
                this->nbAff_ = 0;
        else
            this->nbAff_ = atoi(comboBox->currentText().toUTF8().c_str());
                this->nbAffBegin_ = 1;
                update();
        }));
    return comboBox;
}

void
CreatePageWidget::resourceBeAff()
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
        if ((int) it->first == 0)
        {
            if (nb >= this->nbAffBegin_ && nb <= (this->nbAff_ + this->nbAffBegin_ - 1))
                tableRow->show();
            else
                tableRow->hide();
            nb++;
        }
        else
            tableRow->hide();
    }
}

void
CreatePageWidget::searchName(Wt::WLineEdit *arg)
{
    int cpt(0);
    bool check;
    if (arg->text().empty())
        for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
        {
            this->nbAffBegin_ = 1;
            it->first = 0;
        }
    else
        for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
        {
            check = false;
            Wt::WTableRow *tableRow = (Wt::WTableRow *)it->second;
            for (int j(0); j < this->nbResource_; j++)
            {
                Wt::WText *text = (Wt::WText*)tableRow->elementAt(j)->widget(0);
                std::string compareType("PN2Wt5WTextE");
                if (compareType.compare(typeid (text).name()) == 0)
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

int
CreatePageWidget::sizeAff()
{
    int cpt(0);

    for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
    {
        if (it->first == 0)
            cpt++;
    }
    return cpt;
}

Wt::WDialog *
CreatePageWidget::deleteResource(long long id)
{
    Wt::WDialog *box = new Wt::WDialog(tr("Alert." + this->nameResourcePage
            + ".delete-" + this->nameResourcePage));

    box->contents()->addWidget(new Wt::WText(tr("Alert." + this->nameResourcePage
            + ".delete-" + this->nameResourcePageSpec_ + "-message")));

    Wt::WPushButton *ok = new Wt::WPushButton(tr("Alert."
            + this->nameResourcePage + ".button-confirm"),
            box->footer());
    ok->clicked().connect(box, &Wt::WDialog::accept);
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul = new Wt::WPushButton(tr("Alert."
            + this->nameResourcePage + ".button-cancel"),
            box->footer());
    annul->clicked().connect(box, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");

    this->created_ = false;
    return box;
}

// Set attribut for option. -------------------------------------

void
CreatePageWidget::setButtonModif(bool check)
{
    this->butModif_ = check;
}

void
CreatePageWidget::setButtonSup(bool check)
{
    this->butSup_ = check;
}

void
CreatePageWidget::setResourceNumberAdd(int nbResource)
{
    this->nbResource_ = nbResource;
}

void
CreatePageWidget::setLocalTable(bool background)
{
    this->mainPage_ = background;
}

void
CreatePageWidget::setUpdate(bool update)
{
    this->update_ = update;
}

void
CreatePageWidget::setNameSpecial(std::string nameResourcePageSpec)
{
    this->nameResourcePageSpec_ = nameResourcePageSpec;
}

// POPUP : ----------------------------------------------------

void
CreatePageWidget::popupWindow()
{
    Wt::WLineEdit *input;
    std::vector<Wt::WInteractWidget*> inputName;
    std::vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogAdd_ =
            new Wt::WDialog(tr("Alert." + this->nameResourcePage
            + ".add-" + this->nameResourcePageSpec_));
    std::vector<std::string> titleHeader = getTitlesTableText();
    std::vector<std::string>::iterator i = titleHeader.begin();
    for (int j(0); j < this->nbResource_; j++)
    {
        if (i == titleHeader.end())
        {
            std::cout << "Error : Too many title for header table" << std::endl;
            return;
        }
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + boost::lexical_cast<std::string>(*i))
                + " : <br />", dialogAdd_->contents());

        input = new Wt::WLineEdit(dialogAdd_->contents());
        input->setValidator(editValidator(j));
        input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
        input->setWidth(Wt::WLength(150));
        if (inputName.size() == 0)
            input->setFocus();
        inputName.push_back(input);

        Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                + boost::lexical_cast<std::string>(*i)),
                dialogAdd_->contents());
        error->hide();
        errorMessage.push_back(error);

        new Wt::WText("<br />", dialogAdd_->contents());

        //

        i++;
    }

    popupComplete(dialogAdd_, 0);

    dialogAdd_->finished().connect(std::bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogAdd_, -1);
        return;
    }));
    dialogAdd_->show();
    this->created_ = false;
}

void
CreatePageWidget::popupForModif(long long id)
{
    std::vector<Wt::WInteractWidget*> inputName;
    std::vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif_ = new Wt::WDialog(tr("Alert." + this->nameResourcePage + ".modif-" + this->nameResourcePageSpec_));

    vector_type resourceTable = getResourceRowTable(id);
    std::vector<std::string> titleHeader = getTitlesTableText();
    std::vector<std::string>::iterator i = titleHeader.begin();

    int cpt(0);
    vector_type::iterator j;
    for (j = resourceTable.begin(); j != resourceTable.end() && i != titleHeader.end(); j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        std::string nameRessouce("N2Wt5WTextE");
        if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
        {
            new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + boost::lexical_cast<std::string>(*i))
                    + " : <br />", dialogModif_->contents());
            nameRessouce = boost::lexical_cast<std::string>(((Wt::WText*)(widgetAdd))->text());
            std::string newName = nameRessouce;
            if (nameRessouce.size() > SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE)
            {
                newName.resize(SIZE_NAME_RESOURCE + SIZE_NAME_RESOURCE);
                newName.resize(newName.size() + 3, '.');
            }


            Wt::WLineEdit *input = new Wt::WLineEdit(newName, dialogModif_->contents());
            input->setValidator(editValidator(cpt));
            input->enterPressed().connect(dialogModif_, &Wt::WDialog::accept);
            input->setWidth(Wt::WLength(150));

            if (inputName.size() == 0)
                input->setFocus();
            input->setToolTip(nameRessouce);

            inputName.push_back(input);

            Wt::WText *error2 = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                    + boost::lexical_cast<std::string>(*i)),
                    dialogModif_->contents());
            error2->hide();
            errorMessage.push_back(error2);
            new Wt::WText("<br />", dialogModif_->contents());
            i++;
            cpt++;
        }
    }
    if (i == titleHeader.end() && j != resourceTable.end())
        std::cout << "Warning : Too many title for header table (popupforModif" << std::endl;

    popupComplete(dialogModif_, id);

    dialogModif_->finished().connect(std::bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogModif_, id);
        return;
    }));
    dialogModif_->show();
    this->created_ = false;
}

void
CreatePageWidget::popupCheck(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id)
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

void
CreatePageWidget::popupComplete(Wt::WDialog *dialog, long long id)
{
    dialog->setResizable(true);
    dialog->setClosable(true);

    new Wt::WText("\n", dialog->contents());
    popupAddWidget(dialog, id); //Methode overload // id == 0 Add || id > 0 Modif

    buttonInDialogFooter(dialog);
}

void
CreatePageWidget::buttonInDialogFooter(Wt::WDialog *dialog)
{
    Wt::WPushButton *ok = new Wt::WPushButton(tr("Alert."
            + this->nameResourcePage + ".button-save"),
            dialog->footer());
    ok->clicked().connect(dialog, &Wt::WDialog::accept);
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul = new Wt::WPushButton(tr("Alert."
            + this->nameResourcePage + ".button-cancel"),
            dialog->footer());
    annul->clicked().connect(dialog, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");
}

/**/

void
CreatePageWidget::handleJsonGet(vectors_Json jsonResources)
{
//    std::cout << "Handle Json Get " << std::endl;
//    int cpt(0);
//    for (vector_Json jsonResource : jsonResources)
//    {
//        std::cout << "before before" << std::endl;
//        if (jsonResource.size() > 0)
//        {
//            for (Wt::Json::Array& jsonArray : jsonResource)
//            {
//                std::cout << "before" << std::endl;
//                if (!jsonArray.empty())
//                {
//                        std::cout << "after" << std::endl;
//                    for (Wt::Json::Object jsonObject : jsonArray)
//                    {
//                        Wt::WString name = jsonObject.get("name");
//                        long long id = jsonObject.get("id");
//                        std::cout << "JSON : " << cpt
//                                << " name : " << name << " id : " << id << std::endl;
//                    }
//                }
//            }
//        }
//        cpt++;
//    }
//    std::cout << "Handle Json Get END" << std::endl;
}

void
CreatePageWidget::recursiveGetResources(lists_string listsUrl, vectors_Json jsonResource, Wt::Http::Client *client)
{
    std::string apiAddress = this->getApiUrl() + "/" + (*(*listsUrl.begin()).begin())
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "[GET] address to call : " << apiAddress;

    client->done().connect(boost::bind(&CreatePageWidget::handleHttpResponseGet, this, _1, _2, listsUrl, jsonResource, client));
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

int
CreatePageWidget::handleHttpResponseGet(boost::system::error_code err, const Wt::Http::Message& response,
        lists_string listsUrl, vectors_Json jsonResource, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        Wt::Json::Value result = Wt::Json::Value::Null;
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), result);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".parsing-error-title"), tr("Alert." + this->nameResourcePage + ".parsing-error"), Wt::Ok);
                return EXIT_FAILURE;
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".typexception-error-title"), tr("Alert." + this->nameResourcePage + ".typexception-error"), Wt::Ok);
                return EXIT_FAILURE;
            }
        }
        if (response.status() == 200 || response.status() == 404)
        {
            if (jsonResource.size() == 0)
            {
                jsonResource.push_back(vector_Json());
            }
            if ((*(jsonResource.begin())).size() == 0)
            {
                (*(jsonResource.begin())).push_back(result);
            }
            else
            {
                jsonResource.back().push_back(result);
            }

            (*listsUrl.begin()).pop_front();
            if ((*listsUrl.begin()).size() == 0 || response.status() == 404)
            {
                listsUrl.pop_front();
                jsonResource.push_back(vector_Json());
            }
            else if ((*(*listsUrl.begin()).begin()).find(":id") != std::string::npos && response.status() == 200)
            {
                vector_Json itJ = jsonResource.back();
                Wt::Json::Array& result1 = itJ.back();
                list_string::iterator listUrl = (*listsUrl.begin()).begin();
                std::string saveUrl = (*listUrl);
                Wt::Json::Array::iterator itA = result1.begin();
                while (itA != result1.end() && saveUrl.compare((*listUrl)) == 0)
                {
                    Wt::Json::Object jsonObject = (*itA);
                    long long idJ = jsonObject.get("id");
                    (*listUrl).replace((*listUrl).find(":id"), 3, boost::lexical_cast<std::string>(idJ));
                    if (++itA != result1.end())
                    {
                        (*listsUrl.begin()).push_back(saveUrl);
                        listUrl++;
                    }
                }
            }
            if (listsUrl.size() == 0)
            {
                handleJsonGet(jsonResource);
            }
            else
            {
                recursiveGetResources(listsUrl, jsonResource);
            }
            
            return EXIT_SUCCESS;
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"), tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
    }
    return EXIT_FAILURE;
}

void
CreatePageWidget::setApiUrl(std::string apiUrl)
{
    apiUrl_ = apiUrl;
}

std::string
CreatePageWidget::getApiUrl()
{
    return apiUrl_;
}


