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

AbstractPage::AbstractPage(Echoes::Dbo::Session *session, string apiUrl, string pageName, bool selectable)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + pageName + ".Management.template"))
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(pageName, false);

    m_xmlPageName = pageName; // XML name
    m_isDeleteButtonPresent = true; // Button delete
    m_isModifButtonPresent = true; // Button modifi
//    m_isMainPage = true; // Dialog/True
    m_nameResourcePageSpec = ""; // when more one class use same xml file.
    m_session = session;
    m_undidName = "name";
    m_footerOkButtonActive = true;
    m_selectable = selectable;
    m_selectedID = 0;
    setApiUrl(apiUrl);
    
    
//    _timer = new Wt::WTimer(this);
//    _timer->setInterval(5) ;
//    _timer->timeout().connect(boost::bind(&AbstractPage::updatePage, this)) ;
//    _timer->start() ;
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
        getResourceList();
    }

    createTable();
   
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
}

// -------- Creates Elements to table. ------

Wt::WContainerWidget *AbstractPage::createTableFirstHeader()
{
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget();
    headerTableContainer->addStyleClass("widget-title header-pers");
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + tr("Alert." + m_xmlPageName + ".add-form." + m_nameResourcePageSpec)
                  + "</h5>", headerTableContainer);

      Wt::WAnchor *headerButton = new Wt::WAnchor(headerTableContainer);
//    if (m_isMainPage) //gkr: popup is created  in popupWindow.
//    {
        headerButton->clicked().connect(boost::bind(&AbstractPage::addResourcePopup, this));
//    }
//    else // gkr: Input is created in createBodyTable, showInputForAdd() is there just for show this.
//    {
//        headerButton->clicked().connect(boost::bind(&AbstractPage::showInputForAdd, this));
//    }

    headerButton->setStyleClass("button-add btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    return headerTableContainer;
}

Wt::WContainerWidget *AbstractPage::createTableBody()
{
    m_resources.clear();

    Wt::WContainerWidget *resourceTableContainer = new Wt::WContainerWidget();

    resourceTableContainer->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");

    m_resourceTable = new Wt::WTable(resourceTableContainer);
    m_resourceTable->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    m_resourceTable->setHeaderCount(1, Wt::Horizontal);
    addTableSecondHeader();

    fillBodyTable();            

    return resourceTableContainer;
}

void AbstractPage::tableHandler(long long id) {
    m_selectedID = id;
    updatePage();
}

Wt::WContainerWidget *AbstractPage::createTableFooter()
{

        
    Wt::WContainerWidget *footerTable = new Wt::WContainerWidget();
    footerTable->addStyleClass("fg-toolbar ui-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix footer-pers");

//    new Wt::WText(tr("Alert." + m_xmlPageName + ".search-bar"), footerTable);

//    Wt::WNavigationBar *navBar = new Wt::WNavigationBar(footerTable);
//    navBar->addStyleClass("paginate-edit");

    return footerTable;
}

// Add Resource For Elements -----------------------------------------------------------------

