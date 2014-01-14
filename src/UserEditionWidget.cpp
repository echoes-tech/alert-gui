/* 
 * Gui UserEditionWidget.cpp
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 15/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "UserEditionWidget.h"

UserEditionWidget::UserEditionWidget(Echoes::Dbo::Session *session, std::string apiUrl, int type)
: AbstractPage(session, apiUrl, "media-user")
{
    this->session_ = session;
    this->apiUrl_ = apiUrl;
    this->type_ = type;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
    
    std::string nameSpe = type == 1 ? "mail" : type == 2 ? "sms" : type == 3 ? "push" : "error";
    this->setNameSpecial(nameSpe);
    
    vector_pair_string titles;
    titles.push_back(std::make_pair(ETypeJson::text, "value"));
    titles.push_back(std::make_pair(ETypeJson::undid, "user"));
    setUndidName("last_name");
    setTitles(titles);

    lists_string lListUrl;
    list_string listUrl;
    listUrl.push_back("medias");
    listUrl.push_back("medias/:id");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    listUrl.push_back("users");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(lListUrl);

}

Wt::WComboBox *UserEditionWidget::popupAdd(Wt::WDialog *dialog)
{
    Wt::WComboBox *comboBox = new Wt::WComboBox(dialog->contents());
    comboBox->setModel(usersModel_);
    return comboBox;
}

std::string UserEditionWidget::addParameter()
{
    return "&type_id=" + boost::lexical_cast<std::string>(this->type_);
}

void UserEditionWidget::handleJsonGet(vectors_Json jsonResources)
{
    mediasTokens.clear();
    
    try
    {

        vector_Json jsonMedia = jsonResources.at(0);
        Wt::Json::Array& result = jsonMedia.at(0);
        for (int cpt(0); cpt < (int) result.size(); cpt++)
        {
            Wt::Json::Object obj = result.at(cpt);
            Wt::WString token = obj.get("token");
            long long id = obj.get("id");
            mediasTokens[id] = token.toUTF8();
        }

        vector_Json jsonUsers = jsonResources.at(1);
        jsonResources.pop_back();
        AbstractPage::handleJsonGet(jsonResources);

        usersModel_ = new Wt::WStandardItemModel(0, 2, this);

        result = jsonUsers.at(0);
        for (int cpt(0); cpt < (int) result.size(); cpt++)
        {
            Wt::WStandardItem *itemId = new Wt::WStandardItem();
            Wt::WStandardItem *itemName = new Wt::WStandardItem();

            Wt::Json::Object obj = result.at(cpt);
            Wt::WString firstName = obj.get("first_name");
            Wt::WString lastName = obj.get("last_name");
            std::string name = firstName.toUTF8() + " " + lastName.toUTF8();
            long long id = obj.get("id");

            std::vector<Wt::WStandardItem*> rowVector;

            itemId->setText(boost::lexical_cast<std::string>(id));
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
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
}

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
    return validator;
}

void UserEditionWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{
    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();

    Wt::Http::Message messageMedia;
    messageMedia.addBodyText("{\n\"type_id\": " + boost::lexical_cast<std::string>(this->type_)
            + ",\n\"value\": \"" + boost::lexical_cast<std::string>(((Wt::WLineEdit*)(*i++))->text()) + "\"");

    Wt::WStandardItemModel *userModel = (Wt::WStandardItemModel*)((Wt::WComboBox*)(*i))->model();
    messageMedia.addBodyText(",\n\"user_id\" : " + userModel->item(((Wt::WComboBox*)(*i))->currentIndex(), 1)->text().toUTF8());

    messageMedia.addBodyText("\n}");

    std::string apiAddress = this->getApiUrl() + "/medias"
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


void UserEditionWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
    MapLongString::iterator it = mediasTokens.find(id);
    std::vector<Wt::WInteractWidget*>::iterator i = arguments.begin();

    Wt::Http::Message message;
    message.addBodyText("{\n\"token\": \"" + (*it).second
                        + "\",\n\"value\":\"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");

    Wt::WStandardItemModel *userModel = (Wt::WStandardItemModel*)((Wt::WComboBox*)(*i))->model();
    message.addBodyText(",\n\"user_id\" : " + userModel->item(((Wt::WComboBox*)(*i))->currentIndex(), 1)->text().toUTF8());

    message.addBodyText("\n}");

    std::string apiAddress = this->getApiUrl() + "/medias/"
            + boost::lexical_cast<std::string>(id)
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



