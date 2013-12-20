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
: AbstractPage(session, apiUrl, "information")
{
    session_ = session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);

    vector_pair_string titles;
    titles.push_back(std::make_pair(ETypeJson::string, "name"));
    titles.push_back(std::make_pair(ETypeJson::string, "desc"));
    titles.push_back(std::make_pair(ETypeJson::string, "calculate"));
    titles.push_back(std::make_pair(ETypeJson::undid, "information_unit"));
    titles.push_back(std::make_pair(ETypeJson::boolean, "display"));
    setTitles(titles);

    lists_string lListUrl;
    list_string listUrl;
    listUrl.push_back("informations");
    listUrl.push_back("informations/:id");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(lListUrl);
}

Wt::WValidator *InformationsWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator =
            new Wt::WRegExpValidator("[^\\\\<>/.&;?!§,{}()*|\"]{1,255}");
    validator->setMandatory(true);
    return validator;
}

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

    std::string apiAddress = this->getApiUrl() + "/informations"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::returnApiPostResource, this, _1, _2, client));

    Wt::log("debug") << "InformationsWidget : [POST] address to call : " << apiAddress;    

    if (client->post(apiAddress, messageInformation))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
}

Wt::WDialog *InformationsWidget::deleteResource(long long id)
{
    Wt::WDialog *box = AbstractPage::deleteResource(id);
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
    return box;
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


