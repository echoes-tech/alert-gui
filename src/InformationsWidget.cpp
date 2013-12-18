/* 
 * Informations Widget
 * @author ECHOES Technologies (MLA)
 * @date 12/12/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */



#include "InformationsWidget.h"

InformationsWidget::InformationsWidget(Echoes::Dbo::Session *session, std::string apiUrl)
: CreatePageWidget(session, apiUrl, "information")
{
    session_ = session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
}

// TABLE(S) FOR MAIN PAGE -------------------------------------

void InformationsWidget::update()
{
    CreatePageWidget::update();
    if (!newClass_)
    {
        newClass_ = true;
    }
}

// TABLE(S) FOR POPUP ------------------------------------------

void InformationsWidget::popupAddWidget(Wt::WDialog *dialog, long long id)
{
}

// GET INFO FOR MOTHER ---------------------------------------

std::vector<std::string> InformationsWidget::getTitlesTableWidget()
{
    std::vector<std::string> titleWidget;
    return titleWidget;
}

std::vector<std::string> InformationsWidget::getTitlesTableText()
{
    std::vector<std::string> titleText;
    titleText.push_back("name");
    titleText.push_back("desc");
    titleText.push_back("calculate");
    titleText.push_back("information_unit");
    titleText.push_back("display");
    return titleText;
}

std::vector<long long> InformationsWidget::getIdsTable()
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
            ids.push_back(tmp.get("id"));
        }
    }
    return ids;
}

vector_type InformationsWidget::getResourceRowTable(long long id)
{
    vector_type rowTable;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;
    long long i(0);

    if (result_.isNull() == false)
    {
        result1 = result_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
        {
            tmp = (*idx1);
            i = tmp.get("id");
            if (i == id)
            {
                Wt::WString name = "";
                Wt::WString desc = "";
                Wt::WString calculate = "";
                Wt::Json::Object information_unit;
                int information_unit_id = 0;
                bool display = "";


                // Name
                name = tmp.get("name");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(name)));

                // Desc                
                name = tmp.get("desc");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(desc)));
                
                // Calculate
                calculate = tmp.get("calculate");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(calculate)));

                // Information Unit
                information_unit = tmp.get("information_unit");
                information_unit_id = information_unit.get("id");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string>(information_unit_id)));
                
                // Display
                display = tmp.get("display");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string>(display)));
                
                return (rowTable);
            }
        }
    }
    return rowTable;
}

Wt::WValidator *InformationsWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator =
            new Wt::WRegExpValidator(REG_EXP);
    validator->setMandatory(true);
    return validator;
}

void InformationsWidget::closePopup()
{
    recoverListInformations();
}

