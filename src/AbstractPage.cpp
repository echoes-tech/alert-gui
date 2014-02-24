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

AbstractPage::AbstractPage(Echoes::Dbo::Session *session, string apiUrl, string pageName)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + pageName + ".Management.template"))
{
    addStyleClass("template");

    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(pageName, false);

    m_xmlPageName = pageName; // XML name
    m_isDeleteButtonPresent = true; // Button delete
    m_isModifButtonPresent = true; // Button modifi
    m_isMainPage = true; // Dialog/True
    m_isCreated = false;
    m_toUpdate = true;
    m_nameResourcePageSpec = ""; // whether more one class use same xml file.
    m_session = session;
    m_nbAff = 5;
    m_nbAffBegin = 1;
    m_undidName = "name";
    setApiUrl(apiUrl);
}

AbstractPage::~AbstractPage() {}

void AbstractPage::updatePage(bool getResources)
{
    if (m_nameResourcePageSpec.empty())
    {
        m_nameResourcePageSpec = m_xmlPageName;
    }
    if (getResources)
    {
        recursiveGetResources();
    }
    if (m_toUpdate)
    {
        
        if (!m_isCreated && m_rowsTable.size() > 0)
        {
            createTable();
            selectLinesToBeDisplayed();
        }
        else if (!m_isCreated && m_rowsTable.size() == 0)
        {
            createEmptyResourceTable();
        }
        else
        {
            selectLinesToBeDisplayed();
            doThePaginationTrick();            
        }
    }
}

void AbstractPage::clearStructures()
{
    m_rowsTable.clear();
}

void AbstractPage::getResourceList()
{
    clearStructures();
    recursiveGetResources();
}

void AbstractPage::setResources(vector_pair resources)
{
    this->m_resources = resources;
}

vector_pair AbstractPage::getResources() const
{
    return m_resources;
}

void AbstractPage::setResourceTable(Wt::WTable* resourceTable)
{
    this->m_resourceTable = resourceTable;
}

Wt::WTable* AbstractPage::getResourceTable() const
{
    return m_resourceTable;
}

void AbstractPage::createTable()
{
    clear();
    bindWidget("resource-header", createTableFirstHeader());
    bindWidget("resource-table", createTableBody());
    bindWidget("resource-footer", createTableFooter());
    m_isCreated = true;
}

void AbstractPage::createEmptyResourceTable()
{
    clear();
    Wt::WTable *table = new Wt::WTable();
    new Wt::WText(tr("Alert." + m_xmlPageName + ".nothing-" + m_nameResourcePageSpec),
      table->elementAt(0, 0));
    new Wt::WText(tr("Alert." + m_xmlPageName + ".nothing-" + m_nameResourcePageSpec + "-text"),
      table->elementAt(1, 0));
    Wt::WPushButton *headerButton =
    new Wt::WPushButton(tr("Alert." + m_xmlPageName + ".nothing-button")
                    , table->elementAt(2, 0));
    bindWidget("resource-header", new Wt::WText());
    bindWidget("resource-table", table);
    bindWidget("resource-footer", new Wt::WText());
    table->addStyleClass("table table-striped table-hover data-table dataTable test");

    headerButton->clicked().connect(boost::bind(&AbstractPage::addResourcePopup, this));
    headerButton->setStyleClass("buttons btn");

    m_isCreated = true;
}
// -------- Creates Elements to table. ------

Wt::WContainerWidget *AbstractPage::createTableFirstHeader()
{
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget();
    headerTableContainer->addStyleClass("widget-title header-pers");
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + tr("Alert." + m_xmlPageName + ".add-form." + m_nameResourcePageSpec)
                  + "</h5>", headerTableContainer);

    if (m_isMainPage) // gkr: whether class is not a dialog(popup).
    {
        headerTableContainer->addWidget(getNumberOfLineDisplayedComboBox());
    }

    Wt::WAnchor *headerButton = new Wt::WAnchor(headerTableContainer);
    if (m_isMainPage) //gkr: popup is created  in popupWindow.
    {
        headerButton->clicked().connect(boost::bind(&AbstractPage::addResourcePopup, this));
    }
    else // gkr: Input is created in createBodyTable, showInputForAdd() is there just for show this.
    {
        headerButton->clicked().connect(boost::bind(&AbstractPage::showInputForAdd, this));
    }

    headerButton->setStyleClass("button-add btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    return headerTableContainer;
}

