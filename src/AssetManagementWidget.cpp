/**
 * @file AssetManagementWidget.cpp
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

#include "AssetManagementWidget.h"

using namespace std;

AssetManagementWidget::AssetManagementWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "asset")
{
    setButtonModif(true);
    setButtonSup(true);
//    setLocalTable(true);

    std::vector<std::pair <int, string>>titles;
    titles.push_back(make_pair(setValidatorType(ETypeJson::text, 0, EMandatory::is), "name"));
    titles.push_back(make_pair(setValidatorType(ETypeJson::widget, 0, 0), "download-script"));
    setTitles(titles);
    
    list<list<pair<string, vector<string>>>> listsUrl;
    list<pair<string, vector<string>>> listUrl;
    
    listUrl.push_back(pair<string, vector<string>>("assets", vector<string>()));    
    listsUrl.push_back(listUrl);
    listUrl.clear();
    
    setUrl(listsUrl);
}

/*
Wt::WValidator    *AssetManagementWidget::editValidator(int cpt)
{
    Wt::WRegExpValidator *validator = 
            new Wt::WRegExpValidator("[^\\\\<>/&;?!§,{}()*|\"]{1,255}");
    validator->setMandatory(true);
    return validator;
}
*/
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

void AssetManagementWidget::apiDeleteResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == Enums::EReturnCode::NO_CONTENT)
        {
            if (response.body() != "")
            {
                Wt::log("error") << " [Asset Widget] Response should be empty : "
                        << response.body() << ".";
            }
        }
        else if (response.status() == Enums::EReturnCode::CONFLICT)
        {
            Wt::WMessageBox::show(tr("Alert.asset.conflict-title"), tr("Alert.asset.conflict"), Wt::Ok);
        }
        else
        {
            Wt::log("error") << "[Asset Widget] Unexpected return code : " << response.status();
            
            Wt::log("error") << "[Asset Widget] Response body : " << response.body();
            // FIXME: Useless once http://redmine.webtoolkit.eu/issues/3541 is implemented
            try
            {
                Wt::Json::Value result;
                Wt::Json::Object obj;

                Wt::Json::parse(response.body(), result); 
                obj = result;
                Wt::WString message = obj.get("message");
                
                if(L"Conflict" == message.value())
                {
                    Wt::WMessageBox::show(tr("Alert.asset.conflict-title"), tr("Alert.asset.conflict"), Wt::Ok);
                }
                else
                {
                    Wt::log("error") << "[Asset Widget] Unexpected message : " << message;
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
    }
    else
    {
        Wt::log("warning") << "[Asset Widget] Http::Client error: " << response.body();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),
                              tr("Alert.asset.database-error"), Wt::Ok);
    }
    updatePage();
}



// API RETURN INFOS ------------------------------------------


void    AssetManagementWidget::checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response,
                                                  Wt::Http::Client *client, Wt::WDialog *box, long long id)
{
    delete client;
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
                {
                    textInBox = "<p>" + tr("Alert.asset-list.delete-alerts-message") + "</p>";
                }
                else
                {
                    textInBox = "<p>" + tr("Alert.asset-list.delete-alert-message") + "</p>";
                }
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

                const string apiAddress = getApiUrl() + "/probes"
                        + "?login=" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8())
                        + "&token=" + m_session->user()->token.toUTF8();
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
    updatePage();
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
        Wt::Dbo::Transaction transaction(*(m_session));
        scriptCustomPart = "\nASSET_ID=" + boost::lexical_cast<string>(astId) + "\n"
                //TEMPORARY!! ToDo: Implement a method to retrieve id Probe for this Asset
                "PROBE_ID=" + boost::lexical_cast<string>(astId) + "\n"
                "TOKEN='" + m_session->user()->organization.get()->token.toUTF8() + "'\n\n"
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
        scriptCustomPart += "\nLOGIN='" + Wt::Utils::urlEncode(m_session->user()->eMail.toUTF8()) + "'\n";
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
    UserActionManagement::registerUserAction(Enums::EAction::download,fileName,0);
}

// ----------------------------------------------
