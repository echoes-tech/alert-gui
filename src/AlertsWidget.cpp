/* 
 * Gui AlertsWidget.cpp
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 20/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * AlertsWidget
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include <Wt/WWidget>
#include <Wt/WDoubleSpinBox>

#include <boost/regex.hpp>
#include <bits/stl_map.h>
#include <bits/stl_vector.h>
#include <bits/basic_string.h>
#include "AlertsWidget.h"

using namespace std;

AlertsWidget::AlertsWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "alert")
{
    m_session = session;
    m_apiUrl = apiUrl;

    setButtonModif(false);
    setButtonSup(true);
    
    m_assets = new Wt::WStandardItemModel(0,3,this);
    m_plugins = new Wt::WStandardItemModel(0,3,this);
    m_informations = new Wt::WStandardItemModel(0,3,this);
    
    std::vector<std::pair <int, string>> listTitles;
    listTitles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    listTitles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, 0), "last_attempt"));
    listTitles.push_back(make_pair(setValidatorType(ETypeJson::number, 0, 0), "alert_media_specializations"));
    setTitles(listTitles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;  
    
    listUrl.push_back(pair<string, vector<string>>("alerts", vector<string>()));                     
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("assets", vector<string>()));        
    listUrl.push_back(pair<string, vector<string>>("assets/:id/plugins", vector<string>()));                
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("plugins", vector<string>()));        
    listUrl.push_back(pair<string, vector<string>>("plugins/:id/informations", vector<string>()));                
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("plugins", vector<string>()));        
    listUrl.push_back(pair<string, vector<string>>("plugins/:id/assets", vector<string>()));                
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("informations", vector<string>()));        
    listUrl.push_back(pair<string, vector<string>>("informations/:id/plugins", vector<string>()));                
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("informations", vector<string>()));        
    listUrl.push_back(pair<string, vector<string>>("informations/:id", vector<string>()));                
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("users", vector<string>()));                     
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("medias", vector<string>()));                     
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
    
    m_booleanOperators = new Wt::WStandardItemModel();
    addEnumToModel(m_booleanOperators, Echoes::Dbo::EBooleanOperator::AND, tr("Alert.alert.boolean-operator.and"));
    addEnumToModel(m_booleanOperators, Echoes::Dbo::EBooleanOperator::OR, tr("Alert.alert.boolean-operator.or"));
}

void AlertsWidget::setDisplayedTitlesPopups()
{
    std::vector<std::pair <int, std::string>> displayedTitles;
    displayedTitles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    m_displayedTitlesPopups = displayedTitles;
}

vector<string> AlertsWidget::getTitlesTableWidget()
{
    vector<string> titleWidget;
    titleWidget.empty();
    titleWidget.push_back("nb-dest");
    titleWidget.push_back("last-out");
    titleWidget.push_back("modif-dest");
    return titleWidget;
}

vector<string> AlertsWidget::getTitlesTableText()
{
    vector<string> titleText;
    titleText.empty();
    titleText.push_back("name");
    return titleText;
}

void AlertsWidget::saveTimeSlots(std::vector<struct TimeSlot> &timeSlots)
{
    struct TimeSlot newTimeSlot;

    std::string startHour(m_startHour->text().toUTF8());
    std::string duration(m_duration->text().toUTF8());
    startHour.erase(startHour.size() - 1);
    duration.erase(duration.size() - 1);
    
    newTimeSlot.start = boost::lexical_cast<int> (startHour);
    newTimeSlot.duration = boost::lexical_cast<int> (duration);
    if (m_everyDay->checkState() == Wt::Checked)
    {
        newTimeSlot.everyday = true;
        for (int itD = 0; itD < 7; ++itD)
        {
            newTimeSlot.days.push_back(std::make_pair(true, m_days.at(itD).second->text()));
        }
    }
    else
    {
        newTimeSlot.everyday = false;
        for (std::vector<std::pair < Wt::WCheckBox*, Wt::WText*>>::iterator itD = m_days.begin();
                itD != m_days.end(); ++itD)
        {
            newTimeSlot.days.push_back(std::make_pair(itD->first->isChecked(), itD->second->text()));
        }
    }

    if (m_everyMonth->checkState() == Wt::Checked)
    {
        newTimeSlot.everymonth = true;
        for (int itM = 0; itM < 12; ++itM)
        {
            newTimeSlot.months.push_back(std::make_pair(true, m_months.at(itM).second->text()));
        }
    }
    else
    {
        newTimeSlot.everymonth = false;
        for (std::vector<std::pair < Wt::WCheckBox*, Wt::WText*>>::iterator itM = m_months.begin();
                itM != m_months.end(); ++itM)
        {
            newTimeSlot.months.push_back(std::make_pair(itM->first->isChecked(), itM->second->text()));
        }
    }
    timeSlots.push_back(newTimeSlot);
}

void AlertsWidget::setSelectInteractions(int id)
{
    m_messageArea->keyWentUp().connect(bind( [ = ] ()
    {
        if (m_rowMedia == id)
        {
            m_messages.find(id)->second.str = new Wt::WString(m_messageArea->text());
        }
            
    }));
    
    m_timer->keyWentUp().connect(bind( [ = ]()
    {
        if (m_rowMedia == id)
        {
            if (m_timer->validate() == Wt::WValidator::Valid)
            {
                m_messages.find(id)->second.timer = boost::lexical_cast<long long>(m_timer->text().toUTF8());
            }
        }
    }));
}

void AlertsWidget::timeSlotsSummary(long long id)
{
    m_timeSlotsSummary->clear();
    m_timeSlotsSummary->elementAt(0, 0)->setAttributeValue("style", "border-left:0");
    m_timeSlotsSummary->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.start")));
    m_timeSlotsSummary->elementAt(0, 0)->resize(Wt::WLength(10, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.alert.form.duration")));
    m_timeSlotsSummary->elementAt(0, 1)->resize(Wt::WLength(10, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.alert.form.days")));
    m_timeSlotsSummary->elementAt(0, 2)->resize(Wt::WLength(30, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 3)->addWidget(new Wt::WText(tr("Alert.alert.form.months")));
    m_timeSlotsSummary->elementAt(0, 3)->resize(Wt::WLength(30, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 4)->addWidget(new Wt::WText(tr("Alert.alert.delete-button")));
    m_timeSlotsSummary->elementAt(0, 4)->resize(Wt::WLength(10, Wt::WLength::Percentage), 20);
    
    int i = 1;
    if (id  >= 0)
    {
        for (std::vector<struct TimeSlot>::iterator itTS = m_messages.at(id).timeSlots.begin(); itTS != m_messages.at(id).timeSlots.end(); ++itTS)
        {
            m_timeSlotsSummary->elementAt(i, 0)->addWidget(new Wt::WText(boost::lexical_cast<string>(itTS->start) + "h"));
            m_timeSlotsSummary->elementAt(i, 0)->setContentAlignment(Wt::AlignmentFlag::AlignCenter);
            m_timeSlotsSummary->elementAt(i, 1)->addWidget(new Wt::WText(boost::lexical_cast<string>(itTS->duration) + "h"));
            m_timeSlotsSummary->elementAt(i, 1)->setContentAlignment(Wt::AlignmentFlag::AlignCenter);
            Wt::WString daySummary("");
            if (itTS->everyday)
            {
                daySummary += tr("Alert.alert.form.all-days");
            }
            else
            {
                bool sep = false;
                for (std::vector < std::pair<bool, Wt::WString>>::iterator itD = itTS->days.begin(); itD != itTS->days.end(); ++itD)
                {
                    if (itD->first == true)
                    {
                        if (sep == true)
                        {
                            daySummary += ", ";
                        }
                        daySummary += itD->second;
                        sep = true;
                    }
                }
            }
            m_timeSlotsSummary->elementAt(i, 2)->addWidget(new Wt::WText(daySummary));
            m_timeSlotsSummary->elementAt(i, 2)->setMaximumSize(150, 20);

            Wt::WString monthSummary("");
            if (itTS->everymonth)
            {
                monthSummary += tr("Alert.alert.form.all-months");
            }
            else
            {
                bool sep = false;
                for (std::vector < std::pair<bool, Wt::WString>>::iterator itM = itTS->months.begin(); itM != itTS->months.end(); ++itM)
                {
                    if (itM->first == true)
                    {
                        if (sep == true)
                        {
                            monthSummary += ", ";
                        }
                        monthSummary += itM->second;
                        sep = true;
                    }
                }
            }
            Wt::WTemplate *removeButton = new Wt::WTemplate(Wt::WString::tr("Alert.alert.remove-button"));
            removeButton->clicked().connect(bind([ = ] (){
                                                 m_messages.at(id).timeSlots.erase(itTS);
                                                 timeSlotsSummary(id);
            }));
            m_timeSlotsSummary->elementAt(i, 3)->addWidget(new Wt::WText(monthSummary));
            m_timeSlotsSummary->elementAt(i, 3)->setMaximumSize(150, 20);
            m_timeSlotsSummary->elementAt(i, 4)->addWidget(removeButton);

            ++i;
        }
    }
}

void AlertsWidget::selectMedia(long long id, long long index, TrundleTable *trundleTable)
{
    if (trundleTable->getIndex() > 0)
    {
        m_messageTable->enable();
    }

    /* Set receiver/media reminder */
    if (m_nameFromReceiver.find(trundleTable->parentId) != m_nameFromReceiver.end()
            && m_nameFromMedia.find(id) != m_nameFromMedia.end())
    {
        m_messageReceiver->setText(m_nameFromReceiver.at(trundleTable->parentId));
        m_messageMedia->setText(m_nameFromMedia.at(id));
    }
    
    /* Creating message */
    m_rowMedia = id;

    std::map<long long, struct Message>::iterator itMsg = m_messages.find(id);
    if (itMsg == m_messages.end())
    {
        struct Message newMessage;
        newMessage.receiverId = trundleTable->parentId;
        newMessage.mediaId = id;
        newMessage.timer = 0;
        if (m_userAliases.find(trundleTable->parentId) != m_userAliases.end())
        {
            switch (m_typesFromMedia.at(id))
            {
                case Enums::EMedia::email:
                {
                    newMessage.str = m_userAliases.at(trundleTable->parentId).email;
                    break ;
                }
                case Enums::EMedia::sms:
                {
                    newMessage.str = m_userAliases.at(trundleTable->parentId).sms;
                    break ;
                }
                case Enums::EMedia::mobileapp:
                {
                    newMessage.str = m_userAliases.at(trundleTable->parentId).mobile;
                    break ;
                }
            }
        }
        else
        {
            newMessage.str = new Wt::WString("");
        }
        m_messages.insert(make_pair(id, newMessage));
    }
    m_messageArea->setText(*m_messages.find(id)->second.str);
    m_timer->setText(boost::lexical_cast<string>(m_messages.find(id)->second.timer));
    
    m_messageArea->keyWentUp().connect(bind( [ = ] ()
    {
        if (m_rowMedia == id)
        {
            m_messages.find(id)->second.str = new Wt::WString(m_messageArea->text());
        }
    }));
    
    m_timer->keyWentUp().connect(bind( [ = ]()
    {
        if (m_rowMedia == id)
        {
            if (m_timer->validate() == Wt::WValidator::Valid)
            {
                m_messages.find(id)->second.timer = boost::lexical_cast<long long>(m_timer->text().toUTF8());
            }
        }
    }));
    m_currentMedia = id;
    setSelectInteractions(id);
    timeSlotsSummary(id);
    m_addTimeSlot->enable();
}