Wt::WContainerWidget *AbstractPage::createTableBody()
{
    m_resources.clear();

    Wt::WContainerWidget *resourceTableContainer = new Wt::WContainerWidget();
    // gkr: Init body of table
    resourceTableContainer->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");

    m_resourceTable = new Wt::WTable(resourceTableContainer);
    m_resourceTable->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    m_resourceTable->setHeaderCount(1, Wt::Horizontal);
    addTableSecondHeader();
    if (m_rowsTable.size() > 0)
    {
        fillBodyTable();
    }
    else
    {
        Wt::log("error") << "Too many argument in rowsTable_ plz setRowsTable";
    }
    return resourceTableContainer;
}

Wt::WContainerWidget *AbstractPage::createTableFooter()
{
    m_butPaginate.clear();
    m_butPaginateExt.clear();
        
    Wt::WContainerWidget *footerTable = new Wt::WContainerWidget();
    footerTable->addStyleClass("fg-toolbar ui-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix footer-pers");
    // revoir ui-toolbar quand css finit  
    new Wt::WText(tr("Alert." + m_xmlPageName + ".search-bar")
                  , footerTable);

    Wt::WLineEdit *search = new Wt::WLineEdit(footerTable);
    search->resize(Wt::WLength(100), Wt::WLength(15));
    search->enterPressed().connect(boost::bind(&AbstractPage::searchName, this, search));

    Wt::WNavigationBar *navBar = new Wt::WNavigationBar(footerTable);
    navBar->addStyleClass("paginate-edit");
    initPagination(navBar);

    return footerTable;
}

// Add Resource For Elements -----------------------------------------------------------------

void AbstractPage::addTableSecondHeader()
{
    int columnTable(0);

    m_resourceTable->elementAt(0, 0)->setAttributeValue("style", "border-left:0;");
    for (vector_pair_string::iterator it = m_titles.begin(); it != m_titles.end(); it++)
    {
        new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + (*it).second),
                      m_resourceTable->elementAt(0, columnTable++));
    }
    if (m_isModifButtonPresent) // gkr: whether button Modif is true.
    {
        new Wt::WText(tr("Alert." + m_xmlPageName + ".modif-button"),
                      m_resourceTable->elementAt(0, columnTable++));
    }
    if (m_isDeleteButtonPresent) // gkr: whether button Sup(Delete) is true.
    {
        new Wt::WText(tr("Alert." + m_xmlPageName + ".delete-button"),
                      m_resourceTable->elementAt(0, columnTable));
    }
}

void AbstractPage::fillBodyTable()
{
    int columnTable(0);
    int rowBodyTable(1);
    
    for (multimap_long_widgets::iterator itRowTable = m_rowsTable.begin();
            itRowTable != m_rowsTable.end() ;itRowTable++)
    {
        columnTable = 0;
        m_resourceTable->elementAt(rowBodyTable, columnTable)->setAttributeValue("style", "border-left:0;");
        if ((*itRowTable).first >= 0)
        {
            for (vector_widget::iterator k = (*m_rowsTable.find((*itRowTable).first)).second.begin();
                    k != (*m_rowsTable.find((*itRowTable).first)).second.end(); k++)
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
                                if (resizeString.size() > (unsigned int) (SIZE_NAME / m_titles.size()))
                                {
                                    resizeString.resize(SIZE_NAME / m_titles.size());
                                    resizeString.resize(resizeString.size() + 3, '.');
                                }
                            }
                            newName += resizeString;
                            resizeString.clear();
                        }
                    }
                    else
                    {
                        if (nameRessouce.size() > (unsigned int) (SIZE_NAME / m_titles.size()))
                        {
                            newName.resize(SIZE_NAME / m_titles.size());
                            newName.resize(newName.size() + 3, '.');
                        }
                    }
                    Wt::WText *newColumn = new Wt::WText(Wt::WString::fromUTF8(newName),
                                                         m_resourceTable->elementAt(rowBodyTable, columnTable));
                    newColumn->setTextFormat(Wt::TextFormat::XHTMLUnsafeText);
                    newColumn->setToolTip(nameRessouce);
                    columnTable++;
                }
                else
                {
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->addWidget(widgetAdd);
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
                    columnTable++;
                }
            }
            m_resources.push_back(pair<int, Wt::WTableRow*>(0, m_resourceTable->rowAt(rowBodyTable)));
            addGenericButtonsToResourceTable((*itRowTable).first, rowBodyTable, columnTable);
            rowBodyTable++;
        }
        
    }
    if (!m_isMainPage)
    {
        addInputForAffix(rowBodyTable);
    }
}

