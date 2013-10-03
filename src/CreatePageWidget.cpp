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

#include <Wt/WTabWidget>

#include "CreatePageWidget.h"

CreatePageWidget::CreatePageWidget(std::string namePage)
: Wt::WTemplateFormView(Wt::WString::tr("Alert." + namePage + ".Management.template"))
{
    this->addStyleClass("template");
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(namePage,false);
    nameResourcePage = namePage;
}

CreatePageWidget::~CreatePageWidget() { }

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
    headerButton->clicked().connect(boost::bind(&CreatePageWidget::popupForAdd, this));
    headerButton->setTextFormat(Wt::XHTMLUnsafeText);
    headerButton->setStyleClass("buttons btn");
    headerButton->setText("<span class='btn-pink'><i class='icon-plus '></i></span>");

    //gkr: Init dialog popup
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

    columnTable = 0;
    rowTable = 0;
    
    return resourceTable;
}

void    CreatePageWidget::addResourceInHeaderTable(std::vector<std::string> titleHeader, int nb)
{
    int columnHeadTable(0);
    titleHeader_ = titleHeader;
    std::vector<std::string>::iterator i = titleHeader_.end();
    //For have title, no button.
    for (int cpt = titleHeader_.size();
            cpt > nb && i != titleHeader_.begin();
            cpt--)
        titleHeader_.erase(i--);
    
    mediaTable_->setHeaderCount(1,Wt::Horizontal);
    for (std::vector<std::string>::const_iterator i = titleHeader.begin(); i != titleHeader.end(); i++)
    {
        new Wt::WText(tr("Alert." + nameResourcePage + "." + boost::lexical_cast<std::string>(*i)),
//        + "<span><i class='DataTables_sort_icon css_right ui-icon ui-icon-triangle-1-n'></i></span>",
                mediaTable_->elementAt(rowTable, columnHeadTable++));
    }
    new Wt::WText(tr("Alert." + nameResourcePage + ".modif-button"),
            mediaTable_->elementAt(rowTable, columnHeadTable++));
    new Wt::WText(tr("Alert." + nameResourcePage + ".delete-button"),
            mediaTable_->elementAt(rowTable, columnHeadTable++));
    rowTable++;
}

void    CreatePageWidget::addColumnInTable(map_Type myTable)
{
    map_Type::const_iterator                    i;
    std::vector<std::string>::const_iterator    vectorString;
    std::vector<Wt::WInteractWidget*>           linkButton;
    std::vector<std::string>                    resources;
    std::string                                 nameRessouce;
    std::string                                 newName;
    Wt::WLabel                                  *newColumn;
    long long                                   id;

    for (i = myTable.begin(); i != myTable.end(); i++)
    {
        for (vectorString = i->first.begin(); vectorString != i->first.end(); vectorString++)
        {
            nameRessouce = *vectorString;
            if (nameRessouce.compare(0, 3, "id:"))
            {
                resources.push_back(nameRessouce);
                newName = nameRessouce;
                if (nameRessouce.size() > SIZE_NAME_RESOURCE)
                {
                    newName.resize(SIZE_NAME_RESOURCE);
                    newName.resize(newName.size() + 3, '.');
                }
                newColumn = new Wt::WLabel(newName);  
                newColumn->setToolTip(nameRessouce);
                mediaTable_->elementAt(rowTable, columnTable++)->addWidget(newColumn);
            }
            else
            {
                id = atoll(nameRessouce.c_str() + 3);
            }
        }
        for (std::vector<Wt::WInteractWidget*>::const_iterator j = i->second.begin(); j != i->second.end(); j++)
        {
            mediaTable_->elementAt(rowTable, columnTable)->addWidget(*j);
            mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
            mediaTable_->elementAt(rowTable, columnTable++)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
        }
        addButtons(resources, id);
        resources.clear();
        rowTable++;
        columnTable = 0;
    }
}

void    CreatePageWidget::addButtons(std::vector<std::string> resources, long long id)
{
    Wt::WPushButton *modifButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    modifButton->setAttributeValue("class","btn btn-inverse");
    modifButton->setTextFormat(Wt::XHTMLUnsafeText);
    modifButton->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
    modifButton->clicked().connect(boost::bind(&CreatePageWidget::popupForModif, this, resources, id));
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(5, Wt::WLength::Percentage));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);

    columnTable++;

    Wt::WPushButton *delButton = new Wt::WPushButton(mediaTable_->elementAt(rowTable, columnTable));
    delButton->setAttributeValue("class","btn btn-danger");
    delButton->setTextFormat(Wt::XHTMLUnsafeText);
    delButton->setText("<span class='input-group-btn'><i class='icon-remove icon-white'></i></span>");
    delButton->clicked().connect(boost::bind(&CreatePageWidget::deleteResource, this, id));
    mediaTable_->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    mediaTable_->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
}


// POPUP :