void AlertsWidget::addMedia(long long id, long long index, TrundleTable *menu)
{
    std::string media = ((Wt::WComboBox*)menu->m_header.at(1))->currentText().toUTF8();
    Wt::WText *mediaName = new Wt::WText(media);
    mediaName->setWordWrap(false);
    std::vector<Wt::WWidget*> widgets;
    
    widgets.push_back(mediaName);
    
    /* Set buttons */
    std::map<int, bool> buttons;
    buttons.insert(make_pair(EInteractions::REMOVE, true));
    
    /* Set functors */
    std::map<int, boost::function<void (long long, long long, TrundleTable*)>> functorMapMediaRow;
    functorMapMediaRow.insert(make_pair(EInteractions::REMOVE, boost::bind(&AlertsWidget::deleteMedia, this, _1, _2, _3)));
    functorMapMediaRow.insert(make_pair(EInteractions::SELECT, boost::bind(&AlertsWidget::selectMedia, this, _1, _2, _3)));
    
    for (auto itM = m_nameFromMedia.begin(); itM != m_nameFromMedia.end(); itM++)
    {
        if (itM->second == media)
        {
            menu->addRow(itM->first, 1, widgets, buttons, functorMapMediaRow, NULL);
            break ;
        }
    }
    m_addTimeSlot->enable();
}

void AlertsWidget::deleteMedia(long long id, long long index, TrundleTable *menu)
{
   menu->deleteRow(index);
   for (std::map<long long, struct Message>::iterator itM = m_messages.begin() ; itM != m_messages.end() ; itM++)
   {
       if (itM->second.mediaId == id)
       {
           m_messages.erase(itM);
           break ;
       }
   }
   
   if (menu->getIndex() <= 0)
   {
        timeSlotsUnfocused();
        m_messageMedia->setText("");
        m_messageReceiver->setText("");
        m_messageArea->setText("");
        m_timer->setText("0");
        m_messageTable->disable();
        m_currentMedia = -1;
        m_addTimeSlot->disable();
   }
}

void AlertsWidget::addReceiver(long long id, long long index, TrundleTable *menu)
{
    long long currentIndex = ((Wt::WComboBox*)menu->m_header.at(1))->currentIndex();
    long long selectedId = m_userIds.at(currentIndex);
    
    if (menu->rowIdExist(selectedId) && selectedId != -1)
    {
        TrundleTable *subMenu = menu->getSubMenu(selectedId);
        if (subMenu)
        {
            if (subMenu->getIndex() == 0)
            {            
                m_messageTable->disable();
            }
        }
        return ;
    }
    else
    {
        m_messageTable->disable();
        timeSlotsUnfocused();
    }
    getAliases(m_userIds.at(currentIndex));
    
    /* Set ComboBox */
    Wt::WComboBox *boxMedias = new Wt::WComboBox();
    boxMedias->setMinimumSize(Wt::WLength(220), Wt::WLength(20));
    for (auto itM = m_nameFromMedia.begin(); itM != m_nameFromMedia.end(); itM++)
    {
        if (m_userIds.at(currentIndex) == m_userFromMedia.find(itM->first)->second)
        {
            boxMedias->addItem(itM->second);
        }
    }
    
    /* Set format */
    std::vector<Wt::WLength> rowLengths;
    rowLengths.push_back(Wt::WLength(50));
    rowLengths.push_back(Wt::WLength(200));
    rowLengths.push_back(Wt::WLength(50));
    
    /* Set widgets */
    std::vector<Wt::WWidget*> widgets;
    widgets.push_back(new Wt::WText(m_nameFromReceiver.find(m_userIds.at(currentIndex))->second));

    /* Set m_header */
    std::vector<Wt::WWidget*> headerMedia;
    headerMedia.push_back(new Wt::WText(tr("Alert.alert.form.media")));
    headerMedia.push_back(boxMedias);
    
    /* Set functors maps */
    std::map<int, boost::function<void (long long, long long, TrundleTable*)>> functorMapMediaHeader;
    functorMapMediaHeader.insert(make_pair(EInteractions::ADD, boost::bind(&AlertsWidget::addMedia, this, _1, _2, _3)));
    
    std::map<int, boost::function<void (long long, long long, TrundleTable*)>> functorMapReceiverRow;
    functorMapReceiverRow.insert(make_pair(EInteractions::REMOVE, boost::bind(&AlertsWidget::deleteReceiver, this, _1, _2, _3)));
    functorMapReceiverRow.insert(make_pair(EInteractions::SELECT, boost::bind(&AlertsWidget::selectReceiver, this, _1, _2, _3)));
    
    /* Creating menu */
    Wt::WTable *sepTable = new Wt::WTable(m_table->elementAt(0, 1));
    sepTable->resize(Wt::WLength(20), Wt::WLength(20));
    Wt::WTable *MediaTable = new Wt::WTable(m_table->elementAt(0, 2));
    MediaTable->resize(Wt::WLength(300), Wt::WLength(20));
    MediaTable->setMaximumSize(Wt::WLength(380), Wt::WLength(20));
    
    TrundleTable *menuMedia = new TrundleTable(MediaTable
            , rowLengths, Wt::WLength(20));

    /* Set buttons Receiver Row */
    std::map<int, bool> buttonsReceiverRow;
    buttonsReceiverRow.insert(make_pair(EInteractions::REMOVE, true));
    
    /* Set buttons Media*/
    std::map<int, bool> buttonsMediaHeader;
    buttonsMediaHeader.insert(make_pair(EInteractions::ADD, true));
    
    menuMedia->addRow(0, 0, headerMedia, buttonsMediaHeader, functorMapMediaHeader, NULL);
    menuMedia->parentId = m_userIds.at(currentIndex);
    
   menu->addRow(m_userIds.at(currentIndex), 1, widgets, buttonsReceiverRow, functorMapReceiverRow, menuMedia);
    
    if (menuMedia->getIndex() > 0)
    {
        m_messageTable->enable();
    }
}

void AlertsWidget::deleteReceiver(long long id, long long index, TrundleTable *menu)
{
   menu->deleteRow(index);
   if (menu->getIndex() == 0)
   {
       timeSlotsUnfocused();
        m_messageTable->disable();
   }
}

void AlertsWidget::selectReceiver(long long id, long long index, TrundleTable *menu)
{
    TrundleTable *subMenu = menu->getSubMenu(id);
    if (subMenu)
    {
        if (subMenu->getIndex() == 0)
        {
            timeSlotsUnfocused();
            m_messageTable->disable();
        }
    }
}

void AlertsWidget::initDateStructs()
{
    m_days.clear();
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.mon"))));
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.thu"))));
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.wed"))));
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.tue"))));
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.fri"))));
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.sat"))));
    m_days.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.sun"))));
    
    m_months.clear();
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.jan"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.feb"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.mar"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.apr"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.may"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.jun"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.jul"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.aug"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.sep"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.oct"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.nov"))));
    m_months.push_back(std::make_pair(new Wt::WCheckBox(), new Wt::WText(tr("Alert.alert.form.dec"))));
}

void AlertsWidget::reInitCheckboxesDates(std::vector<std::pair<Wt::WCheckBox*, Wt::WText*>> &datesCheckBox)
{
    for (std::vector<std::pair<Wt::WCheckBox*, Wt::WText*>>::iterator itD = datesCheckBox.begin() ; itD != datesCheckBox.end() ; ++itD)
    {
        itD->first->enable();
    }
}