void AbstractPage::addInputForAffix(int rowBodyTable)
{
    //TSA à décommenter et renommer quand le cas d'utilisation se présentera
//    vector<Wt::WText*> errorMessage;
//    int columnTable(0);
//    inputs_.clear();
//
//    int cpt(0);
//    for (vector_pair_string::iterator it = titles_.begin();
//            it != titles_.end(); it++)
//    {
//        if ((*it).first > 0)
//        {
//            Wt::WText *error = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
//                                                + (*it).second));
//            error->hide();
//            errorMessage.push_back(error);
//
//            Wt::WLineEdit *input = new Wt::WLineEdit("");
//            input->setWidth(Wt::WLength(150));
//            input->setValidator(editValidator(cpt++));
//            input->hide();
//
//            m_resourceTable->elementAt(rowBodyTable, columnTable)->addWidget(input);
//            m_resourceTable->elementAt(rowBodyTable, columnTable++)->addWidget(error);
//
//            inputs_.push_back(input);
//        }
//    }
//    Wt::WPushButton *acceptButton = new Wt::WPushButton(m_resourceTable->elementAt(rowBodyTable, columnTable));
//
//    acceptButton->setTextFormat(Wt::XHTMLUnsafeText);
//    acceptButton->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
//    acceptButton->clicked().connect(boost::bind(&AbstractPage::checkAdd, this, errorMessage));
//    acceptButton->hide();
//
//    m_resourceTable->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
//    m_resourceTable->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);
//
//    inputs_.push_back(acceptButton);
}

// POPUP : ----------------------------------------------------

void AbstractPage::popupAddWidget(Wt::WDialog *dialog, long long id)
{
    
}

Wt::WComboBox *AbstractPage::popupAdd(Wt::WDialog *dialog) 
{   
    return new Wt::WComboBox(dialog->contents());
}

void AbstractPage::addResourcePopup()
{
    Wt::WLineEdit *input;
    vector<Wt::WInteractWidget*> inputName;
    vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogAdd_ =
            new Wt::WDialog(tr("Alert." + m_xmlPageName
                               + ".add-" + m_nameResourcePageSpec));
    
    int cpt(0);
    for (vector_pair_string::iterator title = m_titles.begin();
            title != m_titles.end(); title++)
    {
        if ((*title).first >= 0)
        {
            new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + (*title).second)
                          + " : <br />", dialogAdd_->contents());

            if ((*title).first == ETypeJson::text)
            {
                input = new Wt::WLineEdit(dialogAdd_->contents());
                //FIXME
//                input->setValidator(editValidator(cpt));
                input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
                input->setWidth(Wt::WLength(150));
                if (inputName.size() == 0)
                {
                    input->setFocus();
                }
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
                {
                    input->setFocus();
                }
                inputName.push_back(input);
            }
            else if ((*title).first == ETypeJson::undid)
            {
                inputName.push_back(popupAdd(dialogAdd_));
            }

            Wt::WText *error = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                + (*title).second),
                                             dialogAdd_->contents());
            error->hide();
            errorMessage.push_back(error);

            new Wt::WText("<br />", dialogAdd_->contents());
        }
        cpt++;
    }

    popupFinalization(dialogAdd_, 0);

    dialogAdd_->finished().connect(bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogAdd_, -1);
        return;
    }));
    dialogAdd_->show();
    m_isCreated = false;
}

void AbstractPage::modifResourcePopup(long long id)
{
    vector<Wt::WInteractWidget*> inputName;
    vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif_ = new Wt::WDialog(tr("Alert." + m_xmlPageName + ".modif-" + m_nameResourcePageSpec));

    for (multimap_long_widgets::iterator itTable = m_rowsTable.begin();
            itTable != m_rowsTable.end(); itTable++)
    {
        int cpt(0);
        if ((*itTable).first == id)
        {
            vector_pair_string::iterator title = m_titles.begin();
            for (Wt::WInteractWidget *itElem : (*itTable).second)
            {
                if ((*title).first >= 0)
                {
                        new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + (*title).second)
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
                            {
                                input->setFocus();
                            }
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
                            {
                                input->setFocus();
                            }
                            input->setToolTip(Wt::WString::fromUTF8(nameRessouce));
                            inputName.push_back(input);
                        }


                        Wt::WText *error2 = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                             + (*title).second), dialogModif_->contents());
                        error2->hide();
                        errorMessage.push_back(error2);
                        new Wt::WText("<br />", dialogModif_->contents());
                    }
                }
                else if ((*title).first == 1)
                {
                    multimap_long_widgets::iterator rowTable = m_rowsTable.find(id);
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
                    multimap_long_widgets::iterator rowTable = m_rowsTable.find(id);
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

    popupFinalization(dialogModif_, id);

    dialogModif_->finished().connect(bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogModif_, id);
        return;
    }));
    dialogModif_->show();
    m_isCreated = false;
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
    {
        check = checkInput(inputName, errorMessage);
    }
    
    if (check == 0)
    {
        id >= 0 ? modifResource(inputName, id) : addResource(inputName);
    }
    else if (check == 1)
    {
        dialog->show();
    }
    return;
}