void AbstractPage::addTableSecondHeader()
{
    int columnTable(0);

    m_resourceTable->elementAt(0, 0)->setAttributeValue("style", "border-left:0;");
    for (multimap<int, string>::iterator it = m_titles.begin(); it != m_titles.end(); it++)
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
    
    for (multimap<long long, vector_widget>::iterator itRowTable = m_rowsTable.begin();
            itRowTable != m_rowsTable.end() ;itRowTable++)
    {
        columnTable = 0;
        m_resourceTable->elementAt(rowBodyTable, columnTable)->setAttributeValue("style", "border-left:0;");
        if (itRowTable->first >= 0)
        {
            for (vector_widget::iterator k = m_rowsTable.find(itRowTable->first)->second.begin();
                    k != m_rowsTable.find(itRowTable->first)->second.end(); k++)
            {
                Wt::WInteractWidget *widgetAdd = *k; 
                string nameRessource("N2Wt5WTextE");
                if (nameRessource.compare(typeid (*widgetAdd).name()) == 0)
                {
                    nameRessource = ((Wt::WText*)(widgetAdd))->text().toUTF8();
                    string newName = nameRessource;
                    if (newName.find('|') != string::npos)
                    {
                        vector<string> dataline;
                        boost::split(dataline, newName, boost::is_any_of("|"));
                        newName.clear();
                        nameRessource.clear();
                        for (vector<string>::iterator it = dataline.begin();
                                it != dataline.end(); it++)
                        {
                            string resizeString = (*it);
                            if (((string)(*it)).find('<') == string::npos)
                            {
                                nameRessource += (*it) + "\n";
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
                        if (nameRessource.size() > (unsigned int) (SIZE_NAME / m_titles.size()))
                        {
                            newName.resize(SIZE_NAME / m_titles.size());
                            newName.resize(newName.size() + 3, '.');
                        }
                    }
                    Wt::WText *newColumn = new Wt::WText(Wt::WString::fromUTF8(newName),
                                                         m_resourceTable->elementAt(rowBodyTable, columnTable));
                    newColumn->setTextFormat(Wt::TextFormat::XHTMLUnsafeText);
                    newColumn->setToolTip(nameRessource);
                }
                else
                {
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->addWidget(widgetAdd);
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
                }
                columnTable++;
            }
            m_resources.push_back(pair<int, Wt::WTableRow*>(0, m_resourceTable->rowAt(rowBodyTable)));
            addGenericButtonsToResourceTable((*itRowTable).first, rowBodyTable, columnTable);
            if(m_selectable)
            {
                long long id = itRowTable->first;
                if(m_selectedID == 0)
                    m_selectedID = id;
                for(int column = 0; column < m_resourceTable->columnCount(); column++)
                {
                    m_resourceTable->elementAt(rowBodyTable, column)->clicked().connect(boost::bind(&AbstractPage::tableHandler, this, id));
                    if (m_selectedID == id)
                        m_resourceTable->elementAt(rowBodyTable, column)->decorationStyle().setBackgroundColor(Wt::red);
                }
            }
            rowBodyTable++;
        }
        
    }
}

// POPUP : ----------------------------------------------------

void AbstractPage::popupAddWidget(Wt::WDialog *dialog, long long id)
{
    
}

//add a combo box to a popup (add / modify) FIXME: only if here is one and only one combobox to add
Wt::WComboBox *AbstractPage::popupAdd(Wt::WDialog *dialog) 
{   
    return new Wt::WComboBox(dialog->contents());
}

void AbstractPage::addResourcePopup()
{
    Wt::WLineEdit *input;
    vector<Wt::WInteractWidget*> inputName;
    vector<Wt::WText*> errorMessage;

    Wt::WDialog *dialogAdd_ = new Wt::WDialog(tr("Alert." + m_xmlPageName + ".add-" + m_nameResourcePageSpec));

    int cpt(0);
    for (multimap<int, string>::iterator title = m_displayedTitlesPopups.begin();
            title != m_displayedTitlesPopups.end(); title++)
    {
        if (title->first >= 0)
        {
            new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + (*title).second)
                          + " : <br />", dialogAdd_->contents());

            if (title->first == ETypeJson::text)
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
            else if (title->first == ETypeJson::boolean)
            {
                Wt::WCheckBox *checkBox = new Wt::WCheckBox(dialogAdd_->contents());
                inputName.push_back(checkBox);
            }
            else if (title->first == ETypeJson::integer)
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
            else if (title->first == ETypeJson::undid)
            {
                inputName.push_back(popupAdd(dialogAdd_));
            }
            else if (title->first == ETypeJson::object)
            {
                inputName.push_back(popupAdd(dialogAdd_));
            }
            Wt::WText *error = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                + title->second),
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
}

void AbstractPage::modifResourcePopup(long long id)
{
    vector<Wt::WInteractWidget*> inputName;
    vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif = new Wt::WDialog(tr("Alert." + m_xmlPageName + ".modif-" + m_nameResourcePageSpec));
    for (map<long long, vector_widget>::iterator itTable = m_rowsTable.begin();
            itTable != m_rowsTable.end(); itTable++)
    {
        
        unsigned int cpt(0);
        if (itTable->first == id)
        {
        
            for (Wt::WInteractWidget *itElem : itTable->second)
            {
                
                multimap<int, string>::iterator title;
                unsigned int correctTitlefinder = 0;
                for(title = m_displayedTitlesPopups.begin(); title != m_displayedTitlesPopups.end() ; title++ )
                {
                    if (cpt == correctTitlefinder++)
                    {
                        break;
                    }
                }
                if (title->first >= 0)
                {
                        new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + title->second)
                                      + " : <br />", dialogModif->contents());                    
                }
                
                switch (title->first)
                {
                    case ETypeJson::text:
                    {
                        string nameResource = ((Wt::WText*)(itElem))->text().toUTF8();
                        string newName = nameResource;
                        if (nameResource.size() > SIZE_NAME + SIZE_NAME)
                        {
                            newName.resize(newName.size() + 3, '.');
                        }
                        Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName), dialogModif->contents());
                        // FIXME: ne marche pas quand c'est un mail / tel, cf. medias
                        input->setValidator(editValidator(ETypeJson::text));
                        input->enterPressed().connect(dialogModif, &Wt::WDialog::accept);
                        input->setWidth(Wt::WLength(150));
                        if (inputName.size() == 0)
                        {
                            input->setFocus();
                        }
                        input->setToolTip(Wt::WString::fromUTF8(nameResource));
                        inputName.push_back(input);
                        Wt::WText *error2 = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                             + title->second), dialogModif->contents());
                        error2->hide();
                        errorMessage.push_back(error2);
                        new Wt::WText("<br />", dialogModif->contents());
                        break;
                    }
                    case ETypeJson::integer:
                    {
                        string nameResource = ((Wt::WText*)(itElem))->text().toUTF8();
                        string newName = nameResource;
                        if (nameResource.size() > SIZE_NAME + SIZE_NAME)
                        {
                            newName.resize(newName.size() + 3, '.');
                        }
                        Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName), dialogModif->contents());
                        input->setValidator(new Wt::WRegExpValidator(Wt::WString::fromUTF8("^[0123456789]+")));
                        input->enterPressed().connect(dialogModif, &Wt::WDialog::accept);
                        input->setWidth(Wt::WLength(150));
                        if (inputName.size() == 0)
                        {
                            input->setFocus();
                        }
                        input->setToolTip(Wt::WString::fromUTF8(nameResource));
                        inputName.push_back(input);
                        Wt::WText *error2 = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                             + (*title).second), dialogModif->contents());
                        error2->hide();
                        errorMessage.push_back(error2);
                        new Wt::WText("<br />", dialogModif->contents());
                        break;
                    }
                    case ETypeJson::boolean:
                    {
                        unsigned int cpt2(0);
                        for (vector_widget::iterator widgetAdd = itTable->second.begin();
                                widgetAdd != itTable->second.end(); widgetAdd++)
                        {
                            if (cpt2 == cpt)
                            {
                                
                                Wt::WCheckBox *checkBox = new Wt::WCheckBox(dialogModif->contents());
                                new Wt::WText("<br />", dialogModif->contents());
                                Wt::WCheckBox *originalCheckBox = (Wt::WCheckBox*)(*widgetAdd);
                                if (originalCheckBox->checkState() == Wt::CheckState::Checked)
                                {
                                    checkBox->setChecked();
                                }
                                inputName.push_back(checkBox);
                                break;
                            }
                            cpt2++;
                        }
                        break;
                    }
                    case ETypeJson::undid:
                    {
                        Wt::WComboBox *comboBox = popupAdd(dialogModif);
                        unsigned int cpt2(0);
                        for (vector_widget::iterator widg = itTable->second.begin();
                                widg != itTable->second.end(); widg++)
                        {
                            Wt::WStandardItemModel *model = (Wt::WStandardItemModel*)comboBox->model();
                            if (cpt2 == cpt)
                            {
                                for (int nb(0); nb < model->rowCount(); nb++)
                                {
                                    if (((Wt::WText*)(*widg))->text().toUTF8().compare(model->item(nb, 0)->text().toUTF8()) == 0)
                                    {
                                        comboBox->setCurrentIndex(nb);
                                    }
                                }
                                inputName.push_back(comboBox);
                                break;
                            }
                            cpt2++;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    
                }

                title++;
                cpt++;
                correctTitlefinder++;
            }
            break;
        }
    }

    popupFinalization(dialogModif ,id);

    dialogModif->finished().connect(bind([ = ] (){
        popupCheck(inputName, errorMessage, dialogModif, id);
        return;
    }));
    dialogModif->show();
}

