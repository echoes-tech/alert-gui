/* 
 * File:   UserMailWidget.cpp
 * Author: gkr
 * 
 * Created on 6 novembre 2013, 17:49
 */

#include "UserMailWidget.h"

UserMailWidget::UserMailWidget(Session *session, std::string apiUrl)
: CreatePageWidget("media-mail")
{
    this->session_= session;
    this->apiUrl_ = apiUrl;
    this->created_ = false;
    this->newClass_ = false;
    
    this->result_ = Wt::Json::Value::Null;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);

}

void    UserMailWidget::update()
{
    CreatePageWidget::update();
   if (!newClass_)
    {
       newClass_ = true;
    }
}

void  UserMailWidget::popupAddTables(Wt::WTabWidget *tabW) { }

pair_type        UserMailWidget::getTitlesTableWidget()
{
    pair_type   titleWidget;
    return titleWidget;
}

std::vector<std::string>        UserMailWidget::getTitlesTableText()
{
   std::vector<std::string>     titleText;
   titleText.push_back("mail");
   return titleText;
}

std::vector<long long>          UserMailWidget::getIdsTable()
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

vector_type     UserMailWidget::getResourceRowTable(long long id)
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

Wt::WValidator    *UserMailWidget::editValidator(int who)
{
    Wt::WValidator *validator = 0;
    return validator;
}

void  UserMailWidget::closePopup()
{
    recoverListAsset();
}

void    UserMailWidget::recoverListAsset()
{

    std::string apiAddress = this->getApiUrl() + "/medias/1/list-media";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserMailWidget::getMail, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        std::cout << "Error Client Http" << std::endl;
}



void UserMailWidget::getMail(boost::system::error_code err, const Wt::Http::Message& response)
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
            /*
            Wt::log("error") << "[Alert Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
             * */
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

void UserMailWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{
    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();
    Wt::WLineEdit *assetEdit = (Wt::WLineEdit*)(*i);

    try
    {
        Wt::Dbo::Transaction transaction(*this->session_);
        Wt::Dbo::ptr<User> ptrUser = this->session_->user();
        Wt::Dbo::ptr<Media> media = this->session_->find<Media>().where("\"MED_ID\" = ?").bind(1);
        MediaValue *mev = new MediaValue();
        mev->user = ptrUser;
        mev->media = media;
        mev->value = boost::lexical_cast<std::string>(assetEdit->text());
        mev->isDefault = false;
        mev->isConfirmed = false;
        mev->token = Wt::WRandom::generateId(25);
        Wt::Dbo::ptr<MediaValue> ptrMev = this->session_->add<MediaValue>(mev);
        ptrMev.flush();
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[UserEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.user.problem-adding-media-title"),tr("Alert.user.problem-adding-media"),Wt::Ok);
    }

/*
    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\"name\" : \""
    + boost::lexical_cast<std::string>(assetEdit->text()) + "\"\n}\n");

    std::string apiAddress = this->getApiUrl() + "/assets";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::postAsset, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->post(apiAddress, messageAsset))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
    */
    recoverListAsset();
}

Wt::WDialog *UserMailWidget::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    box->show();
    box->finished().connect(std::bind([=] () {
        if (box->result() == Wt::WDialog::Accepted)
        {
            try
            {
                Wt::Dbo::Transaction transaction2(*this->session_);
        
                std::string qryString = "DELETE FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" "
                                " WHERE \"AMS_ALE_ALE_ID\" IS NULL"
                                " AND \"AMS_MEV_MEV_ID\" IN "
                                " (SELECT \"MEV_ID\" FROM \"T_MEDIA_VALUE_MEV\" WHERE \"MEV_USR_USR_ID\" = "
                        + boost::lexical_cast<std::string>(this->session_->user().id()) + ")";
                this->session_->execute(qryString);
                transaction2.commit();
            }
            catch (Wt::Dbo::Exception e)
            {
                Wt::log("error") << "[UserEditionWidget] [deleteMedia]" << e.what();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
                return box;
            }
            try
            {
                Wt::Dbo::Transaction transaction(*this->session_);
                Wt::Dbo::ptr<MediaValue> mediaValue = this->session_->find<MediaValue>().where("\"MEV_ID\" = ?").bind(id)
                                                                           .where("\"MEV_MED_MED_ID\" = ?").bind(1)
                                                                           .where("\"MEV_USR_USR_ID\" = ?").bind(this->session_->user().id());
                
                mediaValue.modify()->deleteTag = Wt::WDateTime::currentDateTime();

                transaction.commit();
                UserActionManagement::registerUserAction(Enums::del,Constants::T_MEDIA_VALUE_MEV,id);
            }
            catch (Wt::Dbo::Exception e)
            {
                Wt::log("error") << "[UserEditionWidget] " << e.what();
                Wt::WMessageBox::show(tr("Alert.user.dependant-alert-exists-title"),tr("Alert.user.dependant-alert-exists"),Wt::Ok);
                return box;
            }
            
            
            /*
            Wt::Http::Message message;
            message.addBodyText("");
            std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id);
            Wt::Http::Client *client = new Wt::Http::Client(this);
            client->done().connect(boost::bind(&AssetManagementWidget::deleteAsset, this, _1, _2));
            apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
            if (client->deleteRequest(apiAddress, message))
                Wt::WApplication::instance()->deferRendering();
            else
                std::cout << "Error Client Http" << std::endl;
            */
        }
        recoverListAsset();
        return box;
    }));
    return box;
}

void UserMailWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
    Wt::Http::Message message;
    
    Wt::WLineEdit *test;
    for (std::vector<Wt::WInteractWidget*>::iterator i = arguments.begin(); i != arguments.end(); i++)
    {
        test = (Wt::WLineEdit*)(*i);
        if (i == arguments.begin())
            message.addBodyText("{\"name\":\"" + boost::lexical_cast<std::string>(test->text()) + "\"}");
    }

    /*
    std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id);
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::putAsset, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
     */
    recoverListAsset();
}

void UserMailWidget::close()
{
    delete this;
}

void    UserMailWidget::setSession(Session *session)
{
    session_ = session;
}

void    UserMailWidget::setApiUrl(std::string apiUrl)
{
    apiUrl_ = apiUrl;
}

std::string   UserMailWidget::getApiUrl()
{
    return apiUrl_;
}