void AbstractPage::popupFinalization(Wt::WDialog *dialog, long long id)
{
    dialog->setResizable(true);
    dialog->setClosable(true);

    new Wt::WText("\n", dialog->contents());
    popupAddWidget(dialog, id); //Methode overload // id == 0 Add || id > 0 Modif

    addButtonsToPopupFooter(dialog);
}

// Methodes useful ----------------------------------------

void AbstractPage::addGenericButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    columnTable = addCustomButtonsToResourceTable(id, rowTable, columnTable);
    if (m_isModifButtonPresent)
    {
        Wt::WPushButton *modifButton = new Wt::WPushButton(m_resourceTable->elementAt(rowTable, columnTable));
        modifButton->setAttributeValue("class", "btn btn-inverse");
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
        if (m_isMainPage)
        {
            modifButton->clicked().connect(boost::bind(&AbstractPage::modifResourcePopup, this, id));
        }
        else
        {
            modifButton->clicked().connect(boost::bind(&AbstractPage::inputForModif, this, id, rowTable, columnTable));
        }
        m_resourceTable->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        m_resourceTable->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
        columnTable++;
    }
    if (m_isDeleteButtonPresent)
    {
        Wt::WPushButton *delButton = new Wt::WPushButton(m_resourceTable->elementAt(rowTable, columnTable));
        delButton->setAttributeValue("class", "btn btn-danger");
        delButton->setTextFormat(Wt::XHTMLUnsafeText);
        delButton->setText("<span class='input-group-btn'><i class='icon-remove icon-white'></i></span>");
        delButton->clicked().connect(boost::bind(&AbstractPage::deleteResource, this, id));
        m_resourceTable->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
        m_resourceTable->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    }
}

int AbstractPage::addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    return columnTable;
}

void AbstractPage::addButtonsToPopupFooter(Wt::WDialog *dialog)
{
    Wt::WPushButton *ok = new Wt::WPushButton(tr("Alert."
                                                 + m_xmlPageName + ".button-save"),
                                              dialog->footer());
    ok->clicked().connect(dialog, &Wt::WDialog::accept);
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul = new Wt::WPushButton(tr("Alert."
                                                    + m_xmlPageName + ".button-cancel"),
                                                 dialog->footer());
    annul->clicked().connect(dialog, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");
}

// Set/Get attribut to init or option. -------------------------------------

void AbstractPage::setRowsTable(multimap_long_widgets rowsTable)
{
    m_rowsTable = rowsTable;
}

multimap_long_widgets AbstractPage::getRowsTable()
{
    return m_rowsTable;
}

void AbstractPage::setUndidName(string undidName)
{
    m_undidName = undidName;
}

void AbstractPage::setTitles(vector_pair_string titles)
{
    m_titles = titles;
}

void AbstractPage::setUrl(lists_string listsUrl)
{
    m_listsUrl = listsUrl;
}

void AbstractPage::setButtonModif(bool check)
{
    m_isModifButtonPresent = check;
}

void AbstractPage::setButtonSup(bool check)
{
    m_isDeleteButtonPresent = check;
}

void AbstractPage::setLocalTable(bool background)
{
    m_isMainPage = background;
}

void AbstractPage::setUpdate(bool update)
{
    m_toUpdate = update;
}

void AbstractPage::setNameSpecial(string nameResourcePageSpec)
{
    m_nameResourcePageSpec = nameResourcePageSpec;
}

void AbstractPage::setApiUrl(string apiUrl)
{
    m_apiUrl = apiUrl;
}

string AbstractPage::getApiUrl()
{
    return m_apiUrl;
}

// -------- CALL RETURN API, HANDLE -----------


void AbstractPage::handleJsonGet(vectors_Json jsonResources)
{
    m_rowsTable.clear();

    try
    {
        std::vector<Wt::Json::Value> jsonResource = jsonResources.at(0);
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = jsonResource.at(0);
            for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
            {
                Wt::Json::Object jsonObject = jsonArray.at(cpt);

                vector<Wt::WInteractWidget *> nameW;
                for (vector_pair_string::iterator itTitles = m_titles.begin();
                        itTitles != m_titles.end(); itTitles++)
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
                        Wt::WString name = nameObjet.get(m_undidName);
                        nameW.push_back(new Wt::WText(name));
                        break;
                    }
                    }
                    
                }
                long long id = jsonObject.get("id");
                m_rowsTable.insert(make_pair(id, nameW));
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
    updatePage(false);
}