void AlertsWidget::initTimeSlotsTables(Wt::WDialog *dialog)
{
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget(dialog->contents());
    headerTableContainer->addStyleClass("widget-title header-pers");
    
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
            + tr("Alert.alert.form.timeslot")
            + "</h5>", headerTableContainer);
    
    m_addTimeSlot = new Wt::WAnchor(headerTableContainer);
    
    m_addTimeSlot->setStyleClass("button-add btn");
    m_addTimeSlot->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    m_addTimeSlot->disable();
    
    Wt::WContainerWidget *timeSlotsSummaryContainer = new Wt::WContainerWidget(dialog->contents());
    
    timeSlotsSummaryContainer->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength());
    timeSlotsSummaryContainer->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");
    timeSlotsSummaryContainer->setPadding(Wt::WLength(10, Wt::WLength::Percentage), Wt::Left);
    m_timeSlotsSummary = new Wt::WTable(timeSlotsSummaryContainer);
    m_timeSlotsSummary->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    m_timeSlotsSummary->setHeaderCount(1, Wt::Horizontal);
    
    m_timeSlotsSummary->elementAt(0, 0)->setAttributeValue("style", "border-left:0");
    m_timeSlotsSummary->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.start")));
    m_timeSlotsSummary->elementAt(0, 0)->resize(Wt::WLength(10, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.alert.form.duration")));
    m_timeSlotsSummary->elementAt(0, 1)->resize(Wt::WLength(10, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.alert.form.days")));
    m_timeSlotsSummary->elementAt(0, 2)->resize(Wt::WLength(30, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 3)->addWidget(new Wt::WText(tr("Alert.alert.form.months")));
    m_timeSlotsSummary->elementAt(0, 3)->resize(Wt::WLength(30, Wt::WLength::Percentage), 20);
    m_timeSlotsSummary->elementAt(0, 4)->addWidget(new Wt::WText(tr("Alert.alert.delete-button")));
    m_timeSlotsSummary->elementAt(0, 4)->resize(Wt::WLength(10, Wt::WLength::Percentage), 20);
    timeSlotsSummaryContainer->setMargin(10, Wt::Bottom);
    
    m_timeSlotsTable = new Wt::WTable(dialog->contents());
    m_timeSlotsTable->hide();
    m_timeSlotsTable->resize(Wt::WLength(90, Wt::WLength::Percentage), Wt::WLength());
    
    m_addTimeSlot->clicked().connect(bind([=]()
    {
        if (m_currentMedia > 0 && m_timeSlotsTable->isHidden() == true)
        {
            m_timeSlotsTable->show();
        }
    }));
    
    m_timeSlotsTable->setMargin(15, Wt::Bottom);
    
    Wt::WTable *setTimeslotTimerTable = new Wt::WTable(m_timeSlotsTable->elementAt(0, 0));
    setTimeslotTimerTable->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength());
    setTimeslotTimerTable->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.start") + ":"));
    setTimeslotTimerTable->elementAt(0, 0)->resize(Wt::WLength(10, Wt::WLength::Percentage), 50);
    setTimeslotTimerTable->elementAt(0, 0)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    setTimeslotTimerTable->elementAt(0, 0)->setVerticalAlignment(Wt::AlignmentFlag::AlignMiddle);
    
        /* Set start slider */
    Wt::WContainerWidget *startHourWC = new Wt::WContainerWidget();
    startHourWC->addStyleClass("control-group controls");
    Wt::WDoubleSpinBox *startHourSpinBox = new Wt::WDoubleSpinBox(startHourWC);
    startHourSpinBox->setDecimals(0);
    startHourSpinBox->setValue(0);
    startHourSpinBox->setMinimum(0);
    startHourSpinBox->setMaximum(23);
    startHourSpinBox->resize(Wt::WLength(40), Wt::WLength());
    startHourSpinBox->setValidator(new Wt::WIntValidator(0, 23));
    m_startHour = new Wt::WText(startHourSpinBox->valueText() + "h");
    
    setTimeslotTimerTable->elementAt(0, 1)->addWidget(m_startHour);
    setTimeslotTimerTable->elementAt(0, 1)->setPadding(5, Wt::Left);
    setTimeslotTimerTable->elementAt(0, 1)->setVerticalAlignment(Wt::AlignmentFlag::AlignMiddle);
    setTimeslotTimerTable->elementAt(0, 1)->resize(Wt::WLength(10, Wt::WLength::Percentage), 50);
    setTimeslotTimerTable->elementAt(0, 2)->addWidget(startHourWC);
    setTimeslotTimerTable->elementAt(0, 2)->setVerticalAlignment(Wt::AlignmentFlag::AlignBottom);
    
    setTimeslotTimerTable->elementAt(0, 3)->addWidget(new Wt::WText(tr("Alert.alert.form.duration") + ":"));
    setTimeslotTimerTable->elementAt(0, 3)->resize(Wt::WLength(10, Wt::WLength::Percentage), 50);
    setTimeslotTimerTable->elementAt(0, 3)->setVerticalAlignment(Wt::AlignmentFlag::AlignMiddle);
    setTimeslotTimerTable->elementAt(0, 3)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    setTimeslotTimerTable->elementAt(0, 3)->setPadding(10, Wt::Left);
    
        /* set duration slider */
    Wt::WContainerWidget *durationWC = new Wt::WContainerWidget();
    durationWC->addStyleClass("control-group controls");
    Wt::WDoubleSpinBox *durationSpinBox = new Wt::WDoubleSpinBox(durationWC);
    durationSpinBox->setDecimals(0);
    durationSpinBox->setValue(1);
    durationSpinBox->setMinimum(1);
    durationSpinBox->setMaximum(24);
    durationSpinBox->resize(Wt::WLength(40), Wt::WLength());
    durationSpinBox->setValidator(new Wt::WIntValidator(1, 24));
    m_duration = new Wt::WText(durationSpinBox->valueText() + "h");
    
    setTimeslotTimerTable->elementAt(0, 4)->addWidget(m_duration);
    setTimeslotTimerTable->elementAt(0, 4)->resize(Wt::WLength(10, Wt::WLength::Percentage), 50);
    setTimeslotTimerTable->elementAt(0, 4)->setPadding(5, Wt::Left);
    setTimeslotTimerTable->elementAt(0, 4)->setVerticalAlignment(Wt::AlignmentFlag::AlignMiddle);
    setTimeslotTimerTable->elementAt(0, 4)->setPadding(20, Wt::Right);
    setTimeslotTimerTable->elementAt(0, 5)->addWidget(durationWC);
    setTimeslotTimerTable->elementAt(0, 5)->setVerticalAlignment(Wt::AlignmentFlag::AlignBottom);
    
    startHourSpinBox->valueChanged().connect(bind ([=]()
    {
        if (startHourSpinBox->validate() == Wt::WValidator::Valid)
        {
            m_startHour->setText(startHourSpinBox->valueText() + "h");
        }
    }));
    
    durationSpinBox->valueChanged().connect(bind ([=]()
    {
        if (durationSpinBox->validate() == Wt::WValidator::Valid)
        {
            m_duration->setText(durationSpinBox->valueText() + "h");
        }
    }));
    
    m_everyDay = new Wt::WCheckBox();
    
    Wt::WTable *setEveryDaysMonthsTable = new Wt::WTable(m_timeSlotsTable->elementAt(1, 0));
    setEveryDaysMonthsTable->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.all-days")));
    setEveryDaysMonthsTable->elementAt(0, 1)->addWidget(m_everyDay);
    setEveryDaysMonthsTable->elementAt(0, 1)->setPadding(30, Wt::Left);
    setEveryDaysMonthsTable->elementAt(0, 1)->setPadding(100, Wt::Right);

    m_everyMonth = new Wt::WCheckBox();
    setEveryDaysMonthsTable->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.alert.form.all-months")));
    setEveryDaysMonthsTable->elementAt(0, 2)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    setEveryDaysMonthsTable->elementAt(0, 3)->addWidget(m_everyMonth);
    setEveryDaysMonthsTable->elementAt(0, 3)->setPadding(30, Wt::Left);
    
    m_everyDay->setCheckState(Wt::Checked);
    m_everyMonth->setCheckState(Wt::Checked);
    
    Wt::WTable *setDaysMonthsTable = new Wt::WTable(m_timeSlotsTable->elementAt(1, 0));
    Wt::WTable *daysTable = new Wt::WTable(setDaysMonthsTable->elementAt(0, 0));
    setDaysMonthsTable->elementAt(0, 0)->resize(Wt::WLength(40, Wt::WLength::Percentage), Wt::WLength());
    
    int i = 0;
    for (std::vector<std::pair<Wt::WCheckBox*, Wt::WText*>>::iterator itD = m_days.begin() ; itD != m_days.end() ; ++itD)
    {
        itD->first->setCheckState(Wt::Checked);
        daysTable->elementAt(i, 0)->addWidget(itD->second);
        daysTable->elementAt(i, 0)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
        daysTable->elementAt(i, 0)->setPadding(30, Wt::Left);
        daysTable->elementAt(i, 1)->addWidget(itD->first);
        daysTable->elementAt(i, 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
        daysTable->elementAt(i, 1)->setPadding(40, Wt::Left);
        ++i;
    }
    daysTable->hide();
    
    Wt::WTable *monthsTable = new Wt::WTable(setDaysMonthsTable->elementAt(0, 1));
    setDaysMonthsTable->elementAt(0, 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    
    i = 0;
    int column = 0;
    for (std::vector<std::pair<Wt::WCheckBox*, Wt::WText*>>::iterator itM = m_months.begin() ; itM != m_months.end() ; ++itM)
    {
        if (i == 6)
        {
            column = 2;
            i = 0;
        }
        itM->first->setCheckState(Wt::Checked);
        monthsTable->elementAt(i, column)->addWidget(itM->second);
        monthsTable->elementAt(i, column)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
        monthsTable->elementAt(i, column)->setPadding((column == 2 ? 40 : 60), Wt::Left);
        monthsTable->elementAt(i, column + 1)->addWidget(itM->first);
        monthsTable->elementAt(i, column + 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
        monthsTable->elementAt(i, column + 1)->setPadding(30, Wt::Left);
        ++i;
    }
    monthsTable->hide();
    setDaysMonthsTable->show();
    
    m_timeSlotsTable->elementAt(2, 0)->setPadding(Wt::WLength(80, Wt::WLength::Percentage), Wt::Left);

    Wt::WPushButton *saveTimeSlot = new Wt::WPushButton(m_timeSlotsTable->elementAt(2, 0));
    saveTimeSlot->setText(tr("Alert.alert.form.add-timeslot"));
    saveTimeSlot->setStyleClass("btn-success");
    m_timeSlotsTable->elementAt(3, 0)->setContentAlignment(Wt::AlignmentFlag::AlignRight);
    
    m_everyDay->changed().connect(bind ([=]()
    {
        if (m_everyDay->checkState() == Wt::Checked)
        {
            reInitCheckboxesDates(m_days);
            daysTable->hide();
        }
        else
        {
            daysTable->show();
        }
    }));
    
    m_everyMonth->changed().connect(bind ([=]()
    {
        if (m_everyMonth->checkState() == Wt::Checked)
        {
            reInitCheckboxesDates(m_months);
            monthsTable->hide();
        }
        else
        {
            monthsTable->show();
        }
    }));
    
    saveTimeSlot->clicked().connect(bind([=]()
    {
        saveTimeSlots(m_messages.at(m_currentMedia).timeSlots);
        timeSlotsSummary(m_currentMedia);
        startHourSpinBox->setValue(0);
        durationSpinBox->setValue(1);
        m_startHour->setText("0h");
        m_duration->setText("1h");
        m_timeSlotsTable->hide();
    }));
}

void AlertsWidget::timeSlotsUnfocused()
{
    m_timeSlotsTable->hide();
    timeSlotsSummary(-1);
}

void AlertsWidget::popupRecipients(std::string nameAlert, std::string message)
{
    /* clear and initiate */
    for (std::map<long long, struct Message>::iterator itMsg = m_messages.begin() ; itMsg != m_messages.end() ; itMsg++)
    {
        m_messages.erase(itMsg);
    }    
    if (m_messageTable)
    {
        m_messageTable->clear();
    }
    initDateStructs();
    
    /* Set ComboBox */
    Wt::WComboBox *boxUsers = new Wt::WComboBox();
    boxUsers->setMinimumSize(Wt::WLength(150), Wt::WLength(20));
    for (std::map<long long, std::string>::iterator itU = m_nameFromReceiver.begin(); itU != m_nameFromReceiver.end(); itU++)
    {
        boxUsers->addItem(itU->second);
    }
    
    /* Set Table */
    Wt::WDialog *dialog = new Wt::WDialog();
    dialog->setClosable(true);
    AbstractPage::addButtonsToPopupFooter(dialog);
    dialog->resize(Wt::WLength(60, Wt::WLength::Percentage), Wt::WLength(90, Wt::WLength::Percentage));
    
    Wt::WTable *alertTable = new Wt::WTable(dialog->contents());
    alertTable->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.alert")));
    alertTable->elementAt(0, 1)->addWidget(new Wt::WText(nameAlert));
    alertTable->setMargin(15, Wt::Bottom);
    
    m_table = new Wt::WTable(dialog->contents());
    m_table->resize(Wt::WLength(90, Wt::WLength::Percentage), Wt::WLength(20));
    
    Wt::WTable *ReceiverTable = new Wt::WTable(m_table->elementAt(0, 0));
    ReceiverTable->resize(Wt::WLength(310), Wt::WLength(20));
    ReceiverTable->setMaximumSize(Wt::WLength(310), Wt::WLength(20));
    ReceiverTable->setMargin(15, Wt::Bottom);
    
    /* Message table */
    m_messageTable = new Wt::WTable(dialog->contents());
    
        /* set receiver | media name */
    m_messageReceiver = new Wt::WText("");
    m_messageTable->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.rec")));
    m_messageTable->elementAt(0, 0)->setContentAlignment(Wt::AlignmentFlag::AlignRight);
    m_messageTable->elementAt(0, 1)->addWidget(m_messageReceiver);
    m_messageTable->elementAt(0, 1)->resize(Wt::WLength(650), Wt::WLength::Auto);
    m_messageTable->elementAt(0, 1)->setColumnSpan(2);
    m_messageTable->elementAt(0, 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    
    m_messageMedia = new Wt::WText("");
    m_messageTable->elementAt(1, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.media")));
    m_messageTable->elementAt(1, 0)->setContentAlignment(Wt::AlignmentFlag::AlignRight);
    m_messageTable->elementAt(1, 1)->addWidget(m_messageMedia);
    m_messageTable->elementAt(1, 1)->resize(Wt::WLength(650), Wt::WLength::Auto);
    m_messageTable->elementAt(1, 1)->setColumnSpan(2);
    m_messageTable->elementAt(1, 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    
        /* set timer area */
    Wt::WTemplate *t = new Wt::WTemplate(tr("Alert.alert.time.template"));
    t->setMaximumSize(Wt::WLength(60), Wt::WLength::Auto);
    m_timer = new Wt::WLineEdit();
    m_timer->setValidator(AbstractPage::editValidator(setValidatorType(ETypeJson::number, ENumberSpecial::notnull, EMandatory::is)));
    t->bindWidget("time", m_timer);
    Wt::WText *timerUnit = new Wt::WText("minutes");

    m_messageTable->elementAt(3, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.snooze")));    
    m_messageTable->elementAt(3, 0)->setPadding(Wt::WLength(5), Wt::Top);
    m_messageTable->elementAt(3, 1)->addWidget(t);
    m_messageTable->elementAt(3, 1)->setPadding(Wt::WLength(5), Wt::Right);
    m_messageTable->elementAt(3, 1)->resize(Wt::WLength(60), Wt::WLength::Auto);
    m_messageTable->elementAt(3, 2)->addWidget(timerUnit);
    m_messageTable->elementAt(3, 2)->setPadding(Wt::WLength(5), Wt::Left);
    m_messageTable->elementAt(3, 2)->setPadding(Wt::WLength(5), Wt::Top);
    m_messageTable->elementAt(3, 2)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);

        /* set message area*/
    m_messageTable->elementAt(4, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.mess")));
    m_messageTable->elementAt(4, 0)->setContentAlignment(Wt::AlignmentFlag::AlignRight);
    m_messageTable->elementAt(4, 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    m_messageTable->elementAt(4, 1)->resize(Wt::WLength(650), Wt::WLength(150));
    m_messageTable->elementAt(4, 1)->setColumnSpan(2);
    m_messageArea = new Wt::WTextArea(m_messageTable->elementAt(4, 1));
    
    m_messageTable->disable();
    
    /* set time slots area */
    initTimeSlotsTables(dialog);
    
    /* set receiver rows trundleTable */
        /* Set format */
    std::vector<Wt::WLength> rowLengths;
    rowLengths.push_back(Wt::WLength(50));
    rowLengths.push_back(Wt::WLength(200));
    rowLengths.push_back(Wt::WLength(50));
    
        /* Set functors map */
    std::map<int, boost::function<void (long long, long long, TrundleTable*)>> functorMapReceiverHeader;
    functorMapReceiverHeader.insert(make_pair(EInteractions::ADD, boost::bind(&AlertsWidget::addReceiver, this, _1, _2, _3)));
    
        /* Set m_headers */
    std::vector<Wt::WWidget*> m_headerReceiver;
    
    m_headerReceiver.push_back(new Wt::WText(tr("Alert.alert.form.rec")));
    m_headerReceiver.push_back(boxUsers);
    
    
        /* Creating menu */    
    TrundleTable *menuReceiver = new TrundleTable(ReceiverTable
            , rowLengths, Wt::WLength(20));
    
        /* Set buttons templates */
    std::map<int, bool> buttons;
    buttons.insert(make_pair(0, true));
    
    menuReceiver->addRow(-1, 0, m_headerReceiver, buttons, functorMapReceiverHeader,  NULL);

    /* End of current popup */
    dialog->finished().connect(bind([ = ] ()
    {
        if (dialog->result() == Wt::WDialog::Rejected)
        {
            closePopup();
            return;
        }
        if (m_timer->validate() == Wt::WValidator::Valid)
        {
            checkNewPopupRecipientsRework(message);
            return;
        }
        dialog->show();
    }
    ));
    dialog->show();
}

void AlertsWidget::checkNewPopupRecipientsRework(string initialMessage)
{
    string message(initialMessage);
    message += "\"alert_media_specializations\":\n[\n";
    for (std::map<long long, struct Message>::const_iterator itMsg = m_messages.begin() ; itMsg != m_messages.end() ; itMsg++)
    {
        if (itMsg != m_messages.begin())
        {
            message += ",\n";
        }
        message += "{\n\"media_id\": " + boost::lexical_cast<string>(itMsg->second.mediaId);
        message += ",\n\"snooze\": " + boost::lexical_cast<string>((itMsg->second.timer * 60));
        message += ",\n\"time_slots\":\n[";
        for (std::vector<struct TimeSlot>::const_iterator itTS = itMsg->second.timeSlots.begin() ; itTS != itMsg->second.timeSlots.end() ; ++itTS)
        {
            if (itTS != itMsg->second.timeSlots.begin())
            {
                message += ",";
            }
            message += "\n{\n\"start\": " + boost::lexical_cast<string>(itTS->start);
            message += ",\n\"duration\": " + boost::lexical_cast<string>(itTS->duration);
            message += ",\n\"days\": \"";
            int i = 1;
            bool sep = false;
            for (std::vector<std::pair<bool, Wt::WString>>::const_iterator itD = itTS->days.begin() ; itD != itTS->days.end() ; ++itD)
            {
                if (itD->first == true)
                {
                    if (sep == true)
                    {
                        message += ",";
                    }
                    
                    /* (i % 7) because cron set sunday at 0 */
                    message += boost::lexical_cast<string>(itD->first * (i % 7));
                    sep = true;
                }
                ++i;
            }
            message += "\",\n\"months\": \"";
            i = 1;
            sep = false;
            for (std::vector<std::pair<bool, Wt::WString>>::const_iterator itM = itTS->months.begin() ; itM != itTS->months.end() ; ++itM)
            {
                if (itM->first == true)
                {
                    if (sep == true)
                    {
                        message += ",";
                    }
                    message += boost::lexical_cast<string>(itM->first * i);
                    sep = true;
                }
                ++i;
            }
            message +=  "\"\n}";
        }
        message +=  "\n]";
        message += ",\n\"message\": \"" + itMsg->second.str->toUTF8() + "\"\n}";
    }
    message += "\n]\n}";
    
    postAlertCallApi(message);
    updatePage();
}

/* artefact */
void AlertsWidget::fillInTabMessage()
{
    if (m_tabWidgetMessages->count() == 0)
    {
        Wt::WTextArea *textAreaMail = new Wt::WTextArea(m_tabContentMessageMail);
        Wt::WTextArea *textAreaSMS = new Wt::WTextArea(m_tabContentMessageSMS);
        Wt::WTextArea *textAreaMobileApp = new Wt::WTextArea(m_tabContentMessageMobileApp);
        
        m_tabWidgetMessages->addTab(textAreaMail, tr("Alert.alert.form.mail"), Wt::WTabWidget::PreLoading);
        m_tabWidgetMessages->addTab(textAreaSMS, tr("Alert.alert.form.sms"), Wt::WTabWidget::PreLoading);
        m_tabWidgetMessages->addTab(textAreaMobileApp, tr("Alert.alert.form.push"), Wt::WTabWidget::PreLoading);

        textAreaMail->changed().connect(bind([ = ] (){
                                      m_tabContentMessageMail = textAreaMail->text().toUTF8();
        }));
        textAreaSMS->changed().connect(bind([ = ] (){
                                      m_tabContentMessageSMS = textAreaSMS->text().toUTF8();
        }));
        textAreaMobileApp->changed().connect(bind([ = ] (){
                                      m_tabContentMessageMobileApp = textAreaMobileApp->text().toUTF8();
        }));
    }
    else
    {
        ((Wt::WTextArea*)m_tabWidgetMessages->widget(0))->setText(m_tabContentMessageMail);
        ((Wt::WTextArea*)m_tabWidgetMessages->widget(1))->setText(m_tabContentMessageSMS);
        ((Wt::WTextArea*)m_tabWidgetMessages->widget(2))->setText(m_tabContentMessageMobileApp);
    }
}

void AlertsWidget::setBox(Wt::WSelectionBox *box, Wt::WStandardItemModel *model)
{
    box->resize(Wt::WLength(150), Wt::WLength(150));
    box->setSelectionMode(Wt::ExtendedSelection);
    box->setSelectable(true);
    box->setModel(model);
    box->setModelColumn(1);
}

void AlertsWidget::assetSelected()
{
    int assetId = 0;
    const set<int> setSelectedIndexes = m_boxAsset->selectedIndexes();
    if (!setSelectedIndexes.empty())
    {
        // le faire pour select multiple également
        if (setSelectedIndexes.size() == 1)
        {
            assetId = getSelectedIdFromSelectionBox(m_boxAsset);
            
            int rowPlug = 0;
            m_plugins->clear();
            m_boxPlugin->clear();
            m_informations->clear();
            m_boxInfo->clear();
            set<long long> setPluginIds;
            for(unsigned int i = 0 ; i < m_mapAssetPlugins[assetId].size() ; i++)
            {
                setPluginIds.insert(m_mapAssetPlugins[assetId][i]);
                Wt::WStandardItem *id = new Wt::WStandardItem();
                m_plugins->setItem(rowPlug,0,id);
                m_plugins->item(rowPlug,0)->setText(boost::lexical_cast<string>(m_mapAssetPlugins[assetId][i]));
                Wt::WStandardItem *name = new Wt::WStandardItem();
                m_plugins->setItem(rowPlug,1,name);
                m_plugins->item(rowPlug,1)->setText(m_mapPluginsNames[m_mapAssetPlugins[assetId][i]]);
                rowPlug++;
            }
            
            int rowInfo = 0;
            set<long long>::iterator iteratorSetPluginsIds;
            for(iteratorSetPluginsIds = setPluginIds.begin() ; iteratorSetPluginsIds != setPluginIds.end() ; iteratorSetPluginsIds++)
            {
                for(unsigned int i = 0 ; i < m_mapPluginInfos[*iteratorSetPluginsIds].size() ; i++)
                {
                    Wt::WStandardItem *idInf = new Wt::WStandardItem();
                    m_informations->setItem(rowInfo,0,idInf);
                    m_informations->item(rowInfo,0)->setText(boost::lexical_cast<string>(m_mapPluginInfos[*iteratorSetPluginsIds][i]));
                    Wt::WStandardItem *nameInf = new Wt::WStandardItem();
                    m_informations->setItem(rowInfo,1,nameInf);
                    m_informations->item(rowInfo,1)->setText(m_mapInformationsNames[m_mapPluginInfos[*iteratorSetPluginsIds][i]]);
                    rowInfo++;
                }
            }
        }
        else
        {
            Wt::log("debug") << "[AlertsWidget][selectAsset] Multiple selection";
        }
        sortModels();
//        changeButtonAddCriteriaState();
    }
    else
    {
        Wt::log("debug") << "[AlertsWidget][selectAsset] Box empty";
    }
}

void AlertsWidget::pluginSelected()
{
    int pluginId = 0;
    const set<int> setSelectedIndexes = m_boxPlugin->selectedIndexes();
    if (!setSelectedIndexes.empty())
    {
        // le faire pour select multiple également
        if (setSelectedIndexes.size() == 1)
        {
            pluginId = getSelectedIdFromSelectionBox(m_boxPlugin);
            
            int rowAsset = 0;
            m_assets->clear();
            m_boxAsset->clear();
            m_informations->clear();
            m_boxInfo->clear();
            for(unsigned int i = 0 ; i < m_mapPluginAssets[pluginId].size() ; i++)
            {
                Wt::WStandardItem *id = new Wt::WStandardItem();
                m_assets->setItem(rowAsset,0,id);
                m_assets->item(rowAsset,0)->setText(boost::lexical_cast<string>(m_mapPluginAssets[pluginId][i]));
                Wt::WStandardItem *name = new Wt::WStandardItem();
                m_assets->setItem(rowAsset,1,name);
                m_assets->item(rowAsset,1)->setText(m_mapAssetsNames[m_mapPluginAssets[pluginId][i]]);
                rowAsset++;
            }
            
            int rowInfo = 0;
            for(unsigned int i = 0 ; i < m_mapPluginInfos[pluginId].size() ; i++)
            {
                Wt::WStandardItem *id = new Wt::WStandardItem();
                m_informations->setItem(rowInfo,0,id);
                m_informations->item(rowInfo,0)->setText(boost::lexical_cast<string>(m_mapPluginInfos[pluginId][i]));
                Wt::WStandardItem *name = new Wt::WStandardItem();
                m_informations->setItem(rowInfo,1,name);
                m_informations->item(rowInfo,1)->setText(m_mapInformationsNames[m_mapPluginInfos[pluginId][i]]);
                rowInfo++;
            }
        }
        else
        {
            Wt::log("debug") << "[AlertsWidget][selectPlugin] Multiple selection";
        }
        sortModels();
//        changeButtonAddCriteriaState();
    }
    else
    {
        Wt::log("debug") << "[AlertsWidget][selectPlugin] Box empty";
    }
}

void AlertsWidget::informationSelected()
{
    int infoId = 0;
    const set<int> setSelectedIndexes = m_boxInfo->selectedIndexes();
    if (!setSelectedIndexes.empty())
    {
        // le faire pour select multiple également
        if (setSelectedIndexes.size() == 1)
        {
            infoId = getSelectedIdFromSelectionBox(m_boxInfo);
            
            int rowPlug = 0;
            m_plugins->clear();
            m_boxPlugin->clear();
            m_assets->clear();
            m_boxAsset->clear();
            set<long long> setPluginIds;
            for(unsigned int i = 0 ; i < m_mapInfoPlugins[infoId].size() ; i++)
            {
                setPluginIds.insert(m_mapInfoPlugins[infoId][i]);
                Wt::WStandardItem *id = new Wt::WStandardItem();
                m_plugins->setItem(rowPlug,0,id);
                m_plugins->item(rowPlug,0)->setText(boost::lexical_cast<string>(m_mapInfoPlugins[infoId][i]));
                Wt::WStandardItem *name = new Wt::WStandardItem();
                m_plugins->setItem(rowPlug,1,name);
                m_plugins->item(rowPlug,1)->setText(m_mapPluginsNames[m_mapInfoPlugins[infoId][i]]);
                rowPlug++;
            }
            
            int rowAsset = 0;
            set<long long>::iterator iteratorSetPluginsIds;
            for(iteratorSetPluginsIds = setPluginIds.begin() ; iteratorSetPluginsIds != setPluginIds.end() ; iteratorSetPluginsIds++)
            {
                for(unsigned int i = 0 ; i < m_mapPluginAssets[*iteratorSetPluginsIds].size() ; i++)
                {
                    Wt::WStandardItem *idInf = new Wt::WStandardItem();
                    m_assets->setItem(rowAsset,0,idInf);
                    m_assets->item(rowAsset,0)->setText(boost::lexical_cast<string>(m_mapPluginAssets[*iteratorSetPluginsIds][i]));
                    Wt::WStandardItem *nameInf = new Wt::WStandardItem();
                    m_assets->setItem(rowAsset,1,nameInf);
                    m_assets->item(rowAsset,1)->setText(m_mapAssetsNames[m_mapPluginAssets[*iteratorSetPluginsIds][i]]);
                    rowAsset++;
                }
            }
        }
        else
        {
            Wt::log("debug") << "[AlertsWidget][selectInfo] Multiple selection";
        }
        sortModels();
//        changeButtonAddCriteriaState();
    }
    else
    {
        Wt::log("debug") << "[AlertsWidget][selectInfo] Box empty";
    }
    
    // ToDo
    // afficher les champs de paramétrages liés à l'unité de l'information
}

void AlertsWidget::fillModels()
{
    fillModel(m_assets, m_mapAssetsNames);
    fillModel(m_plugins, m_mapPluginsNames);
    fillModel(m_informations, m_mapInformationsNames);
}

void AlertsWidget::fillModel(Wt::WStandardItemModel * model, std::map<long long, std::string> m_mapNames)
{
    model->clear();
    std::map<long long, std::string>::iterator mapNamesIt;
    int row = 0;
    for (mapNamesIt = m_mapNames.begin() ; mapNamesIt != m_mapNames.end() ; ++mapNamesIt)
    {
        Wt::WStandardItem *id = new Wt::WStandardItem();
        model->setItem(row,0,id);
        model->item(row,0)->setText(boost::lexical_cast<string>(mapNamesIt->first));
        Wt::WStandardItem *name = new Wt::WStandardItem();
        model->setItem(row,1,name);
        model->item(row,1)->setText(mapNamesIt->second);
        row++;
    }
}

void AlertsWidget::sortModels()
{
    m_assets->sort(1);
    m_plugins->sort(1);
    m_informations->sort(1);
}

// ------- init popup one -------

void AlertsWidget::setDevicesSelection()
{
    fillModels();
    sortModels();
    
    m_boxAsset->clearSelection();
    m_boxPlugin->clearSelection();
    m_boxInfo->clearSelection();
    
    setBox(m_boxAsset,m_assets);
    m_boxAsset->changed().connect(this, &AlertsWidget::assetSelected);
    setBox(m_boxPlugin,m_plugins);
    m_boxPlugin->changed().connect(this, &AlertsWidget::pluginSelected);
    setBox(m_boxInfo,m_informations);
    m_boxInfo->changed().connect(this, &AlertsWidget::informationSelected);
}

void AlertsWidget::customButtonFooter(Wt::WDialog *dialog)
{
    if (m_alertCriteria.size() == 0)
    {
        Wt::WPushButton *ButtonSC = new Wt::WPushButton(tr("Alert.alert.button-save-continue"), dialog->footer());

        ButtonSC->clicked().connect(bind([ = ] ()
        {
            if (m_alertCriteria.size() > 0)
            {
                checkAll_ = 0;
                dialog->accept();
            }
        }));
    }
    else
    {
        AbstractPage::saveButtonFooter(dialog);
    }
}

void AlertsWidget::saveButtonFooter(Wt::WDialog *dialog)
{
}

void AlertsWidget::popupAddWidget(Wt::WDialog *dialog, long long id)
{
    m_alertCriteria.clear();
    checkAll_ = 1;
    dialog->resize(Wt::WLength(60, Wt::WLength::Percentage), Wt::WLength(90, Wt::WLength::Percentage));
    
    Wt::WContainerWidget *headerTableContainer = new Wt::WContainerWidget(dialog->contents());
    headerTableContainer->addStyleClass("widget-title header-pers");
    
    new Wt::WText("<span class='icon'><i class='icon-tasks'></i></span><h5>"
            + tr("Alert.alert.add-compare")
            + "</h5>", headerTableContainer);
    
    Wt::WAnchor *newCriteria = new Wt::WAnchor(headerTableContainer);
    
    newCriteria->setStyleClass("button-add btn");
    newCriteria->setText("<span class='btn-pink'><i class='icon-plus'></i></span>");
    
    Wt::WContainerWidget *criteriaSummaryContainer = new Wt::WContainerWidget(dialog->contents());
    
    criteriaSummaryContainer->resize(Wt::WLength(100, Wt::WLength::Percentage), Wt::WLength());
    criteriaSummaryContainer->addStyleClass("widget-content nopadding DataTables_Table_0_wrapper dataTables_wrapper body-pers");
    criteriaSummaryContainer->setPadding(Wt::WLength(10, Wt::WLength::Percentage), Wt::Left);
    
    m_criteriaSummaryTable = new Wt::WTable(criteriaSummaryContainer);
    updateCriteriaSummaryTable();
    criteriaSummaryContainer->setMargin(10, Wt::Bottom);
    
    
    Wt::WContainerWidget *addCrieriaWC = new Wt::WContainerWidget(dialog->contents());
    
    Wt::WTable *boxesTable = new Wt::WTable(addCrieriaWC);
    boxesTable->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-asset")));
    boxesTable->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-plugin")));
    boxesTable->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-info")));
    boxesTable->hide();
    
    m_boxAsset = new Wt::WSelectionBox(boxesTable->elementAt(1, 0));
    m_boxPlugin = new Wt::WSelectionBox(boxesTable->elementAt(1, 1));
    m_boxInfo = new Wt::WSelectionBox(boxesTable->elementAt(1, 2));
    
    setDevicesSelection();
    
    Wt::WContainerWidget *selectButtonWC = new Wt::WContainerWidget();
    boxesTable->elementAt(2, 0)->addWidget(selectButtonWC);
    boxesTable->elementAt(2, 0)->setColumnSpan(3);
    Wt::WPushButton *selectButton = new Wt::WPushButton(selectButtonWC);
    selectButtonWC->setContentAlignment(Wt::AlignmentFlag::AlignRight);
    selectButton->setText(tr("Alert.alert.add-button-add-critera"));
    selectButton->setStyleClass("btn-success");
    
    Wt::WTable *parametersTable = new Wt::WTable(addCrieriaWC);
    parametersTable->hide();
    
    newCriteria->clicked().connect(bind([ = ] ()
    {
        boxesTable->show();
    }));
    
    selectButton->clicked().connect(bind([ = ] ()
    {
        parametersTable->clear();
        fillParametersTable(parametersTable, boxesTable);
    }));
}

void AlertsWidget::fillParametersTable(Wt::WTable *parametersTable, Wt::WTable *boxesTable)
{
    long long assetID = getSelectedIdFromSelectionBox(m_boxAsset);
    long long pluginID = getSelectedIdFromSelectionBox(m_boxPlugin);
    long long infoID = getSelectedIdFromSelectionBox(m_boxInfo);
    Wt::WObject *valueWidget;
    Wt::WWidget *criteriaWidget;
    Wt::WComboBox *operatorCB;
    Wt::WLineEdit *flappingDuration;
    Wt::WComboBox *flappingUnit;
    Wt::WLineEdit *keyValueLineEdit;
    
    if (assetID > 0 && pluginID > 0 && infoID > 0)
    {
        int unitTypeID = m_mapInformationsUnitTypes[infoID];
        int column = 0;
        int row = 0;
        
        parametersTable->elementAt(row, column)->resize(Wt::WLength(120), Wt::WLength());
        parametersTable->elementAt(row, column + 1)->resize(Wt::WLength(120), Wt::WLength());
        
        // OPERATOR
        parametersTable->elementAt(row, column)->setPadding(Wt::WLength(10), Wt::Bottom);
        if (m_alertCriteria.size() != 0)
        {
            operatorCB = new Wt::WComboBox();
            operatorCB->insertItem(0, Wt::WString(tr("Alert.alert.boolean-operator.and")));
            operatorCB->insertItem(1, Wt::WString(tr("Alert.alert.boolean-operator.or")));
            
            parametersTable->elementAt(row, column)->addWidget(operatorCB);
        }

        // VALUE
        column = 0;
        parametersTable->elementAt(++row, column)->setContentAlignment(Wt::AlignRight);
        parametersTable->elementAt(row, column)->setPadding(Wt::WLength(10, Wt::WLength::Percentage), Wt::Left);
        parametersTable->elementAt(row, column)->setWidth(Wt::WLength(70));
        parametersTable->elementAt(row, column)->addWidget(new Wt::WText(tr("Alert.alert.message-value") + ":"));
        
        parametersTable->elementAt(row, ++column)->setContentAlignment(Wt::AlignRight);
        parametersTable->elementAt(row, column)->setWidth(Wt::WLength(150));
        if (unitTypeID == Enums::EInformationUnitType::text ||
                unitTypeID == Enums::EInformationUnitType::number)
        {
            Wt::WContainerWidget *lineEditWC = new Wt::WContainerWidget(parametersTable->elementAt(row, column));
            lineEditWC->addStyleClass("control-group controls");
            Wt:: WLineEdit *lineEditValue = new Wt::WLineEdit(lineEditWC);
            switch (unitTypeID)
            {
            case Enums::EInformationUnitType::number:
                lineEditValue->setValidator(editValidator(setValidatorType(ETypeJson::number, 0, EMandatory::is)));
                break;
            default:
                lineEditValue->setValidator(editValidator(setValidatorType(ETypeJson::text, 0, EMandatory::is)));
                break;
            }
            valueWidget = lineEditValue;
            valueWidget->setObjectName("lineEditValue");
        }
        else if (unitTypeID == Enums::EInformationUnitType::boolean)
        {
            Wt::WContainerWidget* containerTrueFalse = new Wt::WContainerWidget();

            Wt::WButtonGroup* groupTrueFalse = new Wt::WButtonGroup(containerTrueFalse);
            parametersTable->elementAt(row, column)->addWidget(containerTrueFalse);
            
            Wt::WRadioButton* buttonTrue = new Wt::WRadioButton("true", containerTrueFalse);
            buttonTrue->setChecked();
            groupTrueFalse->addButton(buttonTrue);

            Wt::WRadioButton* buttonFalse = new Wt::WRadioButton("false", containerTrueFalse);
            groupTrueFalse->addButton(buttonFalse);
            valueWidget = groupTrueFalse;
            valueWidget->setObjectName("groupTrueFalse");
        }
        else if (unitTypeID == Enums::EInformationUnitType::custom)
        {
            parametersTable->elementAt(row, column)->setWidth(Wt::WLength(70, Wt::WLength::Percentage));
            Wt::WContainerWidget *textAreaEditWC = new Wt::WContainerWidget(parametersTable->elementAt(row, column));
            textAreaEditWC->addStyleClass("control-group controls");
            Wt:: WTextArea *textAreaEdit = new Wt::WTextArea(textAreaEditWC);
            switch (unitTypeID)
            {
            case Enums::EInformationUnitType::number:
                textAreaEdit->setValidator(editValidator(setValidatorType(ETypeJson::number, 0, EMandatory::is)));
                break;
            default:
                textAreaEdit->setValidator(editValidator(setValidatorType(ETypeJson::text, 0, EMandatory::is)));
                break;
            }
            valueWidget = textAreaEdit;
            textAreaEditWC->resize(Wt::WLength(70, Wt::WLength::Percentage), Wt::WLength(20, Wt::WLength::Percentage));
            valueWidget->setObjectName("textAreaEdit");
        }
        else
        {
            valueWidget = NULL;
            Wt::log("error") << "Unknown unit type : " << unitTypeID;
        }
        
        if (unitTypeID != Enums::EInformationUnitType::custom)
        {
            // CRITERIA      
            parametersTable->elementAt(row, ++column)->setContentAlignment(Wt::AlignRight);
            parametersTable->elementAt(row, column)->setWidth(Wt::WLength(100));
            if (unitTypeID == Enums::EInformationUnitType::text ||
                    unitTypeID == Enums::EInformationUnitType::number)
            {
                Wt::WComboBox* comboBoxCriteria = createCompareCriteriaComboBox(unitTypeID);
                comboBoxCriteria->setCurrentIndex(0);
                parametersTable->elementAt(row, column)->addWidget(comboBoxCriteria);
                criteriaWidget = comboBoxCriteria;
            }
            else if (unitTypeID == Enums::EInformationUnitType::boolean)
            {
                // TO DO
                criteriaWidget = NULL;
            }
            else
            {
                Wt::log("error") << "Unknown unit type : " << unitTypeID;
            }

            /* Set flapping */
            column = 0;
            parametersTable->elementAt(++row, column)->setContentAlignment(Wt::AlignRight);
            parametersTable->elementAt(row, column)->setPadding(Wt::WLength(10, Wt::WLength::Percentage), Wt::Left);
            parametersTable->elementAt(row, column)->setWidth(Wt::WLength(70));
            parametersTable->elementAt(row, column)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-flapping") + ":"));

            Wt::WContainerWidget *flappingDurationWC = new Wt::WContainerWidget();
            flappingDuration = new Wt::WLineEdit(flappingDurationWC);
            flappingDuration->setValidator(new Wt::WIntValidator());
            flappingDurationWC->addStyleClass("control-group controls");

            flappingUnit = new Wt::WComboBox();
            flappingUnit->insertItem(0, Wt::WString(tr("Alert.alert.form.alert-unit-min")));
            flappingUnit->insertItem(1, Wt::WString(tr("Alert.alert.form.alert-unit-hour")));

            parametersTable->elementAt(row, ++column)->setWidth(Wt::WLength(150));
            parametersTable->elementAt(row, column)->setContentAlignment(Wt::AlignRight);
            parametersTable->elementAt(row, column)->addWidget(flappingDurationWC);

            parametersTable->elementAt(row, ++column)->setContentAlignment(Wt::AlignRight);
            parametersTable->elementAt(row, column)->setWidth(Wt::WLength(100));
            parametersTable->elementAt(row, column)->addWidget(flappingUnit);
            
            // KEY VALUE
            column = 0;
            parametersTable->elementAt(++row, column)->setContentAlignment(Wt::AlignRight);
            parametersTable->elementAt(row, column)->setPadding(Wt::WLength(10, Wt::WLength::Percentage), Wt::Left);
            parametersTable->elementAt(row, column)->setPadding(Wt::WLength(10), Wt::Right);
            parametersTable->elementAt(row, column)->setWidth(Wt::WLength(70));
            parametersTable->elementAt(row, column)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-key") + ":"));

            parametersTable->elementAt(row, ++column)->setPadding(Wt::WLength(10), Wt::Bottom);
            keyValueLineEdit = new Wt::WLineEdit(parametersTable->elementAt(row, column));
        }
        else
        {
            keyValueLineEdit = NULL;
            flappingDuration = NULL;
            flappingUnit = NULL;
            criteriaWidget = NULL;
        }
        
        Wt::WPushButton *saveCriteria = new Wt::WPushButton();
        saveCriteria->setText(tr("Alert.alert.button-save"));
        saveCriteria->setStyleClass("btn-success");
        parametersTable->elementAt(++row, 0)->setContentAlignment(Wt::AlignRight);
        parametersTable->elementAt(row, 0)->addWidget(saveCriteria);
        parametersTable->elementAt(row, 0)->setColumnSpan(5);
        
        saveCriteria->clicked().connect(bind([ = ] ()
        {
            
            bool valid = true;
            if (valueWidget->objectName().compare("lineEditValue") == 0)
            {
                if (((Wt::WLineEdit *)valueWidget)->validate() == Wt::WValidator::Invalid
                        || ((Wt::WLineEdit *)valueWidget)->validate() == Wt::WValidator::InvalidEmpty)
                {
                    return ;
                }
            }
            else if (flappingDuration && (flappingDuration->validate() == Wt::WValidator::Invalid
                        || flappingDuration->validate() == Wt::WValidator::InvalidEmpty))
            {
                valid = false;
            }
            
            if (valid == true)
            {
                parametersTable->hide();
                boxesTable->hide();

                Wt::WString value("");
                if (unitTypeID == Enums::EInformationUnitType::text ||
                unitTypeID == Enums::EInformationUnitType::number)
                {
                    value += ((Wt::WLineEdit*)valueWidget)->text().toUTF8();
                }
                else if (unitTypeID == Enums::EInformationUnitType::boolean)
                {
                    value += ((Wt::WButtonGroup*)valueWidget)->checkedButton()->text().toUTF8();
                }
                else if (unitTypeID == Enums::EInformationUnitType::custom)
                {
                    value += ((Wt::WTextArea*)valueWidget)->text().toUTF8();
                }

                long long criteriaID = 1;
                if (criteriaWidget != NULL)
                {
                    Wt::WComboBox *criterionComboBox = (Wt::WComboBox *)criteriaWidget;
                    string criteria_id = ((Wt::WStandardItemModel*)(criterionComboBox->model()))->item(criterionComboBox->currentIndex(),0)->text().toUTF8();
                    criteriaID = boost::lexical_cast<long long> (criteria_id);
                }
                
                int ope(0);
                if (m_alertCriteria.size() != 0)
                {
                    ope = operatorCB->currentIndex() + 1;
                }

                int flappingDurationInt = (flappingDuration && !flappingDuration->text().toUTF8().empty() ? boost::lexical_cast<int> (flappingDuration->text().toUTF8()) : 0);
                int flappingUnitInt = (flappingUnit ? flappingUnit->currentIndex() : -1);
                flappingDurationInt = (flappingUnitInt == ETimeUnit::HOUR ? flappingDurationInt * 60 : flappingDurationInt);

                struct AlertCriterion newAlertCriterion;

                newAlertCriterion.ope = ope;
                newAlertCriterion.value = value;
                newAlertCriterion.keyValue = (keyValueLineEdit ? keyValueLineEdit->text() : "");
                newAlertCriterion.infoID = infoID;
                newAlertCriterion.assetID = assetID;
                newAlertCriterion.pluginID = pluginID;
                newAlertCriterion.criteriaID = criteriaID;
                newAlertCriterion.unitTypeID = unitTypeID;
                newAlertCriterion.flappingDuration = flappingDurationInt;
                m_alertCriteria.push_back(newAlertCriterion);

                parametersTable->clear();
                updateCriteriaSummaryTable();
                setDevicesSelection();
            }
        }));
        
        parametersTable->show();
    }
}

void AlertsWidget::updateCriteriaSummaryTable()
{
    int row = 0;
    int column = 0;
    
    m_criteriaSummaryTable->clear();
    
    m_criteriaSummaryTable->addStyleClass("table table-bordered table-striped table-hover data-table dataTable");
    m_criteriaSummaryTable->setHeaderCount(1, Wt::Horizontal);
    
    m_criteriaSummaryTable->elementAt(row, 0)->setAttributeValue("style", "border-left:0");
    m_criteriaSummaryTable->elementAt(row, 0)->addWidget(new Wt::WText(tr("Alert.alert.add-title-operator")));
    m_criteriaSummaryTable->elementAt(row, 0)->resize(Wt::WLength(5, Wt::WLength::Percentage), 5);
    
    m_criteriaSummaryTable->elementAt(row, 1)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-asset")));
    m_criteriaSummaryTable->elementAt(row, 1)->resize(Wt::WLength(20, Wt::WLength::Percentage), 20);
    
    m_criteriaSummaryTable->elementAt(row, 2)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-plugin")));
    m_criteriaSummaryTable->elementAt(row, 2)->resize(Wt::WLength(20, Wt::WLength::Percentage), 20);
    
    m_criteriaSummaryTable->elementAt(row, 3)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-info")));
    m_criteriaSummaryTable->elementAt(row, 3)->resize(Wt::WLength(20, Wt::WLength::Percentage), 20);
    
    m_criteriaSummaryTable->elementAt(row, 4)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-key")));
    m_criteriaSummaryTable->elementAt(row, 4)->resize(Wt::WLength(10, Wt::WLength::Percentage), 10);
    
    m_criteriaSummaryTable->elementAt(row, 5)->addWidget(new Wt::WText(tr("Alert.alert.message-value")));
    m_criteriaSummaryTable->elementAt(row, 5)->resize(Wt::WLength(10, Wt::WLength::Percentage), 10);
    
    m_criteriaSummaryTable->elementAt(row, 6)->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-flapping")));
    m_criteriaSummaryTable->elementAt(row, 6)->resize(Wt::WLength(10, Wt::WLength::Percentage), 10);
    
    m_criteriaSummaryTable->elementAt(row, 7)->addWidget(new Wt::WText(tr("Alert.alert.delete-button")));
    m_criteriaSummaryTable->elementAt(row, 7)->resize(Wt::WLength(5, Wt::WLength::Percentage), 5);
    
    for (auto itA = m_alertCriteria.begin() ; itA != m_alertCriteria.end() ; ++itA)
    {
        ++row;
        column = 0;
        if (itA->ope != 0)
        {
            Wt::WText *operatorText = new Wt::WText((itA->ope == 1 ? tr("Alert.alert.boolean-operator.and") : tr("Alert.alert.boolean-operator.or")));
            m_criteriaSummaryTable->elementAt(row, column)->addWidget(operatorText);
        }
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(new Wt::WText(m_mapAssetsNames[itA->assetID]));
        
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(new Wt::WText(m_mapPluginsNames[itA->pluginID]));
        
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(new Wt::WText(m_mapInformationsNames[itA->infoID]));
        
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(new Wt::WText(itA->keyValue));
        
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(new Wt::WText(itA->value));
        
        Wt::WText *flapping = new Wt::WText(boost::lexical_cast<string>(itA->flappingDuration) + " min.");
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(flapping);


        Wt::WPushButton *buttonDelete = new Wt::WPushButton("<i class='icon-remove icon-white'></i>");
        buttonDelete->setStyleClass("btn-danger");
        buttonDelete->setTextFormat(Wt::XHTMLUnsafeText);
        m_criteriaSummaryTable->elementAt(row, ++column)->addWidget(buttonDelete);
        
        buttonDelete->clicked().connect(bind([ = ] ()
        {
            m_alertCriteria.erase(itA);
            updateCriteriaSummaryTable();
        }));
    }
}

// ------------------ Unit ------------------------------

void AlertsWidget::changeButtonAddCriteriaState()
{
    m_buttonAddCriteria->setEnabled(m_boxAsset->selectedIndexes().size() == 1 && m_boxPlugin->selectedIndexes().size() == 1 && m_boxInfo->selectedIndexes().size() == 1);
}

void AlertsWidget::createItemsCriteriaComboBox(long long id, Wt::WString criterion, Wt::WStandardItemModel *model)
{
    Wt::WStandardItem *itemIndex = new Wt::WStandardItem();
    Wt::WStandardItem *itemComparison = new Wt::WStandardItem();
    itemIndex->setText(boost::lexical_cast<string>(id));
    itemComparison->setText(criterion);
    vector<Wt::WStandardItem*> items;
    items.push_back(itemIndex);
    items.push_back(itemComparison);
    model->insertRow(0,items);
}

Wt::WComboBox *AlertsWidget::createCompareCriteriaComboBox(long long type)
{
    Wt::WComboBox *comboBox = new Wt::WComboBox();
    Wt::WStandardItemModel *model = new Wt::WStandardItemModel(1,2,this);
//    vector<Wt::WString> criterion;
    switch(type)
    {
    case Enums::EInformationUnitType::text:
    {
        //FIXME : create contains
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::EQ,tr("Alert.alert.operator.contains"),model);
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::EQ,tr("Alert.alert.operator.eq"),model);
        break;
    }
    case Enums::EInformationUnitType::number:
    {
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::LE,tr("Alert.alert.operator.le"),model);
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::LT,tr("Alert.alert.operator.lt"),model);
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::GE,tr("Alert.alert.operator.ge"),model);
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::GT,tr("Alert.alert.operator.gt"),model);
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::NE,tr("Alert.alert.operator.ne"),model);
        createItemsCriteriaComboBox(Echoes::Dbo::EAlertCriteria::EQ,tr("Alert.alert.operator.eq"),model);
        break;
    }
//    case Enums::EInformationUnitType::boolean:
//        break;
//    case Enums::EInformationUnitType::custom:
//        break;
    default:
        break;
    }
    comboBox->setModel(model);
    comboBox->setModelColumn(1);
    return comboBox;
}

//void AlertsWidget::createCompareWidgetCustom()
//{
//    Wt::WTable *table = new Wt::WTable(m_compareWidgetContainer);
//    Wt::WTextArea *textArea = new Wt::WTextArea(table->elementAt(0,0));
//    textArea->setColumns(80);
//    textArea->setRows(6);
//    textArea->setText(tr("Alert.alert.sec-base-message"));
//    m_customCompareWidget = textArea;
//}

//                                                                                    ^
//                                                                                    |
// Work popup One End --------------------------------------------------------------- |

void AlertsWidget::modifRecip(long long id)
{
}

/* TO DO */
int AlertsWidget::checkInput(vector<Wt::WInteractWidget*> inputName, vector<Wt::WText*> errorMessage)
{
    int checkAll = AbstractPage::checkInput(inputName, errorMessage);

    return checkAll;
}

void AlertsWidget::addResource(vector<Wt::WInteractWidget*>* argument)
{

    string message;

    string data = ((Wt::WLineEdit*) * argument->begin())->text().toUTF8();
//    boost::algorithm::to_lower(data);
    message += "{\n\"name\": \"" + data + "\",\n";
    message += "\"thread_sleep\": 0,\n";
    
    message += "\"alert_values\":\n[";
    for (auto it = m_alertCriteria.begin(); it != m_alertCriteria.end(); it++)
    {
        message += "\n{\n";
        switch (it->unitTypeID)
        {
            //same for text & number
            case Enums::EInformationUnitType::text:
            case Enums::EInformationUnitType::number:
                message += "\"alert_criterion_id\": " + boost::lexical_cast<string>(it->criteriaID) + ",\n";
                break;
            case Enums::EInformationUnitType::boolean:
            case Enums::EInformationUnitType::custom:
                message += "\"alert_criterion_id\": 1,\n";
                break ;
            default:
                Wt::log("error") << "Unknown unit type : " << it->unitTypeID;   
                break;
        }
        message += "\"value\": \"" + Wt::Utils::base64Encode(it->value.toUTF8()) + "\",\n";
        message += "\"key_value\": \"" + it->keyValue.toUTF8() + "\",\n";
        message += "\"flapping\": " + boost::lexical_cast<string>(it->flappingDuration) + ",\n";
        message += "\"operator\":" + boost::lexical_cast<string>(it->ope) + ",\n";
        message += "\"asset_id\": " + boost::lexical_cast<string>(it->assetID) + ",\n";
        message += "\"plugin_id\": " + boost::lexical_cast<string>(it->pluginID) + ",\n";
        message += "\"information_id\": " + boost::lexical_cast<string>(it->infoID);
        message += "\n}";
        if (it < m_alertCriteria.end() - 1)
        {
            message += ",";
        }
    }
    message += "\n],\n";

    if (checkAll_ == 1)
    {
        message += "\"alert_media_specializations\":\n[\n{\n\"media_id\": "
                + boost::lexical_cast<string>(1)
                + ",\n\"snooze\": 0,\n\"message\": \""
                + m_tabContentMessageMobileApp
                + "\"\n}\n]\n}";

        postAlertCallApi(message);
        updatePage();
    }
    else
    {
//        message += ",\n";
        Wt::log("debug") << message;
        popupRecipients(data, message);
    }
}

// Utility --------------------------------------------------------

void AlertsWidget::closePopup()
{
    updatePage();
}

long long AlertsWidget::getSelectedIdFromSelectionBox(Wt::WSelectionBox * box)
{
    const set<int> setSelectedIndexes = box->selectedIndexes();
    long long res = -1;
    if (!setSelectedIndexes.empty())
    {
        // le faire pour select multiple également
        if (setSelectedIndexes.size() == 1)
        {
            int indexSelected = -1;
            set<int>::iterator iteratorSelectedIndexes;
            for(iteratorSelectedIndexes = setSelectedIndexes.begin() ; iteratorSelectedIndexes != setSelectedIndexes.end() ; iteratorSelectedIndexes++)
            {
                indexSelected = *iteratorSelectedIndexes;
            }
            res = boost::lexical_cast<long long>(((Wt::WStandardItemModel*)box->model())->item(indexSelected,0)->text());
        }
    }
    return res;
}

long long AlertsWidget::getSelectedIdFromComboBox(Wt::WComboBox * box)
{

    long long res = -1;
    
    res = boost::lexical_cast<long long>(((Wt::WStandardItemModel*)box->model())->item(box->currentIndex(),0)->text());

    return res;
}

void AlertsWidget::close()
{
    delete this;
}

// API CALL AND RETURN --------------------------------------------------

/* ARTEFACT */
void AlertsWidget::getAliasListFromRecipient(long long userRoleId)
{
    getAliases(userRoleId);
}

void AlertsWidget::clearStructures()
{
    AbstractPage::clearStructures();
    m_alerts = Wt::Json::Value::Null;
    userInfo_.clear();
    mediaInfo_.clear();
    m_alertCriteria.clear();
    
    clearMessages();
}

void AlertsWidget::clearMessages()
{
    m_tabContentMessageMail.clear();
    m_tabContentMessageSMS.clear();
    m_tabContentMessageMobileApp.clear();
    
    for (std::vector<struct AlertCriterion>::iterator criteria = m_alertCriteria.begin(); criteria != m_alertCriteria.end() ; ++criteria)
    {
        for (std::map<long long, struct CriterionResponse>::iterator itCR = criteria->response.begin() ; itCR != criteria->response.end() ; ++itCR)
        {
            itCR->second.message = "";
            itCR->second.asset = "";
            itCR->second.plugin = "";
            itCR->second.criteria = "";
            itCR->second.information = "";
        }
    }
}

void AlertsWidget::postAlertCallApi(string message)
{
    Wt::Http::Message messageAlert;
    messageAlert.addBodyText(message);

    string apiAddress = this->getApiUrl() + "/alerts"
            + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
            + "&token=" + m_session->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AlertsWidget::postAlert, this, _1, _2));

    Wt::log("debug") << "AlertsWidget : [POST] address to call : " << apiAddress;
    Wt::log("debug") << "[POST] Message : " << messageAlert.body();

    if (client->post(apiAddress, messageAlert))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

}

/* TO DO */
void AlertsWidget::modifResource(vector<Wt::WInteractWidget*>* arguments, long long id)
{
    
}

void AlertsWidget::handleJsonGet(vectors_Json jsonResources)
{
    // asset plugins
    vector<Wt::Json::Value> jsonResource = jsonResources.at(1);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonAsset = jsonArray.at(cpt);
                    Wt::Json::Value jsonAssetPlugins = jsonResource.at(cpt + 1);
                    long long assetId = jsonAsset.get("id");
                    string assetName = jsonAsset.get("name");
                    m_mapAssetsNames[assetId] = assetName;
                    vector<long long> pluginsIds;
                    if (jsonAssetPlugins.type() == Wt::Json::ArrayType)
                    {
                        Wt::Json::Array jsonAssetPluginsArray = jsonAssetPlugins;
                        for (int cpt1(0); cpt1 < (int) jsonAssetPluginsArray.size(); cpt1++)
                        {
                            Wt::Json::Object jsonPlugin = jsonAssetPluginsArray.at(cpt1);
                            long long pluginId = jsonPlugin.get("id");
                            pluginsIds.push_back(pluginId);
                        }
                    }
                    m_mapAssetPlugins[assetId] = pluginsIds;
                }
            }
        }
            
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][AST_PLG] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][AST_PLG] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    
    // plugins // infos
    jsonResource = jsonResources.at(2);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonPlugin = jsonArray.at(cpt);
                    Wt::Json::Value jsonPluginInfos = jsonResource.at(cpt + 1);
                    long long pluginId = jsonPlugin.get("id");
                    string pluginName = jsonPlugin.get("name");
                    m_mapPluginsNames[pluginId] = pluginName;
                    vector<long long> infosIds;
                    if (jsonPluginInfos.type() == Wt::Json::ArrayType)
                    {
                        Wt::Json::Array jsonPluginInfosArray = jsonPluginInfos;
                        for (int cpt1(0); cpt1 < (int) jsonPluginInfosArray.size(); cpt1++)
                        {
                            Wt::Json::Object jsonInfo = jsonPluginInfosArray.at(cpt1);
                            long long infoId = jsonInfo.get("id");
                            bool infoDisplay = jsonInfo.get("display");
                            if (infoDisplay)
                            {
                                infosIds.push_back(infoId);
                            }
                        }
                    }
                    m_mapPluginInfos[pluginId] = infosIds;
                }
            }
        }
        
        
        map<long long, vector<long long>>::iterator assetPluginsIterator;
        for(assetPluginsIterator = m_mapAssetPlugins.begin() ; assetPluginsIterator != m_mapAssetPlugins.end() ; assetPluginsIterator++)
        {
            for (unsigned int i = 0 ; i < assetPluginsIterator->second.size() ; i++)
            {
                for (unsigned int j = 0 ; j < m_mapPluginInfos[assetPluginsIterator->second.at(i)].size() ; j++)
                {
                    m_mapAssetInfos[assetPluginsIterator->first].push_back(m_mapPluginInfos[assetPluginsIterator->second.at(i)][j]);
                }
            }
        }
        
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][PLG_INF] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][PLG_INF] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // plugins // assets
    jsonResource = jsonResources.at(3);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonPlugin = jsonArray.at(cpt);
                    Wt::Json::Value jsonPluginAssets = jsonResource.at(cpt + 1);
                    long long pluginId = jsonPlugin.get("id");
                    vector<long long> assetsIds;
                    if (jsonPluginAssets.type() == Wt::Json::ObjectType)
                    {
                        Wt::Json:: Object jsonAsset = jsonPluginAssets;
                        long long assetId = jsonAsset.get("id");
                        assetsIds.push_back(assetId);
                    }
                    m_mapPluginAssets[pluginId] = assetsIds;
                }
            }
        }
            
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][PLG_AST] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][PLG_AST] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // informations // plugins
    jsonResource = jsonResources.at(4);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonInformation = jsonArray.at(cpt);
                    Wt::Json::Value jsonInformationPlugins = jsonResource.at(cpt + 1);
                    long long infoId = jsonInformation.get("id");
                    string infoName = jsonInformation.get("name");
                    Wt::Json::Object jsonInfoUnit = jsonInformation.get("information_unit");
                    long long infoUnitId = jsonInfoUnit.get("id");
                    m_mapInformationsUnits[infoId] = infoUnitId;
                    bool infoDisplay = jsonInformation.get("display");
                    if (infoDisplay)
                    {
                        m_mapInformationsNames[infoId] = infoName;
                        vector<long long> pluginsIds;
                        if (jsonInformationPlugins.type() == Wt::Json::ArrayType)
                        {
                            Wt::Json::Array jsonInformationPluginsArray = jsonInformationPlugins;
                            for (int cpt1(0); cpt1 < (int) jsonInformationPluginsArray.size(); cpt1++)
                            {
                                Wt::Json::Object jsonPlugin = jsonInformationPluginsArray.at(cpt1);
                                long long pluginId = jsonPlugin.get("id");
                                pluginsIds.push_back(pluginId);
                            }
                        }
                        m_mapInfoPlugins[infoId] = pluginsIds;
                    }
                    
                }
            }
        }
        
        //info assets
        map<long long, vector<long long>>::iterator infoPluginsIterator;
        for(infoPluginsIterator = m_mapInfoPlugins.begin() ; infoPluginsIterator != m_mapInfoPlugins.end() ; infoPluginsIterator++)
        {
            for (unsigned int i = 0 ; i < infoPluginsIterator->second.size() ; i++)
            {
                for (unsigned int j = 0 ; j < m_mapPluginAssets[infoPluginsIterator->second.at(i)].size() ; j++)
                {
                    m_mapInfoAssets[infoPluginsIterator->first].push_back(m_mapPluginAssets[infoPluginsIterator->second.at(i)][j]);
                }
            }
        }
        
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][INFO_PLG] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][INFO_PLG] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // informations // information
    jsonResource = jsonResources.at(5);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonInformation = jsonArray.at(cpt);
                    Wt::Json::Object jsonInformationDetailed = jsonResource.at(cpt + 1);
                    long long infoId = jsonInformation.get("id");
                    Wt::Json::Object jsonInfoUnit = jsonInformationDetailed.get("information_unit");
                    Wt::Json::Object jsonInfoUnitType = jsonInfoUnit.get("information_unit_type");
                    long long infoUnitTypeId = jsonInfoUnitType.get("id");
                    m_mapInformationsUnitTypes[infoId] = infoUnitTypeId;
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][INFO_PLG] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][INFO_PLG] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
          
    //users
    jsonResource = jsonResources.at(6);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonUser = jsonArray.at(cpt);
                    long long userId = jsonUser.get("id");
                    string userFirstName = jsonUser.get("first_name");
                    string userLastName = jsonUser.get("last_name");
                    
                    /* new maps and vectors */
                    m_nameFromReceiver.insert(std::make_pair(userId, userFirstName + " " + userLastName));
                    m_userIds.push_back(userId);
                    
                    userInfo_.insert(std::make_pair(cpt, std::make_pair(userId, userFirstName + " " + userLastName)));
                }
            }
        }
            
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][Users] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][Users] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    //medias
    jsonResource = jsonResources.at(7);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonMedia = jsonArray.at(cpt);
                    long long mediaId = jsonMedia.get("id");
                    Wt::Json::Object mediaType = jsonMedia.get("media_type");
                    long long mediaTypeId = mediaType.get("id");
                    string mediaValue = jsonMedia.get("value");
                    Wt::Json::Object mediaUser = jsonMedia.get("user");
                    long long userId = mediaUser.get("id");
                    
                    m_mediaUserRelation.insert(std::make_pair(cpt, userId));
                    /* new maps and vectors */
                    m_userFromMedia.insert(std::make_pair(mediaId, userId));
                    m_typesFromMedia.insert(std::make_pair(mediaId, mediaTypeId));
                    m_nameFromMedia.insert(std::make_pair(mediaId, mediaValue));
                    m_mediasFromUser.insert(std::make_pair(userId, mediaId));
                    m_mediaIds.push_back(mediaId);
                                        
                    mediaInfo_.insert(std::make_pair(cpt, std::make_pair(std::make_pair(mediaId, mediaTypeId), mediaValue)));
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Alerts][Users] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Alerts][Users] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
                
    
    AbstractPage::handleJsonGet(jsonResources);
}

