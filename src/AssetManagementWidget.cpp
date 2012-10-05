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
    Wt::WTemplateFormView *mainForm = new Wt::WTemplateFormView(Wt::WString::tr("Alert.Asset.Management.template"));    
    
    // widgets of the template
    mainForm->bindWidget("asset-name", createFormWidget(AssetManagementModel::AssetName));
    
    Wt::WPushButton *addAssetButton = new Wt::WPushButton(tr("Alert.asset.add-asset-button"));
    mainForm->bindWidget("add-asset-button", addAssetButton);

    mainForm->updateModel(model_);
    mainForm->refresh();
    // Table where the links / buttons are added
    Wt::WTable *linksTable = new Wt::WTable();
    
    Wt::WLabel *label;
    int row = 0;

    linksTable->elementAt(row, 0)->setColumnSpan(3);
    linksTable->elementAt(row, 0)->setContentAlignment(Wt::AlignTop | Wt::AlignCenter);
    linksTable->elementAt(row, 0)->setPadding(10);
    Wt::WText *title = new Wt::WText(tr("example.form"),linksTable->elementAt(row, 0));
    title->decorationStyle().font().setSize(Wt::WFont::XLarge);
    
    
    {
        Wt::Dbo::Transaction transaction(*session);
        std::string queryString =  "select ast from \"T_ASSET_AST\" ast where \"AST_PRB_PRB_ID\" IN" 
                                    "("
                                    "    SELECT \"PRB_ID\" FROM \"T_PROBE_PRB\" WHERE \"PRB_ORG_ORG_ID\" = (SELECT \"CURRENT_ORG_ID\" FROM \"T_USER_USR\" WHERE \"USR_ID\" = 6)"
                                    ")";
        Wt::Dbo::Query<Wt::Dbo::ptr<Asset> > resQuery = session->query<Wt::Dbo::ptr<Asset> >(queryString);
        
        Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > listAssets = resQuery.resultList();
        for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = listAssets.begin(); i != listAssets.end(); ++i) 
        {
            ++row;
            Wt::WFileResource *file = generateScript(i->id());
            Wt::WAnchor *anchor = new Wt::WAnchor(file,tr("download-script"),linksTable->elementAt(row, 2));
            anchor->setTarget(Wt::TargetNewWindow);
            label = new Wt::WLabel(i->get()->name,linksTable->elementAt(row, 0));
            
            Wt::WPushButton *delButton = new Wt::WPushButton(tr("delete-asset"), linksTable->elementAt(row, 3));
            delButton->clicked().connect(boost::bind(&AssetManagementWidget::deleteAsset,this,i->id()));
        }
        transaction.commit();
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

void AssetManagementWidget::deleteAsset(long long id)
{
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<Asset>  ptrAsset = session->find<Asset>().where("\"AST_ID\" = ?").bind(id);
        ptrAsset.remove();
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
    }
            
    refresh();

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
    mkstemp(tmpname);
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