void AbstractPage::recursiveGetResources(vectors_Json jsonResource, lists_string listsUrl)
{
    if (listsUrl.size() == 0)
    {
        listsUrl = m_listsUrl;
    }
    
    string credentialParametersFirstChar = "?";
    if ((*(*listsUrl.begin()).begin()).find("?") != string::npos)
    {
        credentialParametersFirstChar = "&";
    }
    
    string apiAddress = getApiUrl() + "/" + (*(*listsUrl.begin()).begin())
            + credentialParametersFirstChar
            + "login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8()
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
                        << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".parsing-error-title"),
                                      tr("Alert." + m_xmlPageName + ".parsing-error"), Wt::Ok);
                return EXIT_FAILURE;
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".typexception-error-title"),
                                      tr("Alert." + m_xmlPageName + ".typexception-error"), Wt::Ok);
                return EXIT_FAILURE;
            }
        }
        if (response.status() == 200 || response.status() == 404)
        {
            if (jsonResource.size() == 0)
            {
                jsonResource.push_back(std::vector<Wt::Json::Value>());
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
            if ((*listsUrl.begin()).size() == 0)
            {
                listsUrl.pop_front();
                jsonResource.push_back(std::vector<Wt::Json::Value>());
            }
            else if ((*(*listsUrl.begin()).begin()).find(":id") != string::npos && response.status() == 200)
            {
                std::vector<Wt::Json::Value> itJ = jsonResource.back();
                Wt::Json::Array& result1 = itJ.back();
                list_string::iterator listUrl = (*listsUrl.begin()).begin();
                string saveUrl = (*listUrl);
                Wt::Json::Array::iterator itA = result1.begin();
                while (itA != result1.end() && saveUrl.compare((*listUrl)) == 0)
                {
                    Wt::Json::Object jsonObject = (*itA);
                    long long idJ = jsonObject.get("id");
                    // On remplace celui en cours
                    (*listUrl).replace((*listUrl).find(":id"), 3,
                                       boost::lexical_cast<string>(idJ));
                    // on ajoute des éléments pour les autres IDs
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
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    return EXIT_FAILURE;
}

string AbstractPage::addParameter()
{
    return "";
}


// ---- ADD MODIF DELETE ----------

void AbstractPage::addResource(vector<Wt::WInteractWidget*> argument)
{
    // Post resource -------
    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\t\"name\": \"" + ((Wt::WLineEdit*)(*argument.begin()))->text().toUTF8() + "\"\n}");

    string apiAddress = getApiUrl() + "/" + (*(*m_listsUrl.begin()).begin())
    + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8();
    
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AbstractPage::postResourceCallback, this, _1, _2, client));

    Wt::log("debug") << m_xmlPageName + " : [POST] address to call : " << apiAddress;

    if (client->post(apiAddress, messageAsset))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "[" + m_xmlPageName + "] Error Client Http";
    }
}

void AbstractPage::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    string messageString;

    messageString = "{\n\"name\":\"" + ((Wt::WLineEdit*)(*arguments.begin()))->text().toUTF8() + "\"\n}";

    Wt::Http::Message message;
    message.addBodyText(messageString);

    string apiAddress = getApiUrl() + "/" + (*(*m_listsUrl.begin()).begin()) + "/"
            + boost::lexical_cast<string> (id)
            + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AbstractPage::putResourceCallback, this, _1, _2, client));
    Wt::log("debug") << m_xmlPageName + " : [PUT] address to call : " << apiAddress;
    Wt::log("debug") << m_xmlPageName + " : [PUT] message : " << messageString;
    if (client->put(apiAddress, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "[" + m_xmlPageName + "] Error Client Http";
    }
}

Wt::WDialog *AbstractPage::deleteResource(long long id)
{
    Wt::WDialog *box =
            new Wt::WDialog(tr("Alert." + m_xmlPageName
            + ".delete-" + m_xmlPageName));

    box->contents()->
            addWidget(new Wt::WText(tr("Alert." + m_xmlPageName
            + ".delete-" + m_nameResourcePageSpec + "-message")));

    Wt::WPushButton *ok =
            new Wt::WPushButton(tr("Alert."
            + m_xmlPageName + ".button-confirm"),
                                box->footer());
    ok->clicked().connect(box, &Wt::WDialog::accept);
    ok->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul =
            new Wt::WPushButton(tr("Alert."
            + m_xmlPageName + ".button-cancel"),
                                box->footer());
    annul->clicked().connect(box, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");

    m_isCreated = false;

    box->finished().connect(bind([=] () {
    if (box->result() == Wt::WDialog::Accepted)
    {
        Wt::Http::Message message;
        message.addBodyText("");
        string apiAddress = getApiUrl() + "/" + (*(*m_listsUrl.begin()).begin()) + "/"
                + boost::lexical_cast<string> (id)
                + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
                + "&token=" + m_session->user()->token.toUTF8();

        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&AbstractPage::apiDeleteResourceCallback, this, _1, _2, client));
        Wt::log("debug") << m_xmlPageName + " : [DELETE] address to call : " << apiAddress;
        if (client->deleteRequest(apiAddress, message))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
    }
    return box;
    }));

    box->show();
    return box;
}

