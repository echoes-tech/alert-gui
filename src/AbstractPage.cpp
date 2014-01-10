/* 
 * Gui AbstractPage.cpp
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 21/12/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * AbstractPage
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>

#include "AbstractPage.h"

using namespace std;

AbstractPage::AbstractPage(Echoes::Dbo::Session *session, string apiUrl, string namePage)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + namePage + ".Management.template"))
{
    this->addStyleClass("template");

    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(namePage, false);

    this->nameResourcePage = namePage; // XML name
    this->butDel_ = true; // Button delete
    this->butModif_ = true; // Button modifi
    this->mainPage_ = true; // Dialog/True
    this->created_ = false;
    this->update_ = true;
    this->nameResourcePageSpec_ = ""; // whether more one class use same xml file.
    this->session_ = session;
    this->nbAff_ = 5;
    this->nbAffBegin_ = 1;
    this->undidName_ = "name";
    setApiUrl(apiUrl);
}

AbstractPage::~AbstractPage() {}

void AbstractPage::update()
{
    if (this->nameResourcePageSpec_.empty() == true)
        this->nameResourcePageSpec_ = this->nameResourcePage;
    if (this->update_ == true)
    {
        if (!this->created_ && rowsTable_.size() > 0)
        {
            bindElements();
            resourceBeAff();
        }
        else if (!this->created_ && rowsTable_.size() == 0)
        {
            nothingElement();
        }
        else
        {
            resourceBeAff();
            paginatePage();            
        }
    }
}

void AbstractPage::bindElements()
{
    this->clear();
    this->bindWidget("resource-header", createHeaderTable());
    this->bindWidget("resource-table", createBodyTable());
    this->bindWidget("resource-footer", createFooterTable());
    this->created_ = true;
}

void AbstractPage::nothingElement()
{
    this->clear();
    Wt::WTable *table = new Wt::WTable();
    new Wt::WText(tr("Alert." + this->nameResourcePage + ".nothing-" + this->nameResourcePageSpec_),
      table->elementAt(0, 0));
    new Wt::WText(tr("Alert." + this->nameResourcePage + ".nothing-" + this->nameResourcePageSpec_ + "-text"),
      table->elementAt(1, 0));
    Wt::WPushButton *headerButton =
    new Wt::WPushButton(tr("Alert." + this->nameResourcePage + ".nothing-button")
                    , table->elementAt(2, 0));
    this->bindWidget("resource-header", new Wt::WText());
    this->bindWidget("resource-table", table);
    this->bindWidget("resource-footer", new Wt::WText());
    table->addStyleClass("table table-striped table-hover data-table dataTable test");

    headerButton->clicked().connect(boost::bind(&AbstractPage::popupWindow, this));
    headerButton->setStyleClass("buttons btn");

    this->created_ = true;
}
// -------- Creates Elements to table. ------

Wt::WContainerWidget *AbstractPage::createHeaderTable()
{
    Wt::WContainerWidget *headerTable = new Wt::WContainerWidget();
    headerTable->addStyleClass("widget-title header-pers");
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePageSpec_)
                  + "</h5>", headerTable);

    if (this->mainPage_) // gkr: whether class is not a dialog(popup).
        headerTable->addWidget(getSelectDrop());

    Wt::WAnchor *headerButton = new Wt::WAnchor(headerTable);
    if (this->mainPage_) //gkr: popup is created  in popupWindow.
        headerButton->clicked().connect(boost::bind(&AbstractPage::popupWindow, this));
    else if (!this->mainPage_) // gkr: Input is created in createBodyTable, showInputForAdd() is there just for show this.
        headerButton->clicked().connect(boost::bind(&AbstractPage::showInputForAdd, this));

    headerButton->setStyleClass("button-add btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    return headerTable;
}

Wt::WContainerWidget *AbstractPage::createBodyTable()
{
    resources_.clear();

    Wt::WContainerWidget *resourceTable = new Wt::WContainerWidget();
    // gkr: Init body of table
    resourceTable->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");

    mediaTable_ = new Wt::WTable(resourceTable);
    mediaTable_->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    mediaTable_->setHeaderCount(1, Wt::Horizontal);
    addResourceBodyHeader();
    if (this->rowsTable_.size() > 0)
        fillInBodyTable();
    else
        Wt::log("error") << "Too many argument in rowsTable_ plz setRowsTable";
    return resourceTable;
}

Wt::WContainerWidget *AbstractPage::createFooterTable()
{
    butPaginate_.clear();
    butPaginateExt_.clear();
        
    Wt::WContainerWidget *footerTable = new Wt::WContainerWidget();
    footerTable->addStyleClass("fg-toolbar ui-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix footer-pers");
    // revoir ui-toolbar quand css finit  
    new Wt::WText(tr("Alert." + this->nameResourcePage + ".search-bar")
                  , footerTable);

    Wt::WLineEdit *search = new Wt::WLineEdit(footerTable);
    search->resize(Wt::WLength(100), Wt::WLength(15));
    search->enterPressed().connect(boost::bind(&AbstractPage::searchName, this, search));

    Wt::WNavigationBar *navBar = new Wt::WNavigationBar(footerTable);
    navBar->addStyleClass("paginate-edit");
    initPaginatePage(navBar);

    return footerTable;
}

// Add Resource For Elements -----------------------------------------------------------------

void AbstractPage::addResourceBodyHeader()
{
    int columnTable(0);

    mediaTable_->elementAt(0, 0)->setAttributeValue("style", "border-left:0;");
    for (vector_pair_string::iterator it = titles_.begin(); it != titles_.end(); it++)
    {
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + (*it).second),
                      mediaTable_->elementAt(0, columnTable++));
    }
    if (this->butModif_) // gkr: whether button Modif is true.
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".modif-button"),
                      mediaTable_->elementAt(0, columnTable++));
    if (this->butDel_) // gkr: whether button Sup(Delete) is true.
        new Wt::WText(tr("Alert." + this->nameResourcePage + ".delete-button"),
                      mediaTable_->elementAt(0, columnTable));
}

void AbstractPage::fillInBodyTable()
{
    int columnTable(0);
    int rowBodyTable(1);
    
    for (multimap_long_widgets::iterator itRowTable = this->rowsTable_.begin();
            itRowTable != this->rowsTable_.end() ;itRowTable++)
    {
        columnTable = 0;
        mediaTable_->elementAt(rowBodyTable, columnTable)->setAttributeValue("style", "border-left:0;");
        if ((*itRowTable).first > 0)
        {
            for (vector_widget::iterator k = (*rowsTable_.find((*itRowTable).first)).second.begin();
                    k != (*rowsTable_.find((*itRowTable).first)).second.end(); k++)
            {
                Wt::WInteractWidget *widgetAdd = *k;
                string nameRessouce("N2Wt5WTextE");
                if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
                {
                    nameRessouce = ((Wt::WText*)(widgetAdd))->text().toUTF8();
                    string newName = nameRessouce;
                    if (newName.find('|') != string::npos)
                    {
                        vector<string> dataline;
                        boost::split(dataline, newName, boost::is_any_of("|"));
                        newName.clear();
                        nameRessouce.clear();
                        for (vector<string>::iterator it = dataline.begin();
                                it != dataline.end(); it++)
                        {
                            string resizeString = (*it);
                            if (((string)(*it)).find('<') == string::npos)
                            {
                                nameRessouce += (*it) + "\n";
                                if (resizeString.size() > (unsigned int) (SIZE_NAME / this->titles_.size()))
                                {
                                    resizeString.resize(SIZE_NAME / this->titles_.size());
                                    resizeString.resize(resizeString.size() + 3, '.');
                                }
                            }
                            newName += resizeString;
                            resizeString.clear();
                        }
                    }
                    else
                    {
                        if (nameRessouce.size() > (unsigned int) (SIZE_NAME / this->titles_.size()))
                        {
                            newName.resize(SIZE_NAME / this->titles_.size());
                            newName.resize(newName.size() + 3, '.');
                        }
                    }
                    Wt::WText *newColumn = new Wt::WText(Wt::WString::fromUTF8(newName),
                                                         mediaTable_->elementAt(rowBodyTable, columnTable));
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
        }
        resources_.push_back(pair<int, Wt::WTableRow*>(0, mediaTable_->rowAt(rowBodyTable)));
        addButtonsModifDel((*itRowTable).first, rowBodyTable, columnTable);
        rowBodyTable++;
    }
    if (!this->mainPage_)
        addInputForAffix(rowBodyTable);
}

void AbstractPage::addInputForAffix(int rowBodyTable)
{
    vector<Wt::WText*> errorMessage;
    int columnTable(0);
    inputs_.clear();

    int cpt(0);
    for (vector_pair_string::iterator it = titles_.begin();
            it != titles_.end(); it++)
    {
        if ((*it).first > 0)
        {
            Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                                                + (*it).second));
            error->hide();
            errorMessage.push_back(error);

            Wt::WLineEdit *input = new Wt::WLineEdit("");
            input->setWidth(Wt::WLength(150));
            input->setValidator(editValidator(cpt++));
            input->hide();

            mediaTable_->elementAt(rowBodyTable, columnTable)->addWidget(input);
            mediaTable_->elementAt(rowBodyTable, columnTable++)->addWidget(error);

            inputs_.push_back(input);
        }
    }
    Wt::WPushButton *acceptButton = new Wt::WPushButton(mediaTable_->elementAt(rowBodyTable, columnTable));

    acceptButton->setTextFormat(Wt::XHTMLUnsafeText);
    acceptButton->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    acceptButton->clicked().connect(boost::bind(&AbstractPage::checkAdd, this, errorMessage));
    acceptButton->hide();

    mediaTable_->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    inputs_.push_back(acceptButton);
}

// POPUP : ----------------------------------------------------

void AbstractPage::popupWindow()
{
    Wt::WLineEdit *input;
    vector<Wt::WInteractWidget*> inputName;
    vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogAdd_ =
            new Wt::WDialog(tr("Alert." + this->nameResourcePage
                               + ".add-" + this->nameResourcePageSpec_));
    
    int cpt(0);
    for (vector_pair_string::iterator title = titles_.begin();
            title != titles_.end(); title++)
    {
        if ((*title).first >= 0)
        {
            new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + (*title).second)
                          + " : <br />", dialogAdd_->contents());

            if ((*title).first == ETypeJson::text)
            {
                input = new Wt::WLineEdit(dialogAdd_->contents());
                input->setValidator(editValidator(cpt));
                input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
                input->setWidth(Wt::WLength(150));
                if (inputName.size() == 0)
                    input->setFocus();
                inputName.push_back(input);
            }
            else if ((*title).first == ETypeJson::boolean)
            {
                Wt::WCheckBox *checkBox = new Wt::WCheckBox(dialogAdd_->contents());
                inputName.push_back(checkBox);
            }
            else if ((*title).first == ETypeJson::integer)
            {
                input = new Wt::WLineEdit(dialogAdd_->contents());
                input->setValidator(editValidator(cpt));
                input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
                input->setWidth(Wt::WLength(150));
                if (inputName.size() == 0)
                    input->setFocus();
                inputName.push_back(input);
            }
            else if ((*title).first == ETypeJson::undid)
            {
                inputName.push_back(popupAdd(dialogAdd_));
            }

            Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                                                + (*title).second),
                                             dialogAdd_->contents());
            error->hide();
            errorMessage.push_back(error);

            new Wt::WText("<br />", dialogAdd_->contents());
        }
        cpt++;
    }

    popupComplete(dialogAdd_, 0);

    dialogAdd_->finished().connect(bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogAdd_, -1);
        return;
    }));
    dialogAdd_->show();
    this->created_ = false;
}

void AbstractPage::popupForModif(long long id)
{
    vector<Wt::WInteractWidget*> inputName;
    vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif_ = new Wt::WDialog(tr("Alert." + this->nameResourcePage + ".modif-" + this->nameResourcePageSpec_));

    for (multimap_long_widgets::iterator itTable = rowsTable_.begin();
            itTable != rowsTable_.end(); itTable++)
    {
        int cpt(0);
        if ((*itTable).first == id)
        {
            vector_pair_string::iterator title = titles_.begin();
            for (Wt::WInteractWidget *itElem : (*itTable).second)
            {
                if ((*title).first >= 0)
                {
                        new Wt::WText(tr("Alert." + this->nameResourcePage + ".name-" + (*title).second)
                                      + " : <br />", dialogModif_->contents());                    
                }
                if ((*title).first == 0 || (*title).first == 2)
                {
                    string nameRessouce("N2Wt5WTextE");
                    if (nameRessouce.compare(typeid (*itElem).name()) == 0)
                    {
                        nameRessouce = ((Wt::WText*)(itElem))->text().toUTF8();
                        string newName = nameRessouce;
                        if (nameRessouce.size() > SIZE_NAME + SIZE_NAME)
                        {
                            newName.resize(SIZE_NAME + SIZE_NAME);
                            newName.resize(newName.size() + 3, '.');
                        }

                        if ((*title).first == 0)
                        {
                            Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName), dialogModif_->contents());
                            input->setValidator(editValidator(cpt));
                            input->enterPressed().connect(dialogModif_, &Wt::WDialog::accept);
                            input->setWidth(Wt::WLength(150));
                            if (inputName.size() == 0)
                                input->setFocus();
                            input->setToolTip(Wt::WString::fromUTF8(nameRessouce));
                            inputName.push_back(input);
                        }
                        else if ((*title).first == 2)
                        {
                            Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName), dialogModif_->contents());
                            input->setValidator(new Wt::WRegExpValidator(Wt::WString::fromUTF8("^[0123456789]+")));
                            input->enterPressed().connect(dialogModif_, &Wt::WDialog::accept);
                            input->setWidth(Wt::WLength(150));
                            if (inputName.size() == 0)
                                input->setFocus();
                            input->setToolTip(Wt::WString::fromUTF8(nameRessouce));
                            inputName.push_back(input);
                        }


                        Wt::WText *error2 = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                                                             + (*title).second), dialogModif_->contents());
                        error2->hide();
                        errorMessage.push_back(error2);
                        new Wt::WText("<br />", dialogModif_->contents());
                    }
                }
                else if ((*title).first == 1)
                {
                    multimap_long_widgets::iterator rowTable = rowsTable_.find(id);
                    for (vector_widget::iterator widg = (*rowTable).second.begin();
                            widg != (*rowTable).second.end(); widg++)
                    {
                        Wt::WInteractWidget *widgetAdd = *widg;
                        string nameRessouce("N2Wt9WCheckBoxE");
                        if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
                        {
                            Wt::WCheckBox *checkBox = new Wt::WCheckBox(dialogModif_->contents());
                            checkBox->setChecked(((Wt::WCheckBox*)(*widg))->isChecked());
                            inputName.push_back(checkBox);
                        }
                    }
                }
                else if ((*title).first == 3)
                {
                    Wt::WComboBox *comboBox = popupAdd(dialogModif_);
                    multimap_long_widgets::iterator rowTable = rowsTable_.find(id);
                    int cpt2(0);
                    for (vector_widget::iterator widg = (*rowTable).second.begin();
                            widg != (*rowTable).second.end(); widg++)
                    {
                        Wt::WStandardItemModel *unitModel = (Wt::WStandardItemModel*)comboBox->model();
                        if (cpt2 == cpt)
                        {
                            for (int nb(0); nb < unitModel->rowCount(); nb++)
                            {
                                if (((Wt::WText*)(*widg))->text().toUTF8().compare(unitModel->item(nb, 0)->text().toUTF8()) == 0)
                                {
                                    comboBox->setCurrentIndex(nb);
                                }
                            }
                            inputName.push_back(comboBox);
                        }
                        cpt2++;
                    }                    
                }
                title++;
                cpt++;
            }
        }
    }

    popupComplete(dialogModif_, id);

    dialogModif_->finished().connect(bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogModif_, id);
        return;
    }));
    dialogModif_->show();
    this->created_ = false;
}

void AbstractPage::popupCheck(vector<Wt::WInteractWidget*> inputName, vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id)
{
    int check(0);
    if (dialog->result() == Wt::WDialog::Rejected)
    {
        recursiveGetResources();
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

void AbstractPage::popupComplete(Wt::WDialog *dialog, long long id)
{
    dialog->setResizable(true);
    dialog->setClosable(true);

    new Wt::WText("\n", dialog->contents());
    popupAddWidget(dialog, id); //Methode overload // id == 0 Add || id > 0 Modif

    addButtonsInFooterDialog(dialog);
}

// Methodes useful ----------------------------------------

void AbstractPage::addButtonsModifDel(long long id, int rowTable, int columnTable)
{
    if (this->butModif_)
    {
        Wt::WPushButton *modifButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
        modifButton->setAttributeValue("class", "btn btn-inverse");
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
        if (this->mainPage_)
            modifButton->clicked().connect(boost::bind(&AbstractPage::popupForModif, this, id));
        else
            modifButton->clicked().connect(boost::bind(&AbstractPage::inputForModif, this, id, rowTable, columnTable));
        mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
        columnTable++;
    }
    if (this->butDel_)
    {
        Wt::WPushButton *delButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
        delButton->setAttributeValue("class", "btn btn-danger");
        delButton->setTextFormat(Wt::XHTMLUnsafeText);
        delButton->setText("<span class='input-group-btn'><i class='icon-remove icon-white'></i></span>");
        delButton->clicked().connect(boost::bind(&AbstractPage::deleteResource, this, id));
        mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
        mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    }
}


void AbstractPage::addButtonsInFooterDialog(Wt::WDialog *dialog)
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

// Set/Get attribut to init or option. -------------------------------------

void AbstractPage::setRowsTable(multimap_long_widgets rowsTable)
{
    this->rowsTable_ = rowsTable;
}

multimap_long_widgets AbstractPage::getRowsTable()
{
    return this->rowsTable_;
}

void AbstractPage::setUndidName(string undidName)
{
    this->undidName_ = undidName;
}

void AbstractPage::setTitles(vector_pair_string titles)
{
    this->titles_ = titles;
}

void AbstractPage::setUrl(lists_string listsUrl)
{
    this->listsUrl_ = listsUrl;
}

void AbstractPage::setButtonModif(bool check)
{
    this->butModif_ = check;
}

void AbstractPage::setButtonSup(bool check)
{
    this->butDel_ = check;
}

void AbstractPage::setLocalTable(bool background)
{
    this->mainPage_ = background;
}

void AbstractPage::setUpdate(bool update)
{
    this->update_ = update;
}

void AbstractPage::setNameSpecial(string nameResourcePageSpec)
{
    this->nameResourcePageSpec_ = nameResourcePageSpec;
}

void AbstractPage::setApiUrl(string apiUrl)
{
    apiUrl_ = apiUrl;
}

string AbstractPage::getApiUrl()
{
    return apiUrl_;
}

// -------- CALL RETURN API, HANDLE -----------


void AbstractPage::handleJsonGet(vectors_Json jsonResources)
{
    rowsTable_.clear();

    try
    {
        vector_Json jsonResource = jsonResources.at(0);
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = jsonResource.at(0);
            for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
            {
                Wt::Json::Object jsonObject = jsonArray.at(cpt);

                vector<Wt::WInteractWidget *> nameW;
                for (vector_pair_string::iterator itTitles = titles_.begin();
                        itTitles != titles_.end(); itTitles++)
                {
                    switch ((*itTitles).first)
                    {
                    case ETypeJson::text:
                    {
                        Wt::WString string = jsonObject.get((*itTitles).second);
                        nameW.push_back(new Wt::WText(string));
                        break;
                    }
                    case ETypeJson::boolean:
                    {
                        bool boolean = jsonObject.get((*itTitles).second);
                        Wt::WCheckBox *checkBox = new Wt::WCheckBox();
                        checkBox->setChecked(boolean);
                        checkBox->setDisabled(true);
                        nameW.push_back(checkBox);
                        break;
                    }
                    case ETypeJson::integer:
                    {
                        int number = jsonObject.get((*itTitles).second);
                        nameW.push_back(new Wt::WText(boost::lexical_cast<string>(number)));
                        break;
                    }
                    case ETypeJson::undid:
                    {
                        Wt::Json::Object jsonObjectParam = jsonResource.at(cpt + 1);
                        Wt::Json::Object nameObjet = jsonObjectParam.get((*itTitles).second);
                        Wt::WString name = nameObjet.get(undidName_);
                        nameW.push_back(new Wt::WText(name));
                        break;
                    }
                    }
                }
                long long id = jsonObject.get("id");
                rowsTable_.insert(make_pair(id, nameW));
            }
        }

    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[AbstractPage] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[AbstractPage] JSON Type Exception";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    update();
}

void AbstractPage::recursiveGetResources(vectors_Json jsonResource, lists_string listsUrl)
{
    if (listsUrl.size() == 0)
        listsUrl = listsUrl_;
    string apiAddress = this->getApiUrl() + "/" + (*(*listsUrl.begin()).begin())
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8()
            + addParameter();

    Wt::log("debug") << "[GET] address to call : " << apiAddress;
    Wt::Http::Client *client = new Wt::Http::Client();
    client->done().connect(boost::bind(&AbstractPage::handleHttpResponseGet, this, _1, _2, listsUrl, jsonResource, client));
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

int AbstractPage::handleHttpResponseGet(boost::system::error_code err, const Wt::Http::Message& response,
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
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".parsing-error-title"),
                                      tr("Alert." + this->nameResourcePage + ".parsing-error"), Wt::Ok);
                return EXIT_FAILURE;
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".typexception-error-title"),
                                      tr("Alert." + this->nameResourcePage + ".typexception-error"), Wt::Ok);
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
            else if ((*(*listsUrl.begin()).begin()).find(":id") != string::npos && response.status() == 200)
            {
                vector_Json itJ = jsonResource.back();
                Wt::Json::Array& result1 = itJ.back();
                list_string::iterator listUrl = (*listsUrl.begin()).begin();
                string saveUrl = (*listUrl);
                Wt::Json::Array::iterator itA = result1.begin();
                while (itA != result1.end() && saveUrl.compare((*listUrl)) == 0)
                {
                    Wt::Json::Object jsonObject = (*itA);
                    long long idJ = jsonObject.get("id");
                    (*listUrl).replace((*listUrl).find(":id"), 3,
                                       boost::lexical_cast<string>(idJ));
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
                recursiveGetResources(jsonResource, listsUrl);
            }

            return EXIT_SUCCESS;
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
    }
    return EXIT_FAILURE;
}


// ---- ADD MODIF DELETE ----------

void AbstractPage::addResource(vector<Wt::WInteractWidget*> argument)
{
    // Post resource -------
    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\t\"name\": \"" + ((Wt::WLineEdit*)(*argument.begin()))->text().toUTF8() + "\"\n}");

    string apiAddress = this->getApiUrl() + "/" + (*(*listsUrl_.begin()).begin())
    + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AbstractPage::returnApiPostResource, this, _1, _2, client));

    Wt::log("debug") << this->nameResourcePage + " : [POST] address to call : " << apiAddress;

    if (client->post(apiAddress, messageAsset))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "[" + this->nameResourcePage + "] Error Client Http";
}

void AbstractPage::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    string messageString;

    messageString = "{\n\"name\":\"" + ((Wt::WLineEdit*)(*arguments.begin()))->text().toUTF8() + "\"\n}";

    Wt::Http::Message message;
    message.addBodyText(messageString);

    string apiAddress = this->getApiUrl() + "/" + (*(*listsUrl_.begin()).begin()) + "/"
            + boost::lexical_cast<string> (id)
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AbstractPage::returnApiPutResource, this, _1, _2, client));
    Wt::log("debug") << this->nameResourcePage + " : [PUT] address to call : " << apiAddress;
    Wt::log("debug") << this->nameResourcePage + " : [PUT] message : " << messageString;
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "[" + this->nameResourcePage + "] Error Client Http";
}

Wt::WDialog *AbstractPage::deleteResource(long long id)
{
    Wt::WDialog *box =
            new Wt::WDialog(tr("Alert." + this->nameResourcePage
            + ".delete-" + this->nameResourcePage));

    box->contents()->
            addWidget(new Wt::WText(tr("Alert." + this->nameResourcePage
            + ".delete-" + this->nameResourcePageSpec_ + "-message")));

    Wt::WPushButton *ok =
            new Wt::WPushButton(tr("Alert."
            + this->nameResourcePage + ".button-confirm"),
                                box->footer());
    ok->clicked().connect(box, &Wt::WDialog::accept);
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul =
            new Wt::WPushButton(tr("Alert."
            + this->nameResourcePage + ".button-cancel"),
                                box->footer());
    annul->clicked().connect(box, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");

    this->created_ = false;

    box->finished().connect(bind([=] () {
    if (box->result() == Wt::WDialog::Accepted)
    {
        Wt::Http::Message message;
        message.addBodyText("");
        string apiAddress = this->getApiUrl() + "/" + (*(*listsUrl_.begin()).begin()) + "/"
                + boost::lexical_cast<string> (id)
                + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                + "&token=" + session_->user()->token.toUTF8();

        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&AbstractPage::returnApiDeleteResource, this, _1, _2, client));
        Wt::log("debug") << this->nameResourcePage + " : [DELETE] address to call : " << apiAddress;
        if (client->deleteRequest(apiAddress, message))
            Wt::WApplication::instance()->deferRendering();
        else
            Wt::log("error") << "Error Client Http";
    }
    return box;
    }));

    box->show();
    return box;
}

// RETURNS API --------------------------------------

void AbstractPage::returnApiPostResource(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == 201)
        {
            try
            {
                Wt::Json::Object result;
                Wt::Json::parse(response.body(), result);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".parsing-error-title"),
                                      tr("Alert." + this->nameResourcePage + ".parsing-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".typexception-error-title"),
                                      tr("Alert." + this->nameResourcePage + ".typexception-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
            Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
    }
    created_ = false;
    recursiveGetResources();
}

void AbstractPage::returnApiPutResource(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client) 
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
   
    if (!err)
    {
        if(response.status() >= 200 && response.status() < 300) // Revoir le retour
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".parsing-error-title"),
                                      tr("Alert." + this->nameResourcePage + ".parsing-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + this->nameResourcePage + ".add-form." + this->nameResourcePage)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".typexception-error-title"),
                                      tr("Alert." + this->nameResourcePage + ".typexception-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
            Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
    }
    created_ = false;
    recursiveGetResources();
}

void    AbstractPage::returnApiDeleteResource(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == Enums::EReturnCode::NO_CONTENT)
        {
            if (response.body() != "")
            {
                Wt::log("error") << "[" + this->nameResourcePage + " Widget] Response should be empty : "
                        << response.body() << ".";
            }
        }
        else
        {
            Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
        }
    }
    else
    {
            Wt::log("warning") << "[" + tr("Alert." + this->nameResourcePage + ".add-form."
                                       + this->nameResourcePage) + " Widget] Http::Client error: " << response.body();
            Wt::WMessageBox::show(tr("Alert." + this->nameResourcePage + ".database-error-title"),
                              tr("Alert." + this->nameResourcePage + ".database-error"), Wt::Ok);
    }
    recursiveGetResources();
}



// Check input ----------------------------------------------

void AbstractPage::checkAdd(vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs_, errorMessage) == 0)
    {
        this->created_ = false;
        inputs_.pop_back();
        addResource(inputs_);
        inputs_.clear();
    }
}

void AbstractPage::checkModif(vector_widget inputs, long long id, vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs, errorMessage) == 0)
    {
        this->created_ = false;
        modifResource(inputs, id);
        inputs.clear();
    }
}

int AbstractPage::checkName(string inputText, vector<long long> ids)
{
//    for (vector<long long>::iterator j = ids.begin(); j != ids.end(); j++)
//    {
////        vector_widget resourceRow = getResourceRowTable(*j);
//        vector_widget resourceRow =
//                rowsTable_.size() > 0 ? rowsTable_ : getResourceRowTable(*j);
//        for (vector_widget::iterator k = resourceRow.begin(); k != resourceRow.end(); k++)
//        {
//            Wt::WInteractWidget *widgetAdd = *k;
//            string nameRessouce("N2Wt5WTextE");
//            if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
//            {
//                if (inputText.compare(((Wt::WText*)(*k))->text().toUTF8()) == 0)
//                    return 1;
//            }
//        }
//    }
    return 0;
}

int AbstractPage::checkInput(vector<Wt::WInteractWidget*> inputName, vector<Wt::WText*> errorMessage)
{
    int check(0);

    vector<Wt::WText*>::const_iterator i = errorMessage.begin();
    for (vector<Wt::WInteractWidget*>::const_iterator j = inputName.begin();
            j != inputName.end();
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        string nameRessouce("N2Wt9WLineEditE");
        if (nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
        {
            if (((Wt::WLineEdit*)(*j))->validate() == Wt::WValidator::Invalid)
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

//  INPUT --------------------

void AbstractPage::showInputForAdd()
{
    mediaTable_->rowAt(mediaTable_->rowCount() - 1)->show();
    for (vector<Wt::WInteractWidget*>::const_iterator j = inputs_.begin();
            j != inputs_.end();
            j++)
    {
        Wt::WInteractWidget *widgetAdd = *j;
        string nameRessouce("N2Wt9WLineEditE");
        if (inputs_.begin() == j && nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
            ((Wt::WLineEdit*)(*j))->setFocus();
        ((Wt::WInteractWidget*)(*j))->show();
    }
}

void AbstractPage::inputForModif(long long id, int rowTable, int columnTable)
{
    vector<Wt::WText*> errorMessage;
    vector_widget inputs;
    int column(0);

    for (multimap_long_widgets::iterator itTable = rowsTable_.begin();
            itTable != rowsTable_.end(); itTable++)
    {
        int cpt(0);
        if ((*itTable).first == id)
        {
            vector_pair_string::iterator title = titles_.begin();
            for (Wt::WInteractWidget *itElem : (*itTable).second)
            {
                string nameRessouce("N2Wt5WTextE");
                if (nameRessouce.compare(typeid (*itElem).name()) == 0)
                {
                    nameRessouce = ((Wt::WText*)(itElem))->text().toUTF8();
                    string newName = nameRessouce;
                    if (nameRessouce.size() > SIZE_NAME + SIZE_NAME)
                    {
                        newName.resize(SIZE_NAME + SIZE_NAME);
                        newName.resize(newName.size() + 3, '.');
                    }

                    Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName));
                    inputs.push_back(input);
                    input->setToolTip(Wt::WString::fromUTF8(nameRessouce));
                    input->setValidator(editValidator(cpt));
                    if (inputs.size() == 0)
                        input->setFocus();
                    input->setWidth(Wt::WLength(150));

                    Wt::WText *error = new Wt::WText(tr("Alert." + this->nameResourcePage + ".invalid-name-"
                                                        + (*title).second));
                    error->hide();
                    errorMessage.push_back(error);

                    mediaTable_->elementAt(rowTable, column)->clear();
                    mediaTable_->elementAt(rowTable, column)->addWidget(input);
                    mediaTable_->elementAt(rowTable, column++)->addWidget(error);

                    cpt++;
                }
            }
            title++;
        }
    }

    mediaTable_->elementAt(rowTable, columnTable)->clear();
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    Wt::WPushButton *valideBut = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    valideBut->setTextFormat(Wt::XHTMLUnsafeText);
    valideBut->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    valideBut->clicked().connect(boost::bind(&AbstractPage::checkModif, this, inputs, id, errorMessage));
}



// Methodes disposable --------------------------------

Wt::WComboBox *AbstractPage::getSelectDrop()
{
    this->nbAff_ = 5;
    Wt::WComboBox *selectDrop = new Wt::WComboBox();
    selectDrop->addItem("5");
    selectDrop->addItem("10");
    selectDrop->addItem("20");
    selectDrop->addItem("40");
    selectDrop->addItem("all");
    selectDrop->setStyleClass("dataTables_length");
    selectDrop->setWidth(Wt::WLength(61));
    selectDrop->setMargin(50, Wt::Right);
    selectDrop->changed().connect(bind([ = ] (){
    if (selectDrop->currentText().toUTF8().compare("all") == 0)
        this->nbAff_ = 0;
    else
        this->nbAff_ = atoi(selectDrop->currentText().toUTF8().c_str());
    this->nbAffBegin_ = 1;
    update();
        }));
    return selectDrop;
}

void AbstractPage::builtPaginate(Wt::WNavigationBar *navBar)
{
    Wt::WPushButton *butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-first"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -1));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);

    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-prev"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -2));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);


    for (int cpt(0); cpt < ((sizeAff() / this->nbAff_) + ((sizeAff() % this->nbAff_) > 0 ? 1 : 0)); cpt++)
    {
        butPaginate = new Wt::WPushButton(boost::lexical_cast<string>(cpt + 1));
        butPaginate->setStyleClass("fg-button ui-button ui-state-default btn");
        navBar->addWidget(butPaginate);
        butPaginate_.push_back(butPaginate);
    }


    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-next"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -3));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);

    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + this->nameResourcePage + ".paginate-last"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -4));
    navBar->addWidget(butPaginate);
    butPaginateExt_.push_back(butPaginate);
}

void AbstractPage::switchPage(int rst)
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

void AbstractPage::initPaginatePage(Wt::WNavigationBar *navBar)
{
    int nbRow = sizeAff();
    if (nbAff_ == 0)
        nbAff_ = nbRow;
    if (nbRow > this->nbAff_)
    {
        builtPaginate(navBar);
    }
}

void AbstractPage::paginatePage()
{
    int nbRow = sizeAff();
    int cpt(0);
    bool check = false;
    if (nbRow > this->nbAff_)
        check = true;

    for (vector_widget::iterator it = butPaginateExt_.begin(); it != butPaginateExt_.end(); it++)
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
    for (vector_widget::iterator i = butPaginate_.begin(); i != butPaginate_.end(); i++)
    {
        if (check)
        {
            ((Wt::WPushButton*)(*i))->setText(boost::lexical_cast<string>(cpt + 1));
            ((Wt::WPushButton*)(*i))->setAttributeValue("style",
                                                        "background-image: linear-gradient(#ffffff, #ffffff 25%, #e6e6e6)");
            ((Wt::WPushButton*)(*i))->clicked().connect(boost::bind(&AbstractPage::switchPage, this, cpt));
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

void AbstractPage::resourceBeAff()
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

void AbstractPage::searchName(Wt::WLineEdit *arg)
{
    int cpt(0);
    bool check;
    if (arg->text().empty())
    {
        for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
        {
            this->nbAffBegin_ = 1;
            it->first = 0;
        }
    }
    else
    {
        for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
        {
            check = false;
            Wt::WTableRow *tableRow = (Wt::WTableRow *)it->second;
            for (int j(0); j < (int)this->rowsTable_.size(); j++)
            {
                Wt::WText *text = (Wt::WText*)tableRow->elementAt(j)->widget(0);
                string compareType("PN2Wt5WTextE");
                if (compareType.compare(typeid (text).name()) == 0)
                {
                    string argSearch = arg->text().toUTF8();
                    transform(argSearch.begin(), argSearch.end(), argSearch.begin(), ::tolower);
                    string argInTable = text->text().toUTF8();
                    transform(argInTable.begin(), argInTable.end(), argInTable.begin(), ::tolower);
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
    }
    update();
}

int AbstractPage::sizeAff()
{
    int cpt(0);
    for (vector_pair::iterator it = resources_.begin(); it != resources_.end(); it++)
    {
        if (it->first == 0)
            cpt++;
    }
    return cpt;
}


