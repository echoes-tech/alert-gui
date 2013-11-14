/* 
 * File:   UserEditionWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "UserEditionWidget.h"

UserEditionWidget::UserEditionWidget(Session *session, std::string apiUrl, int type)
: CreatePageWidget("media-user")
{
    this->session_= session;
    this->apiUrl_ = apiUrl;
    this->created_ = false;
    this->newClass_ = false;
    this->type_ = type;
    
    this->result_ = Wt::Json::Value::Null;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
}



void    UserEditionWidget::update()
{
    CreatePageWidget::update();
   if (!newClass_)
    {
       newClass_ = true;
    }
}

void  UserEditionWidget::popupAddTables(Wt::WTabWidget *tabW) { }

std::vector<std::string>        UserEditionWidget::getTitlesTableWidget()
{
    std::vector<std::string>    titleWidget;
    return titleWidget;
}

std::vector<std::string>        UserEditionWidget::getTitlesTableText()
{
   std::vector<std::string>     titleText;
   titleText.empty();
   if (type_ == 1)
       titleText.push_back("mail");
   else if (type_ == 2)
       titleText.push_back("sms");
   else if (type_ == 3)
       titleText.push_back("app");
   return titleText;
}

std::vector<long long>          UserEditionWidget::getIdsTable()
{
    std::vector<long long>      ids;

    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;

    if (result_.isNull() == false)
    {
        result1 = result_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
    {
        tmp = (*idx1);
        ids.push_back(tmp.get("id"));
    }
    }
    return ids;
}

vector_type     UserEditionWidget::getResourceRowTable(long long id)
{
    vector_type    rowTable;
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

Wt::WValidator    *UserEditionWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator = 0;
    if (type_ == 1)
        validator = new Wt::WRegExpValidator("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");
    else if (type_ == 2)
        validator = new Wt::WRegExpValidator("^(0|\\+33)[0-9]{9}");
    return validator;
}

void  UserEditionWidget::closePopup()
{
    recoverListAsset();
}

void    UserEditionWidget::recoverListAsset()
{

    std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string>(this->type_) + "/list-media";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::getMedia, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        std::cout << "Error Client Http" << std::endl;
}



void UserEditionWidget::getMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    result_ = 0;
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), result_);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            this->result_ = Wt::Json::Value::Null;
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
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
    messageAsset.addBodyText("{\n\"med_id\": " + boost::lexical_cast<std::string>(this->type_) +
            ",\n\"mev_value\": \"" + boost::lexical_cast<std::string>(assetEdit->text()) + "\"\n}");
    
    std::string apiAddress = this->getApiUrl() + "/medias/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::postMedia, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->post(apiAddress, messageAsset))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
}

Wt::WDialog *UserEditionWidget::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    box->show();
    box->finished().connect(std::bind([=] () {
        if (box->result() == Wt::WDialog::Accepted)
        {
            Wt::Http::Message message;
            message.addBodyText("");
            std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string> (id);
            Wt::Http::Client *client = new Wt::Http::Client(this);
            client->done().connect(boost::bind(&UserEditionWidget::deleteMedia, this, _1, _2));
            apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
            if (client->deleteRequest(apiAddress, message))
                Wt::WApplication::instance()->deferRendering();
            else
                std::cout << "Error Client Http" << std::endl;
        }
        return box;
    }));
    return box;
}

void UserEditionWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
    Wt::Http::Message message;
    
    Wt::WLineEdit *test;
    test = (Wt::WLineEdit*)(*arguments.begin());
    message.addBodyText("{\n\"med_id\": " + boost::lexical_cast<std::string>(this->type_)
            + ",\n\"mev_value\":\"" + boost::lexical_cast<std::string>(test->text()) + "\"\n}");

    std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string>(id);
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::putMedia, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
}

void UserEditionWidget::close()
{
    delete this;
}

void    UserEditionWidget::setSession(Session *session)
{
    session_ = session;
}

void    UserEditionWidget::setApiUrl(std::string apiUrl)
{
    apiUrl_ = apiUrl;
}

std::string   UserEditionWidget::getApiUrl()
{
    return apiUrl_;
}

// API RETURN INFOS ------------------------------------------


void    UserEditionWidget::deleteMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else if (response.status() == 409)
        {
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status", "Conflict, an alert use this media",Wt::Ok);
        }
        else
        {
            Wt::log("error") << "[Alert Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
    recoverListAsset();
}

void    UserEditionWidget::postMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Alert Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
        recoverListAsset();
}

void UserEditionWidget::putMedia(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
   
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Alert Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
        recoverListAsset();
}