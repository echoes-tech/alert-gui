/* 
 * File:   UnitsWidget.cpp
 * Author: tsa
 * 
 * Created on 23 mai 2014, 16:00
 */

#include "UnitsWidget.h"

using namespace std;

UnitsWidget::UnitsWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "unit")
{
    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);

    multimap<int, string> titles;
    titles.insert(make_pair(ETypeJson::text, "name"));
    titles.insert(make_pair(ETypeJson::object, "information_unit_type"));
//    titles.insert(make_pair(ETypeJson::widget, ""));
    setTitles(titles);

    lists_string lListUrl;
    list_string listUrl;
    listUrl.push_back("units");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(lListUrl);
}

UnitsWidget::~UnitsWidget()
{
}

void UnitsWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*> argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument.begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"type_id\": " + ((Wt::WLineEdit*)(*it++))->text().toUTF8());
    message->addBodyText("\n}");
}

