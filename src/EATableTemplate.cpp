/* 
 * File:   EATableTemplate.cpp
 * Author: tsa
 * 
 * Created on 30 avril 2014, 17:11
 */

#include "EATableTemplate.h"

using namespace std;

EATableTemplate::EATableTemplate(const Wt::WString& tableName, Wt::WContainerWidget *parent)
: Wt::WTemplate(tableName, parent)
{
    addFunction("tr", &Functions::tr);
    
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(tableName.toUTF8(), false);
    
    
    
    m_tableName = tableName;
}

EATableTemplate::EATableTemplate(const EATableTemplate& orig)
{
}

EATableTemplate::~EATableTemplate()
{
}

void EATableTemplate::createTable()
{
    clear();
    bindWidget("resource-header", createTableHeader());
    bindWidget("resource-table", createTableBody());
    bindWidget("resource-footer", createTableFooter());
//    m_isCreated = true;
}

Wt::WContainerWidget *EATableTemplate::createTableHeader()
{
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget();
    headerTableContainer->addStyleClass("widget-title header-pers");
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
                  + Wt::WString::tr("Alert." + m_tableName.toUTF8() + ".add-button")
                  + "</h5>", headerTableContainer);

    
    headerTableContainer->addWidget(createDisplayedLinesComboBox());
    
    m_headerPlusButton->clicked().connect(boost::bind(&EATableTemplate::headerPlusButtonBehaviour, this));
    setStyleClass("button-add btn");
    m_headerPlusButton->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    return headerTableContainer;
}

void EATableTemplate::headerPlusButtonBehaviour(){}

Wt::WContainerWidget *EATableTemplate::createTableBody()
{

    Wt::WContainerWidget *resourceTableContainer = new Wt::WContainerWidget();

    resourceTableContainer->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");

    m_mainTable = new Wt::WTable(resourceTableContainer);
    m_mainTable->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    m_mainTable->setHeaderCount(1, Wt::Horizontal);
    addTableColumnHeader();

    fillTableBody();
    
    return resourceTableContainer;
}

Wt::WContainerWidget *EATableTemplate::createTableFooter()
{
//    m_butPaginate.clear();
//    m_butPaginateExt.clear();
        
    m_footer = new Wt::WContainerWidget();
    m_footer->addStyleClass("fg-toolbar ui-toolbar ui-widget-header ui-corner-bl ui-corner-br ui-helper-clearfix footer-pers");

    new Wt::WText(Wt::WString::tr("Alert." + m_tableName.toUTF8() + ".search-bar"), m_footer);

    m_searchWidget = new Wt::WLineEdit(m_footer);
    m_searchWidget->resize(Wt::WLength(100), Wt::WLength(15));
    m_searchWidget->enterPressed().connect(boost::bind(&EATableTemplate::searchInTable, this));

    Wt::WNavigationBar *navBar = new Wt::WNavigationBar(m_footer);
    navBar->addStyleClass("paginate-edit");
//    initPagination(navBar);

    return m_footer;
}

void EATableTemplate::searchInTable()
{
    
}

// Add Resource For Elements -----------------------------------------------------------------

void EATableTemplate::addTableColumnHeader()
{
    int columnTable(0);

    m_mainTable->elementAt(0, 0)->setAttributeValue("style", "border-left:0;");
    for (std::vector<std::pair <int, std::string>>::iterator it = m_titles.begin(); it != m_titles.end(); it++)
    {
        new Wt::WText(Wt::WString::tr("Alert." + m_tableName.toUTF8() + ".name-" + it->second),
                      m_mainTable->elementAt(0, columnTable++));
    }
    if (m_hasModifButton)
    {
        new Wt::WText(Wt::WString::tr("Alert." + m_tableName.toUTF8() + ".modif-button"),
                      m_mainTable->elementAt(0, columnTable++));
    }
    if (m_hasDeleteButton)
    {
        new Wt::WText(Wt::WString::tr("Alert." + m_tableName.toUTF8() + ".delete-button"),
                      m_mainTable->elementAt(0, columnTable));
    }
}

Wt::WComboBox *EATableTemplate::createDisplayedLinesComboBox()
{
    m_numberOfLinesToDisplay = 0;
    Wt::WComboBox *nbLineToBeDisplayedCombo = new Wt::WComboBox();
//    vector<string> linePerPage = {"5","10","20","40",tr("Alert.combo.all")};
    
    nbLineToBeDisplayedCombo->addItem("5");
    nbLineToBeDisplayedCombo->addItem("10");
    nbLineToBeDisplayedCombo->addItem("20");
    nbLineToBeDisplayedCombo->addItem("40");
    nbLineToBeDisplayedCombo->addItem(tr("Alert.combo.all"));
    nbLineToBeDisplayedCombo->setStyleClass("dataTables_length");
    nbLineToBeDisplayedCombo->setWidth(Wt::WLength(61));
    nbLineToBeDisplayedCombo->setMargin(50, Wt::Right);
    nbLineToBeDisplayedCombo->changed().connect(bind([ = ] (){
    if (nbLineToBeDisplayedCombo->currentText().toUTF8().compare(Wt::WString::tr("Alert.combo.all").toUTF8()) == 0)
    {
        m_numberOfLinesToDisplay = 0;
    }
    else
    {
        m_numberOfLinesToDisplay = boost::lexical_cast<int>(nbLineToBeDisplayedCombo->currentText());
    }
//    m_nbAffBegin = 1;
//    updatePage();
        }));
    return nbLineToBeDisplayedCombo;
}

void EATableTemplate::fillTableBody()
{
    
}