// API CALLS ------------------------------------------

void AlertsWidget::postAlert(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 201)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alerts Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alerts Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        /* FIX ME : else log */
    }
    else
    {
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
}

Wt::WRegExpValidator *AlertsWidget::validateCriterionType(long long unitType)
{
    Wt::WRegExpValidator *validator = new Wt::WRegExpValidator();
    validator->setMandatory(true);
    validator->javaScriptValidate();

    switch (unitType)
    {
        case Enums::EInformationUnitType::text:
            validator->setRegExp(".*");
            break;
        case Enums::EInformationUnitType::number:
            validator->setRegExp("^[+-]?[0-9]+(\\.[0-9]+)?$");
            break;
        default:
            validator->setRegExp(".*");
            break ;
    }

    return validator;
}

void AlertsWidget::getAliases(long long userRoleId)
{
    clearMessages();
    
    for (unsigned long criteria = 0 ; criteria != m_alertCriteria.size() ; ++criteria)
    {
        for (long long mediaType(Enums::EMedia::email); mediaType <= Enums::EMedia::mobileapp; mediaType++)
        {
            for (long long requestType(ERequestType::RTASSET); requestType <= ERequestType::RTCRITERIA; requestType++)
            {
                httpAsk(userRoleId, mediaType, requestType, criteria);
            }
        }
    }
}

