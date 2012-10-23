/* 
 * File:   ProbeDownloadWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "AssetManagementWidget.h"


#include "Wt/WAnchor"
#include "Wt/WApplication"
#include "Wt/WContainerWidget"
#include "Wt/WDialog"
#include "Wt/WImage"
#include "Wt/WLineEdit"
#include "Wt/WPushButton"
#include "Wt/WText"

#include <Wt/WComboBox>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>
#include <Wt/Dbo/Query>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/Mail/Client>
#include <Wt/Mail/Message>

#include <Wt/WMessageBox>

#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WTable>


#include "tools/Session.h"
//#include "Login.h"
#include "user/User.h"


#include <memory>
#include <Wt/WTableView>
#include <Wt/Utils>
#include <Wt/WLabel>
#include <Wt/WVBoxLayout>

#include <iostream>
#include <fstream>


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
//        update();
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
    mainForm->bindWidget("add-asset-button", addAssetButton);
    addAssetButton->clicked().connect(boost::bind(&AssetManagementWidget::addAsset, this));

    mainForm->updateModel(model_);
    mainForm->refresh();
    // Table where the links / buttons are added
    Wt::WTable *linksTable = new Wt::WTable();
    
    int row = 0;

    linksTable->elementAt(row, 0)->setColumnSpan(3);
    linksTable->elementAt(row, 0)->setContentAlignment(Wt::AlignTop | Wt::AlignCenter);
    linksTable->elementAt(row, 0)->setPadding(10);
    Wt::WText *title = new Wt::WText(tr("Alert.asset.add-asset-form"),linksTable->elementAt(row, 0));
    title->decorationStyle().font().setSize(Wt::WFont::XLarge);
    
    try
    {
        Wt::log("info") << "Debug : before transaction";
        Wt::Dbo::Transaction transaction(*this->session);
        //TODO : don't understand why the two lines below are needed, clean this
        Wt::Dbo::ptr<User> tempUser = session->find<User>().where("\"USR_ID\" = ?").bind(session->user().id());
        Wt::log("info") << "Debug : user found";
        if (tempUser)
        {
            Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
    //        Wt::log("info") << "Debug : " << session->user().get()->currentOrganization.id();
            std::string queryString =  "select ast from \"T_ASSET_AST\" ast where \"AST_PRB_PRB_ID\" IN" 
                                        " ("
                                        "    SELECT \"PRB_ID\" FROM \"T_PROBE_PRB\" WHERE \"PRB_ORG_ORG_ID\" = " + boost::lexical_cast<std::string>(tempUser->currentOrganization.id()) +
                                        ")"
                                        " AND \"AST_DELETE\" IS NULL";
            Wt::log("info") << "Debug : " << queryString ;
            Wt::Dbo::Query<Wt::Dbo::ptr<Asset> > resQuery = session->query<Wt::Dbo::ptr<Asset> >(queryString);

            Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > listAssets = resQuery.resultList();
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = listAssets.begin(); i != listAssets.end(); ++i) 
            {
                ++row;
                Wt::WFileResource *file = generateScript(i->id());
                Wt::WAnchor *anchor = new Wt::WAnchor(file,tr("Alert.asset.download-script"),linksTable->elementAt(row, 2));
                anchor->setTarget(Wt::TargetNewWindow);
                new Wt::WLabel(i->get()->name,linksTable->elementAt(row, 0));

                Wt::WPushButton *delButton = new Wt::WPushButton(tr("Alert.asset.delete-asset"), linksTable->elementAt(row, 3));
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
    
    

//    // Submit
//    ++row;
//    Wt::WPushButton *submit = new Wt::WPushButton(tr("submit"),
//                      linksTable->elementAt(row, 0));
//    submit->clicked().connect(this, &AssetManagementWidget::downloadScript);
//    submit->setMargin(15, Wt::Top);
//    linksTable->elementAt(row, 0)->setColumnSpan(3);
//    linksTable->elementAt(row, 0)->setContentAlignment(Wt::AlignTop | Wt::AlignCenter);
//
//    // Set column widths for label and validation icon
//    linksTable->elementAt(2, 0)->resize(Wt::WLength(30, Wt::WLength::FontEx), Wt::WLength::Auto);
//    linksTable->elementAt(2, 1)->resize(20, Wt::WLength::Auto);
    
    
    
    
    Wt::WVBoxLayout *mainVerticalLayout = new Wt::WVBoxLayout();
    Wt::WHBoxLayout *topHorizontalLayout = new Wt::WHBoxLayout();
    Wt::WHBoxLayout *bottomHorizontalLayout = new Wt::WHBoxLayout();
    
    
    
    topHorizontalLayout->addWidget(mainForm);
    bottomHorizontalLayout->addWidget(linksTable);
    
    mainVerticalLayout->addLayout(topHorizontalLayout);
    mainVerticalLayout->addLayout(bottomHorizontalLayout);
    
    this->setLayout(mainVerticalLayout);
    
}

Wt::WFormWidget *AssetManagementWidget::createFormWidget(AssetManagementModel::Field field)
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
        }
        else
        {
            Wt::WMessageBox::show(tr("Alert.asset.asset-name-invalid"),tr("Alert.asset.asset-name-invalid"),Wt::Ok);
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
        ptrAsset.modify()->deleteTag = Wt::WDateTime::currentDateTime();
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AssetManagementWidget] [deleteAsset] " << e.what();
    }
            
//    refresh();
    created_ = false;
    model_->reset();
    createUI();

}



Wt::WFileResource *AssetManagementWidget::generateScript(long long i)
{
    // static part of file
    std::string disclaimerString = getStringFromFile("scripts/disclaimer");
    std::string bodyString = getStringFromFile("scripts/scriptbody");
    
    // custom part
    std::string scriptCustomPart = "";
    try
    {
        Wt::Dbo::ptr<User> tempUser = this->session->find<User>().where("\"USR_ID\" = ?").bind(this->session->user().id());
        scriptCustomPart = "\nLOGIN=\"" + tempUser->eMail.toUTF8() + "\"\n" 
        + "ASSET_ID=" + boost::lexical_cast<std::string, long long>(i) + "\n";
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
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
    
    // creating resource to send to the client
    Wt::WFileResource *res = new Wt::WFileResource();
    res->setFileName(tmpname);
    res->suggestFileName("ea-probe-install.sh",Wt::WResource::Attachment);
    res->setMimeType("application/x-sh");
    return res;
}

std::string AssetManagementWidget::getStringFromFile(std::string resourcePath)
{
    std::string filePath = "." + Wt::WApplication::instance()->resourcesUrl()+resourcePath;
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
        Wt::log("info") << filePath;
        Wt::log("error") << "fla";
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

