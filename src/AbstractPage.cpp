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
#include <bits/stl_vector.h>

#include "AbstractPage.h"

using namespace std;

const std::string AbstractPage::xmlDirectory = "resources/xml/";

AbstractPage::AbstractPage(Echoes::Dbo::Session *session, string apiUrl, string pageName, bool selectable)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + pageName + ".Management.template"))
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + pageName, false);

    m_hasAddButton = true;
    m_autoUpdate = true;
    m_xmlPageName = pageName; // XML name
    m_isDeleteButtonPresent = true; // Button delete
    m_isModifButtonPresent = true; // Button modifi
    //    m_isMainPage = true; // Dialog/True
    m_session = session;
    
    m_undidNames.push_back("name");
    m_footerOkButtonActive = true;
    m_selectable = selectable;
    m_selectedID = 0;
    setApiUrl(apiUrl);
}

AbstractPage::~AbstractPage()
{
}

void AbstractPage::updatePage()
{
    if (m_autoUpdate)
    {
        getResourceList();
    }    
}

void AbstractPage::fillTable()
{
    if (m_selectable && m_selectedID == 0)
    {
        m_selectedID = m_rowsTable.begin()->first;
    }
    createTable();
    for (size_t i(0); i < m_pagesToUpdate.size(); ++i)
    {
        m_pagesToUpdate[i]->updatePage();
    }
}

void AbstractPage::clearStructures()
{
    m_rowsTable.clear();
}

void AbstractPage::getResourceList()
{
    clearStructures();
    boost::function<void (vectors_Json)> functorHandleJsonGets = boost::bind(&AbstractPage::handleJsonGet, this, _1);   
    recursiveGetResources(m_listsUrl, functorHandleJsonGets);  
}

void AbstractPage::setResources(vector_pair resources)
{
    m_resources = resources;
}

vector_pair AbstractPage::getResources() const
{
    return m_resources;
}

void AbstractPage::setResourceTable(Wt::WTable* resourceTable)
{
    m_resourceTable = resourceTable;
}

Wt::WTable* AbstractPage::getResourceTable() const
{
    return m_resourceTable;
}

void AbstractPage::setSelectedID(long long selectedID)
{
    m_selectedID = selectedID;
}

long long AbstractPage::getSelectedID()
{
    return m_selectedID;
}

void AbstractPage::setAddButtonEnable(bool enable)
{
    m_buttonAddEnable = enable;
}

void AbstractPage::addPageToUpdate(AbstractPage* abstractPage)
{
    m_pagesToUpdate.push_back(abstractPage);
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
    
    Wt::WText *nameMainTable = new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + tr("Alert." + m_xmlPageName + ".main-table")
                  + "</h5>", headerTableContainer);
    nameMainTable->setObjectName(m_xmlPageName + "-main-table");

    if (m_hasAddButton)
    {
        Wt::WAnchor *headerButton = new Wt::WAnchor(headerTableContainer);

        addPopupAddHandler(headerButton);
        headerButton->setStyleClass("button-add btn");
        headerButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
        headerButton->setObjectName("add-button");
    }
    return headerTableContainer;
}