// RETURNS API --------------------------------------

void AbstractPage::postResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
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
                        << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".parsing-error-title"),
                                      tr("Alert." + m_xmlPageName + ".parsing-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".typexception-error-title"),
                                      tr("Alert." + m_xmlPageName + ".typexception-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error: " << response.body();
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    m_isCreated = false;
    recursiveGetResources();
}

void AbstractPage::putResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client) 
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
                        << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".parsing-error-title"),
                                      tr("Alert." + m_xmlPageName + ".parsing-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".typexception-error-title"),
                                      tr("Alert." + m_xmlPageName + ".typexception-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error: " << response.body();
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    m_isCreated = false;
    recursiveGetResources();
}

void    AbstractPage::apiDeleteResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == Enums::EReturnCode::NO_CONTENT)
        {
            if (response.body() != "")
            {
                Wt::log("error") << "[" + m_xmlPageName + " Widget] Response should be empty : "
                        << response.body() << ".";
            }
        }
        else
        {
            Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
        }
    }
    else
    {
            Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error: " << response.body();
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    recursiveGetResources();
}



// Check input ----------------------------------------------

void AbstractPage::checkAdd(vector<Wt::WText*> errorMessage)
{
    if (checkInput(m_inputs, errorMessage) == 0)
    {
        m_isCreated = false;
        m_inputs.pop_back();
        addResource(m_inputs);
        m_inputs.clear();
    }
}

void AbstractPage::checkModif(vector_widget inputs, long long id, vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs, errorMessage) == 0)
    {
        m_isCreated = false;
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
            else
            {
                ((Wt::WLineEdit*)(*j))->addStyleClass("form-group has-success");
                if (((Wt::WText*)(*i))->isHidden() == false)
                {
                    ((Wt::WText*)(*i))->hide();
                }
            }
        }
        i++;
    }
    return check;
}

//  INPUT --------------------

void AbstractPage::showInputForAdd()
{
    //TSA à décommenter et renommer quand le cas d'utilisation se présentera
//    m_resourceTable->rowAt(m_resourceTable->rowCount() - 1)->show();
//    for (vector<Wt::WInteractWidget*>::const_iterator j = inputs_.begin();
//            j != inputs_.end();
//            j++)
//    {
//        Wt::WInteractWidget *widgetAdd = *j;
//        string nameRessouce("N2Wt9WLineEditE");
//        if (inputs_.begin() == j && nameRessouce.compare(typeid (*widgetAdd).name()) == 0)
//        {
//            ((Wt::WLineEdit*)(*j))->setFocus();
//        }
//        ((Wt::WInteractWidget*)(*j))->show();
//    }
}

void AbstractPage::inputForModif(long long id, int rowTable, int columnTable)
{
    vector<Wt::WText*> errorMessage;
    vector_widget inputs;
    int column(0);

    for (multimap_long_widgets::iterator itTable = m_rowsTable.begin();
            itTable != m_rowsTable.end(); itTable++)
    {
        int cpt(0);
        if ((*itTable).first == id)
        {
            vector_pair_string::iterator title = m_titles.begin();
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

                    Wt::WText *error = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                        + (*title).second));
                    error->hide();
                    errorMessage.push_back(error);

                    m_resourceTable->elementAt(rowTable, column)->clear();
                    m_resourceTable->elementAt(rowTable, column)->addWidget(input);
                    m_resourceTable->elementAt(rowTable, column++)->addWidget(error);

                    cpt++;
                }
            }
            title++;
        }
    }

    m_resourceTable->elementAt(rowTable, columnTable)->clear();
    m_resourceTable->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    m_resourceTable->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    Wt::WPushButton *valideBut = new Wt::WPushButton(m_resourceTable->elementAt(rowTable, columnTable));
    valideBut->setTextFormat(Wt::XHTMLUnsafeText);
    valideBut->setText("<span class='input-group-btn'><i class='icon-ok '></i></span>");
    valideBut->clicked().connect(boost::bind(&AbstractPage::checkModif, this, inputs, id, errorMessage));
}



