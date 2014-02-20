/* 
 * Asset Management Widget
 * @author ECHOES Technologies (TSA)
 * @date 14/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "AssetManagementWidget.h"

using namespace std;

AssetManagementWidget::AssetManagementWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "asset")
{
    session_= session;
    apiUrl_ = apiUrl;
//    created_ = false;
//    newClass_ = false;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);

    vector_pair_string titles;
    titles.push_back(make_pair(ETypeJson::text, "name"));
    titles.push_back(make_pair(ETypeJson::widget, "download-script"));
    setTitles(titles);

    lists_string lListUrl;
    list_string listUrl;
    listUrl.push_back("assets");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(lListUrl);
}

Wt::WValidator    *AssetManagementWidget::editValidator(int cpt)
{
    Wt::WRegExpValidator *validator = 
            new Wt::WRegExpValidator("[^\\\\<>/&;?!§,{}()*|\"]{1,255}");
    validator->setMandatory(true);
    return validator;
}

int AssetManagementWidget::addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable)
{
    Wt::WFileResource *file = generateScript(id, ((Wt::WText*)getResourceTable()->elementAt(rowTable, 0)->widget(0))->text());
    Wt::WAnchor *downloadButton = new Wt::WAnchor(file, "");
    downloadButton->setAttributeValue("class", "btn btn-info");
    downloadButton->setTextFormat(Wt::XHTMLUnsafeText);
    downloadButton->setText("<span class='input-group-btn'><i class='icon-download icon-white'></i></span>");
    downloadButton->clicked().connect(boost::bind(&AssetManagementWidget::downloadScript, this, file->fileName()));
    getResourceTable()->elementAt(rowTable, columnTable)->addWidget(downloadButton); 
    getResourceTable()->elementAt(rowTable, columnTable)->setWidth(Wt::WLength(Wt::WLength(5, Wt::WLength::Percentage)));
    getResourceTable()->elementAt(rowTable, columnTable)->setContentAlignment(Wt::AlignCenter);
    return ++columnTable;
}

// Call API - POST(ADD) DELETE PUT(MODIF) ----------------------------------------

Wt::WDialog *AssetManagementWidget::deleteResource(long long id)
{
    Wt::WDialog *box = AbstractPage::deleteResource(id);
    // a REVOIR !! Récupération des alerts par rapport a id de l'asset a sup
    string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<string> (id) + "/alerts/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::checkAlertsInAsset, this, _1, _2, client, box, id));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "AssetManagementWidget : [GET] address to call : " << apiAddress;
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

// API RETURN INFOS ------------------------------------------


void    AssetManagementWidget::checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response,
                                                  Wt::Http::Client *client, Wt::WDialog *box, long long id)
{
    delete client;
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
                int         i(0);
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
                    textInBox = "<p>" + tr("Alert.asset-list.delete-alerts-message") + "</p>";
                else
                    textInBox = "<p>" + tr("Alert.asset-list.delete-alert-message") + "</p>";
                textInBox += textInJSON;
                box->contents()->addWidget(new Wt::WText(textInBox));
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"),Wt::Ok);
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"),Wt::Ok);
    }
}

void AssetManagementWidget::postResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    long long astId;
    Wt::WString astName;

    if (!err)
    {
        if(response.status() == 201)
        {
            try
            {
                Wt::Json::Object result;
                Wt::Json::parse(response.body(), result);

                astId = result.get("id");
                astName = result.get("name");
                // ToDo : check object

                // Post Probe -------
                Wt::Http::Message messageAsset;
                messageAsset.addBodyText("{\n\t\"name\": \"Probe " + astName.toUTF8() + "\",\n\t\"asset_id\": " + boost::lexical_cast<string>(astId) + "\n}");

                const string apiAddress = this->getApiUrl() + "/probes"
                        + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                        + "&token=" + session_->user()->token.toUTF8();
                Wt::Http::Client *client = new Wt::Http::Client(this);
                client->done().connect(boost::bind(&AssetManagementWidget::postProbe, this, _1, _2, client));

                Wt::log("debug") << "AssetManagementWidget : [POST] address to call : " << apiAddress;

                if (client->post(apiAddress, messageAsset))
                {
                    Wt::WApplication::instance()->deferRendering();
                }
                else
                {
                    Wt::log("error") << "Error Client Http";
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
    }
    recursiveGetResources();
}

void AssetManagementWidget::postProbe(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();

    if (!err)
    {
        if(response.status() == 201)
        {
            try
            {
                Wt::Json::Object result;
                Wt::Json::parse(response.body(), result);

                // ToDo : check object
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
    }
}


// SCRIPT DOWNLOAD ---------------------------------------

Wt::WFileResource *AssetManagementWidget::generateScript(long long astId, Wt::WString assetName)
{
    // static part of file
    string disclaimerString = getStringFromFile("resources/scripts/disclaimer");
    string bodyString = getStringFromFile("resources/scripts/scriptbody");

    // custom part
    string scriptCustomPart = "";
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session_));
        scriptCustomPart = "\nASSET_ID=" + boost::lexical_cast<string>(astId) + "\n"
                //TEMPORARY!! ToDo: Implement a method to retrieve id Probe for this Asset
                "PROBE_ID=" + boost::lexical_cast<string>(astId) + "\n"
                "TOKEN='" + this->session_->user()->organization.get()->token.toUTF8() + "'\n\n"
                "API_HOST='" + conf.getApiHost() + "'\n"
                "API_PORT=" + boost::lexical_cast<string>(conf.getApiPort()) + "\n"
                "API_HTTPS=";
        if (conf.isApiHttps())
        {
            scriptCustomPart += "true";
        }
        else
        {
            scriptCustomPart += "false";
        }
        scriptCustomPart += "\nLOGIN='" + Wt::Utils::urlEncode(this->session_->user()->eMail.toUTF8()) + "'\n";
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
        return NULL;
    }

    // full script to send
    string contentToSend = disclaimerString + scriptCustomPart + bodyString;

    // temp file
    char *tmpname = strdup("/tmp/echoes-tmp-fileXXXXXX");
    int mkstempRes = mkstemp(tmpname);
    Wt::log("debug") << "[AssetManagementWidget] Res temp file creation: " << mkstempRes;
    ofstream f(tmpname);
    f << contentToSend;
    f.close();

    string assetNameSpacesReplaced = assetName.toUTF8();
    boost::replace_all(assetNameSpacesReplaced, " ", "_");

    // creating resource to send to the client
    Wt::WFileResource *res = new Wt::WFileResource();
    res->setFileName(tmpname);
    res->suggestFileName("ea-probe-install_" + assetNameSpacesReplaced + ".sh",Wt::WResource::Attachment);
    res->setMimeType("application/x-sh");

    return res;
}

string AssetManagementWidget::getStringFromFile(string resourcePath)
{
    string filePath = Wt::WApplication::instance()->appRoot()+resourcePath;
    ifstream file(filePath.c_str());
    
    string res = "";
    if (file.is_open())
    {
        string tmpResString((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        res = tmpResString;
        file.close();
    }
    else
    {
        Wt::log("error") << filePath;
        Wt::log("error") << "File not found.";
    }
    return res;
}

void AssetManagementWidget::downloadScript(string fileName)
{
    UserActionManagement::registerUserAction(Enums::download,fileName,0);
}

// ----------------------------------------------

void    AssetManagementWidget::setSession(Echoes::Dbo::Session *session)
{
    session_ = session;
}

void    AssetManagementWidget::setApiUrl(string apiUrl)
{
    apiUrl_ = apiUrl;
}

string   AssetManagementWidget::getApiUrl()
{
    return apiUrl_;
}