void AlertsWidget::httpAsk(long long userRoleId, long long mediaType, long long requestType, long long criteria)
{
    string requestTypeString;
    string requestTypeId;
    string requestNeedInformation = "";
    
    switch (requestType)
    {
        case ERequestType::RTASSET:
        {
            requestTypeString = "assets/";
            requestTypeId = boost::lexical_cast<string>(m_alertCriteria[criteria].assetID);
            break;
        }
        case ERequestType::RTPLUGIN:
        {
            requestTypeString = "plugins/";
            requestTypeId = boost::lexical_cast<string>(m_alertCriteria[criteria].pluginID);
            break;
        }
        case ERequestType::RTINFORMATION:
        {
            requestTypeString = "informations/";
            requestTypeId = boost::lexical_cast<string>(m_alertCriteria[criteria].infoID);
            break;
        }
        case ERequestType::RTCRITERIA:
        {
            requestTypeString = "criteria/";
            requestTypeId = boost::lexical_cast<string>(m_alertCriteria[criteria].criteriaID);
            requestNeedInformation = "information_id=" + boost::lexical_cast<string>(m_alertCriteria[criteria].infoID);
            break;
        }
    }
    
    string ressource = requestTypeString + requestTypeId + "/alias";
    std::vector<std::string> listParameter;
    listParameter.push_back("user_role_id=" + boost::lexical_cast<string>(userRoleId));
    listParameter.push_back("media_type_id=" + boost::lexical_cast<string>(mediaType));
    if (!requestNeedInformation.empty())
    {
        listParameter.push_back("information_id=" + boost::lexical_cast<string>(m_alertCriteria[criteria].infoID));
    }
    
    boost::function<void (Wt::Json::Value)> functorHandleResponse = boost::bind(&AlertsWidget::handleHttpResponse, this, _1, mediaType, requestType, criteria, userRoleId);
    sendHttpRequestGet(ressource, listParameter, functorHandleResponse);
}