// Methodes disposable --------------------------------

Wt::WComboBox *AbstractPage::getNumberOfLineDisplayedComboBox()
{
    m_nbAff = 5;
    Wt::WComboBox *nbLineToBeDisplayedCombo = new Wt::WComboBox();
    nbLineToBeDisplayedCombo->addItem("5");
    nbLineToBeDisplayedCombo->addItem("10");
    nbLineToBeDisplayedCombo->addItem("20");
    nbLineToBeDisplayedCombo->addItem("40");
    nbLineToBeDisplayedCombo->addItem("all");
    nbLineToBeDisplayedCombo->setStyleClass("dataTables_length");
    nbLineToBeDisplayedCombo->setWidth(Wt::WLength(61));
    nbLineToBeDisplayedCombo->setMargin(50, Wt::Right);
    nbLineToBeDisplayedCombo->changed().connect(bind([ = ] (){
    if (nbLineToBeDisplayedCombo->currentText().toUTF8().compare("all") == 0)
    {
        m_nbAff = 0;
    }
    else
    {
        m_nbAff = atoi(nbLineToBeDisplayedCombo->currentText().toUTF8().c_str());
    }
    m_nbAffBegin = 1;
    updatePage();
        }));
    return nbLineToBeDisplayedCombo;
}

void AbstractPage::builtPagination(Wt::WNavigationBar *navBar)
{
    Wt::WPushButton *butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + m_xmlPageName + ".paginate-first"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -1));
    navBar->addWidget(butPaginate);
    m_butPaginateExt.push_back(butPaginate);

    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + m_xmlPageName + ".paginate-prev"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -2));
    navBar->addWidget(butPaginate);
    m_butPaginateExt.push_back(butPaginate);


    for (int cpt(0); cpt < ((countResources() / m_nbAff) + ((countResources() % m_nbAff) > 0 ? 1 : 0)); cpt++)
    {
        butPaginate = new Wt::WPushButton(boost::lexical_cast<string>(cpt + 1));
        butPaginate->setStyleClass("fg-button ui-button ui-state-default btn");
        navBar->addWidget(butPaginate);
        m_butPaginate.push_back(butPaginate);
    }


    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + m_xmlPageName + ".paginate-next"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -3));
    navBar->addWidget(butPaginate);
    m_butPaginateExt.push_back(butPaginate);

    butPaginate = new Wt::WPushButton();
    butPaginate->addStyleClass("fg-button ui-button ui-state-default");
    butPaginate->setText(tr("Alert." + m_xmlPageName + ".paginate-last"));
    butPaginate->clicked().connect(boost::bind(&AbstractPage::switchPage, this, -4));
    navBar->addWidget(butPaginate);
    m_butPaginateExt.push_back(butPaginate);
}

// pagination ???
void AbstractPage::switchPage(int rst)
{
    int nbRow = countResources();
    if (rst == -4)
    {
        m_nbAffBegin = (nbRow - (nbRow % m_nbAff)) + 1;
    }
    else if (rst == -3)
    {
        m_nbAffBegin = (m_nbAffBegin + m_nbAff) <= nbRow ? (m_nbAffBegin + m_nbAff) : m_nbAffBegin;
    }
    else if (rst == -2)
    {
        m_nbAffBegin = (m_nbAffBegin - m_nbAff) >= 1 ? (m_nbAffBegin - m_nbAff) : 1;
    }
    else if (rst == -1)
    {
        m_nbAffBegin = 1;
    }
    else
    {
        m_nbAffBegin = (m_nbAff * rst) + 1;
    }
    updatePage();
}

void AbstractPage::initPagination(Wt::WNavigationBar *navBar)
{
    int nbRow = countResources();
    if (m_nbAff == 0)
    {
        m_nbAff = nbRow;
    }
    if (nbRow > m_nbAff)
    {
        builtPagination(navBar);
    }
}

