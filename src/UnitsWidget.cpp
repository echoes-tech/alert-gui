/**
 * @file UnitsWidget.cpp
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#include "UnitsWidget.h"

using namespace std;

UnitsWidget::UnitsWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "unit")
{
    setButtonModif(true);
    setButtonSup(true);
    
    m_unitTypeComboBox = new Wt::WComboBox();
    m_unitTypeModel = new Wt::WStandardItemModel(0,2,this);
    
    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::undid, 0, EMandatory::is), "information_unit_type"));
    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("units", vector<string>())); 
    listUrl.push_back(pair<string, vector<string>>("units/:id", vector<string>()));    
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
    
    fillModel();
}

UnitsWidget::~UnitsWidget()
{
}

void UnitsWidget::fillModel()
{
    m_unitTypeModel->clear();
    addEnumToModel(m_unitTypeModel, Enums::EInformationUnitType::text, "text");
    addEnumToModel(m_unitTypeModel, Enums::EInformationUnitType::number, "number");
    addEnumToModel(m_unitTypeModel, Enums::EInformationUnitType::boolean, "boolean");
    addEnumToModel(m_unitTypeModel, Enums::EInformationUnitType::custom, "custom");
}

Wt::WComboBox *UnitsWidget::popupAdd(Wt::WDialog *dialog)
{
    m_unitTypeComboBox = new Wt::WComboBox(dialog->contents());
    m_unitTypeComboBox->setModel(m_unitTypeModel);
    m_unitTypeComboBox->setCurrentIndex(0);
    return m_unitTypeComboBox;
}

void UnitsWidget::setAddResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"type_id\": " + m_unitTypeModel->item(m_unitTypeComboBox->currentIndex(), 1)->text().toUTF8());
    message->addBodyText("\n}");
}

void UnitsWidget::setModifResourceMessage(Wt::Http::Message *message,vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator it = argument->begin();
    
    message->addBodyText("{");
    message->addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    message->addBodyText(",\n\"type_id\": " + m_unitTypeModel->item(((Wt::WComboBox*)(*it++))->currentIndex(), 1)->text().toUTF8());
    message->addBodyText("\n}");
}
