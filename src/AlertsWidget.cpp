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
//    setLocalTable(true);
    
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
/*
void AlertsWidget::popupRecipients(string nameAlert, string message)
{
    Wt::WComboBox *criterionComboBox;
    for (unsigned long criteria = 0 ; criteria < m_alertCriteria.size() ; criteria++)
    {
        criterionComboBox = m_alertCriteria[criteria].comboBoxCriteria;

        string criteria_id = ((Wt::WStandardItemModel*)(criterionComboBox->model()))->item(criterionComboBox->currentIndex(),0)->text().toUTF8();
    
        m_alertCriteria[criteria].criteriaID = boost::lexical_cast<long long> (criteria_id);
    }
    
    getAliases(this->m_session->user().id());
    
    Wt::WDialog *dialog = new Wt::WDialog();
    dialog->setClosable(true);
    AbstractPage::addButtonsToPopupFooter(dialog);
    dialog->resize(Wt::WLength(750), Wt::WLength(500));
    
    Wt::WTable *tablePopup = new Wt::WTable(dialog->contents());
    
    tablePopup->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.alert")));
    tablePopup->elementAt(0, 1)->addWidget(new Wt::WText(nameAlert));

    tablePopup->elementAt(1, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.rec")));
    Wt::WComboBox *boxUsers = new Wt::WComboBox(tablePopup->elementAt(1, 1));
    tablePopup->elementAt(1, 1)->resize(Wt::WLength(285), Wt::WLength(15));

    tablePopup->elementAt(2, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.media")));
    Wt::WComboBox *boxMedias = new Wt::WComboBox(tablePopup->elementAt(2, 1));
    
    Wt::WTable *tabMessages = new Wt::WTable(dialog->contents());
    tabMessages->resize(Wt::WLength(650), Wt::WLength(100));
    tabMessages->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.mess")));
    tabMessages->elementAt(0, 1)->addWidget(m_tabWidgetMessages);

    Wt::WTable *tabNotice = new Wt::WTable(tabMessages->elementAt(1, 1));
    Wt::WText *notice = new Wt::WText();
    tabNotice->resize(Wt::WLength(550), Wt::WLength(100));
    tabNotice->elementAt(0, 0)->addWidget(notice);
    notice->setWordWrap(true);
    notice->setText("*notice: elements like value/threshold/detection-time/alerting-time/unit within % (ex - %unit%) will be replaced by database association.<br />"
        "If you have more than one criterion, you should retrieve the element by adding a number from 0 to n, ex - \"%value0% on %unit0%...\"");

    for (multimap<long long, pair<long long, string>> ::iterator itU = userInfo_.begin(); itU != userInfo_.end(); itU++)
    {
        boxUsers->addItem(itU->second.second);
    }

    boxUsers->activated().connect(bind([ = ] ()
    {        
        long long id = userInfo_.find(boxUsers->currentIndex())->second.first;
        boxMedias->clear();
        getAliases(id);
        for (multimap<long long, pair < pair<long long, long long>, string>>::iterator itM = mediaInfo_.begin(); itM != mediaInfo_.end(); itM++)
        {
            if (id == m_mediaUserRelation.find(itM->first)->second)
            {
                boxMedias->addItem(itM->second.second);
            }
        }
    }));
    
    for (multimap<long long, pair < pair<long long, long long>, string>>::iterator itM = mediaInfo_.begin(); itM != mediaInfo_.end(); itM++)
    {
        if (userInfo_.find(boxUsers->currentIndex())->second.first == m_mediaUserRelation.find(itM->first)->second)
        {
            boxMedias->addItem(itM->second.second);
        }
    }
    Wt::WTable *table = new Wt::WTable(tablePopup->elementAt(2, 2));

    Wt::WTemplate *t = new Wt::WTemplate(tr("Alert.alert.time.template"));
    Wt::WLineEdit *time = new Wt::WLineEdit();
    
    t->bindWidget("time", time);
    time->setValidator(editValidator(EValidatorType::VALIDATOR_INT));
    table->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.snooze")));
    table->elementAt(0, 0)->setPadding(Wt::WLength(5), Wt::Top);
    table->elementAt(0, 1)->addWidget(t);
    table->elementAt(0, 1)->resize(Wt::WLength(60), Wt::WLength(15));
    table->elementAt(0, 1)->setPadding(Wt::WLength(5), Wt::Right);
    Wt::WText *error = new Wt::WText(tr("Alert.alert.invalid-number"));
    table->elementAt(1, 1)->addWidget(error);
    error->hide();
    
    Wt::WComboBox *comboBox = new Wt::WComboBox();
    comboBox->addItem(tr("Alert.alert.form.minutes")); 
//    comboBox->addItem(tr("Alert.alert.form.hours")); 
//    comboBox->addItem(tr("Alert.alert.form.times")); 
    comboBox->setWidth(Wt::WLength(80));
    table->elementAt(0, 2)->addWidget(comboBox);
    time_ = 0;
    comboBox->changed().connect(bind([ = ] ()
    {
        time_ = comboBox->currentIndex();
    }));

    dialog->finished().connect(bind([ = ] ()
    {
        if (dialog->result() == Wt::WDialog::Rejected)
        {
            closePopup();
            return;
        }
        if (time->validate() == Wt::WValidator::Valid)
        {
            checkPopupRecipients(message, time->text().toUTF8(), (boxMedias->currentIndex() + 1));
            return;
        }
        else
            error->show();
            dialog->show();
    }
    ));
    dialog->show();
}
*/

