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
: CreatePageWidget(session, apiUrl, "media-user")
{
    this->session_ = session;
    this->apiUrl_ = apiUrl;
    this->created_ = false;
    this->newClass_ = false;
    this->type_ = type;

    this->result_ = Wt::Json::Value::Null;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
}

void UserEditionWidget::update()
{
    CreatePageWidget::update();
    if (!newClass_)
    {
        newClass_ = true;
    }
}

std::vector<std::string> UserEditionWidget::getTitlesTableWidget()
{
    std::vector<std::string> titleWidget;
    return titleWidget;
}

std::vector<std::string> UserEditionWidget::getTitlesTableText()
{
    std::vector<std::string> titleText;
    titleText.empty();
    if (type_ == 1)
        titleText.push_back("mail");
    else if (type_ == 2)
        titleText.push_back("sms");
    else if (type_ == 3)
        titleText.push_back("push");
    return titleText;
}

std::vector<long long> UserEditionWidget::getIdsTable()
{
    std::vector<long long> ids;

    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;

    if (result_.isNull() == false)
    {
        result1 = result_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
        {
            tmp = (*idx1);
            long long id = tmp.get("id");
            ids.push_back(id);
            std::string token = tmp.get("token");
            mediasTokens[id] = token;
        }
    }
    return ids;
}

vector_type UserEditionWidget::getResourceRowTable(long long id)
{
    vector_type rowTable;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;
    Wt::WString nameSms = "";
    long long i(0);
    if (result_.isNull() == false)
    {
        result1 = result_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
        {
            tmp = (*idx1);
            nameSms = tmp.get("value");
            i = tmp.get("id");
            if (i == id)
            {
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(nameSms)));
                return (rowTable);
            }
        }
    }
    return rowTable;

}

Wt::WValidator *UserEditionWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator = 0;
    if (type_ == 1)
        validator = new Wt::WRegExpValidator("[a-zA-Z0-9._%+-]+@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,6}$"); //[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}
    else if (type_ == 2)
        validator = new Wt::WRegExpValidator("(0)[0-9]{9}");
    else if (type_ == 3)
        validator = new Wt::WRegExpValidator("[[a-zA-Z0-9.-]");
    return validator;
}

void UserEditionWidget::closePopup()
{
    recoverListMedia();
}

void    UserEditionWidget::recoverListMedia()
{
    std::string apiAddress = this->getApiUrl() + "/medias"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8()
            + "&type_id=" + boost::lexical_cast<std::string>(this->type_);

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::getMedia, this, _1, _2));
    Wt::log("debug") << "UserEditionWidget : [GET] address to call : " << apiAddress;
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void UserEditionWidget::getMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Get Media response : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    result_ = 0;
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::parse(response.body(), result_);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[User Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.media-user.database-error-title"), tr("Alert.media-user.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[User Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "TypeException", tr("Alert.media-user.database-error"), Wt::Ok);
            }
        }
        else
        {
            this->result_ = Wt::Json::Value::Null;
        }
    }
    else
    {
        Wt::log("error") << "[User Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "err", tr("Alert.media-user.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
    update();
}

void UserEditionWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{
    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();
    Wt::WLineEdit *assetEdit = (Wt::WLineEdit*)(*i);

    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\"type_id\": " + boost::lexical_cast<std::string>(this->type_)
                             + ",\n\"value\": \"" + boost::lexical_cast<std::string>(assetEdit->text()) + "\"\n}");

    std::string apiAddress = this->getApiUrl() + "/medias"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::postMedia, this, _1, _2));
    std::cout << "Add Media ! " << std::endl;
    Wt::log("debug") << "UserEditionWidget : [POST] address to call : " << apiAddress;
    Wt::log("debug") << " Message for [POST] : " << messageAsset.body();
    if (client->post(apiAddress, messageAsset))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
}

Wt::WDialog *UserEditionWidget::deleteResource(long long id)
{
    std::cout << "Id : " << id << std::endl;
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    box->show();
    box->finished().connect(std::bind([ = ] (){
        if (box->result() == Wt::WDialog::Accepted)
        {
            Wt::Http::Message message;
            message.addBodyText("");
            std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string> (id)
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

            Wt::log("debug") << "UserEditionWidget : [DELETE] address to call : " << apiAddress;

            Wt::Http::Client *client = new Wt::Http::Client(this);
            client->done().connect(boost::bind(&UserEditionWidget::deleteMedia, this, _1, _2));
            if (client->deleteRequest(apiAddress, message))
            Wt::WApplication::instance()->deferRendering();
            else
                Wt::log("error") << "Error Client Http";
        }
        return box;
    }));
    return box;
}

void UserEditionWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
    Wt::Http::Message message;

    MapLongString::iterator it = mediasTokens.find(id);
    Wt::WLineEdit *test;
    test = (Wt::WLineEdit*)(*arguments.begin());
    message.addBodyText("{\n\"token\": \"" + (*it).second
                        + "\",\n\"value\":\"" + boost::lexical_cast<std::string>(test->text()) + "\"\n}");

    std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string>(id)
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "UserEditionWidget : [PUT] address to call : " << apiAddress;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::putMedia, this, _1, _2));
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
}

void UserEditionWidget::close()
{
    delete this;
}

// API RETURN INFOS ------------------------------------------

void UserEditionWidget::deleteMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Delete Media : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
        }
        else
        {
            Wt::log("error") << "[User Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.media-user.database-error-title"), tr("Alert.media-user.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[User Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.media-user.database-error-title"), tr("Alert.media-user.database-error"), Wt::Ok);
    }
    recoverListMedia();
}

void UserEditionWidget::postMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[User Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.media-user.database-error-title"), tr("Alert.media-user.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[User Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "TypeException", tr("Alert.media-user.database-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[User Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "status", tr("Alert.media-user.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[User Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "err", tr("Alert.media-user.database-error"), Wt::Ok);
    }
    recoverListMedia();
}

void UserEditionWidget::putMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;

    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[User Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.media-user.database-error-title"), tr("Alert.media-user.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[User Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "TypeException", tr("Alert.media-user.database-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[User Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "status", tr("Alert.media-user.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[User Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.media-user.database-error-title") + "err", tr("Alert.media-user.database-error"), Wt::Ok);
    }
    recoverListMedia();
}