void AlertsWidget::getRequestRsp(long long requestType, Wt::WString message, CriterionResponse &response)
{
    switch (requestType)
    {
        case ERequestType::RTASSET:
            response.asset = message;
            break;
        case ERequestType::RTCRITERIA:
            response.criteria = message;
            break;
        case ERequestType::RTPLUGIN:
            response.plugin = message;
            break;
        case ERequestType::RTINFORMATION:
            response.information = message;
            break;
    }
    response.message = response.asset + " "
            + response.criteria + " "
            + response.plugin + " "
            + response.information;
}

void AlertsWidget::updateMessage(std::string &tabContent, Wt::WString &message, unsigned long criteria)
{
    if (criteria > 0 && !tabContent.empty())
    {
        Wt::WString ope = (m_alertCriteria[criteria].ope == 1 ? tr("Alert.alert.boolean-operator.and") : tr("Alert.alert.boolean-operator.or"));
        tabContent += ope.toUTF8() + std::string(" ");
    }
    
    std::string result = message.toUTF8();
    boost::replace_all(result, "%value%",  "%value" + boost::lexical_cast<string> (criteria) + "%");
    boost::replace_all(result, "%threshold%",  "%threshold" + boost::lexical_cast<string> (criteria) + "%");
    boost::replace_all(result, "%detection-time%",  "%detection-time" + boost::lexical_cast<string> (criteria) + "%");
    boost::replace_all(result, "%alerting-time%",  "%alerting-time" + boost::lexical_cast<string> (criteria) + "%");
    boost::replace_all(result, "%unit%",  "%unit" + boost::lexical_cast<string> (criteria) + "%");

    tabContent += result;
    if (!tabContent.empty())
    {
        tabContent += "\n";
    }
}