void AbstractPage::doThePaginationTrick()
{
    int nbRow = countResources();
    int cpt(0);
    bool check = false;
    if (nbRow > m_nbAff)
        check = true;

    for (vector_widget::iterator it = m_butPaginateExt.begin(); it != m_butPaginateExt.end(); it++)
    {
        if (check)
        {
            if (cpt < 2 && m_nbAffBegin > 1)
            {
                ((Wt::WPushButton*)(*it))->setDisabled(false);
            }
            else if (cpt > 1 && (nbRow + 1) > (m_nbAffBegin + m_nbAff))
            {
                ((Wt::WPushButton*)(*it))->setDisabled(false);
            }
            else
            {
                ((Wt::WPushButton*)(*it))->setDisabled(true);
            }
            ((Wt::WPushButton*)(*it))->show();
        }
        else
        {
            ((Wt::WPushButton*)(*it))->hide();
        }
        cpt++;
    }
    cpt = 0;
    int rst = (m_nbAffBegin / m_nbAff);
    for (vector_widget::iterator i = m_butPaginate.begin(); i != m_butPaginate.end(); i++)
    {
        if (check)
        {
            ((Wt::WPushButton*)(*i))->setText(boost::lexical_cast<string>(cpt + 1));
            ((Wt::WPushButton*)(*i))->setAttributeValue("style",
                                                        "background-image: linear-gradient(#ffffff, #ffffff 25%, #e6e6e6)");
            ((Wt::WPushButton*)(*i))->clicked().connect(boost::bind(&AbstractPage::switchPage, this, cpt));
            if (cpt == rst)
            {
                ((Wt::WPushButton*)(*i))->setAttributeValue("style",
                                                            "background-image: linear-gradient(#ffffff, #ffffff 25%, #CFCFCF)");
            }
            ((Wt::WPushButton*)(*i))->hide();
            if (((nbRow / m_nbAff) + ((nbRow % m_nbAff) > 0 ? 1 : 0)) <= 5)
            {
                if (cpt < ((nbRow / m_nbAff) + ((nbRow % m_nbAff) > 0 ? 1 : 0)))
                {
                    ((Wt::WPushButton*)(*i))->show();
                }
            }
            else
            {
                if (rst <= 2 && (cpt - 2) <= 2)
                {
                    if ((cpt - 2) == 2)
                    {
                        ((Wt::WPushButton*)(*i))->setText("...");
                    }
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if (((cpt + 3) == rst || (cpt + 4) == rst)
                        && (cpt + 4) >= (int) (m_butPaginate.size() - 1))
                {
                    if ((cpt + 4) == (int) (m_butPaginate.size() - 1))
                    {
                        ((Wt::WPushButton*)(*i))->setText("...");
                    }
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if ((cpt + 2) == rst)
                {
                    if ((cpt + 3) < (int) (m_butPaginate.size() - 1) && cpt > 0)
                    {
                        ((Wt::WPushButton*)(*i))->setText("...");
                    }
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if ((cpt - 2) > 2 && (cpt - 2) == rst)
                {
                    if ((cpt + 1) < (int) (m_butPaginate.size()))
                    {
                        ((Wt::WPushButton*)(*i))->setText("...");
                    }
                    ((Wt::WPushButton*)(*i))->show();
                }
                else if (cpt + 1 != rst
                        && cpt - 1 != rst
                        && cpt != rst)
                {
                    /* Hide */
                }
                else
                {
                    ((Wt::WPushButton*)(*i))->show();
                }
            }
        }
        else
        {
            ((Wt::WPushButton*)(*i))->hide();
        }
        cpt++;
    }
}

void AbstractPage::selectLinesToBeDisplayed()
{
    int nb(0);
    int nbRow = countResources();

    if (m_nbAffBegin > nbRow)
    {
        m_nbAffBegin -= m_nbAff;
    }
    if (m_nbAff == 0)
    {
        m_nbAff = nbRow;
        m_nbAffBegin = 1;
    }
    nb = 1;
    for (vector_pair::iterator it = m_resources.begin(); it != m_resources.end(); it++)
    {
        Wt::WTableRow *tableRow = (Wt::WTableRow *)it->second;
        if ((int) it->first == 0)
        {
            if (nb >= m_nbAffBegin && nb <= (m_nbAff + m_nbAffBegin - 1))
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
        {
            tableRow->hide();
        }
    }
}

void AbstractPage::searchName(Wt::WLineEdit *arg)
{
    int cpt(0);
    bool check;
    if (arg->text().empty())
    {
        for (vector_pair::iterator it = m_resources.begin(); it != m_resources.end(); it++)
        {
            m_nbAffBegin = 1;
            it->first = 0;
        }
    }
    else
    {
        for (vector_pair::iterator it = m_resources.begin(); it != m_resources.end(); it++)
        {
            check = false;
            Wt::WTableRow *tableRow = (Wt::WTableRow *)it->second;
            for (int j(0); j < (int)m_rowsTable.size(); j++)
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
                    {
                        check = true;
                    }
                }
            }
            if (check == true)
            {
                it->first = 0;
            }
            else
            {
                it->first = 1;
            }
            cpt++;
        }
    }
    updatePage();
}

int AbstractPage::countResources()
{
    int cpt(0);
    for (vector_pair::iterator it = m_resources.begin(); it != m_resources.end(); it++)
    {
        if (it->first == 0)
        {
            cpt++;
        }
    }
    return cpt;
}


