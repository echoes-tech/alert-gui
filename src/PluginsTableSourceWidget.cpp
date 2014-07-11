/* 
 * GUI PluginsTableSourceWidget.cpp
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 11/07/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "PluginsTableSourceWidget.h"

using namespace std;

PluginsTableSourceWidget::PluginsTableSourceWidget(Echoes::Dbo::Session *session, string apiUrl,
        AbstractPage* abstractPage)
: AbstractPage(session, apiUrl, "plugins-source", true)
{
    setButtonModif(true);
    setButtonSup(true);
    
    multimap<int, string> titles;
    titles.insert(make_pair(ETypeJson::integer, "id"));
    setTitles(titles);

    lists_string lListUrl;
    list_string listUrl;
    listUrl.push_back("sources");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(lListUrl);
    
    m_abstractPage = abstractPage;
    m_selectedPluginID = 0;
}

void PluginsTableSourceWidget::updatePage(bool getResources)
{
    if(m_selectedPluginID != m_abstractPage->getSelectedID())
    {
        m_selectedPluginID = m_abstractPage->getSelectedID();
        setSelectedID(0);
    }
    AbstractPage::updatePage(getResources);
}

string PluginsTableSourceWidget::addParameter()
{
    return "&plugin_id=" + boost::lexical_cast<string>(m_abstractPage->getSelectedID());
}

void PluginsTableSourceWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*> argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument.begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"desc\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText("\n}");
}

void PluginsTableSourceWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*> argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument.begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"desc\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText("\n}");
}

