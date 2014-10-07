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

using namespace std;

InformationsWidget::InformationsWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "information")
{
    session_ = session;
    m_unitComboBox = new Wt::WComboBox();
    m_unitModel = new Wt::WStandardItemModel(0,2,this);

    setButtonModif(true);
    setButtonSup(true);
//    setLocalTable(true);

    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isnot), "desc"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::isnot), "calculate"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::undid, 0, EMandatory::is), "information_unit"));
    
    std::vector<std::string> undidNames;
    undidNames.push_back("name");
    setUndidName(undidNames);
    titles.push_back(make_pair(setValidatorType(ETypeJson::boolean, 0, EMandatory::is), "display"));
    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("informations", vector<string>())); 
    listUrl.push_back(pair<string, vector<string>>("informations/:id", vector<string>()));    
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back(pair<string, vector<string>>("units", vector<string>()));   
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
}

/*
Wt::WValidator *InformationsWidget::editValidator(int jsonType)
{
    Wt::WRegExpValidator *validator;
    switch(jsonType)
    {
        case ETypeJson::text:
        {
            validator = new Wt::WRegExpValidator("^.+$");
            validator->setMandatory(false);
            break;
        }
        case ETypeJson::number:
        {
            validator = new Wt::WRegExpValidator("[^\\\\<>/.&;?!§,{}()*|\"]{1,255}");
            validator->setMandatory(true);
            break;
        }
        default:
        {
            break;
        }
    }
    return validator;
}
*/

Wt::WComboBox *InformationsWidget::popupAdd(Wt::WDialog *dialog)
{
    m_unitComboBox = new Wt::WComboBox(dialog->contents());
    m_unitComboBox->setModel(m_unitModel);
    // Fixme : WTF ?
    return m_unitComboBox;
}

void InformationsWidget::addResource(vector<Wt::WInteractWidget*>* arguments)
{    
    vector<Wt::WInteractWidget*>::iterator it = (*arguments).begin();
    // Post Information -------
    Wt::Http::Message messageInformation;
    
    messageInformation.addBodyText("{");
    messageInformation.addBodyText("\n\"name\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    messageInformation.addBodyText(",\n\"desc\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    if (!((Wt::WLineEdit*)(*it++))->text().toUTF8().empty())
    {
        messageInformation.addBodyText(",\n\"calculate\": \"" + ((Wt::WLineEdit*)(*it++))->text().toUTF8() + "\"");
    }
    if (((Wt::WCheckBox*)(*it++))->isChecked())
    {
        messageInformation.addBodyText(",\n\"display\": true");
    }
    else
    {
        messageInformation.addBodyText(",\n\"display\": false");
    }
    
    messageInformation.addBodyText(",\n\"unit_id\" : " + m_unitModel->item(m_unitComboBox->currentIndex(), 1)->text().toUTF8());

    

    messageInformation.addBodyText("\n}");
    
    string apiAddress = this->getApiUrl() + "/informations"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::postResourceCallback, this, _1, _2, client));

    Wt::log("debug") << "InformationsWidget : [POST] address to call : " << apiAddress;    

    if (client->post(apiAddress, messageInformation))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void InformationsWidget::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    vector<Wt::WInteractWidget*>::iterator i = arguments.begin();

    // Post Information -------
    Wt::Http::Message messageInformation;
    
    messageInformation.addBodyText("{");
    messageInformation.addBodyText("\n\"name\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
    messageInformation.addBodyText(",\n\"desc\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
    messageInformation.addBodyText(",\n\"calculate\" : \"" + ((Wt::WLineEdit*)(*i++))->text().toUTF8() + "\"");
    
    Wt::WCheckBox * displayBox = (Wt::WCheckBox*)(*i++);
    if (displayBox->isChecked())
    {
        messageInformation.addBodyText(",\n\"display\" : true");
    }
    else
    {
        messageInformation.addBodyText(",\n\"display\" : false");
    }

    
    Wt::WStandardItemModel *unitModel = (Wt::WStandardItemModel*)((Wt::WComboBox*)(*i))->model();
    // FIXME: segfault ici !
    messageInformation.addBodyText(",\n\"unit_id\" : " + unitModel->item(((Wt::WComboBox*)(*i++))->currentIndex(), 1)->text().toUTF8());

    
    messageInformation.addBodyText("\n}");
    
    string apiAddress = this->getApiUrl() + "/informations/"
            + boost::lexical_cast<string>(id)
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::putResourceCallback, this, _1, _2, client));

    Wt::log("debug") << "InformationsWidget : [PUT] address to call : " << apiAddress;    

    if (client->put(apiAddress, messageInformation))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void InformationsWidget::handleJsonGet(vectors_Json jsonResources)
{
    vector<Wt::Json::Value> infoUnit = jsonResources.at(1);
    jsonResources.pop_back();
    AbstractPage::handleJsonGet(jsonResources);

    m_unitModel->clear();

    try
    {
        Wt::Json::Array& result = infoUnit.at(0);
        for (int cpt(0); cpt < (int)result.size(); cpt++)
        {    
           Wt::WStandardItem *itemId = new Wt::WStandardItem();
           Wt::WStandardItem *itemName = new Wt::WStandardItem();

            Wt::Json::Object obj = result.at(cpt);
            Wt::WString name = obj.get("name");
            long long id = obj.get("id");

            vector<Wt::WStandardItem*> rowVector;

            itemId->setText(boost::lexical_cast<string>(id));
            itemName->setText(name);

            rowVector.push_back(itemName);
            rowVector.push_back(itemId);
            m_unitModel->insertRow(cpt, rowVector);
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[AbstractPage] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[AbstractPage] JSON Type Exception";
//            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
}

Wt::WDialog *InformationsWidget::deleteResource(long long id)
{
    Wt::WDialog *box = AbstractPage::deleteResource(id);
    // a REVOIR !! Récupération des alerts par rapport a id de l'asset a sup
    string apiAddress = this->getApiUrl() + "/informations/" + boost::lexical_cast<string> (id) + "/alerts/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&InformationsWidget::checkAlertsInInformation, this, _1, _2, box, id));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "InformationsWidget : [GET] address to call : " << apiAddress;
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
    return box;
}

void InformationsWidget::checkAlertsInInformation(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id)
{
    Wt::WApplication::instance()->resumeRendering();
    if (idsAlert_.size() > 0)
    {
        idsAlert_.clear();
    }
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
//            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Information Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
}