void AlertsWidget::updateTabContent(Wt::WString message, long long mediaType, long long requestType, long long criteria)
{
    if (m_alertCriteria[criteria].response.find(mediaType) == m_alertCriteria[criteria].response.end())
    {
        struct CriterionResponse response;
        m_alertCriteria[criteria].response.insert(std::make_pair(mediaType, response));
    }
    getRequestRsp(requestType, message, m_alertCriteria[criteria].response.at(mediaType));
     
    std::cout << "== updateTabContent ==" << std::endl;
    // TO DO - adapt for a vector of medias instead of this m_tabContent...
    switch (mediaType)
    {
        case Enums::EMedia::email:
        {
            std::cout << ":: email ::" << std::endl;
            m_tabContentMessageMail.clear();
            
            std::cout << "criteria :" << boost::lexical_cast<string> (criteria) << std::endl;
            std::cout << "m_alertCriteria size :" << boost::lexical_cast<string> (m_alertCriteria.size()) << std::endl;
            updateMessage(m_tabContentMessageMail, m_alertCriteria[criteria].response.at(mediaType).message, criteria);
            break;
        }
        case Enums::EMedia::sms:
        {
            m_tabContentMessageSMS.clear();
            updateMessage(m_tabContentMessageSMS, m_alertCriteria[criteria].response.at(mediaType).message, criteria);

            break;
        }
        case Enums::EMedia::mobileapp:
        {
            m_tabContentMessageMobileApp.clear();
            updateMessage(m_tabContentMessageMobileApp, m_alertCriteria[criteria].response.at(mediaType).message, criteria);
            break;
        }
    }
}

