/* 
 * File:   ClassTest.cpp
 * Author: gkr
 * 
 * Created on 23 octobre 2013, 14:24
 */

#include "ClassTest.h"

ClassTest::ClassTest(Session *session, std::string apiUrl)
: CreatePageWidget("test")
{
    
    session_= session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;
    setResourceNumberAdd(2);
}


void    ClassTest::update()
{
    CreatePageWidget::update();
   if (!newClass_)
    {

    }
}

std::vector<std::string>        ClassTest::getHeaderTableTitle()
{
   std::vector<std::string>     title;
   title.push_back("name");
   title.push_back("name");
   title.push_back("download-script");
   return title;
}

std::vector<long long>          ClassTest::getIdsTable()
{
    std::vector<long long>      ids;

    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;

    result1 = result_;
    for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
    {
        tmp = (*idx1);
        ids.push_back(tmp.get("id"));
    }
    return ids;
}

vector_type     ClassTest::getResourceRowTable(long long id)
{
    vector_type    rowTable;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Array::const_iterator idx1;
    Wt::WFileResource *file = 0;
    Wt::WString nameAsset = "";
    long long i(0); 

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
            rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(nameAsset)));

            //gkr: Add button for row.
            file = generateScript(boost::lexical_cast<long long>(i), nameAsset);
            Wt::WAnchor *downloadButton = new Wt::WAnchor(file,"");
            downloadButton->setAttributeValue("class","btn btn-info");
            downloadButton->setTextFormat(Wt::XHTMLUnsafeText);
            downloadButton->setText("<span class='input-group-btn'><i class='icon-download icon-white'></i></span>");
            downloadButton->clicked().connect(boost::bind(&ClassTest::downloadScript, this, file->fileName()));
            rowTable.push_back(downloadButton);
            return (rowTable);
        }
    }
    return rowTable;
}

Wt::WValidator    *ClassTest::editValidator(int who)
{
    Wt::WRegExpValidator *validator = 
            new Wt::WRegExpValidator(REG_EXP);
    validator->setMandatory(true);
    return validator;
}

// DB - ADD DEL MODIF ----------------------------------------

void ClassTest::addResource(std::vector<Wt::WInteractWidget*> argument)
{

    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();
    assetEdit = (Wt::WLineEdit*)(*i);
    doJavaScript("$('#" + boost::lexical_cast<std::string>(assetEdit->text()) + "').toggleClass('error',false)");
    doJavaScript("$('#" + boost::lexical_cast<std::string>(assetEdit->text()) + "-hint').hide()");
    try
    {
        Wt::Dbo::Transaction transaction(*session_);
        Probe *newProbe = new Probe();
        Wt::Dbo::ptr<Probe> ptrNewProbe = session_->add<Probe>(newProbe);
        ptrNewProbe.modify()->organization = session_->user().get()->currentOrganization;
        ptrNewProbe.modify()->name = "Probe_" + session_->user().get()->lastName + "_" + assetEdit->text();
            
        Asset *newAsset = new Asset();
            
        Wt::Dbo::ptr<Asset> ptrNewAsset = session_->add<Asset>(newAsset);
        ptrNewAsset.modify()->name = assetEdit->text();
        ptrNewAsset.modify()->assetIsHost = true;
        ptrNewAsset.modify()->probe = ptrNewProbe;
        ptrNewAsset.modify()->organization = session_->user().get()->currentOrganization;
            
        //fixme : temporaire jusqu'à la gestion des plugins
        Wt::Dbo::ptr<Plugin> plgSystem = session_->find<Plugin>().where("\"PLG_ID\" = ?").bind(1);
        if (plgSystem)
        {
            ptrNewAsset.modify()->plugins.insert(plgSystem);
        }
        UserActionManagement::registerUserAction(Enums::add,Constants::T_ASSET_AST,ptrNewAsset.id());
        transaction.commit();
    }
        catch (Wt::Dbo::Exception e)
        {
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("2"),Wt::Ok);
        Wt::log("error") << "[ClassTest] " << e.what();
    }
    recoverListAsset();
}

void ClassTest::deleteResource(long long id)
{
    try
    {
        Wt::Dbo::Transaction transaction(*session_);
        Wt::Dbo::ptr<Asset>  ptrAsset = session_->find<Asset>().where("\"AST_ID\" = ?").bind(id);

        const std::string executeString = "DELETE FROM \"TJ_AST_PLG\" " 
                " WHERE \"TJ_AST_PLG\".\"T_ASSET_AST_AST_ID\" = " + boost::lexical_cast<std::string>(id);
        session_->execute(executeString);

        ptrAsset.modify()->deleteTag = Wt::WDateTime::currentDateTime();
        UserActionManagement::registerUserAction(Enums::del,Constants::T_ASSET_AST,ptrAsset.id());
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[ClassTest] [deleteAsset] " << e.what();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("3"),Wt::Ok);
        return;
    }
    CreatePageWidget::deleteResource(id);
    recoverListAsset();
}

void ClassTest::modifResource(std::vector<Wt::WInteractWidget*> argument, long long id)
{
    Wt::WLineEdit *test;
    std::cout << "Modif : Resultat des entrées : " << std::endl;
 
    for (std::vector<Wt::WInteractWidget*>::iterator i = argument.begin(); i != argument.end(); i++)
    {
        test = (Wt::WLineEdit*)(*i);
        std::cout << test->text() << std::endl;
    }

    recoverListAsset();
}

void ClassTest::close()
{
    delete this;
}


// DB GET INFO ------------------------------------------

void ClassTest::getAsset(boost::system::error_code err, const Wt::Http::Message& response)
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
            Wt::log("error") << "[Alert Edition Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "status",tr("Alert.alert.database-error"),Wt::Ok);
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

void    ClassTest::recoverListAsset()
{
    /**
     ** Connection API
     */
    std::string apiAddress = this->getApiUrl() + "/assets";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&ClassTest::getAsset, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        std::cout << "Error Client Http" << std::endl;
}


void    ClassTest::postAsset(boost::system::error_code err, const Wt::Http::Message& response)
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
}

void ClassTest::putAsset(boost::system::error_code err, const Wt::Http::Message& response)
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
}

// SCRIPT DOWNLOAD ---------------------------------------

Wt::WFileResource *ClassTest::generateScript(long long i, Wt::WString assetName)
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
        + "TOKEN=\"" + this->session_->user()->currentOrganization.get()->token.toUTF8() + "\"\n";
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[ClassTest] " << e.what();
        return NULL;
    }

    // full script to send
    std::string contentToSend = disclaimerString + scriptCustomPart + bodyString;
    
    // temp file
    char *tmpname = strdup("/tmp/echoes-tmp-fileXXXXXX");
    int mkstempRes = mkstemp(tmpname);
    Wt::log("debug") << "[ClassTest] " << "Res temp file creation : " << mkstempRes;
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

std::string ClassTest::getStringFromFile(std::string resourcePath)
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

void ClassTest::downloadScript(std::string fileName)
{
    UserActionManagement::registerUserAction(Enums::download,fileName,0);
}

// ----------------------------------------------

void    ClassTest::setSession(Session *session)
{
    session_ = session;
}

void    ClassTest::setApiUrl(std::string apiUrl)
{
    apiUrl_ = apiUrl;
}

std::string   ClassTest::getApiUrl()
{
    return apiUrl_;
}


