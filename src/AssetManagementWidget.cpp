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

#include <Wt/WTheme>

#include "AssetManagementWidget.h"

AssetManagementWidget::AssetManagementWidget(Echoes::Dbo::Session *session, std::string apiUrl)
: CreatePageWidget("asset")
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

void  AssetManagementWidget::popupAddWidget(Wt::WDialog *dialog)
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

std::vector<long long>          AssetManagementWidget::getIdsTable()
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

vector_type     AssetManagementWidget::getResourceRowTable(long long id)
{
    vector_type    rowTable;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;
    Wt::WFileResource *file = 0;
    Wt::WString nameAsset = "";
    long long i(0); 

    if (result_.isNull() == false)
    {
        result1 = result_;
    for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
    {
        //gkr: Add name Asset for row, and add id for sort.
        tmp = (*idx1);
        nameAsset = tmp.get("name");
        i = tmp.get("id");
        if (i == id)
        {
            rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(nameAsset)));

            //gkr: Add button for row.
            file = generateScript(boost::lexical_cast<long long>(i), nameAsset);
            Wt::WAnchor *downloadButton = new Wt::WAnchor(file,"");
            downloadButton->setAttributeValue("class","btn btn-info");
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
    std::string apiAddress = this->getApiUrl() + "/assets";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::getAssets, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        std::cout << "Error Client Http" << std::endl;
}

// Call API - POST(ADD) DELETE PUT(MODIF) ----------------------------------------

void AssetManagementWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{
    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();
    Wt::WLineEdit *assetEdit = (Wt::WLineEdit*)(*i);
    
    // Post Asset -------
    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\"name\" : \""
    + boost::lexical_cast<std::string>(assetEdit->text()) + "\"\n}\n");

    std::string apiAddress = this->getApiUrl() + "/assets";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::postAsset, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    if (client->post(apiAddress, messageAsset))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
}

Wt::WDialog *AssetManagementWidget::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id) + "/alerts/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::checkAlertsInAsset, this, _1, _2, box, id));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    if (client->get(apiAddress))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
    box->show();
    box->finished().connect(std::bind([=] () {
        if (box->result() == Wt::WDialog::Accepted)
        {
            Wt::Http::Message message;
            message.addBodyText("");
            std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id);
            Wt::Http::Client *client = new Wt::Http::Client(this);
            client->done().connect(boost::bind(&AssetManagementWidget::deleteAsset, this, _1, _2));
            apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
            if (client->deleteRequest(apiAddress, message))
                Wt::WApplication::instance()->deferRendering();
            else
                std::cout << "Error Client Http" << std::endl;
        }
        return box;
    }));
    return box;
}

void AssetManagementWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
    Wt::Http::Message message;
    
    Wt::WLineEdit *test;
    for (std::vector<Wt::WInteractWidget*>::iterator i = arguments.begin(); i != arguments.end(); i++)
    {
        test = (Wt::WLineEdit*)(*i);
        if (i == arguments.begin())
            message.addBodyText("{\"name\":\"" + boost::lexical_cast<std::string>(test->text()) + "\"}");
    }

    std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id);
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AssetManagementWidget::putAsset, this, _1, _2));
    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
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
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
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
                    textInBox = "<p>" + tr("Alert.alert-list.delete-alerts-message") + "</p>";
                else
                    textInBox = "<p>" + tr("Alert.alert-list.delete-alert-message") + "</p>";
                textInBox += textInJSON;
                box->contents()->addWidget(new Wt::WText(textInBox));
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"), tr("Alert.alert.database-error"),Wt::Ok);
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException", tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err", tr("Alert.alert.database-error"),Wt::Ok);
    }
}


void AssetManagementWidget::getAssets(boost::system::error_code err, const Wt::Http::Message& response)
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
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            result_ = Wt::Json::Value::Null;
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
   newClass_ = false;
   created_ = false;
   update();
}

void    AssetManagementWidget::postAsset(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    Wt::WString id;
    if (!err)
    {
        if(response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::Object result;
                Wt::Json::parse(response.body(), result);
                if (result.get("id").isNull())
                {
                    Wt::log("error") << "[Asset Management Widget] ID error";
                    Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
                }
                else
                {
                    id = result.get("id");
                    UserActionManagement::registerUserAction(Enums::add, Echoes::Dbo::Constants::T_ASSET_AST, boost::lexical_cast<long long>(id));
                    
                    Wt::Http::Message messageAsset;
                    messageAsset.addBodyText("");

                    std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string> (id) + "/plugin/";
                    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();

                    Wt::Http::Client *client = new Wt::Http::Client(this);
                    client->done().connect(boost::bind(&AssetManagementWidget::postPlugin, this, _1, _2));
                    if (client->post(apiAddress, messageAsset))
                        Wt::WApplication::instance()->deferRendering();
                    else
                        std::cout << "Error Client Http" << std::endl;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
        recoverListAsset();
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
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
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
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Asset Management Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Asset Management Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Asset Management Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
}

// SCRIPT DOWNLOAD ---------------------------------------

Wt::WFileResource *AssetManagementWidget::generateScript(long long i, Wt::WString assetName)
{
    // static part of file
    std::string disclaimerString = getStringFromFile("resources/scripts/disclaimer");
    std::string bodyString = getStringFromFile("resources/scripts/scriptbody");
    
    // custom part
    std::string scriptCustomPart = "";
    try
    {
        scriptCustomPart = "\nLOGIN=\"" + this->session_->user()->eMail.toUTF8() + "\"\n" 
        + "ASSET_ID=" + boost::lexical_cast<std::string, long long>(i) + "\n"
        + "TOKEN=\"" + this->session_->user()->organization.get()->token.toUTF8() + "\"\n";
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
    Wt::log("debug") << "[AssetManagementWidget] " << "Res temp file creation : " << mkstempRes;
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

void    AssetManagementWidget::setSession(Echoes::Dbo::Session *session)
{
    session_ = session;
}

void    AssetManagementWidget::setApiUrl(std::string apiUrl)
{
    apiUrl_ = apiUrl;
}

std::string   AssetManagementWidget::getApiUrl()
{
    return apiUrl_;
}