void AlertsWidget::selectMedia(long long id, long long index, BoxInBoxMenu *menu)
{
    if (menu->getIndex() > 0)
    {
        m_messageTable->enable();
    }

    /* Set receiver/media reminder */
    if (m_nameFromReceiver.find(menu->parentId) != m_nameFromReceiver.end()
            && m_nameFromMedia.find(id) != m_nameFromMedia.end())
    {
        m_messageReceiver->setText(m_nameFromReceiver.at(menu->parentId));
        m_messageMedia->setText(m_nameFromMedia.at(id));
    }
    
    /* Creating message */
    m_rowMedia = id;

    std::map<long long, struct Message>::iterator itMsg = m_messages.find(id);
    if (itMsg == m_messages.end())
    {
        struct Message newMessage;
        newMessage.receiverId = menu->parentId;
        newMessage.mediaId = id;
        newMessage.timer = 0;
        if (m_userAliases.find(menu->parentId) != m_userAliases.end())
        {
            switch (m_typesFromMedia.at(id))
            {
                case Enums::EMedia::email:
                {
                    newMessage.str = m_userAliases.at(menu->parentId).email;
                    break ;
                }
                case Enums::EMedia::sms:
                {
                    newMessage.str = m_userAliases.at(menu->parentId).sms;
                    break ;
                }
                case Enums::EMedia::mobileapp:
                {
                    newMessage.str = m_userAliases.at(menu->parentId).mobile;
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
        Wt::log("info") << " === rowMedia: " << m_rowMedia << "id: " << id;
        if (m_rowMedia == id)
        {
            m_messages.find(id)->second.str = new Wt::WString(m_messageArea->text());
        }
        Wt::log("info") << "message:" << m_messages.find(id)->second.str->toUTF8();
            
    }));
    
    m_messageTable->elementAt(0, 0)->keyWentUp().connect(bind( [ = ]()
    {
        if (m_rowMedia == id)
        {
            if (m_timer->validate() == Wt::WValidator::Valid)
            {
                Wt::log("info") << "TIMER: " << boost::lexical_cast<long long>(m_timer->text().toUTF8());
                m_messages.find(id)->second.timer = boost::lexical_cast<long long>(m_timer->text().toUTF8());
            }
        }
    }));
}

void AlertsWidget::addMedia(long long id, long long index, BoxInBoxMenu *menu)
{
    Wt::log("info") << "add Media";
    std::string media = ((Wt::WComboBox*)menu->m_header.at(1))->currentText().toUTF8();
    Wt::WText *mediaName = new Wt::WText(media);
    mediaName->setWordWrap(false);
    std::vector<Wt::WWidget*> widgets;
    
    Wt::log("info") << "id: " << id;
    widgets.push_back(mediaName);
    
    /* Set buttons */
    std::map<int, bool> buttons;
    buttons.insert(make_pair(EInteractions::REMOVE, true));
    
    /* Set functors */
    std::map<int, boost::function<void (long long, long long, BoxInBoxMenu*)>> functorMapMediaRow;
    functorMapMediaRow.insert(make_pair(EInteractions::REMOVE, boost::bind(&AlertsWidget::deleteMedia, this, _1, _2, _3)));
    functorMapMediaRow.insert(make_pair(EInteractions::SELECT, boost::bind(&AlertsWidget::selectMedia, this, _1, _2, _3)));
    
    for (std::vector<long long>::iterator itM = m_mediaIds.begin(); itM != m_mediaIds.end(); itM++)
    {
        if (m_nameFromMedia.find(*itM)->second == media)
        {
            menu->addRow(*itM, 1, widgets, buttons, functorMapMediaRow, NULL);
            break ;
        }
    }
}

void AlertsWidget::deleteMedia(long long id, long long index, BoxInBoxMenu *menu)
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
       m_messageTable->disable();
   }
}