void InformationsWidget::recoverListInformations()
{
    /* Connection API */
    std::string apiAddress = this->getApiUrl() + "/informations";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::getInformations, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "InformationsWidget : [GET] address to call : " << apiAddress;

    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void InformationsWidget::recoverInformation(int id)
{
    /* Connection API */
    std::string apiAddress = this->getApiUrl() + "/informations/" + boost::lexical_cast<std::string>(id);
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::getInformation, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "InformationsWidget : [GET] address to call : " << apiAddress;

    Wt::log("error") << apiAddress;
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

// Call API - POST(ADD) DELETE PUT(MODIF) ----------------------------------------

void InformationsWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{
    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();

    // Post Information -------
    Wt::Http::Message messageInformation;
    
    messageInformation.addBodyText("{");
    messageInformation.addBodyText("\n\"name\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
    messageInformation.addBodyText(",\n\"desc\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
    messageInformation.addBodyText(",\n\"calculate\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
    messageInformation.addBodyText(",\n\"unit_id\" : " + ((Wt::WLineEdit*)(*i++))->text().toUTF8());
    messageInformation.addBodyText(",\n\"display\" : " + ((Wt::WLineEdit*)(*i++))->text().toUTF8());
    messageInformation.addBodyText("\n}");

    std::string apiAddress = this->getApiUrl() + "/informations";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::postInformation, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "InformationsWidget : [POST] address to call : " << apiAddress;    

    if (client->post(apiAddress, messageInformation))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
}

Wt::WDialog *InformationsWidget::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    // a REVOIR !! Récupération des alerts par rapport a id de l'asset a sup
    std::string apiAddress = this->getApiUrl() + "/informations/" + boost::lexical_cast<std::string> (id) + "/alerts/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::checkAlertsInInformation, this, _1, _2, box, id));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "InformationsWidget : [GET] address to call : " << apiAddress;
    if (client->get(apiAddress))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
    box->show();
    box->finished().connect(std::bind([ = ] (){
                                      if (box->result() == Wt::WDialog::Accepted)
        {
                                      Wt::Http::Message message;
                                      message.addBodyText("");
                                      std::string apiAddress = this->getApiUrl() + "/informations/" + boost::lexical_cast<std::string> (id)
                                      + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

                                      Wt::Http::Client *client = new Wt::Http::Client(this);
                                      client->done().connect(boost::bind(&InformationsWidget::deleteInformation, this, _1, _2));
                                      Wt::log("debug") << "InformationsWidget : [DELETE] address to call : " << apiAddress;
                                      if (client->deleteRequest(apiAddress, message))
                                      Wt::WApplication::instance()->deferRendering();
            else
                                      Wt::log("error") << "Error Client Http";
            }
                                      return box;
    }));
    return box;
}

void InformationsWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
//    std::vector<Wt::WInteractWidget*>::iterator i = arguments.begin();
//    std::string messageString;
//    
//    messageString = ("{");
//    messageString += ("\n\"name\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
//    messageString += (",\n\"desc\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
//    messageString += (",\n\"calculate\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
//    messageString += (",\n\"unit_id\" : " + ((Wt::WLineEdit*)(*i++))->text().toUTF8());
//    messageString += (",\n\"display\" : " + ((Wt::WLineEdit*)(*i++))->text().toUTF8());
//    messageString += ("\n}");
//
//    Wt::Http::Message message;
//    message.addBodyText(messageString);
//
//    std::string apiAddress = this->getApiUrl() + "/informations/" + boost::lexical_cast<std::string> (id)
//            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
//            + "&token=" + session_->user()->token.toUTF8();
//
//    Wt::Http::Client *client = new Wt::Http::Client(this);
//    client->done().connect(boost::bind(&InformationsWidget::putInformation, this, _1, _2));
//    Wt::log("error") << "InformationsWidget : [PUT] address to call : " << apiAddress;
//    Wt::log("error") << "InformationsWidget : [POST] address to call : " << messageString;
//    if (client->put(apiAddress, message))
//        Wt::WApplication::instance()->deferRendering();
//    else
//        Wt::log("error") << "[InformationsWidget] Error Client Http";
}

void InformationsWidget::close()
{
    delete this;
}


// API RETURN INFOS ------------------------------------------

void InformationsWidget::deleteInformation(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        std::cout << response.status() << std::endl;
        if (response.status() == Enums::EReturnCode::NO_CONTENT)
        {
            if (response.body() != "")
            {
                Wt::log("error") << "[Information Management Widget] Response should be empty : " << response.body() << ".";
            }
        }
        else
        {
            Wt::log("error") << "[Information Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.asset.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.asset.database-error"), Wt::Ok);
    }
    recoverListInformations();
}

void InformationsWidget::checkAlertsInInformation(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id)
{
    Wt::WApplication::instance()->resumeRendering();
    if (idsAlert_.size() > 0)
        idsAlert_.clear();
    if (!err)
    {
        try
        {
            if (response.status() == 200)
            {
                Wt::Json::Value result;
                Wt::Json::Array& result1 = Wt::Json::Array::Empty;
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Object tmp;

                Wt::WString textInJSON;
                Wt::WString textInBox;

                Wt::Json::parse(response.body(), result);
                result1 = result;
                //Récupération de l'id, grâce au renvois du JSON.
                int i(0);
                textInJSON += "<ul>";
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    Wt::WString name = tmp.get("name");
                    idsAlert_.push_back(tmp.get("id"));
                    textInJSON += "<li> " + name + "</li>";
                    i++;
                }
                textInJSON += "</ul>";
                if (i > 1)
                    textInBox = "<p>" + tr("Alert.alert-list.delete-alerts-message") + "</p>";
                else
                    textInBox = "<p>" + tr("Alert.alert-list.delete-alert-message") + "</p>";
                textInBox += textInJSON;
                box->contents()->addWidget(new Wt::WText(textInBox));
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[Information Management Widget] Problems parsing JSON: " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[Information Management Widget] JSON Type Exception: " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
}

void InformationsWidget::getInformations(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Reponse : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    result_ = 0;
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), result_);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Information Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Information Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        else if (response.status() == 404)
        {
            result_ = Wt::Json::Value::Null;
        }
        else
        {
            result_ = Wt::Json::Value::Null;
            Wt::log("error") << "[Information Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
    update();
}

void InformationsWidget::getInformation(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Reponse : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    Wt::log("error") << "debug";
    result_row_ = 0;
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), result_row_);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Information Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Information Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        else
        {
            result_row_ = Wt::Json::Value::Null;
            Wt::log("error") << "[Information Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
}

void InformationsWidget::postInformation(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Reponse : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    Wt::WString id;
    if (!err)
    {
        if (response.status() == 201)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                // ToDo : check object
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Information Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Information Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Information Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
    recoverListInformations();
}

void InformationsWidget::putInformation(boost::system::error_code err, const Wt::Http::Message& response)
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
                Wt::log("warning") << "[Information Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Information Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Information Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
    recoverListInformations();
}

void InformationsWidget::postPlugin(boost::system::error_code err, const Wt::Http::Message& response)
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
                Wt::log("warning") << "[Information Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Information Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Information Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
}

// ----------------------------------------------



