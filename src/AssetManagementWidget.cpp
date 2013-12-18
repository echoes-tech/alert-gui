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

AssetManagementWidget::AssetManagementWidget(Echoes::Dbo::Session *session, std::string apiUrl)
: CreatePageWidget(session, apiUrl, "asset")
{
    session_= session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
}

// TABLE(S) FOR MAIN PAGE -------------------------------------

void    AssetManagementWidget::update()
{
    CreatePageWidget::update();
   if (!newClass_)
    {
       newClass_ = true;
    }
}

// TABLE(S) FOR POPUP ------------------------------------------

void  AssetManagementWidget::popupAddWidget(Wt::WDialog *dialog, long long id)
{
}

// GET INFO FOR MOTHER ---------------------------------------

std::vector<std::string>        AssetManagementWidget::getTitlesTableWidget()
{
    std::vector<std::string>    titleWidget;
    titleWidget.push_back("download-script");
    return titleWidget;
}

std::vector<std::string>        AssetManagementWidget::getTitlesTableText()
{
   std::vector<std::string>     titleText;
   titleText.push_back("asset");
   return titleText;
}

std::vector<long long> AssetManagementWidget::getIdsTable()
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

vector_type AssetManagementWidget::getResourceRowTable(long long id)
{
    vector_type rowTable;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;
    Wt::WFileResource *file = 0;
    Wt::WString nameAsset = "";
    long long astId(0);

    if (result_.isNull() == false)
    {
        result1 = result_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
        {
            //gkr: Add name Asset for row, and add id for sort.
            tmp = (*idx1);
            nameAsset = tmp.get("name");
            astId = tmp.get("id");
            if (astId == id)
            {
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(nameAsset)));

                //gkr: Add button for row.
                file = generateScript(astId, nameAsset);
                Wt::WAnchor *downloadButton = new Wt::WAnchor(file, "");
                downloadButton->setAttributeValue("class", "btn btn-info");
                downloadButton->setTextFormat(Wt::XHTMLUnsafeText);
                downloadButton->setText("<span class='input-group-btn'><i class='icon-download icon-white'></i></span>");
                downloadButton->clicked().connect(boost::bind(&AssetManagementWidget::downloadScript, this, file->fileName()));
                rowTable.push_back(downloadButton);
                return (rowTable);
            }
        }
    }
    return rowTable;
}

Wt::WValidator    *AssetManagementWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator = 
            new Wt::WRegExpValidator(REG_EXP);
    validator->setMandatory(true);
    return validator;
}

void  AssetManagementWidget::closePopup()
{
    recoverListAsset();
}

void    AssetManagementWidget::recoverListAsset()
{
    /* Connection API */
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::getAssets, this, _1, _2));
    const std::string apiAddress = this->getApiUrl() + "/assets"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::log("debug") << "AssetManagementWidget : [GET] address to call : " << apiAddress;

    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

// Call API - POST(ADD) DELETE PUT(MODIF) ----------------------------------------

void AssetManagementWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{
    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();
    Wt::WLineEdit *assetEdit = (Wt::WLineEdit*)(*i);
    
    // Post Asset -------
    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\t\"name\": \"" + assetEdit->text().toUTF8() + "\"\n}");

    std::string apiAddress = this->getApiUrl() + "/assets";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::postAsset, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

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

Wt::WDialog *AssetManagementWidget::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    // a REVOIR !! Récupération des alerts par rapport a id de l'asset a sup
    std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id) + "/alerts/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::checkAlertsInAsset, this, _1, _2, box, id));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "AssetManagementWidget : [GET] address to call : " << apiAddress;
    if (client->get(apiAddress))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "Error Client Http";
    box->show();
    box->finished().connect(std::bind([=] () {
        if (box->result() == Wt::WDialog::Accepted)
        {
            Wt::Http::Message message;
            message.addBodyText("");
            std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id)
                    + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();

            Wt::Http::Client *client = new Wt::Http::Client(this);
            client->done().connect(boost::bind(&AssetManagementWidget::deleteAsset, this, _1, _2));
            Wt::log("debug") << "AssetManagementWidget : [DELETE] address to call : " << apiAddress;
            if (client->deleteRequest(apiAddress, message))
                Wt::WApplication::instance()->deferRendering();
            else
                Wt::log("error") << "Error Client Http";
        }
        return box;
    }));
    return box;
}

void AssetManagementWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
    std::string messageString;

    messageString = "{\n\"name\":\"" + boost::lexical_cast<std::string>(((Wt::WLineEdit*)(*arguments.begin()))->text()) + "\"\n}";

    Wt::Http::Message message;
    message.addBodyText(messageString);

    std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id)
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::putAsset, this, _1, _2));
    Wt::log("debug") << "AssetManagementWidget : [PUT] address to call : " << apiAddress;
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        Wt::log("error") << "[AssetManagementWidget] Error Client Http";
}

