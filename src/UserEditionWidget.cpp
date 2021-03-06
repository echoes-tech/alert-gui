/**
 * @file UserEditionWidget.cpp
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

#include "UserEditionWidget.h"

using namespace std;

UserEditionWidget::UserEditionWidget(Echoes::Dbo::Session *session, string apiUrl, int type)
: AbstractPage(session, apiUrl, "media-user")
{
    this->session_ = session;
    this->apiUrl_ = apiUrl;
    this->type_ = type;
    usersModel_ = new Wt::WStandardItemModel(0, 2, this);

    setButtonModif(true);
    setButtonSup(true);
//    setLocalTable(true);
    
    string nameSpe = type == 1 ? "mail" : type == 2 ? "sms" : type == 3 ? "push" : "error";
//    this->setNameSpecial(nameSpe);
    
    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "value"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::undid, 0, EMandatory::is), "user"));
    
    std::vector<std::string> undidNames;
    undidNames.push_back("first_name");
    undidNames.push_back("last_name");
    setUndidName(undidNames);
    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    vector<string> listParameter;

    listParameter.push_back("type_id=" + boost::lexical_cast<string>(this->type_));    
    listUrl.push_back(pair<string, vector<string>>("medias", listParameter));  
    listUrl.push_back(pair<string, vector<string>>("medias/:id", listParameter));    
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("users", listParameter));  
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);

}

Wt::WComboBox *UserEditionWidget::popupAdd(Wt::WDialog *dialog)
{
    Wt::WComboBox *comboBox = new Wt::WComboBox(dialog->contents());
    comboBox->setModel(usersModel_);
    comboBox->setCurrentIndex(0);
    return comboBox;
}

/*
Wt::WValidator *UserEditionWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator = 0;
    if (type_ == 1)
    {
        validator = new Wt::WRegExpValidator("[a-zA-Z0-9._%+-]+@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,6}$"); //[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}
    }
    else if (type_ == 2)
    {
        validator = new Wt::WRegExpValidator("(0)[0-9]{9}");
    }
    else if (type_ == 3)
    {
        validator = new Wt::WRegExpValidator("[a-zA-Z0-9.-]{3,}");
    }
    else
    {
        AbstractPage::editValidator(who);
    }
    return validator;
}
*/

void UserEditionWidget::addResource(vector<Wt::WInteractWidget*>* argument)
{
    vector<Wt::WInteractWidget*>::iterator i = argument->begin();

    Wt::Http::Message messageMedia;
    messageMedia.addBodyText("{\n\"type_id\": " + boost::lexical_cast<string>(this->type_)
            + ",\n\"value\": \"" + boost::lexical_cast<string>(((Wt::WLineEdit*)(*i++))->text()) + "\"");

    Wt::WStandardItemModel *userModel = (Wt::WStandardItemModel*)((Wt::WComboBox*)(*i))->model();
    messageMedia.addBodyText(",\n\"user_id\" : " + userModel->item(((Wt::WComboBox*)(*i))->currentIndex(), 1)->text().toUTF8());

    messageMedia.addBodyText("\n}");

    string apiAddress = this->getApiUrl() + "/medias"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::postResourceCallback, this, _1, _2, client));
    
    Wt::log("debug") << "UserEditionWidget : [POST] address to call : " << apiAddress;
    Wt::log("debug") << " Message for [POST] : " << messageMedia.body();
    
    if (client->post(apiAddress, messageMedia))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}


void UserEditionWidget::modifResource(vector<Wt::WInteractWidget*>* arguments, long long id)
{
    MapLongString::iterator it = mediasTokens.find(id);
    vector<Wt::WInteractWidget*>::iterator i = arguments->begin();

    Wt::Http::Message message;
    message.addBodyText("{\n\"token\": \"" + (*it).second
                        + "\",\n\"value\":\"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");

    Wt::WStandardItemModel *userModel = (Wt::WStandardItemModel*)((Wt::WComboBox*)(*i))->model();
    message.addBodyText(",\n\"user_id\" : " + userModel->item(((Wt::WComboBox*)(*i))->currentIndex(), 1)->text().toUTF8());

    message.addBodyText("\n}");

    string apiAddress = this->getApiUrl() + "/medias/"
            + boost::lexical_cast<string>(id)
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "UserEditionWidget : [PUT] address to call : " << apiAddress;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::putResourceCallback, this, _1, _2, client));
    if (client->put(apiAddress, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void UserEditionWidget::handleJsonGet(vectors_Json jsonResources)
{
    mediasTokens.clear();
    
    try
    {
        vector<Wt::Json::Value> jsonMedia = jsonResources.at(0);
        Wt::Json::Array& result = jsonMedia.at(0);
        for (int cpt(0); cpt < (int) result.size(); cpt++)
        {
            Wt::Json::Object obj = result.at(cpt);
            Wt::WString token = obj.get("token");
            long long id = obj.get("id");
            mediasTokens[id] = token.toUTF8();
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[UserEditionWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[UserEditionWidget] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }

    try
    {
        vector<Wt::Json::Value> jsonUsers = jsonResources.at(1);
        jsonResources.pop_back();
        AbstractPage::handleJsonGet(jsonResources);

        usersModel_->clear();

        
        Wt::Json::Array& result = jsonUsers.at(0);
        
        for (int cpt(0); cpt < (int) result.size(); cpt++)
        {
            Wt::WStandardItem *itemId = new Wt::WStandardItem();
            Wt::WStandardItem *itemName = new Wt::WStandardItem();

            Wt::Json::Object obj = result.at(cpt);
            Wt::WString firstName = obj.get("first_name");
            Wt::WString lastName = obj.get("last_name");
            Wt::WString email = obj.get("mail");
            
//            string name = firstName.toUTF8() + " " + lastName.toUTF8();
            string name = email.toUTF8();
            long long id = obj.get("id");

            vector<Wt::WStandardItem*> rowVector;

            itemId->setText(boost::lexical_cast<string>(id));
            itemName->setText(Wt::WString::fromUTF8(name));

            rowVector.push_back(itemName);
            rowVector.push_back(itemId);
            usersModel_->insertRow(cpt, rowVector);
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[UserEditionWidget] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[UserEditionWidget] JSON Type Exception";
//        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
}