void AlertsWidget::handleHttpResponse(Wt::Json::Value result, long long mediaType, long long requestType, long long criteria, long long userRoleId)
{
    //FIXME: not safe, use try catch
    if (result.type() == Wt::Json::ObjectType)
    {
        Wt::WString message;

        Wt::Json::Object tmp = result;
        message = tmp.get("alias");

        updateTabContent(message, mediaType, requestType, criteria);
        if (m_userAliases.find(userRoleId) == m_userAliases.end())
        {
            struct Aliases aliases;
            aliases.email = new Wt::WString(m_tabContentMessageMail);
            aliases.sms = new Wt::WString(m_tabContentMessageSMS);
            aliases.mobile = new Wt::WString(m_tabContentMessageMobileApp);
            m_userAliases.insert(make_pair(userRoleId, aliases));
        }
        else
        {
            m_userAliases.at(userRoleId).email = new Wt::WString(m_tabContentMessageMail);
            m_userAliases.at(userRoleId).sms = new Wt::WString(m_tabContentMessageSMS);
            m_userAliases.at(userRoleId).mobile = new Wt::WString(m_tabContentMessageMobileApp);
        }
    }
    else
    {
        Wt::log("debug") << "Json Value NULL";
    }
}

void AlertsWidget::deleteAlert(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() != 204)
        {
            // a revoir !
            Wt::log("warning") << "[Alerts Widget] Problems parsing JSON: " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
    updatePage();
}

