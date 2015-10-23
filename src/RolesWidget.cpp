/* 
 * File:   RolesWidget.cpp
 * Author: cde
 * 
 * Created on 27 janvier 2015, 15:05
 */

#include "RolesWidget.h"

RolesWidget::RolesWidget(Echoes::Dbo::Session *session, std::string apiUrl)
: AbstractPage(session, apiUrl, "roles")
{
    setButtonModif(true);
    setButtonSup(true);
    
    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("roles", vector<string>()));    
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
}

RolesWidget::~RolesWidget() {
}