void AlertsWidget::addReceiver(long long id, long long index, BoxInBoxMenu *menu)
{
    long long currentIndex = ((Wt::WComboBox*)menu->m_header.at(1))->currentIndex();
    long long selectedId = m_userIds.at(currentIndex);
    
    if (menu->rowIdExist(selectedId) && selectedId != -1)
    {
        BoxInBoxMenu *subMenu = menu->getSubMenu(selectedId);
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
    }
    getAliases(m_userIds.at(currentIndex));
    
    /* Set ComboBox */
    Wt::log("info") << "Set ComboBox";
    Wt::WComboBox *boxMedias = new Wt::WComboBox();
    boxMedias->setMinimumSize(Wt::WLength(220), Wt::WLength(20));
    for (std::vector<long long>::iterator itM = m_mediaIds.begin(); itM != m_mediaIds.end(); itM++)
    {
        if (m_userIds.at(currentIndex) == m_userFromMedia.find(*itM)->second)
        {
            boxMedias->addItem(m_nameFromMedia.find(*itM)->second);
        }
    }
    
    /* Set format */
    Wt::log("info") << "Set format";
    std::vector<Wt::WLength> rowLengths;
    rowLengths.push_back(Wt::WLength(50));
    rowLengths.push_back(Wt::WLength(200));
    rowLengths.push_back(Wt::WLength(50));
    
    /* Set widgets */
    Wt::log("info") << "Set widgets";
    std::vector<Wt::WWidget*> widgets;
    widgets.push_back(new Wt::WText(m_nameFromReceiver.find(m_userIds.at(currentIndex))->second));

    /* Set m_header */
    Wt::log("info") << "Set header";
    std::vector<Wt::WWidget*> headerMedia;
    headerMedia.push_back(new Wt::WText(tr("Alert.alert.form.media")));
    headerMedia.push_back(boxMedias);
    
    /* Set functors maps */
    Wt::log("info") << "Set functors";
    std::map<int, boost::function<void (long long, long long, BoxInBoxMenu*)>> functorMapMediaHeader;
    functorMapMediaHeader.insert(make_pair(EInteractions::ADD, boost::bind(&AlertsWidget::addMedia, this, _1, _2, _3)));
    
    std::map<int, boost::function<void (long long, long long, BoxInBoxMenu*)>> functorMapReceiverRow;
    functorMapReceiverRow.insert(make_pair(EInteractions::REMOVE, boost::bind(&AlertsWidget::deleteReceiver, this, _1, _2, _3)));
    functorMapReceiverRow.insert(make_pair(EInteractions::SELECT, boost::bind(&AlertsWidget::selectReceiver, this, _1, _2, _3)));
    
    /* Creating menu */
    Wt::log("info") << "Set menu";
    Wt::WTable *sepTable = new Wt::WTable(m_table->elementAt(0, 1));
    sepTable->resize(Wt::WLength(20), Wt::WLength(20));
    Wt::WTable *MediaTable = new Wt::WTable(m_table->elementAt(0, 2));
    MediaTable->resize(Wt::WLength(300), Wt::WLength(20));
    MediaTable->setMaximumSize(Wt::WLength(380), Wt::WLength(20));
    
    BoxInBoxMenu *menuMedia = new BoxInBoxMenu(MediaTable
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

void AlertsWidget::deleteReceiver(long long id, long long index, BoxInBoxMenu *menu)
{
   menu->deleteRow(index);
   if (menu->getIndex() == 0)
   {
        m_messageTable->disable();
   }
}

void AlertsWidget::selectReceiver(long long id, long long index, BoxInBoxMenu *menu)
{
    Wt::log("info") << "selected Receiver: " << id;
    BoxInBoxMenu *subMenu = menu->getSubMenu(id);
    if (subMenu)
    {
        if (subMenu->getIndex() == 0)
        {
            m_messageTable->disable();
        }
    }
}
void AlertsWidget::getCriteriaSelection()
{
    Wt::WComboBox *criterionComboBox;
    for (unsigned long criteria = 0 ; criteria < m_alertCriteria.size() ; criteria++)
    {
        criterionComboBox = m_alertCriteria[criteria].comboBoxCriteria;

        string criteria_id = ((Wt::WStandardItemModel*)(criterionComboBox->model()))->item(criterionComboBox->currentIndex(),0)->text().toUTF8();
    
        m_alertCriteria[criteria].criteriaID = boost::lexical_cast<long long> (criteria_id);
    }
}

void AlertsWidget::popupNewRecipientsRework(std::string nameAlert, std::string message)
{
    for (std::map<long long, struct Message>::iterator itMsg = m_messages.begin() ; itMsg != m_messages.end() ; itMsg++)
    {
        m_messages.erase(itMsg);
    }
    
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
    dialog->resize(Wt::WLength(750), Wt::WLength(70, Wt::WLength::Percentage));
    
    Wt::WTable *alertTable = new Wt::WTable(dialog->contents());
    alertTable->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.alert")));
    alertTable->elementAt(0, 1)->addWidget(new Wt::WText(nameAlert));
    
    Wt::WTable *blank1 = new Wt::WTable(dialog->contents());
    blank1->resize(Wt::WLength(20), Wt::WLength(30));
    
    m_table = new Wt::WTable(dialog->contents());
    m_table->resize(Wt::WLength(700), Wt::WLength(20));
    Wt::WTable *ReceiverTable = new Wt::WTable(m_table->elementAt(0, 0));
    ReceiverTable->resize(Wt::WLength(310), Wt::WLength(20));
    ReceiverTable->setMaximumSize(Wt::WLength(310), Wt::WLength(20));
    
    Wt::WTable *blank2 = new Wt::WTable(dialog->contents());
    blank2->resize(Wt::WLength(20), Wt::WLength(30));
    
    if (m_messageTable)
    {
        m_messageTable->clear();
    }
    
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

    m_messageTable->elementAt(2, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.snooze")));    
    m_messageTable->elementAt(2, 0)->setPadding(Wt::WLength(5), Wt::Top);
    m_messageTable->elementAt(2, 1)->addWidget(t);
    m_messageTable->elementAt(2, 1)->setPadding(Wt::WLength(5), Wt::Right);
    m_messageTable->elementAt(2, 1)->resize(Wt::WLength(60), Wt::WLength::Auto);
    m_messageTable->elementAt(2, 2)->addWidget(timerUnit);
    m_messageTable->elementAt(2, 2)->setPadding(Wt::WLength(5), Wt::Left);
    m_messageTable->elementAt(2, 2)->setPadding(Wt::WLength(5), Wt::Top);
    m_messageTable->elementAt(2, 2)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);

    /* set message area*/
    m_messageTable->elementAt(3, 0)->addWidget(new Wt::WText(tr("Alert.alert.form.mess")));
    m_messageTable->elementAt(3, 0)->setContentAlignment(Wt::AlignmentFlag::AlignRight);
    m_messageTable->elementAt(3, 1)->setContentAlignment(Wt::AlignmentFlag::AlignLeft);
    m_messageTable->elementAt(3, 1)->resize(Wt::WLength(650), Wt::WLength(150));
    m_messageTable->elementAt(3, 1)->setColumnSpan(2);
    m_messageArea = new Wt::WTextArea(m_messageTable->elementAt(3, 1));
    
    m_messageTable->disable();
    /* Set format */
    std::vector<Wt::WLength> rowLengths;
    rowLengths.push_back(Wt::WLength(50));
    rowLengths.push_back(Wt::WLength(200));
    rowLengths.push_back(Wt::WLength(50));
    
    /* Set functors map */
    Wt::log("info") << "Set functors";
    std::map<int, boost::function<void (long long, long long, BoxInBoxMenu*)>> functorMapReceiverHeader;
    functorMapReceiverHeader.insert(make_pair(EInteractions::ADD, boost::bind(&AlertsWidget::addReceiver, this, _1, _2, _3)));
    
    /* Set m_headers */
    std::vector<Wt::WWidget*> m_headerReceiver;
    
    m_headerReceiver.push_back(new Wt::WText(tr("Alert.alert.form.rec")));
    m_headerReceiver.push_back(boxUsers);
    
    
    /* Creating menu */    
    BoxInBoxMenu *menuReceiver = new BoxInBoxMenu(ReceiverTable
            , rowLengths, Wt::WLength(20));
    
    /* Set buttons templates */
    std::map<int, bool> buttons;
    buttons.insert(make_pair(0, true));
    
    menuReceiver->addRow(-1, 0, m_headerReceiver, buttons, functorMapReceiverHeader,  NULL);

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
    // FIXME: time time_ ???
        message += "{\n\"media_id\": " + boost::lexical_cast<string>(itMsg->second.mediaId);
        message += ",\n\"snooze\": " + boost::lexical_cast<string>((itMsg->second.timer * 60));
    /*
    switch (time_)
    {
    case 0:
        message += ",\n\"snooze\": " + boost::lexical_cast<string>((boost::lexical_cast<unsigned int>(time) * 60));
        break;
    case 1:
        message += ",\n\"snooze\": " + boost::lexical_cast<string>((boost::lexical_cast<unsigned int>(time) * 3600));
        break;
    case 2:
        message += ",\n\"snooze\": 0";
        break;
    }
    */
        
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

//                                                                                    ^
//                                                                                    |
// Work popup Two End --------------------------------------------------------------- |

/// ///////////////////////////////////////////// ----
// Work popup One --------------------------------------------------------------- |
//                                                                                |
//                                                                                v

void AlertsWidget::initAlertValueDefinitionPopup()
{
    fillModels();
    sortModels(); 
    m_boxAsset->clearSelection();
    m_boxPlugin->clearSelection();
    m_boxInfo->clearSelection();
    m_buttonAddCriteria->disable();
}

void AlertsWidget::setBox(Wt::WSelectionBox *box, Wt::WStandardItemModel *model)
{
    box->resize(Wt::WLength(200), Wt::WLength(150));
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
        changeButtonAddCriteriaState();
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
        changeButtonAddCriteriaState();
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
        changeButtonAddCriteriaState();
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
    fillModel(m_assets,m_mapAssetsNames);
    fillModel(m_plugins,m_mapPluginsNames);
    fillModel(m_informations,m_mapInformationsNames);
}

void AlertsWidget::fillModel(Wt::WStandardItemModel * model, std::map<long long, std::string> m_mapNames)
{
    model->clear();
    std::map<long long, std::string>::iterator mapNamesIt;
    int row = 0;
    for (mapNamesIt = m_mapNames.begin() ; mapNamesIt != m_mapNames.end() ; mapNamesIt++)
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

void AlertsWidget::popupAddWidget(Wt::WDialog *dialog, long long id)
{
    m_tabWidgetMessages = new Wt::WTabWidget();
    m_tabWidgetMessages->resize(Wt::WLength(600), Wt::WLength(150));
    m_alertCriteria.clear();
    checkAll_ = 1;
    
    Wt::WPushButton *ButtonSC = new Wt::WPushButton(tr("Alert.alert.button-save-continue"), dialog->footer());
    ButtonSC->clicked().connect(bind([ = ] ()
    {
        bool validCriterion = true;
        for (std::vector<AlertCriterion>::const_iterator it = m_alertCriteria.begin() ; it != m_alertCriteria.end() ; it++)
        {
            if (it->lineEditValue->validate() != Wt::WValidator::Valid)
            {
                validCriterion = false;
                dialog->show();
            }
            
        }
        if (validCriterion)
        {
            checkAll_ = 0;
            getCriteriaSelection();
            dialog->accept();
        }
    }));

    dialog->resize(Wt::WLength(750), Wt::WLength(500));
    
    new Wt::WText(tr("Alert.alert.add-title-box-key") + " : <br />", dialog->contents());
    keyValue_ = new Wt::WLineEdit(dialog->contents());
    keyValue_->setWidth(100);

    Wt::WContainerWidget *mainContainerWidget = new Wt::WContainerWidget(dialog->contents());
    Wt::WTable *tableBox = new Wt::WTable(mainContainerWidget);
    Wt::WTableCell *cell0_0 = tableBox->elementAt(0, 0);
    cell0_0->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-asset")));
    Wt::WTableCell *cell0_1 = tableBox->elementAt(0, 1);
    cell0_1->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-plugin")));
    Wt::WTableCell *cell0_2 = tableBox->elementAt(0, 2);
    cell0_2->addWidget(new Wt::WText(tr("Alert.alert.add-title-box-info")));
    
    m_boxAsset = new Wt::WSelectionBox(tableBox->elementAt(1, 0));
    m_boxPlugin = new Wt::WSelectionBox(tableBox->elementAt(1, 1));
    m_boxInfo = new Wt::WSelectionBox(tableBox->elementAt(1, 2));
    m_buttonAddCriteria = new Wt::WPushButton(tr("Alert.alert.add-button-add-critera"),tableBox->elementAt(1, 3));
    m_buttonAddCriteria->clicked().connect(bind([ = ] (){
                                      handleAddCriteria();
    }));
    
    initAlertValueDefinitionPopup();    

    setBox(m_boxAsset,m_assets);
    m_boxAsset->changed().connect(this, &AlertsWidget::assetSelected);
    setBox(m_boxPlugin,m_plugins);
    m_boxPlugin->changed().connect(this, &AlertsWidget::pluginSelected);
    setBox(m_boxInfo,m_informations);
    m_boxInfo->changed().connect(this, &AlertsWidget::informationSelected);

    m_textErrorForAsset = new Wt::WText(tr("Alert.alert.invalid-select-asset"));
    Wt::WTableCell *cell2_0 = tableBox->elementAt(2, 0);
    cell2_0->addWidget(m_textErrorForAsset);

    m_textErrorForPlugin = new Wt::WText(tr("Alert.alert.invalid-select-plugin"));
    Wt::WTableCell *cell2_1 = tableBox->elementAt(2, 1);
    cell2_1->addWidget(m_textErrorForPlugin);

    m_textErrorForInformation = new Wt::WText(tr("Alert.alert.invalid-select-info"));
    Wt::WTableCell *cell2_2 = tableBox->elementAt(2, 2);
    cell2_2->addWidget(m_textErrorForAsset);

    m_textErrorForAsset->hide();
    m_textErrorForPlugin->hide();
    m_textErrorForInformation->hide();

    new Wt::WText(tr("Alert.alert.add-compare"), mainContainerWidget);

    // ToDo : remplacer le container (ou le remplir avec ?) par EATableTemplate.
    m_compareWidgetContainerTop = new Wt::WContainerWidget(mainContainerWidget);
    m_compareWidgetContainerTop->setStyleClass("compare-widget-container-top");    
    m_compareTable = new Wt::WTable(mainContainerWidget);
}

// ------------------ Unit ------------------------------

void AlertsWidget::changeButtonAddCriteriaState()
{
    m_buttonAddCriteria->setEnabled(m_boxAsset->selectedIndexes().size() == 1 && m_boxPlugin->selectedIndexes().size() == 1 && m_boxInfo->selectedIndexes().size() == 1);
}

void AlertsWidget::handleAddCriteria()
{
    addCompareWidget(getSelectedIdFromSelectionBox(m_boxAsset), getSelectedIdFromSelectionBox(m_boxPlugin), getSelectedIdFromSelectionBox(m_boxInfo)); 
    initAlertValueDefinitionPopup();
}

void AlertsWidget::addCompareWidget(long long assetID, long long pluginID, long long infoID)
{    
    enum {
        OPERATOR,
        ASSET,
        PLUGIN,
        INFO,
        VALUE,
        CRITERIA,
        DELETE
    };
    
    if (assetID > 0 && pluginID > 0 && infoID > 0)
    {
        AlertCriterion alertCriterion;
        alertCriterion.unitTypeID = m_mapInformationsUnitTypes[infoID];
        alertCriterion.assetID = assetID;
        alertCriterion.pluginID = pluginID;
        alertCriterion.infoID = infoID;
        
        int row = m_compareTable->rowCount();  
            
        // OPERATOR
        m_compareTable->elementAt(row, OPERATOR)->setWidth(Wt::WLength(70));
        m_compareTable->elementAt(row, OPERATOR)->setContentAlignment(Wt::AlignCenter);
        Wt::WComboBox* operatorComboBox = new Wt::WComboBox();
        m_compareTable->elementAt(row, OPERATOR)->addWidget(operatorComboBox);
        alertCriterion.operatorComboBox = operatorComboBox;  
        operatorComboBox->setModel(m_booleanOperators);
        operatorComboBox->setCurrentIndex(0);
        if(m_alertCriteria.size() == 0)
        {
            operatorComboBox->setHidden(true);
            alertCriterion.index = 0;
        }
        else
        {
            alertCriterion.index = m_alertCriteria.end()->index + 1;
        }      
        
        // ASSET
        m_compareTable->elementAt(row, ASSET)->setWidth(Wt::WLength(150));
        Wt::WText* assetName = new Wt::WText(m_mapAssetsNames[assetID]);
        m_compareTable->elementAt(row, ASSET)->addWidget(assetName);      
        
        // PLUGIN
        m_compareTable->elementAt(row, PLUGIN)->setWidth(Wt::WLength(150));
        Wt::WText* pluginName = new Wt::WText(m_mapPluginsNames[pluginID]);
        m_compareTable->elementAt(row, PLUGIN)->addWidget(pluginName);      
        
        // INFO
        m_compareTable->elementAt(row, INFO)->setWidth(Wt::WLength(150));
        Wt::WText* infoName = new Wt::WText(m_mapInformationsNames[infoID]);
        m_compareTable->elementAt(row, INFO)->addWidget(infoName);
        
        // VALUE
        m_compareTable->elementAt(row, VALUE)->setWidth(Wt::WLength(150));
        m_compareTable->elementAt(row, VALUE)->setContentAlignment(Wt::AlignCenter);
        if(alertCriterion.unitTypeID == Enums::EInformationUnitType::text || 
                alertCriterion.unitTypeID == Enums::EInformationUnitType::number ||
                alertCriterion.unitTypeID == Enums::EInformationUnitType::custom)
        {
            Wt::WTemplate *t = new Wt::WTemplate(tr("Alert.alert.criterion.template"));
            Wt::WValidator* validator = editValidator(setValidatorType(ETypeJson::number, 0, EMandatory::is));
            Wt::WLineEdit* lineEditValue = new Wt::WLineEdit();

            lineEditValue->setValidator(validator);
            t->bindWidget("criterion", lineEditValue);
            
            alertCriterion.lineEditValue = lineEditValue;
            alertCriterion.validatorCriteria = validator;
            alertCriterion.templateValid = t;

            m_compareTable->elementAt(row, VALUE)->addWidget(t);
        }
        else if(alertCriterion.unitTypeID == Enums::EInformationUnitType::boolean)
        {
            Wt::WContainerWidget* containerTrueFalse = new Wt::WContainerWidget();
            
            Wt::WButtonGroup* groupTrueFalse = new Wt::WButtonGroup(containerTrueFalse);
            m_compareTable->elementAt(row, VALUE)->addWidget(containerTrueFalse);
            alertCriterion.groupTrueFalse = groupTrueFalse;    
            
            Wt::WRadioButton* buttonTrue = new Wt::WRadioButton("true", containerTrueFalse);
            buttonTrue->setChecked();
            groupTrueFalse->addButton(buttonTrue);
            
            Wt::WRadioButton* buttonFalse = new Wt::WRadioButton("false", containerTrueFalse);
            groupTrueFalse->addButton(buttonFalse);
        }
        else
        {
            Wt::log("error") << "Unknown unit type : " << alertCriterion.unitTypeID;            
        }
              
        // CRITERIA    
        m_compareTable->elementAt(row, CRITERIA)->setWidth(Wt::WLength(100));  
        m_compareTable->elementAt(row, VALUE)->setContentAlignment(Wt::AlignCenter); 
        if(alertCriterion.unitTypeID == Enums::EInformationUnitType::text ||
                alertCriterion.unitTypeID == Enums::EInformationUnitType::number)
        {
            Wt::WComboBox* comboBoxCriteria = createCompareCriteriaComboBox(alertCriterion.unitTypeID); 
            comboBoxCriteria->setCurrentIndex(0);
            m_compareTable->elementAt(row, CRITERIA)->addWidget(comboBoxCriteria);
            alertCriterion.comboBoxCriteria = comboBoxCriteria; 
        }
        else if(alertCriterion.unitTypeID == Enums::EInformationUnitType::boolean ||
                alertCriterion.unitTypeID == Enums::EInformationUnitType::custom)
        {
            
        }
        else
        {
            Wt::log("error") << "Unknown unit type : " << alertCriterion.unitTypeID;            
        }
        
        // DELETE
        m_compareTable->elementAt(row, DELETE)->setContentAlignment(Wt::AlignCenter);
        Wt::WPushButton *buttonDelete = new Wt::WPushButton("<i class='icon-remove icon-white'></i>");
        m_compareTable->elementAt(row, DELETE)->addWidget(buttonDelete);
        buttonDelete->setStyleClass("btn-danger");
        buttonDelete->setTextFormat(Wt::XHTMLUnsafeText);
        buttonDelete->clicked().connect(bind([ = ] ()
        {
            for(int i=0; i < m_compareTable->rowCount(); i++)
            {
                if((Wt::WPushButton*)m_compareTable->elementAt(i, DELETE)->widget(0) == buttonDelete)
                {
                    m_compareTable->deleteRow(i);
                    if(i == 0 && m_compareTable->rowCount() > 0)
                    {     
                        ((Wt::WComboBox*)m_compareTable->elementAt(0, OPERATOR)->widget(0))->setHidden(true);
                    }
                    break;
                }
            }
            for (vector<AlertCriterion>::iterator it = m_alertCriteria.begin(); it != m_alertCriteria.end(); it++)
            {
                if(it->index == alertCriterion.index)
                {
                    m_alertCriteria.erase(it);     
                    break;
                }
            }
        }));
        
        m_alertCriteria.push_back(alertCriterion);
    }
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
//    int rank = 1;
//    for(auto it = criterion.begin(); it != criterion.end(); it++)
//    {
//        Wt::WStandardItem *itemIndex = new Wt::WStandardItem();
//        Wt::WStandardItem *itemComparison = new Wt::WStandardItem();
//        itemIndex->setText(boost::lexical_cast<string>(rank));
//        itemComparison->setText(*it);
//        model->setItem(rank,1,itemIndex);
//        model->setItem(rank++,2,itemComparison);
//    }
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

/*
Wt::WValidator *AlertsWidget::editValidator(int validatorType)
{
    Wt::WRegExpValidator *validator = 0;

    switch (validatorType)
    {
        case EValidatorType::VALIDATOR_FLOAT:
            validator = new Wt::WRegExpValidator("-?[0-9]+\\.[0-9]+$");
            validator->setMandatory(true);
            break;
        case EValidatorType::VALIDATOR_INT:
            validator = new Wt::WRegExpValidator("^[0-9]+$");
            validator->setMandatory(true);
        case EValidatorType::VALIDATOR_TIMER:
            validator = new Wt::WRegExpValidator("^[1-9][0-9]*$");
            validator->setMandatory(true);
            break ;
        case EValidatorType::VALIDATOR_STRING:
        {
            Wt::log("info") << "Just passing through VALIDATOR_STRING";
            validator = new Wt::WRegExpValidator("^.+$");
            validator->setMandatory(true);
            break;
        }
    }

    return validator;
}
*/

/* artefact
void AlertsWidget::checkPopupRecipients(string message, string time, int media)
{
    message += "\"alert_media_specializations\":\n[\n";

    // FIXME: time time_ ???
    message += "{\n\"media_id\": " + boost::lexical_cast<string>((*mediaInfo_.find(media - 1)).second.first.first);
    switch (time_)
    {
    case 0:
        message += ",\n\"snooze\": " + boost::lexical_cast<string>((boost::lexical_cast<unsigned int>(time) * 60));
        break;
    case 1:
        message += ",\n\"snooze\": " + boost::lexical_cast<string>((boost::lexical_cast<unsigned int>(time) * 3600));
        break;
    case 2:
        message += ",\n\"snooze\": 0";
        break;
    }
    
    switch ((*mediaInfo_.find(media - 1)).second.first.second)
    {
    case Enums::EMedia::email:
        message += ",\n\"message\": \"" + m_tabContentMessageMail + "\"\n}";
        break;
    case Enums::EMedia::sms:
        message += ",\n\"message\": \"" + m_tabContentMessageSMS + "\"\n}";
        break;
    case Enums::EMedia::mobileapp:
        message += ",\n\"message\": \"" + m_tabContentMessageMobileApp + "\"\n}";
        break;
    }
    message += "\n]\n}";
    postAlertCallApi(message);
    updatePage();
}
*/
 
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
    for(auto it = m_alertCriteria.begin(); it != m_alertCriteria.end(); it++)
    {
        message += "\n{\n";
        switch (it->unitTypeID)
        {
            //same for text & number
            case Enums::EInformationUnitType::text:
            case Enums::EInformationUnitType::number:
            {
                message += "\"alert_criterion_id\": " + boost::lexical_cast<string>(getSelectedIdFromComboBox(it->comboBoxCriteria)) + ",\n";
                message += "\"value\": \"" + Wt::Utils::base64Encode(it->lineEditValue->text().toUTF8()) + "\",\n";

                if (!keyValue_->text().toUTF8().empty())
                {
                    message += "\"key_value\": \"" + keyValue_->text().toUTF8() + "\",\n";
                }
                break;
            }
            case Enums::EInformationUnitType::boolean:
                message += "\"alert_criterion_id\": 1,\n";                
                switch (it->groupTrueFalse->selectedButtonIndex())
                {
                    case 0:
                        message += "\"value\": \"" + Wt::Utils::base64Encode("true") + "\",\n";                    
                        break;
                    case 1:
                        message += "\"value\": \"" + Wt::Utils::base64Encode("false") + "\",\n";
                        break;
                }
                message += "\"key_value\": \"\",\n";
                break;
            case Enums::EInformationUnitType::custom:
                message += "\"alert_criterion_id\": 1,\n";
                message += "\"value\": \"" + Wt::Utils::base64Encode(it->lineEditValue->text().toUTF8()) + "\",\n";
                message += "\"key_value\": \"\",\n";
                break;
            default:
                Wt::log("error") << "Unknown unit type : " << it->unitTypeID;   
                break;
        }
        message += "\"operator\":" + m_booleanOperators->item(it->operatorComboBox->currentIndex(), 1)->text().toUTF8() + ",\n";
        message += "\"asset_id\": " + boost::lexical_cast<string>(it->assetID) + ",\n";
        message += "\"plugin_id\": " + boost::lexical_cast<string>(it->pluginID) + ",\n";
        message += "\"information_id\": " + boost::lexical_cast<string>(it->infoID);
        message += "\n}";
        if (it < m_alertCriteria.end()-1)
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
        Wt::log(" -- message -- ") << message;
        popupNewRecipientsRework(data, message);
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
    
    for (std::vector<AlertCriterion>::iterator criteria = m_alertCriteria.begin(); criteria != m_alertCriteria.end() ; criteria++)
    {
        criteria->smsRsp.message = "";
        criteria->smsRsp.asset = "";
        criteria->smsRsp.plugin = "";
        criteria->smsRsp.criteria = "";
        criteria->smsRsp.information = "";
        criteria->emailRsp.message = "";
        criteria->emailRsp.asset = "";
        criteria->emailRsp.plugin = "";
        criteria->emailRsp.criteria = "";
        criteria->emailRsp.information = "";
        criteria->mobileappRsp.message = "";
        criteria->mobileappRsp.asset = "";
        criteria->mobileappRsp.plugin = "";
        criteria->mobileappRsp.criteria = "";
        criteria->mobileappRsp.information = "";
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
void AlertsWidget::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    //    Wt::Http::Message message;

    //    Wt::WLineEdit *test;
    //    test = (Wt::WLineEdit*)(*arguments.begin());

    /*
    message.addBodyText("{\n\"med_id\": " + boost::lexical_cast<string>(this->type_)
            + ",\n\"mev_value\":\"" + boost::lexical_cast<string>(test->text()) + "\"\n}");

    string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<string>(id);
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::putMedia, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "AlertsWidget : [PUT] address to call : " << apiAddress;
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
     */
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
                    vector<long long> infosIds;
                    if (jsonPluginAssets.type() == Wt::Json::ArrayType)
                    {
                        Wt::Json::Array jsonPluginAssetsArray = jsonPluginAssets;
                        for (int cpt1(0); cpt1 < (int) jsonPluginAssetsArray.size(); cpt1++)
                        {
                            Wt::Json::Object jsonAsset = jsonPluginAssetsArray.at(cpt1);
                            long long assetId = jsonAsset.get("id");
                            infosIds.push_back(assetId);
                        }
                    }
                    m_mapPluginAssets[pluginId] = infosIds;
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
    
    for (unsigned long criteria = 0 ; criteria != m_alertCriteria.size() ; criteria++)
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

void AlertsWidget::getMediaRsp(Wt::WString message, long long mediaType, long long requestType, long long criteria)
{
    switch (mediaType)
    {
        case Enums::EMedia::sms:
            getRequestRsp(requestType, message, m_alertCriteria[criteria].smsRsp);
            break;
        case Enums::EMedia::email:
            getRequestRsp(requestType, message, m_alertCriteria[criteria].emailRsp);
            break;
        case Enums::EMedia::mobileapp:
            getRequestRsp(requestType, message, m_alertCriteria[criteria].mobileappRsp);
            break;
    }
}

void AlertsWidget::updateMessage(std::string &tabContent, Wt::WString &message, unsigned long criteria)
{
    if (criteria > 0 && !tabContent.empty())
    {
        if (m_booleanOperators->item(m_alertCriteria[criteria].operatorComboBox->currentIndex(), 1)->text().toUTF8()
                == boost::lexical_cast<string>(Echoes::Dbo::EBooleanOperator::AND))
        {
            tabContent += boost::lexical_cast<string>(tr("Alert.alert.boolean-operator.and")) + " ";
        }
        else
        {
            tabContent += boost::lexical_cast<string>(tr("Alert.alert.boolean-operator.or")) + " ";
        }
    }
    
    Wt::log("info") << "criteria: " << criteria;
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

void AlertsWidget::updateTabContent(long long mediaType)
{
    switch (mediaType)
    {
        case Enums::EMedia::email:
        {
            m_tabContentMessageMail.clear();
            for (unsigned long criteria = 0; criteria < m_alertCriteria.size(); criteria++)
            {
                updateMessage(m_tabContentMessageMail, m_alertCriteria[criteria].emailRsp.message, criteria);
            }
            break;
        }
        case Enums::EMedia::sms:
        {
            m_tabContentMessageSMS.clear();
            for (unsigned long criteria = 0; criteria < m_alertCriteria.size(); criteria++)
            {
                updateMessage(m_tabContentMessageSMS, m_alertCriteria[criteria].smsRsp.message, criteria);
            }
            break;
        }
        case Enums::EMedia::mobileapp:
        {
            m_tabContentMessageMobileApp.clear();
            for (unsigned long criteria = 0; criteria < m_alertCriteria.size(); criteria++)
            {
                updateMessage(m_tabContentMessageMobileApp, m_alertCriteria[criteria].mobileappRsp.message, criteria);
            }
            break;
        }
    }
}

void AlertsWidget::handleHttpResponse(Wt::Json::Value result, long long mediaType, long long requestType, long long criteria, long long userRoleId)
{
    if (result.type() == Wt::Json::ObjectType)
    {
        Wt::WString message;

        Wt::Json::Object tmp = result;
        message = tmp.get("alias");

        getMediaRsp(message, mediaType, requestType, criteria);

        updateTabContent(mediaType);
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
        Wt::log("info") << "Json Value NULL";
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