void    CreatePageWidget::popupForAdd()
{
    std::vector<Wt::WInteractWidget*>   inputName;
    Wt::WLineEdit                       *input;
    std::string         check;
    Wt::WText *error;
    int cpt(0);
    
    //gkr: Init dialog popup
    Wt::WDialog *dialog_ = new Wt::WDialog(tr("Alert." + nameResourcePage + ".add-" + nameResourcePage));
    dialog_->setResizable(true);
    dialog_->setClosable(true);

    for (std::vector<std::string>::iterator i = titleHeader_.begin(); i != titleHeader_.end(); i++)
    {
        new Wt::WText(*i, dialog_->contents());
        input = new Wt::WLineEdit(dialog_->contents());
        input->setValidator(editValidator(cpt));
        input->enterPressed().connect(dialog_, &Wt::WDialog::accept);
        input->setFocus();
        inputName.push_back(input);
    }

    error = popupComplete(dialog_);
    
    dialog_->finished().connect(std::bind([=] () {
        int check = popupCheck(inputName, dialog_);
        if (check == 0)
            addResource(inputName);
        else if (check == 1)
        {
            error->show();
            dialog_->show();
        }
        return;
    }));
    dialog_->show(); 
}

void    CreatePageWidget::popupForModif(std::vector<std::string> resources, long long id) 
{
    std::vector<Wt::WInteractWidget*>   inputName;
    Wt::WDialog *dialog_ = new Wt::WDialog(tr("Alert." + nameResourcePage + ".modif-" + nameResourcePage));
    Wt::WText   *error;
    int cpt(0);
    std::vector<std::string>::iterator i = titleHeader_.begin();
    for (std::vector<std::string>::iterator vectorString = resources.begin(); vectorString != resources.end(); vectorString++)
    {
        new Wt::WText(*i, dialog_->contents());
        Wt::WLineEdit *input = new Wt::WLineEdit(*vectorString, dialog_->contents());
        input->setValidator(editValidator(cpt));
        input->enterPressed().connect(dialog_, &Wt::WDialog::accept);
        inputName.push_back(input);
        i++;
    }
    
    error = popupComplete(dialog_);

    dialog_->finished().connect(std::bind([=] () {
        int check = popupCheck(inputName, dialog_);
        if (check == 0)
            modifResource(inputName, id);
        else if (check == 1)
        {
            error->show();
            dialog_->show();
        }
        return;
    }));
    dialog_->show();
}

void    CreatePageWidget::popupAddTable(map_Type rst)
{
    popupTables.push_back(rst);
}

void    CreatePageWidget::popupAddTables(Wt::WDialog *dialog_)
{
    
    Wt::WTabWidget *menu = new Wt::WTabWidget(dialog_->contents());
    menu->addStyleClass("tabwidget");

    Wt::WTable *myTable;
    
    for (std::vector<map_Type>::iterator j = popupTables.begin(); j != popupTables.end(); j++)
    {
        int column(0);
        int row(0);
        map_Type vectorMap = *j;
        myTable = new Wt::WTable();
        myTable->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
        myTable->setHeaderCount(1,Wt::Horizontal);
        for (map_Type::iterator mapMap = vectorMap.begin(); mapMap != vectorMap.end(); mapMap++)
        {
            for (std::vector<std::string>::const_iterator headerString = mapMap->first.begin(); headerString != mapMap->first.end(); headerString++)
            {
                std::string title = *headerString;                    std::cout << "info dans title : " << title.data() << std::endl;
                    new Wt::WText(title.data(), myTable->elementAt(row++, column));
                    for (std::vector<Wt::WInteractWidget*>::const_iterator resTable = mapMap->second.begin(); resTable != mapMap->second.end(); resTable++)
                    {
                        myTable->elementAt(row++, column)->addWidget(*resTable);
                    }
                }
                row = 0;
                column++;
            }
        menu->addTab(myTable, "ma table x", Wt::WTabWidget::PreLoading);
    }
    dialog_->contents()->addWidget(menu);
    popupTables.clear();
     
}

int    CreatePageWidget::popupCheck(std::vector<Wt::WInteractWidget*> inputName, Wt::WDialog *dialog_)
{
    int check = 2;
    if (dialog_->result() == Wt::WDialog::Rejected)
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
                ((Wt::WLineEdit*)(*j))->setAttributeValue("style", "border-color: #FF0000; box-shadow: 0 0 10px #FF0000;");
                ((Wt::WLineEdit*)(*j))->setFocus();
                check = 1;
            }
            else
                ((Wt::WLineEdit*)(*j))->setAttributeValue("style", "");
        }
    }
    return check;
}

Wt::WText    *CreatePageWidget::popupComplete(Wt::WDialog *dialog_)
{
    Wt::WText *error =  new Wt::WText(tr("Alert." + nameResourcePage + ".invalid-name-info"), dialog_->contents());
    error->hide();

    popupAddTables(dialog_);
    
    Wt::WPushButton *ok = new Wt::WPushButton("Ok", dialog_->contents());
    ok->clicked().connect(dialog_, &Wt::WDialog::accept);    

    Wt::WPushButton *annul = new Wt::WPushButton("Annuler", dialog_->contents()); //xml
    annul->clicked().connect(dialog_, &Wt::WDialog::reject);
    
    return error;
}