void AbstractPage::addPopupAddHandler(Wt::WInteractWidget* widget)
{
    widget->clicked().connect(boost::bind(&AbstractPage::addResourcePopup, this));
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

void AbstractPage::tableHandler(long long id)
{
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
    for (std::vector<std::pair <int, string>>::iterator it = m_titles.begin(); it != m_titles.end(); it++)
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
    int tmp;
    int columnTable(0);
    int rowBodyTable(1);

    for (multimap<long long, vector_widget>::iterator itRowTable = m_rowsTable.begin();
            itRowTable != m_rowsTable.end(); itRowTable++)
    {
        columnTable = 0;
        m_resourceTable->elementAt(rowBodyTable, columnTable)->setAttributeValue("style", "border-left:0;");
        if (itRowTable->first >= 0)
        {
            for (vector_widget::iterator k = m_rowsTable.find(itRowTable->first)->second.begin();
                    k != m_rowsTable.find(itRowTable->first)->second.end(); k++)
            {
                Wt::WInteractWidget *widgetAdd = *k;
                const string test("N2Wt5WTextE");
                if (test.compare(typeid (*widgetAdd).name()) == 0)
                {
                    wstring nameRessource = ((Wt::WText*)(widgetAdd))->text().value();
                    wstring newName = nameRessource;
                    if (newName.find('|') != string::npos)
                    {
                        vector<wstring> dataline;
                        boost::split(dataline, newName, boost::is_any_of("|"));
                        newName.clear();
                        nameRessource.clear();
                        for (vector<wstring>::iterator it = dataline.begin();
                                it != dataline.end(); it++)
                        {
                            wstring resizeString = (*it);
                            if (((wstring)(*it)).find('<') == wstring::npos)
                            {
                                nameRessource += (*it) + L"\n";
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
                    Wt::WText *newColumn = new Wt::WText(Wt::WString(newName),
                                                         m_resourceTable->elementAt(rowBodyTable, columnTable));
                    newColumn->setTextFormat(Wt::TextFormat::XHTMLUnsafeText);
                    newColumn->setToolTip(nameRessource);
                }
                else
                {
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->addWidget(widgetAdd);
                    m_resourceTable->elementAt(rowBodyTable, columnTable)->setContentAlignment(Wt::AlignCenter);
                    //m_resourceTable->elementAt(rowBodyTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
                }
                std::string cell_name("cell_" + boost::lexical_cast<string>(rowBodyTable) + "x" + boost::lexical_cast<string>(columnTable));
                m_resourceTable->elementAt(rowBodyTable, columnTable)->setObjectName(cell_name);
                columnTable++;
            }
            
            tmp = columnTable;
            columnTable = addCustomResourceTable((*itRowTable).first, rowBodyTable, columnTable);
            while (tmp != columnTable)
            {
                tmp = columnTable;
                columnTable = addCustomResourceTable((*itRowTable).first, rowBodyTable, columnTable);
                
                std::string cell_name("cell-_" + boost::lexical_cast<string>(rowBodyTable) + "x" + boost::lexical_cast<string>(columnTable));
                m_resourceTable->elementAt(rowBodyTable, columnTable)->setObjectName(cell_name);
            }
            
            m_resources.push_back(pair<int, Wt::WTableRow*>(0, m_resourceTable->rowAt(rowBodyTable)));
            addGenericButtonsToResourceTable((*itRowTable).first, rowBodyTable, columnTable);
            if (m_selectable)
            {
                long long id = itRowTable->first;
                for (int column = 0; column < m_resourceTable->columnCount(); column++)
                {
                    m_resourceTable->elementAt(rowBodyTable, column)->clicked().connect(boost::bind(&AbstractPage::tableHandler, this, id));
                    if (m_selectedID == id)
                    {
                        m_resourceTable->elementAt(rowBodyTable, column)->decorationStyle().setBackgroundColor(*(new Wt::WColor(250, 180, 80)));
                    }
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
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;

    Wt::WDialog *dialogAdd_ = new Wt::WDialog(tr("Alert." + m_xmlPageName + ".popup-add"));
    dialogAdd_->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);

    int cpt(0);
    for (std::vector<std::pair <int, string>>::iterator title = m_displayedTitlesPopups.begin();
            title != m_displayedTitlesPopups.end(); title++)
    {
        /* see setValidatorType and editValidator for better understanding */
        int jsonType = (title->first >> 8) & 0xF;
        int fullType = title->first;
            
        if (jsonType != ETypeJson::widget)
        {
            std::string fieldName("input" + boost::lexical_cast<string>(cpt));
            new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + (*title).second)
                          + " : <br />", dialogAdd_->contents());

            if (jsonType == ETypeJson::text)
            {
                Wt::WContainerWidget *inputCW = new Wt::WContainerWidget(dialogAdd_->contents());
                inputCW->addStyleClass("control-group controls");
                
                input = new Wt::WLineEdit(inputCW);
                //FIXME
                input->setValidator(AbstractPage::editValidator(fullType));
                input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
                
                input->setObjectName(fieldName);
                inputName->push_back(input);
            }
            else if (jsonType == ETypeJson::boolean)
            {
                Wt::WCheckBox *checkBox = new Wt::WCheckBox(dialogAdd_->contents());
                
                checkBox->setObjectName(fieldName);
                inputName->push_back(checkBox);
            }
            else if (jsonType == ETypeJson::number)
            {
                input = new Wt::WLineEdit(dialogAdd_->contents());
                input->setValidator(AbstractPage::editValidator(fullType));
                input->enterPressed().connect(dialogAdd_, &Wt::WDialog::accept);
                if (inputName->size() == 0)
                {
                    input->setFocus();
                }
                
                input->setObjectName(fieldName);
                inputName->push_back(input);
            }
            else if (jsonType == ETypeJson::undid)
            {
                Wt::WComboBox *comboBox = popupAdd(dialogAdd_);
                comboBox->setObjectName(fieldName);
                
                inputName->push_back(comboBox);
            }
            else if (jsonType == ETypeJson::object)
            {
                Wt::WComboBox *comboBox = popupAdd(dialogAdd_);
                comboBox->setObjectName(fieldName);
                
                inputName->push_back(comboBox);
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

    dialogAdd_->finished().connect(bind([ = ] ()
    {
        popupCheck(inputName, errorMessage, dialogAdd_, -1);
        return;
    }));
    dialogAdd_->show();
}

void AbstractPage::modifResourcePopup(long long id)
{
    vector<Wt::WInteractWidget*>* inputName = new vector<Wt::WInteractWidget*>();
    vector<Wt::WText*> errorMessage;

    //gkr: Init dialog popup
    Wt::WDialog *dialogModif = new Wt::WDialog(tr("Alert." + m_xmlPageName + ".popup-modif"));
    dialogModif->setMinimumSize(Wt::WLength(300), Wt::WLength::Auto);
    
    for (map<long long, vector_widget>::iterator itTable = m_rowsTable.begin();
            itTable != m_rowsTable.end(); itTable++)
    {
        unsigned int cpt(0);
        if (itTable->first == id)
        {

            for (Wt::WInteractWidget *itElem : itTable->second)
            {
                std::string fieldName("input" + boost::lexical_cast<string>(cpt));
                std::vector<std::pair <int, string>>::iterator title;
                unsigned int correctTitlefinder = 0;
                for (title = m_displayedTitlesPopups.begin(); title != m_displayedTitlesPopups.end(); title++)
                {
                    if (cpt == correctTitlefinder++)
                    {
                        break;
                    }
                }
                if (((title->first >> 8) & 0xF) != ETypeJson::widget)
                {
                    new Wt::WText(tr("Alert." + m_xmlPageName + ".name-" + title->second)
                                  + " : <br />", dialogModif->contents());
                }

                int fullType = title->first;
                int jsonType = (((title->first >> 8) & 0xF) > 0 ? (title->first >> 8) & 0xF : 0);
                switch (jsonType)
                {
                    case ETypeJson::text:
                    {
                        string nameResource = ((Wt::WText*)(itElem))->text().toUTF8();
                        string newName = nameResource;
                        if (nameResource.size() > SIZE_NAME + SIZE_NAME)
                        {
                            newName.resize(newName.size() + 3, '.');
                        }
                        
                        Wt::WContainerWidget *inputCW = new Wt::WContainerWidget(dialogModif->contents());
                        inputCW->addStyleClass("control-group controls");
                        
                        Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName), inputCW);
                        
                        input->setValidator(AbstractPage::editValidator(fullType));
                        input->enterPressed().connect(dialogModif, &Wt::WDialog::accept);
                        if (inputName->size() == 0)
                        {
                            input->setFocus();
                        }
                        input->setToolTip(Wt::WString::fromUTF8(nameResource));
                        
                        input->setObjectName(fieldName);
                        inputName->push_back(inputCW);
                        Wt::WText *error2 = new Wt::WText(tr("Alert." + m_xmlPageName + ".invalid-name-"
                                                         + title->second), dialogModif->contents());
                        error2->hide();
                        errorMessage.push_back(error2);
                        new Wt::WText("<br />", dialogModif->contents());
                        break;
                    }
                case ETypeJson::number:
                {
                    string nameResource = ((Wt::WText*)(itElem))->text().toUTF8();
                    string newName = nameResource;
                    if (nameResource.size() > SIZE_NAME + SIZE_NAME)
                    {
                        newName.resize(newName.size() + 3, '.');
                    }
                    
                    Wt::WContainerWidget *inputCW = new Wt::WContainerWidget(dialogModif->contents());
                    inputCW->addStyleClass("control-group controls");
                    
                    Wt::WLineEdit *input = new Wt::WLineEdit(Wt::WString::fromUTF8(newName), inputCW);
                    input->setValidator(AbstractPage::editValidator(fullType));
                    input->enterPressed().connect(dialogModif, &Wt::WDialog::accept);
                    input->setWidth(Wt::WLength(150));
                    if (inputName->size() == 0)
                    {
                        input->setFocus();
                    }
                    input->setToolTip(Wt::WString::fromUTF8(nameResource));
                    
                    input->setObjectName(fieldName);
                    inputName->push_back(inputCW);
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
                            
                            checkBox->setObjectName(fieldName);
                            inputName->push_back(checkBox);
                            break;
                        }
                        cpt2++;
                    }
                    break;
                }
                case ETypeJson::object:
                {
                    Wt::WComboBox *comboBox = popupAdd(dialogModif);
                    unsigned int cpt2(0);
                    for (vector_widget::iterator widg = itTable->second.begin(); widg != itTable->second.end(); widg++)
                    {
                        Wt::WStandardItemModel *model = (Wt::WStandardItemModel*)comboBox->model();
                        if (cpt2 == cpt)
                        {
                            for (int nb(0); nb < model->rowCount(); nb++)
                            {
                                if (((Wt::WText*)(*widg))->text() == model->item(nb, 1)->text())
                                {
                                    comboBox->setCurrentIndex(nb);
                                }
                            }
                            inputName->push_back(comboBox);
                            break;
                        }
                        cpt2++;
                    }
                    if (comboBox->currentIndex() == -1)
                    {
                        comboBox->setCurrentIndex(0);
                    }
                    comboBox->setObjectName(fieldName);
                    break;
                }
                case ETypeJson::undid:
                {
                    Wt::WComboBox *comboBox = popupAdd(dialogModif);
                    unsigned int cpt2(0);
                    for (vector_widget::iterator widg = itTable->second.begin(); widg != itTable->second.end(); widg++)
                    {
                        Wt::WStandardItemModel *model = (Wt::WStandardItemModel*)comboBox->model();
                        if (cpt2 == cpt)
                        {
                            for (int nb(0); nb < model->rowCount(); nb++)
                            {
                                if (((Wt::WText*)(*widg))->text() == model->item(nb, 0)->text())
                                {
                                    comboBox->setCurrentIndex(nb);
                                    break;
                                }
                            }
                            inputName->push_back(comboBox);
                            break;
                        }
                        cpt2++;
                    }
                    if (comboBox->currentIndex() == -1)
                    {
                        comboBox->setCurrentIndex(0);
                    }
                    comboBox->setObjectName(fieldName);
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

    popupFinalization(dialogModif, id);

    dialogModif->finished().connect(bind([ = ] ()
    {
        popupCheck(inputName, errorMessage, dialogModif, id);
        return;
    }));
    dialogModif->show();
}

void AbstractPage::popupCheck(vector<Wt::WInteractWidget*>* inputName, vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id)
{
    int check(0);
    if (dialog->result() == Wt::WDialog::Rejected)
    {
        return;
    }
    else
    {
        //check = checkInput(inputName, errorMessage);
    }

    for (vector<Wt::WInteractWidget*>::iterator itInput = inputName->begin() ; itInput != inputName->end() ; itInput++)
    {
        if ((Wt::WContainerWidget*) dynamic_cast <Wt::WContainerWidget*> (*itInput) != NULL)
        {
            Wt::WLineEdit* lineEdit = (Wt::WLineEdit*)(((Wt::WContainerWidget*)(*itInput))->widget(0));

            if (lineEdit->validate() != Wt::WValidator::Valid)
            {
                dialog->show();
                return ;
            }
            
            *itInput = lineEdit;
        }
    }
    if (check == 0)
    {
        id > 0 ? modifResource(inputName, id) : addResource(inputName);
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
        std::string objectName("button-modif" + boost::lexical_cast<string> (rowTable));
        modifButton->setObjectName(objectName);
        addPopupModifHandler(modifButton, id);
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
        
        std::string objectName("del-button" + boost::lexical_cast<string> (rowTable));
        delButton->setObjectName(objectName);
        
        m_resourceTable->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
        m_resourceTable->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    }
}

void AbstractPage::addPopupModifHandler(Wt::WInteractWidget* widget, long long id)
{
    widget->clicked().connect(boost::bind(&AbstractPage::modifResourcePopup, this, id));
}

int AbstractPage::addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    return columnTable;
}

int AbstractPage::addCustomResourceTable(long long id, int rowTable, int columnTable)
{
    return columnTable;
}

void AbstractPage::saveButtonFooter(Wt::WDialog *dialog)
{
    Wt::WPushButton *saveButton = new Wt::WPushButton(tr("Alert." + m_xmlPageName + ".button-save"),
                                                          dialog->footer());
    if (getFooterOkButtonStatus())
    {
        saveButton->clicked().connect(dialog, &Wt::WDialog::accept);
        saveButton->setAttributeValue("style", "margin-left:12px");
        saveButton->setObjectName("save-button");
    }
    else
    {
        saveButton->disable();
    }    
}

void AbstractPage::cancelButtonFooter(Wt::WDialog *dialog)
{
    Wt::WPushButton *cancelButton = new Wt::WPushButton(tr("Alert." + m_xmlPageName + ".button-cancel"),
                                                              dialog->footer());
    cancelButton->clicked().connect(dialog, &Wt::WDialog::reject);
    cancelButton->setAttributeValue("style", "margin-left:12px;");
}

void AbstractPage::customButtonFooter(Wt::WDialog *dialog)
{
    
}

void AbstractPage::addButtonsToPopupFooter(Wt::WDialog *dialog)
{
    customButtonFooter(dialog);
    saveButtonFooter(dialog);
    cancelButtonFooter(dialog);
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

void AbstractPage::setUndidName(std::vector<string> undidNames)
{
    m_undidNames = undidNames;
}

void AbstractPage::setTitles(std::vector<std::pair <int, string>> titles)
{
    m_titles = titles;
    setDisplayedTitlesPopups();
}

void AbstractPage::setDisplayedTitlesPopups()
{
    m_displayedTitlesPopups = m_titles;
}

void AbstractPage::setUrl(list<list<pair<string, vector<string>>>> listsUrl)
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

void AbstractPage::sendHttpRequestGet(string resource, vector<string> listParameter, boost::function<void (Wt::Json::Value)> functor)
{    
    string apiAddress = getApiUrl() + "/" + resource
            + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8();
        
    for(size_t i(0); i<listParameter.size(); i++)
    {
        apiAddress += "&" + listParameter[i];
    }
    
    Wt::log("debug") << "[GET] address to call : " << apiAddress;
    Wt::Http::Client *client = new Wt::Http::Client();
    client->done().connect(boost::bind(&AbstractPage::handleHttpResponseGet, this, _1, _2, client, functor));
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void AbstractPage::handleHttpResponseGet(boost::system::error_code err,
    const Wt::Http::Message& response, Wt::Http::Client *client, boost::function<void (Wt::Json::Value)> functor)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                functor(result);
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
        else if (response.status() == 404)
        {
            functor(Wt::Json::Value::Null);
        }
    }
    else
    {
        Wt::log("warning") << "[" + tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client get error: "
                << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
}

void AbstractPage::recursiveGetResources(list<list<pair<string, vector<string>>>> listsUrl, boost::function<void (vectors_Json)> functorToCallAtEnd, vectors_Json jsonResource)
{
    if (listsUrl.size() == 0)
    {
        functorToCallAtEnd(jsonResource);
        return;
    }

    string resource = listsUrl.begin()->begin()->first;    
    vector<string> listParameter = listsUrl.begin()->begin()->second;
        
    bool containID = resource.find(":id") != string::npos;
    for (size_t i(0); i < listParameter.size(); i++)
    {
        containID |= listParameter[i].find(":id") != string::npos;
    }
    
    if (containID)
    {
        vector<long long> listID;
        if(jsonResource.back().back().isNull())
        {
            listID.push_back(0);
        }
        else
        {
            Wt::Json::Array& jsonArray = jsonResource.back().back();   
            for (size_t i(0); i < jsonArray.size(); i++)
            {
                Wt::Json::Object jsonObject = jsonArray.at(i);
                listID.push_back(jsonObject.get("id"));
            }            
        }
        
        listsUrl.begin()->pop_front();
        
        for (size_t i(0); i < listID.size(); i++)
        {
            long long id = listID[i];
            
            string newResource = resource;            
            if(newResource.find(":id") != string::npos)
            {
                newResource.replace(newResource.find(":id"), 3, boost::lexical_cast<string>(id));
            }
            
            vector<string> newListParameter;
            for (size_t j(0); j < listParameter.size(); j++)
            {
                string newParameter = listParameter[j];                
                if (newParameter.find(":id") != string::npos)
                {
                    newParameter.replace(newParameter.find(":id"), 3, boost::lexical_cast<string>(id));                    
                }               
                newListParameter.push_back(newParameter);                
            }
            
            listsUrl.begin()->push_back(pair<string, vector<string>>(newResource, newListParameter));
        }
    }
    
    resource = listsUrl.begin()->begin()->first;
    listParameter = listsUrl.begin()->begin()->second;
    
    boost::function<void (Wt::Json::Value)> functorHandleRecursiveGetResources = boost::bind(&AbstractPage::handleRecursiveGetResources, this, _1,
                                                                                             listsUrl, functorToCallAtEnd, jsonResource);    
    sendHttpRequestGet(resource, listParameter, functorHandleRecursiveGetResources);
}

void AbstractPage::handleRecursiveGetResources(Wt::Json::Value result, list<list<pair<string, vector<string>>>> listsUrl,
                                                    boost::function<void (vectors_Json)> functorToCallAtEnd, vectors_Json jsonResource)
{
    if (jsonResource.size() == 0)
    {
        jsonResource.push_back(vector<Wt::Json::Value>());
    }

    jsonResource.back().push_back(result);
    
    listsUrl.begin()->pop_front();
    if (listsUrl.begin()->size() == 0)
    {
        listsUrl.pop_front();
        jsonResource.push_back(vector<Wt::Json::Value>());
    }
           
    if (listsUrl.size() == 0)
    {
        functorToCallAtEnd(jsonResource);
    }
    else
    {
        recursiveGetResources(listsUrl, functorToCallAtEnd, jsonResource);
    }
}

void AbstractPage::handleJsonGet(vectors_Json jsonResources)
{
    m_rowsTable.clear();
    vector<Wt::Json::Value> jsonResource;
    try
    {
        if(jsonResources.size() > 0)
        {
            jsonResource = jsonResources.at(0);
            if (jsonResource.size() > 0 && !jsonResource.at(0).isNull())
            {
                bool selectedIDExist = false;
                Wt::Json::Array& jsonArray = jsonResource.at(0);
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonObject = jsonArray.at(cpt);
                    long long id = jsonObject.get("id");
                    selectedIDExist |= id == m_selectedID;                    
                    vector<Wt::WInteractWidget *> widgetsToAdd = initRowWidgets(jsonObject, jsonResource, cpt);
                    if(!widgetsToAdd.empty())
                    {
                        m_rowsTable.insert(make_pair(id, widgetsToAdd));
                    }
                }
                if(!selectedIDExist)
                    m_selectedID = 0;
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
        
        Wt::log("warning") << "[AbstractPage] JSON Type Exception" << boost::lexical_cast<string>(e.what());
        //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    fillTable();
}

vector<Wt::WInteractWidget *> AbstractPage::initRowWidgets(Wt::Json::Object jsonObject, vector<Wt::Json::Value> jsonResource, int cpt)
{
    vector<Wt::WInteractWidget *> rowWidgets;
    for (std::vector<std::pair <int, string>>::iterator itTitles = m_titles.begin();
            itTitles != m_titles.end(); itTitles++)
    {
        Wt::WInteractWidget *intWgt;
        int jsonType = ((itTitles->first >> 8) & 0xF);

        if (!jsonObject.contains(itTitles->second))
        {
            cout << itTitles->second << " not found" << endl;
            intWgt = createTextWidgetFromString("n/a"); 
        }
        else
        {
            cout << "JsonType = " << jsonType << endl;
            switch (jsonType)
            {
                case ETypeJson::text:
                {
                    intWgt = createTextWidgetFromString(jsonObject.get(itTitles->second));  
                    break;
                }
                case ETypeJson::boolean:
                {
                    intWgt = createCheckBoxWidgetFromBoolean(jsonObject.get(itTitles->second));
                    break;
                }
                case ETypeJson::number:
                {
                    intWgt = createTextWidgetFromInt(jsonObject.get(itTitles->second));
                    break;
                }
                case ETypeJson::undid:
                {
                    Wt::Json::Object jsonObjectParam = jsonResource.at(cpt + 1);
                    Wt::Json::Object nameObjet = jsonObjectParam.get(itTitles->second);
                    intWgt = createTextWidgetFromJsonObject(nameObjet);
                    break;
                }
                case ETypeJson::object:
                {
                    Wt::Json::Object subObject = jsonObject.get(itTitles->second);
                    intWgt = createTextWidgetFromJsonSubObject(subObject);
                    break;
                }
            }
        }
        //not necessary to add a widget when it's a widget json type
        if(jsonType != ETypeJson::widget)
        {
            rowWidgets.push_back(intWgt);
        }
    }
    return rowWidgets;
}

Wt::WInteractWidget * AbstractPage::createTextWidgetFromString(Wt::WString input)
{
    Wt::WText *text = new Wt::WText(input);
    text->setObjectName("text");
    return text;
}

Wt::WInteractWidget * AbstractPage::createTextWidgetFromInt(int input)
{
    Wt::WText *text = new Wt::WText(boost::lexical_cast<string>(input));
    return text;
}

Wt::WInteractWidget * AbstractPage::createCheckBoxWidgetFromBoolean(bool input)
{
    Wt::WCheckBox *checkBox = new Wt::WCheckBox();
    checkBox->setChecked(input);
    checkBox->setDisabled(true);
    return checkBox;
}

Wt::WInteractWidget * AbstractPage::createTextWidgetFromJsonObject(Wt::Json::Object input)
{
    string name;
    for (std::vector<string>::const_iterator itNames = m_undidNames.begin() ; itNames != m_undidNames.end() ; itNames++)
    {
        if (itNames != m_undidNames.begin())
        {
            name += " ";
        }

        std::string current = input.get(*itNames);
        name += current;
    }
    Wt::WText *text = new Wt::WText(name);
    return text;
}

Wt::WInteractWidget * AbstractPage::createTextWidgetFromJsonSubObject(Wt::Json::Object input)
{
    long long id = input.get("id");
    Wt::WText *text = new Wt::WText(boost::lexical_cast<string>(id));
    return text;
}


// ---- ADD MODIF DELETE ----------

void AbstractPage::addResource(vector<Wt::WInteractWidget*>* argument)
{
    // Post resource -------
    Wt::Http::Message *message = new Wt::Http::Message();

    setAddResourceMessage(message, argument);

    string apiAddress = getApiUrl() + "/" + m_listsUrl.begin()->begin()->first
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

void AbstractPage::setAddResourceMessage(Wt::Http::Message *message, vector<Wt::WInteractWidget*>* argument)
{
    message->addBodyText("{\n\t\"name\": \"" + ((Wt::WLineEdit*)(*argument->begin()))->text().toUTF8() + "\"\n}");
}

void AbstractPage::modifResource(vector<Wt::WInteractWidget*>* arguments, long long id)
{
    Wt::Http::Message *message = new Wt::Http::Message();

    setModifResourceMessage(message, arguments);

    string apiAddress = getApiUrl()
            + "/" + m_listsUrl.begin()->begin()->first
            + "/" + boost::lexical_cast<string> (id)
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

void AbstractPage::setModifResourceMessage(Wt::Http::Message *message, vector<Wt::WInteractWidget*>* argument)
{
    message->addBodyText("{\n\t\"name\": \"" + ((Wt::WLineEdit*)(*argument->begin()))->text().toUTF8() + "\"\n}");
}

Wt::WDialog *AbstractPage::deleteResource(long long id)
{
    Wt::WDialog *box =
            new Wt::WDialog(tr("Alert." + m_xmlPageName
                               + ".delete-" + m_xmlPageName));

    box->contents()->
            addWidget(new Wt::WText(tr("Alert." + m_xmlPageName + ".delete-message")));

    Wt::WPushButton *confirm =
            new Wt::WPushButton(tr("Alert."
                                   + m_xmlPageName + ".button-confirm"),
                                box->footer());
    confirm->clicked().connect(box, &Wt::WDialog::accept);
    confirm->setObjectName("button-confirm");
    confirm->setAttributeValue("style", "margin-left:12px");

    Wt::WPushButton *annul =
            new Wt::WPushButton(tr("Alert."
                                   + m_xmlPageName + ".button-cancel"),
                                box->footer());
    annul->clicked().connect(box, &Wt::WDialog::reject);
    annul->setAttributeValue("style", "margin-left:12px;");
    
    box->finished().connect(bind([ = ] (){
        if (box->result() == Wt::WDialog::Accepted)
        {
            Wt::Http::Message message;
            message.addBodyText("");
            string apiAddress = getApiUrl() + "/" + m_listsUrl.begin()->begin()->first + "/"
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
        if (response.status() == 201)
        {
            try
            {
                if(m_selectable)
                {
                    Wt::Json::Object result;
                    Wt::Json::parse(response.body(), result);
                    m_selectedID = result.get("id");
                }
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
                                           + m_xmlPageName) + " Widget] Http::Client status unexpected: " << response.body();
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
        if (response.status() >= 200 && response.status() < 300) // Revoir le retour
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

void AbstractPage::apiDeleteResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == Enums::EReturnCode::NO_CONTENT)
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
                                       + m_xmlPageName) + " Widget] Http::Client delete error: " << response.body();
        Wt::WMessageBox::show(tr("Alert." + m_xmlPageName + ".database-error-title"),
                              tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
    updatePage();
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

void AbstractPage::addEnumToModel(Wt::WStandardItemModel* standardItemModel, int enumToAdd, Wt::WString name, Wt::WString optionalParameter)
{
    Wt::WStandardItem *itemId = new Wt::WStandardItem();
    Wt::WStandardItem *itemName = new Wt::WStandardItem();
    Wt::WStandardItem *itemOptional = new Wt::WStandardItem();

    int id = enumToAdd;

    vector<Wt::WStandardItem*> rowVector;

    itemName->setText(name);
    rowVector.push_back(itemName);
    
    itemId->setText(boost::lexical_cast<string>(id));
    rowVector.push_back(itemId);
    
    if(optionalParameter != Wt::WString::Empty)
    {
        itemOptional->setText(optionalParameter);
        rowVector.push_back(itemOptional);
    }
    
    standardItemModel->appendRow(rowVector);
}

int AbstractPage::setValidatorType(int jsonType, int specialType, int mandatory)
{
    int type;
    
    type = jsonType;
    /* 4bits shift to the left: 0000 0000 0001 becomes 0000 0001 0000 */
    type = type << 4;
    type += specialType;
    /* another 4bits shift to the left: 0000 0001 0001 becomes 0001 0001 0000 */
    type = type << 4;
    /* add isMandatory == true: 0001 0001 0000 becomes 0001 0001 0001 */
    type += mandatory;
        
    return (type);
}

Wt::WValidator* AbstractPage::editValidator(int type)
{
    Wt::WRegExpValidator *validator = new Wt::WRegExpValidator();
    
    /* isolate first bit with 0x1 to see if true or false */
    bool isMandatory = (bool)(type & 0x1);
    /* (type >> 4) & 0xF: shift type 4bits to the right, isolate the 'new' first 4bits with & 0xF */
    int specialType = (((type >> 4) & 0xF) > 0 ? (type >> 4) & 0xF : 0);
    /* (type >> 8) & 0xF: shift type 8bits to the right, isolate the 'new' first 4bits with & 0xF */
    int jsonType = (((type >> 8) & 0xF) > 0 ? (type >> 8) & 0xF : 0);
        
    validator->setMandatory(isMandatory);
    switch (jsonType)
    {
        case ETypeJson::text:
        {
            switch (specialType)
            {
                case ETextSpecial::normalText:
                {
                    validator->setRegExp("^.*$");
                    break ;
                }
                case ETextSpecial::date:
                {
                    validator->setRegExp("^(((0[1-9]|[12][0-8])[\\/](0[1-9]|1[012]))|((29|30|31)[\\/](0[13578]|1[02]))|((29|30)[\\/](0[4,6,9]|11)))[\\/](19|[2-9][0-9])\\d\\d$)|(^29[\\/]02[\\/](19|[2-9][0-9])(00|04|08|12|16|20|24|28|32|36|40|44|48|52|56|60|64|68|72|76|80|84|88|92|96)$");
                    break ;
                }
                case ETextSpecial::mail:
                {
                    validator->setRegExp("^[^@]+@([^.]+.[a-z]+)+$");
                    break ;
                }
                case ETextSpecial::phone:
                {
                    validator->setRegExp("^(+[0-9]{2})|(06)|(07))([.-\\s:]?[0-9]{2}){4}$");
                    break ;
                }
                default:
                {
                    validator->setRegExp("^.*$");
                }
            }
            break ;
        }
        case ETypeJson::number:
        {
            switch (specialType)
            {
                case ENumberSpecial::normalNumber:
                {
                    validator->setRegExp("^[+-]?[0-9]*$");
                    break ;
                }
                case ENumberSpecial::notnull:
                {
                    validator->setRegExp("^[+-]?[1-9][0-9]*$");
                    break ;
                }
                case ENumberSpecial::uns:
                {
                    validator->setRegExp("^\\+?[0-9]*?$");
                    break ;
                }
                case ENumberSpecial::flt:
                {
                    validator->setRegExp("^[+-]?[1-9][0-9]*(\\.[0-9]+)?$");
                    break ;
                }
                default:
                {
                    validator->setRegExp("^.*$");
                }
            }
            break ;
        }
    }
    return (validator);
}