void AbstractPage::popupCheck(vector<Wt::WInteractWidget*> inputName, vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id)
{
    int check(0);
    if (dialog->result() == Wt::WDialog::Rejected)
    {
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

// ToDo: refactor
void AbstractPage::addGenericButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    columnTable = addCustomButtonsToResourceTable(id, rowTable, columnTable);
    if (m_isModifButtonPresent)
    {
        Wt::WPushButton *modifButton = new Wt::WPushButton(m_resourceTable->elementAt(rowTable, columnTable));
        modifButton->setAttributeValue("class", "btn btn-inverse");
        modifButton->setTextFormat(Wt::XHTMLUnsafeText);
        modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
//        if (m_isMainPage)
//        {
            modifButton->clicked().connect(boost::bind(&AbstractPage::modifResourcePopup, this, id));
//        }
//        else
//        {
//            modifButton->clicked().connect(boost::bind(&AbstractPage::inputForModif, this, id, rowTable, columnTable));
//        }
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
    Wt::WPushButton *footerOkButton = new Wt::WPushButton(tr("Alert." + m_xmlPageName + ".button-save"),
                                              dialog->footer());
    if (getFooterOkButtonStatus())
    {
        footerOkButton->clicked().connect(dialog, &Wt::WDialog::accept);
        footerOkButton->setAttributeValue("style", "margin-left:12px");
    }
    else
    {
        footerOkButton->disable();
    }
    
    
    

    Wt::WPushButton *footerCancelButton = new Wt::WPushButton(tr("Alert." + m_xmlPageName + ".button-cancel"),
                                                 dialog->footer());
    footerCancelButton->clicked().connect(dialog, &Wt::WDialog::reject);
    footerCancelButton->setAttributeValue("style", "margin-left:12px;");
}

// Set/Get attribut to init or option. -------------------------------------

void AbstractPage::setRowsTable(map<long long, vector_widget> rowsTable)
{
    m_rowsTable = rowsTable;
}

map<long long, vector_widget> AbstractPage::getRowsTable()
{
    return m_rowsTable;
}

void AbstractPage::setUndidName(string undidName)
{
    m_undidName = undidName;
}

void AbstractPage::setTitles(multimap<int, string> titles)
{
    m_titles = titles;
    setDisplayedTitlesPopups();
}

void AbstractPage::setDisplayedTitlesPopups()
{
    m_displayedTitlesPopups = m_titles;
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

bool AbstractPage::getFooterOkButtonStatus()
{
    return m_footerOkButtonActive;
}

void AbstractPage::setFooterOkButtonStatus(bool active)
{
    m_footerOkButtonActive = active;
}

// -------- CALL RETURN API, HANDLE -----------


void AbstractPage::handleJsonGet(vectors_Json jsonResources)
{
    m_rowsTable.clear();
    vector<Wt::Json::Value> jsonResource;
    try
    {
        jsonResource = jsonResources.at(0);
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = jsonResource.at(0);
            for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
            {
                Wt::Json::Object jsonObject = jsonArray.at(cpt);
                vector<Wt::WInteractWidget *> nameW;
                for (multimap<int, string>::iterator itTitles = m_titles.begin();
                        itTitles != m_titles.end(); itTitles++)
                {
                    switch (itTitles->first)
                    {
                    case ETypeJson::text:
                    {
                        Wt::WString wString = jsonObject.get(itTitles->second);
                        Wt::WText *text = new Wt::WText(wString);
                        text->setObjectName("text");
                        nameW.push_back(text);
                        break;
                    }
                    case ETypeJson::boolean:
                    {
                        bool boolean = jsonObject.get(itTitles->second);
                        Wt::WCheckBox *checkBox = new Wt::WCheckBox();
                        checkBox->setChecked(boolean);
                        checkBox->setDisabled(true);
                        nameW.push_back(checkBox);
                        break;
                    }
                    case ETypeJson::integer:
                    {
                        int number = jsonObject.get(itTitles->second);
                        nameW.push_back(new Wt::WText(boost::lexical_cast<string>(number)));
                        break;
                    }
                    case ETypeJson::undid:
                    {
                        Wt::Json::Object jsonObjectParam = jsonResource.at(cpt + 1);
                        Wt::Json::Object nameObjet = jsonObjectParam.get(itTitles->second);
                        Wt::WString name = nameObjet.get(m_undidName);
                        Wt::WText *text = new Wt::WText(name);
                        nameW.push_back(text);
                        break;
                    }
                    case ETypeJson::object:
                    {
                        Wt::Json::Object subObject = jsonObject.get(itTitles->second);
                        long long id = subObject.get("id");
                        nameW.push_back(new Wt::WText(boost::lexical_cast<string>(id)));
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
//            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
}

void AbstractPage::recursiveGetResources(vectors_Json jsonResource, lists_string listsUrl)
{
    if (listsUrl.size() == 0)
    {
        listsUrl = m_listsUrl;
    }
    
    
    if (listsUrl.begin()->begin()->find(":id") != string::npos)
    {
        list_string::iterator listUrl = listsUrl.begin()->begin();
        listUrl->replace(listUrl->find(":id"), 3, boost::lexical_cast<string>(0));
    }
    
    string credentialParametersFirstChar = "?";
    if (listsUrl.begin()->begin()->find("?") != string::npos)
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
                jsonResource.push_back(vector<Wt::Json::Value>());
            }
            
            if (jsonResource.begin()->size() == 0)
            {
                jsonResource.begin()->push_back(result);
            }
            else
            {
                jsonResource.back().push_back(result);
            }

            listsUrl.begin()->pop_front();
            if (listsUrl.begin()->size() == 0)
            {
                listsUrl.pop_front();
                jsonResource.push_back(vector<Wt::Json::Value>());
            }
            else if (listsUrl.begin()->begin()->find(":id") != string::npos && response.status() == 200)
            {
                try
                {
                    vector<Wt::Json::Value> itJ = jsonResource.back();
                    Wt::Json::Array& result1 = itJ.back();
                    list_string::iterator listUrl = listsUrl.begin()->begin();
                    string saveUrl = (*listUrl);
                    Wt::Json::Array::iterator itA = result1.begin();
                    while (itA != result1.end() && saveUrl.compare((*listUrl)) == 0)
                    {
                        Wt::Json::Object jsonObject = (*itA);
                        long long idJ = jsonObject.get("id");
                        // On remplace celui en cours
                        listUrl->replace(listUrl->find(":id"), 3, boost::lexical_cast<string>(idJ));
                        // on ajoute des éléments pour les autres IDs
                        if (++itA != result1.end())
                        {
                            listsUrl.begin()->push_back(saveUrl);
                            listUrl++;
                        }
                    }
                }
                catch (Wt::Json::ParseError const& e)
                {
                    Wt::log("warning")
                            << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                            + " Widget] JSON parse Exception: " << response.body();
                }
                catch (Wt::Json::TypeException const& e)
                {
                    Wt::log("warning")
                            << "[" + tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                            + " Widget] JSON Type Exception: " << response.body();
                }
            }
            if (listsUrl.size() == 0)
            {
                handleJsonGet(jsonResource);
                updatePage(false);
            }
            else if (response.status() == 200)
            {
                recursiveGetResources(jsonResource, listsUrl);
            }
            else if (response.status() == 404)
            {
                recursiveGetResources(jsonResource, listsUrl);
            }

            return EXIT_SUCCESS;
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                  + m_xmlPageName) + " Widget] Http::Client error: " 
                           << response.body();
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
    Wt::Http::Message *message = new Wt::Http::Message();
    
    setAddResourceMessage(message,argument);

    string apiAddress = getApiUrl() + "/" + (*m_listsUrl.begin()->begin())
    + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8();
    
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AbstractPage::postResourceCallback, this, _1, _2, client));

    Wt::log("debug") << m_xmlPageName + " : [POST] address to call : " << apiAddress;

    if (client->post(apiAddress, *message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "[" + m_xmlPageName + "] Error Client Http";
    }
}

void AbstractPage::setAddResourceMessage(Wt::Http::Message *message, vector<Wt::WInteractWidget*> argument)
{
    message->addBodyText("{\n\t\"name\": \"" + ((Wt::WLineEdit*)(*argument.begin()))->text().toUTF8() + "\"\n}");
}

void AbstractPage::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    Wt::Http::Message *message = new Wt::Http::Message();
    
    setModifResourceMessage(message,arguments);
    
    string apiAddress = getApiUrl() + "/" + (*(*m_listsUrl.begin()).begin()) + "/"
            + boost::lexical_cast<string> (id)
            + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AbstractPage::putResourceCallback, this, _1, _2, client));
    Wt::log("debug") << m_xmlPageName + " : [PUT] address to call : " << apiAddress;
    Wt::log("debug") << m_xmlPageName + " : [PUT] message : " << message->body();
    if (client->put(apiAddress, *message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "[" + m_xmlPageName + "] Error Client Http";
    }
}

void AbstractPage::setModifResourceMessage(Wt::Http::Message *message, vector<Wt::WInteractWidget*> argument)
{
    message->addBodyText("{\n\t\"name\": \"" + ((Wt::WLineEdit*)(*argument.begin()))->text().toUTF8() + "\"\n}");
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

    box->finished().connect(bind([=] () {
    if (box->result() == Wt::WDialog::Accepted)
    {
        Wt::Http::Message message;
        message.addBodyText("");
        string apiAddress = getApiUrl() + "/" + (*m_listsUrl.begin()->begin()) + "/"
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
    updatePage();
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
                                       + m_xmlPageName) + " Widget] Http::Client error1: " << response.status() << " " << response.body();
            Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client error2: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    updatePage();
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
        m_inputs.pop_back();
        addResource(m_inputs);
        m_inputs.clear();
    }
}

void AbstractPage::checkModif(vector_widget inputs, long long id, vector<Wt::WText*> errorMessage)
{
    if (checkInput(inputs, errorMessage) == 0)
    {
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

// ToDo: should be a boolean
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

    for (multimap<long long, vector_widget>::iterator itTable = m_rowsTable.begin();
            itTable != m_rowsTable.end(); itTable++)
    {
        int cpt(0);
        if ((*itTable).first == id)
        {
            multimap<int, string>::iterator title = m_displayedTitlesPopups.begin();
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