void AssetManagementWidget::close()
{
    delete this;
}


// API RETURN INFOS ------------------------------------------


void    AssetManagementWidget::deleteAsset(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        std::cout << response.status() << std::endl;
        if(response.status() == Enums::EReturnCode::NO_CONTENT)
        {
            if (response.body() != "")
            {
                Wt::log("error") << "[Asset Management Widget] Response should be empty : " << response.body() << ".";
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
    recoverListAsset();
}

void    AssetManagementWidget::checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id)
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


void AssetManagementWidget::getAssets(boost::system::error_code err, const Wt::Http::Message& response)
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
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
            }
        }
        else
        {
            result_ = Wt::Json::Value::Null;
            //            Wt::log("error") << "[Asset Management Widget] " << response.body();
            //            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status",tr("Alert.asset.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
    update();
}

void AssetManagementWidget::postAsset(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Reponse : " << std::endl << response.body() << std::endl;
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
                messageAsset.addBodyText("{\n\t\"name\": \"Probe " + astName.toUTF8() + "\",\n\t\"asset_id\": " + boost::lexical_cast<std::string>(astId) + "\n}");

                const std::string apiAddress = this->getApiUrl() + "/probes"
                        + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                        + "&token=" + session_->user()->token.toUTF8();
                Wt::Http::Client *client = new Wt::Http::Client(this);
                client->done().connect(boost::bind(&AssetManagementWidget::postProbe, this, _1, _2));

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
    recoverListAsset();
}

void AssetManagementWidget::postProbe(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << "Reponse : " << std::endl << response.body() << std::endl;
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

void AssetManagementWidget::putAsset(boost::system::error_code err, const Wt::Http::Message& response) 
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
   
    if (!err)
    {
        if(response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
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
        recoverListAsset();
}

void    AssetManagementWidget::postPlugin(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::Json::Value error;
   
    if (!err)
    {
        if(response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::parse(response.body(), error);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException",tr("Alert.asset.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status",tr("Alert.asset.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err",tr("Alert.asset.database-error"),Wt::Ok);
    }
}

// SCRIPT DOWNLOAD ---------------------------------------

Wt::WFileResource *AssetManagementWidget::generateScript(long long astId, Wt::WString assetName)
{
    // static part of file
    std::string disclaimerString = getStringFromFile("resources/scripts/disclaimer");
    std::string bodyString = getStringFromFile("resources/scripts/scriptbody");

    // custom part
    std::string scriptCustomPart = "";
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session_));
        scriptCustomPart = "\nLOGIN='" + Wt::Utils::urlEncode(this->session_->user()->eMail.toUTF8()) + "'\n"
                + "ASSET_ID=" + boost::lexical_cast<std::string>(astId) + "\n"
        //TEMPORARY!! ToDo: Implement a method to retrieve id Porbe for this Asset
                + "PROBE_ID=" + boost::lexical_cast<std::string>(astId) + "\n"
                + "TOKEN='" + this->session_->user()->organization.get()->token.toUTF8() + "'\n";
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
        return NULL;
    }

    // full script to send
    std::string contentToSend = disclaimerString + scriptCustomPart + bodyString;

    // temp file
    char *tmpname = strdup("/tmp/echoes-tmp-fileXXXXXX");
    int mkstempRes = mkstemp(tmpname);
    Wt::log("debug") << "[AssetManagementWidget] Res temp file creation: " << mkstempRes;
    std::ofstream f(tmpname);
    f << contentToSend;
    f.close();

    std::string assetNameSpacesReplaced = assetName.toUTF8();
    boost::replace_all(assetNameSpacesReplaced, " ", "_");

    // creating resource to send to the client
    Wt::WFileResource *res = new Wt::WFileResource();
    res->setFileName(tmpname);
    res->suggestFileName("ea-probe-install_" + assetNameSpacesReplaced + ".sh",Wt::WResource::Attachment);
    res->setMimeType("application/x-sh");

    return res;
}

std::string AssetManagementWidget::getStringFromFile(std::string resourcePath)
{
    std::string filePath = Wt::WApplication::instance()->appRoot()+resourcePath;
    std::ifstream file(filePath.c_str());
    
    std::string res = "";
    if (file.is_open())
    {
        std::string tmpResString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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

void AssetManagementWidget::downloadScript(std::string fileName)
{
    UserActionManagement::registerUserAction(Enums::download,fileName,0);
}

// ----------------------------------------------



