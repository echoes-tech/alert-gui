/* 
 * File:   ProbeDownloadWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "AssetManagementWidget.h"

AssetManagementWidget::AssetManagementWidget(AssetManagementModel *model, Session *session)
: Wt::WContainerWidget()
{
    model_ = model;
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("asset",false);      
    createUI();
}

void AssetManagementWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
        created_ = true;
    }

    Wt::WContainerWidget::render(flags);
}

void AssetManagementWidget::createUI()
{
    // Top template
    mainForm = new Wt::WTemplateFormView(Wt::WString::tr("Alert.Asset.Management.template"));    
    
    // widgets of the template
    mainForm->bindWidget("asset-name", createFormWidget(AssetManagementModel::AssetName));
    
    Wt::WPushButton *addAssetButton = new Wt::WPushButton(tr("Alert.asset.add-asset-button"));
    // Todo : voir si la fonction dédiée fonctionne avec les dernières versions de Wt
    addAssetButton->setAttributeValue("class","btn btn-info");
    mainForm->bindWidget("add-asset-button", addAssetButton);
    addAssetButton->clicked().connect(boost::bind(&AssetManagementWidget::addAsset, this));
    

    mainForm->updateModel(model_);
    mainForm->refresh();
    // Table where the links / buttons are added
    Wt::WTable *linksTable = new Wt::WTable();
    linksTable->addStyleClass("table");
    linksTable->addStyleClass("table-bordered");
    linksTable->addStyleClass("table-striped");
    
    int row = 0;

    
    linksTable->setHeaderCount(2,Wt::Horizontal);
    linksTable->elementAt(row, 0)->setColumnSpan(2);
    linksTable->columnAt(1)->setStyleClass("asset-action-width");
    
    Wt::WText *tableTitle = new Wt::WText("<div class='widget-title widget-title-ea-table'><span class='icon'><i class='icon-hdd'></i></span><h5>"+ tr("Alert.asset.add-asset-form") + "</h5></div>",linksTable->elementAt(row, 0));
    linksTable->elementAt(row, 0)->setPadding(*(new Wt::WLength("0px")));
    tableTitle->setTextFormat(Wt::XHTMLUnsafeText);
    ++row;
    new Wt::WText(tr("Alert.asset.asset-name"),linksTable->elementAt(row, 0));
    new Wt::WText(tr("Alert.asset.asset-action"),linksTable->elementAt(row, 1));
         
    try
    {
        Wt::log("info") << "Debug : before transaction";
        Wt::Dbo::Transaction transaction(*this->session);
        std::string queryString =  "select ast from \"T_ASSET_AST\" ast where \"AST_PRB_PRB_ID\" IN" 
                                    " ("
                                    "    SELECT \"PRB_ID\" FROM \"T_PROBE_PRB\" WHERE \"PRB_ORG_ORG_ID\" = " + boost::lexical_cast<std::string>(session->user()->currentOrganization.id()) +
                                    ")"
                                    " AND \"AST_DELETE\" IS NULL";
        Wt::log("info") << "Debug : " << queryString ;
        Wt::Dbo::Query<Wt::Dbo::ptr<Asset> > resQuery = session->query<Wt::Dbo::ptr<Asset> >(queryString);

        Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > listAssets = resQuery.resultList();
        for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = listAssets.begin(); i != listAssets.end(); ++i) 
        {
            ++row;
            Wt::WFileResource *file = generateScript(i->id(),i->get()->name);
            if (file == NULL)
            {
                new Wt::WLabel(Wt::WString::tr("Alert.asset.file-not-generated"),linksTable->elementAt(row, 1));
                new Wt::WLabel(i->get()->name,linksTable->elementAt(row, 0));

                Wt::WPushButton *delButton = new Wt::WPushButton("", linksTable->elementAt(row, 1));
                delButton->clicked().connect(boost::bind(&AssetManagementWidget::deleteAsset,this,i->id()));
            }
            else
            {
                Wt::WAnchor *anchor = new Wt::WAnchor(file,"",linksTable->elementAt(row, 1));
                anchor->setTextFormat(Wt::XHTMLUnsafeText);
                anchor->setText("<i class='icon-download icon-white'></i> " + tr("Alert.asset.download-script"));
                anchor->addStyleClass("btn");
                anchor->addStyleClass("btn-info");
                anchor->setTarget(Wt::TargetNewWindow);
                anchor->clicked().connect(boost::bind(&AssetManagementWidget::downloadScript, this,file->fileName()));

                new Wt::WLabel(i->get()->name,linksTable->elementAt(row, 0));

                Wt::WText *nbspText = new Wt::WText("&nbsp;", linksTable->elementAt(row, 1));
                nbspText->setTextFormat(Wt::XHTMLUnsafeText);

                Wt::WPushButton *delButton = new Wt::WPushButton(tr("Alert.asset.delete-asset"), linksTable->elementAt(row, 1));
                delButton->setAttributeValue("class","btn btn-danger");

                delButton->setTextFormat(Wt::XHTMLUnsafeText);
                delButton->setText("<i class='icon-remove icon-white'></i> " + tr("Alert.asset.delete-asset"));

                delButton->clicked().connect(boost::bind(&AssetManagementWidget::deleteAsset,this,i->id()));
            }
        }
        
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("1"),Wt::Ok);
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
    }
     
    
    Wt::WVBoxLayout *mainVerticalLayout = new Wt::WVBoxLayout();
    Wt::WHBoxLayout *topHorizontalLayout = new Wt::WHBoxLayout();
    Wt::WHBoxLayout *bottomHorizontalLayout = new Wt::WHBoxLayout();
    
      
    topHorizontalLayout->addWidget(mainForm);
    bottomHorizontalLayout->addWidget(linksTable);
    
    mainVerticalLayout->addLayout(topHorizontalLayout);
    mainVerticalLayout->addLayout(bottomHorizontalLayout);
    
    this->setLayout(mainVerticalLayout);
    
}

void AssetManagementWidget::downloadScript(std::string fileName)
{
    UserActionManagement::registerUserAction(Enums::download,fileName,0);
}

Wt::WFormWidget *AssetManagementWidget::createFormWidget(Wt::WFormModel::Field field)
{
    Wt::WFormWidget *result = 0;

    if (field == AssetManagementModel::AssetName)
    {
        Wt::WLineEdit *line = new Wt::WLineEdit();
        result = line;
    }

    return result;
}

void AssetManagementWidget::addAsset()
{
    doJavaScript("$('#" + boost::lexical_cast<std::string>(AssetManagementModel::AssetName) + "').toggleClass('error',false)");
    doJavaScript("$('#" + boost::lexical_cast<std::string>(AssetManagementModel::AssetName) + "-hint').hide()");
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        mainForm->updateModelField(model_, AssetManagementModel::AssetName);
        if (model_->validateField(AssetManagementModel::AssetName))
        {
            Probe *newProbe = new Probe();
            Wt::Dbo::ptr<Probe> ptrNewProbe = session->add<Probe>(newProbe);
            ptrNewProbe.modify()->organization = session->user().get()->currentOrganization;
            ptrNewProbe.modify()->name = "Probe_" + session->user().get()->lastName + "_" + model_->valueText(AssetManagementModel::AssetName);
            
            
            Asset *newAsset = new Asset();
            
            Wt::Dbo::ptr<Asset> ptrNewAsset = session->add<Asset>(newAsset);
            ptrNewAsset.modify()->name = model_->valueText(AssetManagementModel::AssetName);
            ptrNewAsset.modify()->assetIsHost = true;
            ptrNewAsset.modify()->probe = ptrNewProbe;
            ptrNewAsset.modify()->organization = session->user().get()->currentOrganization;
            
            //fixme : temporaire jusqu'à la gestion des plugins
            Wt::Dbo::ptr<Plugin> plgSystem = session->find<Plugin>().where("\"PLG_ID\" = ?").bind(1);
            if (plgSystem)
            {
                ptrNewAsset.modify()->plugins.insert(plgSystem);
            }
            UserActionManagement::registerUserAction(Enums::add,Constants::T_ASSET_AST,ptrNewAsset.id());
        }
        else
        {
            mainForm->updateModelField(model_, AssetManagementModel::AssetName);
            doJavaScript("$('#" + boost::lexical_cast<std::string>(AssetManagementModel::AssetName) + "').toggleClass('error')");
            doJavaScript("$('#" + boost::lexical_cast<std::string>(AssetManagementModel::AssetName) + "-hint').show()");
        }
        transaction.commit();  
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("2"),Wt::Ok);
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
    }
    created_ = false;
    model_->reset();
    createUI();
    Wt::WApplication *app = Wt::WApplication::instance();
    app->root()->widget(0)->refresh();
}

void AssetManagementWidget::deleteAsset(long long id)
{
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<Asset>  ptrAsset = session->find<Asset>().where("\"AST_ID\" = ?").bind(id);
        //FIXME: doesn't work, bug posted in Wt forge #1479
        //ptrAsset.modify()->alerts.clear();
        std::string executeString = "DELETE FROM \"T_ALERT_ALE\" USING \"TJ_AST_ALE\" " 
                                    " WHERE \"TJ_AST_ALE\".\"T_ALERT_ALE_ALE_ID\" = \"T_ALERT_ALE\".\"ALE_ID\" " 
                                    " AND \"TJ_AST_ALE\".\"T_ASSET_AST_AST_ID\" = " + boost::lexical_cast<std::string>(id);
        session->execute(executeString);
        
        executeString = "DELETE FROM \"TJ_AST_PLG\" " 
                                    " WHERE \"TJ_AST_PLG\".\"T_ASSET_AST_AST_ID\" = " + boost::lexical_cast<std::string>(id);
        session->execute(executeString);
        
        ptrAsset.modify()->deleteTag = Wt::WDateTime::currentDateTime();
        UserActionManagement::registerUserAction(Enums::del,Constants::T_ASSET_AST,ptrAsset.id());
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AssetManagementWidget] [deleteAsset] " << e.what();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("3"),Wt::Ok);
        return;
    }
            
    created_ = false;
    model_->reset();
    createUI();
    Wt::WApplication *app = Wt::WApplication::instance();
    app->root()->widget(0)->refresh();
}



Wt::WFileResource *AssetManagementWidget::generateScript(long long i, Wt::WString assetName)
{
    // static part of file
    std::string disclaimerString = getStringFromFile("resources/scripts/disclaimer");
    std::string bodyString = getStringFromFile("resources/scripts/scriptbody");
    
    // custom part
    std::string scriptCustomPart = "";
    try
    {
        scriptCustomPart = "\nLOGIN=\"" + this->session->user()->eMail.toUTF8() + "\"\n" 
        + "ASSET_ID=" + boost::lexical_cast<std::string, long long>(i) + "\n"
        + "TOKEN=\"" + this->session->user()->currentOrganization.get()->token.toUTF8() + "\"\n";
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
        Wt::log("error") << "File not found." ;
    }
    return res;
}

bool AssetManagementWidget::validate()
{
    return model_->validate();
}


void AssetManagementWidget::close()
{
    delete this;
}

