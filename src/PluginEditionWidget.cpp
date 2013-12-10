/* 
 * File:   PluginEditionWidget.cpp
 * Author: gdr
 *
 * Created on 18 mars 2013, 11:59
 */

#include <Wt/Utils>

#include "PluginEditionWidget.h"

Wt::Json::Array Wt::Json::Array::Empty;

PluginEditionWidget::PluginEditionWidget(Echoes::Dbo::Session* session, const std::string &apiUrl)
: Wt::WTemplateFormView(Wt::WString::tr("Alert.plugins.Edition.template"))
{
    this->session = session;
    setApiUrl(apiUrl);

    if(this->session->user().id() != -1)
    {
        Wt::WApplication *app = Wt::WApplication::instance();
            app->enableUpdates(true);
        app->messageResourceBundle().use("plugin",false);
        searchToken();
        createUI();  
    }
    else
    {
        Wt::log("Info") << "[PluginEditionWidget] User not found";
    }
}

PluginEditionWidget::~PluginEditionWidget()
{
    
}

void PluginEditionWidget::fixStyleTable(Wt::WTable *table)
{
    table->columnAt(0)->setWidth("10%");
    table->addStyleClass("table-bordered");
    table->addStyleClass("table");
    table->addStyleClass("table-striped");
    table->addStyleClass("table-condensed");
    table->setHeaderCount(1, Wt::Horizontal);
}

void PluginEditionWidget::refresh1()
{
    Wt::WApplication *app = Wt::WApplication::instance();
    Wt::WServer::instance()->post(app->sessionId(), app->bind(boost::bind(&PluginEditionWidget::updateData, this)));
}

 void PluginEditionWidget::updateData() 
 {
    Wt::WApplication::instance()->triggerUpdate();
 }

void PluginEditionWidget::createUI()
{
    //validator
    validatorInt = new Wt::WIntValidator(0, 10000); 
    validatorInt->setMandatory(true);
    //valide : les backslash doivent être par paire et les " ne sont pas autorisées (sinon crash de l'API ou du GUI suivant les cas lorsque l'on parse le JSON).
    validatorRegExp = new Wt::WRegExpValidator("([^\"]+)*");
//    validatorRegExp = new Wt::WRegExpValidator("(.+)*");
    //    validatorRegExp = new Wt::WRegExpValidator("([\\\\]{2}|[^\\\\\"]+)*");
    validatorRegExp->setMandatory(true);
    
    // creation plugin
    pluginSelectionBox = new Wt::WComboBox();
    
    pluginSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedPlugin);
    bindWidget("plugin-selection-box", pluginSelectionBox); 

    plgNameEdit = new Wt::WLineEdit();
//    bindWidget("plugin-name-edit", plgNameEdit);
    plgNameEdit->setValidator(validatorRegExp);

    plgDescEdit = new Wt::WLineEdit();
//    bindWidget("plugin-description-edit", plgDescEdit);
    plgDescEdit->setValidator(validatorRegExp);

    Wt::WPushButton *addPlugin = new Wt::WPushButton("<span class='icon'><i class='icon-plus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.add-plugin-button"));
    addPlugin->addStyleClass("btn");
    addPlugin->addStyleClass("btn-info");
    addPlugin->setTextFormat(Wt::XHTMLUnsafeText);
    addPlugin->clicked().connect(this, &PluginEditionWidget::addPlugin);
    
   tablePlugin = new Wt::WTable();
    
    tablePlugin->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.name-title")));
    tablePlugin->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.description-title")));
    tablePlugin->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.plugins.action-title")));
    tablePlugin->elementAt(1, 0)->addWidget(plgNameEdit);
    tablePlugin->elementAt(1, 1)->addWidget(plgDescEdit);
    tablePlugin->elementAt(1, 2)->addWidget(addPlugin);
    tablePlugin->setHeaderCount(0, Wt::Horizontal);
    fixStyleTable(tablePlugin);
    tablePlugin->columnAt(0)->setWidth("40%");
    tablePlugin->columnAt(1)->setWidth("40%");
    tablePlugin->columnAt(2)->setWidth("20%");
    bindWidget("plugin-table-plugin-creation", tablePlugin);
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgList, this, _1, _2));
    
    std::string urlAdd = this->getApiUrl() + "/plugins"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if(client1->get(urlAdd))
    {
         Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
 
    // creation source
    sourceSelectionBox = new Wt::WSelectionBox();
    bindWidget("source-selection-box", sourceSelectionBox);
    sourceSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedSource);

    comboAddon = new Wt::WComboBox();
    slmAddons = new Wt::WStringListModel();
    comboAddon->setModel(slmAddons);
    comboAddon->clicked().connect(this, &PluginEditionWidget::createFormSourceParameters);
    bindWidget("plugin-combo-addon", comboAddon);
 
    tableSrcParam = new Wt::WTable();
    bindWidget("plugin-table-source-param", tableSrcParam);
    fixStyleTable(tableSrcParam);
     
    tableSrcParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSrcParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    
    Wt::WPushButton *addSource = new Wt::WPushButton("<span class='icon'><i class='icon-plus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.add-source-button"));  
    addSource->addStyleClass("btn");
    addSource->addStyleClass("btn-info");
    addSource->setTextFormat(Wt::XHTMLUnsafeText);
    addSource->clicked().connect(this, &PluginEditionWidget::addSource);
    bindWidget("plugin-add-source-button", addSource);
    
    buttonModifySource = new Wt::WPushButton( tr("Alert.plugins.modify-source-button"));
    buttonModifySource->addStyleClass("btn");
    buttonModifySource->addStyleClass("btn-primary");
    buttonModifySource->setTextFormat(Wt::XHTMLUnsafeText);
    buttonModifySource->clicked().connect(this, &PluginEditionWidget::modifySource);
    bindWidget("plugin-modify-source-button", buttonModifySource);
    
    Wt::WPushButton *deleteSource = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-source-button"));
    deleteSource->addStyleClass("btn");
    deleteSource->addStyleClass("btn-danger");
    deleteSource->setTextFormat(Wt::XHTMLUnsafeText);
    deleteSource->clicked().connect(this, &PluginEditionWidget::deleteSource);
    bindWidget("plugin-delete-source-button", deleteSource);
    
   // creation search
    searchSelectionBox = new Wt::WSelectionBox();
    searchSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedSearch);
    bindWidget("plugin-search-selection-box", searchSelectionBox);
    
    comboSeaType = new Wt::WComboBox();
    slmSeaType = new Wt::WStringListModel();
    comboSeaType->setModel(slmSeaType);
    comboSeaType->clicked().connect(this, &PluginEditionWidget::createFormSearchParameters);
    bindWidget("plugin-combo-search-type", comboSeaType);
    
    tableSeaTypeParam = new Wt::WTable();
    fixStyleTable(tableSeaTypeParam);
    tableSeaTypeParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSeaTypeParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    bindWidget("plugin-table-search-type-param", tableSeaTypeParam);
    
    periodeLE = new Wt::WLineEdit();
    periodeLE->setValidator(validatorInt);
    bindWidget("plugin-search-periode", periodeLE);
    
    staticCB = new Wt::WCheckBox("Static");
    bindWidget("plugin-search-static", staticCB);
 
    pKValueLE = new Wt::WLineEdit();
    pKValueLE->setText("0");
    pKValueLE->setValidator(validatorInt);
    bindWidget("plugin-search-key-value", pKValueLE);

    nbValueLE = new Wt::WLineEdit();
    nbValueLE->setValidator(validatorInt);
    nbValueLE->changed().connect(this, &PluginEditionWidget::createTableUnit);
    bindWidget("plugin-search-nomber-value", nbValueLE);
        
    tableSeaUnit = new Wt::WTable();
    fixStyleTable(tableSeaUnit);
    tableSeaUnit->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.value-number-title")));
    tableSeaUnit->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.name-title")));
    tableSeaUnit->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.plugins.unit-title")));
    tableSeaUnit->elementAt(0, 3)->addWidget(new Wt::WText(tr("Alert.plugins.display-title")));
    tableSeaUnit->elementAt(0, 4)->addWidget(new Wt::WText(tr("Alert.plugins.calculate-title")));
    tableSeaUnit->elementAt(0, 5)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    bindWidget("plugin-search-information-table", tableSeaUnit);
    slmUnit = new Wt::WStringListModel();
    
    Wt::WPushButton *addSearch = new Wt::WPushButton("<span class='icon'><i class='icon-plus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.add-search-and-information-button"));  
    addSearch->addStyleClass("btn");
    addSearch->addStyleClass("btn-info");
    addSearch->setTextFormat(Wt::XHTMLUnsafeText);
    addSearch->clicked().connect(this, &PluginEditionWidget::addSearch);
    bindWidget("plugin-add-search-information-button", addSearch);
    
    buttonModifySearch = new Wt::WPushButton( tr("Alert.plugins.modify-search-button"));
    buttonModifySearch->addStyleClass("btn");
    buttonModifySearch->addStyleClass("btn-primary");
    buttonModifySearch->setTextFormat(Wt::XHTMLUnsafeText);
    buttonModifySearch->clicked().connect(this, &PluginEditionWidget::modifySearch);
    bindWidget("plugin-modify-search-button", buttonModifySearch);
    
    Wt::WPushButton *deleteSearch = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-search-button"));
    deleteSearch->addStyleClass("btn");
    deleteSearch->addStyleClass("btn-danger");
    deleteSearch->setTextFormat(Wt::XHTMLUnsafeText);
    deleteSearch->clicked().connect(this, &PluginEditionWidget::deleteSearch);
    bindWidget("plugin-delete-search-button", deleteSearch);
    
    createJSONButton = new Wt::WPushButton("<i class='icon-download icon-white'></i> " + tr("Alert.plugins.create-JSON-button"));
    createJSONButton->addStyleClass("btn");
    createJSONButton->addStyleClass("btn-primary");
    createJSONButton->setTextFormat(Wt::XHTMLUnsafeText);
    createJSONButton->clicked().connect(this, &PluginEditionWidget::createJSON);
    bindWidget("plugin-create-JSON-button", createJSONButton);
    
    createJSONAnchor = new Wt::WAnchor();
    createJSONAnchor->setText("<i class='icon-download icon-white'></i> " + tr("Alert.plugins.create-JSON-anchor"));
    createJSONAnchor->addStyleClass("btn");
    createJSONAnchor->addStyleClass("btn-primary");
    createJSONAnchor->setTextFormat(Wt::XHTMLUnsafeText);
    createJSONAnchor->setTarget(Wt::TargetNewWindow);
    createJSONAnchor->clicked().connect(this, &PluginEditionWidget::displayButtonCreateJSON);
    createJSONAnchor->disable();
    bindWidget("plugin-create-JSON-anchor", createJSONAnchor);
}

void PluginEditionWidget::displayButtonCreateJSON()
{
    createJSONAnchor->disable();
}

void PluginEditionWidget::createJSON()
{
    if(pluginSelectionBox->currentIndex() != -1)
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));

        std::string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        
        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
        if(client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-no-selected-plugin"),Wt::Ok);
    }
}

void PluginEditionWidget::createTableUnit()
{
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseUnits, this, _1, _2));

    std::string urlAdd = this->getApiUrl() + "/units"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();
    
    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::createFormSearchParameters()
{
    if(createFormSearchState == "Modify")
    {
        buttonModifySearch->enable();
    }
    else
    {
        buttonModifySearch->disable();
    }
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSeaTypeParameters, this, _1, _2));

    std::string urlAdd = this->getApiUrl() + "/search_types/"
            + boost::lexical_cast<std::string>(mapSeaTypeIdSboxRow[comboSeaType->currentIndex()])
            + "/parameters" 
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}



//void PluginEditionWidget::createFormSourceParameters()
//{
//    
//    
//    for(unsigned idx = 0 ; idx < mapLEAddonParam.size() ; idx++)
//    {
//       delete mapLEAddonParam[mapAddonParameterNameSBoxRow[idx]];
//    }
//    
//    mapAddonParameterIdSboxRow.clear();
//    mapAddonParameterNameSBoxRow.clear();
//    mapAddonParameterValueSBoxRow.clear();
//    mapLEAddonParam.clear();
//    tableSrcParam->clear();
//    tableSrcParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
//    tableSrcParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
//    
//    string name = "";
//    
//    resetSearch();
//    if(createFormSourceState != "Modify")
//    {
//        buttonModifySource->disable();
//    }
//    else
//    {
//        buttonModifySource->enable();
//    }
//    
//    
//    map<int,string> mapESourceParameterName;
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::PATH] = tr("Alert.plugins.path").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::HOST] = tr("Alert.plugins.host").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::USER] = tr("Alert.plugins.user").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::PASSWD] = tr("Alert.plugins.passwd").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::DB] = tr("Alert.plugins.db").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::PORT] = tr("Alert.plugins.port").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::COMMUNITY] = tr("Alert.plugins.community").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::VERSION] = tr("Alert.plugins.version").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::AUTH_PROTO] = tr("Alert.plugins.auth_proto").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::AUTH_PATH] = tr("Alert.plugins.auth_path").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::PRIV_PROTO] = tr("Alert.plugins.priv_proto").toUTF8();
//    mapESourceParameterName[Echoes::Dbo::ESourceParameter::PRIV_PATH] = tr("Alert.plugins.priv_path").toUTF8();
//    
//    
//    for (int i = Echoes::Dbo::ESourceParameter::PATH ; i <= Echoes::Dbo::ESourceParameter::PRIV_PATH ; i++)
//    {
//        mapAddonParameterIdSboxRow[i] = i;
//        name = mapESourceParameterName[i];
//        mapAddonParameterNameSBoxRow[i] = name;
//        
//        mapLEAddonParam[name] = new Wt::WLineEdit();
//        mapLEAddonParam[name]->setValidator(validatorRegExp);
//    
//        /////////////// Attention méthode qui ne marche que pour l'affichage dans le gui (en base le passwd sera en claire.
//        if (i == Echoes::Dbo::ESourceParameter::PASSWD)
//        {
//            mapLEAddonParam[name]->setEchoMode(Wt::WLineEdit::Password);
//        }
//        /////////////// If temporaire
//        tableSrcParam->elementAt(i, 0)->addWidget(new Wt::WText(name));
//        tableSrcParam->elementAt(i, 1)->addWidget(mapLEAddonParam[name]);
//    }
//  
//
//  
//    if(createFormSourceState == "Modify")
//    {
//        completFormSource();
//    }
//  
//    if(createFormSourceState == "Modify")
//    {
//        client1 = new Wt::Http::Client();
//
//        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchList, this, _1, _2));
//    
//        std::string urlAdd = this->getApiUrl() + "/plugins/"
//                + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
//                + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
//                + "/searches"
//                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
//                + "&token=" + session->user()->token.toUTF8();
//
//        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
//
//        if(client1->get(urlAdd))
//        {
//            Wt::WApplication::instance()->deferRendering();
//        }
//        else
//            Wt::log("error") << "Error Client Http";
//    }
//    else
//    {
//        client1 = new Wt::Http::Client();
//        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchTypeList, this, _1, _2));
//    
//        std::string urlAdd = this->getApiUrl() + "/addons/"
//                + boost::lexical_cast<std::string>(mapAddonsIdSboxRow[comboAddon->currentIndex()])
//                + "/search_types"
//                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
//                + "&token=" + session->user()->token.toUTF8();
//
//        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
//
//        if(client1->get(urlAdd))
//        {
//            Wt::WApplication::instance()->deferRendering();
//        }
//        else
//            Wt::log("error") << "Error Client Http";
//    }
//    createFormSourceState = "";
//    refresh1();
//    
//}

void PluginEditionWidget::createFormSourceParameters()
{
    resetSearch();
    if(createFormSourceState != "Modify")
    {
        buttonModifySource->disable();
    }
    else
    {
        buttonModifySource->enable();
    }
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddonParameters, this, _1, _2));
    
    std::string urlAdd = this->getApiUrl() + "/sources/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&addon_id=" + boost::lexical_cast<std::string>(mapAddonsIdSboxRow[comboAddon->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
    
}

bool PluginEditionWidget::validatePlugin(std::string &badField)
{
    if (plgNameEdit->validate() == Wt::WValidator::State::Valid)
    {
        if (plgDescEdit->validate() == Wt::WValidator::State::Valid)
        {
            return true;
        }
        else
        {
            badField = "Alert.plugins.description-label";
        }
    }
    else
    {
        badField = "Alert.plugins.name-label";
    }
    
    return false;
}

void PluginEditionWidget::addPlugin()
{
    createJSONAnchor->disable(); 
    std::string badField = "";
    resetSearch();
    resetSource();
    if(validatePlugin(badField))
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddPlg, this, _1, _2));
        
        //si il y a des backslash dans le nom
        std::string strTmp = boost::lexical_cast<std::string>(plgNameEdit->text()); 
        formatValue(strTmp, 1);//
        std::string strTmp2 = boost::lexical_cast<std::string>(plgDescEdit->text()); 
        formatValue(strTmp2, 1);//
        std::string messAdd("{\n\
            \"name\" : \""+ strTmp +"\",\n\
            \"desc\" : \""+ strTmp2 +"\"\n\
     }\n");

        Wt::Http::Message message;
        message.addBodyText(messAdd);
        
        std::string urlAdd = this->getApiUrl() + "/plugins"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();        
        
        Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

        if(client1->post(urlAdd ,message))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") + 
                                tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                                tr("Alert.plugins.message-cause"),Wt::Ok);
        std::cerr << "[addPlugin] : Mal remplit"<< std::endl;
    }
} 

bool PluginEditionWidget::validateSource(std::string &badField)
{
    if(pluginSelectionBox->currentIndex() != -1)
    {
        for(unsigned i = 0; i < mapAddonParameterNameSBoxRow.size(); i++)
        {
            if (mapLEAddonParam[mapAddonParameterNameSBoxRow[i]]->validate() != Wt::WValidator::State::Valid)
            {
                 badField = "Alert.plugins.parameter-title";
                 return false;
            }
        }
        return true;
    }
    
    return false;
}

void PluginEditionWidget::addSource()
{
    createJSONAnchor->disable();
    std::string badField = "";
    if(validateSource(badField))
    {
        buttonModifySource->enable();
        for(int row = 1; row < tableSrcParam->rowCount(); row ++)
        {
            mapAddonParameterValueSBoxRow[row-1] = mapLEAddonParam[mapAddonParameterNameSBoxRow[row - 1]]->text();
        }

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddSource, this, _1, _2));
        std::string messAdd ="{\n\"addon_id\" : "+ boost::lexical_cast<std::string>(mapAddonsIdSboxRow[comboAddon->currentIndex()]);

        if(mapAddonParameterNameSBoxRow.size() == mapAddonParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapAddonParameterNameSBoxRow.size(); idx++)
            {
                //au cas ou il y ait des backslash dans le nom
                std::string strTmp = boost::lexical_cast<std::string>(mapAddonParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1);//
                messAdd += ",\n\"" + boost::lexical_cast<std::string>(mapAddonParameterNameSBoxRow[idx]) + "\": \""+ strTmp +"\"";
            }
            messAdd += "\n}\n";

            Wt::Http::Message message;
            message.addBodyText(messAdd);

            std::string urlAdd = this->getApiUrl() + "/plugins/"
                    + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                    + "/sources" 
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

            if(client1->post(urlAdd ,message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
                Wt::log("error") << "Error Client Http";
        }
        else
        {
            std::cerr << "[PluginEditionWidget] : mapAddonParameterNameSBoxRow.size() =" << mapAddonParameterNameSBoxRow.size() 
                      <<" != mapAddonParameterValueSBoxRow.size() =" << mapAddonParameterValueSBoxRow.size() <<std::endl;
        }
    }
    else
    {
         Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") + 
                                tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                                tr("Alert.plugins.message-cause"),Wt::Ok);
        std::cerr << "[addSource] : Mal remplit"<< std::endl;
    }
}

bool PluginEditionWidget::validateSearch(std::string &badField)
{
    if(sourceSelectionBox->currentIndex() == -1 || pluginSelectionBox->currentIndex() == -1)
    {
        return false;
    }
    for(unsigned i = 0; i < mapLENameInformation.size(); i++)
    {
        if (mapLENameInformation[i]->validate() != Wt::WValidator::State::Valid)
        {
             badField = "Alert.plugins.name-title";
             return false;
        }
        if(mapCBCalculateInformation[i]->isChecked() && mapLECalculateInformation[i]->validate() != Wt::WValidator::State::Valid)
        {
            badField = "Alert.plugins.calculate-title";
             return false;
        }
    }
    for(unsigned i = 0; i < mapSeaTypeParameterNameSBoxRow.size(); i++)
    {
        if (mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[i]]->validate() != Wt::WValidator::State::Valid)
        {
            badField = "Alert.plugins.parameter-title";
             return false;
        }
    }
    if(periodeLE->validate() != Wt::WValidator::State::Valid)
    {
        badField = "Alert.plugins.search-periode-label";
         return false;
    }
    if(pKValueLE->validate() != Wt::WValidator::State::Valid)
    {
        badField = "Alert.plugins.search-key-value";
         return false;
    }
    if(nbValueLE->validate() != Wt::WValidator::State::Valid)
    {
        badField = "Alert.plugins.search-nomber-value";
         return false;
    }
    return true;
}

void PluginEditionWidget::addSearch()
{
    createJSONAnchor->disable();
    std::string badField = "";
    if(validateSearch(badField))
    {
        for(int row = 1; row < tableSeaTypeParam->rowCount(); row ++)
        {
            mapSeaTypeParameterValueSBoxRow[row-1] = mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[row - 1]]->text();
        }
        std::string tmpBool;
        if(boost::lexical_cast<int>(staticCB->isChecked()))
            tmpBool = "true";
        else
            tmpBool = "false";

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddSearch, this, _1, _2));
        std::string messAdd ="{\n    \"sty_id\" : "+ boost::lexical_cast<std::string>(mapSeaTypeIdSboxRow[comboSeaType->currentIndex()]) +",\n\
    \"sea_period\" : " + boost::lexical_cast<std::string>(periodeLE->text()) +",\n\
    \"sea_is_static\" : " + tmpBool + ",\n\
    \"pos_key_value\" : " + boost::lexical_cast<std::string>(pKValueLE->text()) +",\n\
    \"nb_value\" : " + boost::lexical_cast<std::string>(nbValueLE->text()) +",\n";
        if(mapSeaTypeParameterNameSBoxRow.size() == mapSeaTypeParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapSeaTypeParameterNameSBoxRow.size(); idx++)
            {
                
                 //si il y a des backslash dans le nom
                //////////////////////////////////
                std::string strTmp = boost::lexical_cast<std::string>(mapSeaTypeParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1);//
                messAdd += "\"" + boost::lexical_cast<std::string>(mapSeaTypeParameterNameSBoxRow[idx]) + "\": \""+ strTmp +"\",\n";
            }
            messAdd += "\"units\" : [\n";
            for (long unsigned idx1 = 0; idx1 < mapCBUnit.size(); idx1++)
            {
                messAdd += "\t{\"unit_id\" : " + boost::lexical_cast<std::string>(mapUnitIdSboxRow[mapCBUnit[idx1]->currentIndex()]) + ",\n\
    \t\"val_num\" : " + boost::lexical_cast<std::string>(idx1 + 1) +"},\n";
            }
            if(mapCBUnit.size() > 0)
            {
            messAdd.replace(messAdd.size()-2, 1, "");
            }
            messAdd += "]\n}\n";

            Wt::Http::Message message;
            message.addBodyText(messAdd);
            
            std::string urlAdd = this->getApiUrl() + "/plugins/"
                    + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                    + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
                    + "/searches" 
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;
            
            if(client1->post(urlAdd ,message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
                Wt::log("error") << "Error Client Http";

        }
        else
        {
            std::cerr << "[PluginEditionWidget] : mapSeaTypeParameterNameSBoxRow.size() =" << mapSeaTypeParameterNameSBoxRow.size() 
                      <<" != mapSeaTypeParameterValueSBoxRow.size() =" << mapSeaTypeParameterValueSBoxRow.size() <<std::endl;
         //   std::cerr << "post message" << messAdd << std::endl;
        }
    }
    else
    {
         Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") + 
                                tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                                tr("Alert.plugins.message-cause"),Wt::Ok);
        std::cerr << "[addSearch] : Mal remplit"<< std::endl;
    }
}

void PluginEditionWidget::deletePlugin(int pluginId)
{
    createJSONAnchor->disable();
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseDeletePlg, this, _1, _2));
    Wt::Http::Message message;
    message.addBodyText("");

    std::string urlAdd = this->getApiUrl() + "/plugins/"
           + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginId])
           + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
           + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

    if(client1->deleteRequest(urlAdd ,message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
    
}

void PluginEditionWidget::modifySource()
{
    createJSONAnchor->disable();
    std::string badField = "";
    if(validateSource(badField))
    {
        for(int row = 1; row < tableSrcParam->rowCount(); row ++)
        {
            mapAddonParameterValueSBoxRow[row-1] = mapLEAddonParam[mapAddonParameterNameSBoxRow[row - 1]]->text();
        }

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseModifySource, this, _1, _2));

        std::string messAdd ="{\n\"addon_id\" : "+ boost::lexical_cast<std::string>(mapAddonsIdSboxRow[comboAddon->currentIndex()]) +",\n";

        messAdd += "\"parameters\" : [\n";
        for (long unsigned idx1 = 0; idx1 < mapAddonParameterIdSboxRow.size(); idx1++)
        {
            messAdd += "\t{\"srp_id\" : " + boost::lexical_cast<std::string>(mapAddonParameterIdSboxRow[idx1]) + ",\n";
            //si il y a des backslash dans le nom
            //////////////////////////////////
            std::string strTmp = boost::lexical_cast<std::string>(mapAddonParameterValueSBoxRow[idx1]);
            formatValue(strTmp, 1);//
            messAdd += "\t\"value\" : \"" + strTmp +"\"},\n";
        }
        if(mapAddonParameterIdSboxRow.size() > 0)
        {
            messAdd.replace(messAdd.size()-2, 1, "");
        }
        messAdd += "]\n}\n";

        Wt::Http::Message message;
        message.addBodyText(messAdd);
        
        std::string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
                + "/parameters" 
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

        if(client1->post(urlAdd ,message))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    else
    {
         Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") + 
                                tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                                tr("Alert.plugins.message-cause"),Wt::Ok);
        std::cerr << "[addSource] : Mal remplit"<< std::endl;
    }
}

void PluginEditionWidget::deleteSource()
{
    createJSONAnchor->disable();
    createJSONAnchor->disable();
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseDeleteSource, this, _1, _2));
    Wt::Http::Message message;
    message.addBodyText("");
    
    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

    if(client1->deleteRequest(urlAdd ,message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::modifySearch()
{
    std::string badField = "";
    if(validateSearch(badField))
    {
        for(int row = 1; row < tableSeaTypeParam->rowCount(); row ++)
        {
            mapSeaTypeParameterValueSBoxRow[row-1] = mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[row - 1]]->text();
        }
        std::string tmpBool;
        if(boost::lexical_cast<int>(staticCB->isChecked()))
            tmpBool = "true";
        else
            tmpBool = "false";

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseModifySearch, this, _1, _2));
        
        std::string messAdd ="{\n    \"sea_period\" : " + boost::lexical_cast<std::string>(periodeLE->text()) +",\n\
    \"sea_is_static\" : " + tmpBool + ",\n\
    \"pos_key_value\" : " + boost::lexical_cast<std::string>(pKValueLE->text()) +",\n";
        if(mapSeaTypeParameterNameSBoxRow.size() == mapSeaTypeParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapSeaTypeParameterNameSBoxRow.size(); idx++)
            {
                
                 //si il y a des backslash dans le nom
                //////////////////////////////////
                std::string strTmp = boost::lexical_cast<std::string>(mapSeaTypeParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1);//
                messAdd += "\"" + boost::lexical_cast<std::string>(mapSeaTypeParameterNameSBoxRow[idx]) + "\": \""+ strTmp +"\"\n";
            }
            messAdd += "}\n";

            Wt::Http::Message message;
            message.addBodyText(messAdd);
            
            std::string urlAdd = this->getApiUrl() + "/plugins/"
                    + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                    + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
                    + "/searches/" + boost::lexical_cast<std::string>(mapSearchId[searchSelectionBox->currentIndex()])
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

            if(client1->post(urlAdd ,message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
                Wt::log("error") << "Error Client Http";

        }
        else
        {
            std::cerr << "[PluginEditionWidget] : mapSeaTypeParameterNameSBoxRow.size() =" << mapSeaTypeParameterNameSBoxRow.size() 
                      <<" != mapSeaTypeParameterValueSBoxRow.size() =" << mapSeaTypeParameterValueSBoxRow.size() <<std::endl;
       //     std::cerr << "post message" << messAdd << std::endl;
        }
    }
    else
    {
         Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") + 
                                tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                                tr("Alert.plugins.message-cause"),Wt::Ok);
        std::cerr << "[addSource] : Mal remplit"<< std::endl;
    }
}

void PluginEditionWidget::deleteSearch()
{
    createJSONAnchor->disable();
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseDeleteSearch, this, _1, _2));
    Wt::Http::Message message;
    message.addBodyText("");
    
    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "/searches/" + boost::lexical_cast<std::string>(mapSearchId[searchSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

    
    if(client1->deleteRequest(urlAdd ,message))
    {
        Wt::WApplication::instance()->deferRendering();    
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::addInformation()
{
    std::string tmpBoolDisplay;
    if(boost::lexical_cast<int>(mapCBDisplayInformation[currentInformation]->isChecked()))
    {
        tmpBoolDisplay = "true";
    }
    else
    {
        tmpBoolDisplay = "false";
    }

    
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddInformation, this, _1, _2));
    std::string messAdd ="{\n\"inf_name\" : \""+ boost::lexical_cast<std::string>(mapLENameInformation[currentInformation]->text()) +"\",\n\
\"inf_display\" : "+ tmpBoolDisplay + ",\n";
    if(boost::lexical_cast<int>(mapCBCalculateInformation[currentInformation]->isChecked()))
    {
        messAdd += "\"inf_calculate\" : \"" + boost::lexical_cast<std::string>(mapLECalculateInformation[currentInformation]->text()) +"\",\n"; 
    }

    messAdd += "\"inf_value_num\" : " + boost::lexical_cast<std::string>(currentInformation + 1) +"\n}\n";

    Wt::Http::Message message;
    message.addBodyText(messAdd);

    
    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "/searches/" + boost::lexical_cast<std::string>(mapSearchId[searchSelectionBox->currentIndex()]/*searchId*/)
            + "/informations"  // /informations/
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

    if(client1->post(urlAdd ,message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::createTableInformation()
{
    for(unsigned idx = 0; idx<mapLENameInformation.size(); idx++)
    {
        delete mapLENameInformation[idx];
        delete mapCBDisplayInformation[idx];
        delete mapCBCalculateInformation[idx];
        delete mapLECalculateInformation[idx];
    }
    tableSeaUnit->clear();
    tableSeaUnit->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.value-number-title")));
    tableSeaUnit->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.name-title")));
    tableSeaUnit->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.plugins.unit-title")));
    tableSeaUnit->elementAt(0, 3)->addWidget(new Wt::WText(tr("Alert.plugins.display-title")));
    tableSeaUnit->elementAt(0, 4)->addWidget(new Wt::WText(tr("Alert.plugins.calculate-title")));
    tableSeaUnit->elementAt(0, 5)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSeaUnit);
    mapCBUnit.clear();
    mapLENameInformation.clear();
    mapCBDisplayInformation.clear();
    mapCBCalculateInformation.clear();
    mapLECalculateInformation.clear();
    
    for (int idx=0 ; idx < boost::lexical_cast<int>(nbValueLE->text().toUTF8()); idx++)
    {

        mapCBUnit[idx] = new Wt::WComboBox(); 
        mapCBUnit[idx]->setModel(slmUnit);

        mapLENameInformation[idx] = new Wt::WLineEdit();
        mapLENameInformation[idx]->setValidator(validatorRegExp);

        mapCBDisplayInformation[idx] = new Wt::WCheckBox();

        mapCBCalculateInformation[idx] = new Wt::WCheckBox();
        mapCBCalculateInformation[idx]->clicked().connect(this, &PluginEditionWidget::updateStateLECalculateInformation);

        mapLECalculateInformation[idx] = new Wt::WLineEdit();
        mapLECalculateInformation[idx]->setValidator(validatorRegExp);
        mapLECalculateInformation[idx]->hide();
        
        tableSeaUnit->elementAt(idx + 1, 0)
            ->addWidget(new Wt::WText(boost::lexical_cast<std::string>(idx + 1)));
        tableSeaUnit->elementAt(idx + 1, 1)
            ->addWidget(mapLENameInformation[idx]);
        tableSeaUnit->elementAt(idx + 1, 2)
            ->addWidget(mapCBUnit[idx]);
        tableSeaUnit->elementAt(idx + 1, 3)
            ->addWidget(mapCBDisplayInformation[idx]);
        tableSeaUnit->elementAt(idx + 1, 4)
            ->addWidget(mapCBCalculateInformation[idx]);
        tableSeaUnit->elementAt(idx + 1, 5)
            ->addWidget(mapLECalculateInformation[idx]);

    }
    if(tableInformationState == "Modify")
    {
        client1 = new Wt::Http::Client();
        client1->setTimeout(30);
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseInformationsList, this, _1, _2));
        
        std::string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
                + "/searches/" + boost::lexical_cast<std::string>(mapSearchId[searchSelectionBox->currentIndex()])
                + "/informations"  // /informations/
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
        if(client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
        tableInformationState = "";
    }
    refresh1();
}

void PluginEditionWidget::updateStateLECalculateInformation()
{
    for(unsigned i=0; i<mapCBCalculateInformation.size(); i++)
    {
        if(mapCBCalculateInformation[i]->isChecked())
        {
            mapLECalculateInformation[i]->show();
        }
        else
        {
            mapLECalculateInformation[i]->hide();
        }
    }
}

void PluginEditionWidget::resetSource()
{
    for(unsigned idx = 0 ; idx < mapLEAddonParam.size() ; idx++)
    {
       delete mapLEAddonParam[mapAddonParameterNameSBoxRow[idx]];
//       tableSrcParam->deleteRow(idx+1);
    }
    
    sourceSelectionBox->clear();

    mapAddonParameterIdSboxRow.clear();
    mapAddonParameterNameSBoxRow.clear();
    mapAddonParameterValueSBoxRow.clear();
    mapLEAddonParam.clear();
    tableSrcParam->clear();
    tableSrcParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSrcParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSrcParam);
}

void PluginEditionWidget::resetSearch()
{
    comboSeaType->clear();
    for(unsigned idx = 0 ; idx < mapSeaTypeParameterNameSBoxRow.size() ; idx++)
    {
       delete mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[idx]];
    }
    searchSelectionBox->clear();
    mapSeaTypeParameterIdSboxRow.clear();
    mapSeaTypeParameterNameSBoxRow.clear();
    mapSeaTypeParameterValueSBoxRow.clear();
    mapLESeaTypeParam.clear();
    tableSeaTypeParam->clear();
    tableSeaTypeParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSeaTypeParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSeaTypeParam);
    periodeLE->setText("");
    staticCB->setChecked(false);
    pKValueLE->setText("0");
    nbValueLE->setText("");
    
      for(unsigned idx = 0; idx<mapLENameInformation.size(); idx++)
    {
        delete mapLENameInformation[idx];
        delete mapCBDisplayInformation[idx];
        delete mapCBCalculateInformation[idx];
        delete mapLECalculateInformation[idx];
    }
    tableSeaUnit->clear();
    tableSeaUnit->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.value-number-title")));
    tableSeaUnit->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.name-title")));
    tableSeaUnit->elementAt(0, 2)->addWidget(new Wt::WText(tr("Alert.plugins.unit-title")));
    tableSeaUnit->elementAt(0, 3)->addWidget(new Wt::WText(tr("Alert.plugins.display-title")));
    tableSeaUnit->elementAt(0, 4)->addWidget(new Wt::WText(tr("Alert.plugins.calculate-title")));
    tableSeaUnit->elementAt(0, 5)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSeaUnit);
    mapCBUnit.clear();
    mapLENameInformation.clear();
    mapCBDisplayInformation.clear();
    mapCBCalculateInformation.clear();
    mapLECalculateInformation.clear();
}

void PluginEditionWidget::selectedPlugin()
{
    createJSONAnchor->disable();
//    plgNameEdit->setText(pluginSelectionBox->currentText());
//    plgDescEdit->setText(mapPluginsDescrition[pluginSelectionBox->currentIndex()]);
    resetSource();
    resetSearch();
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSourceList, this, _1, _2));

    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "/sources"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
       
}

void PluginEditionWidget::selectedSource()
{
    resetSearch();
 
    Wt::Json::Object result ;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    try
    {                  
        Wt::Json::parse(strJSON, result);
        result1 = result.get("sources");
//      descriptif
        for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
        {
            Wt::Json::Object tmp = (*idx1);
            long long sourceID = tmp.get("id");

            if(sourceID == mapSourceId[sourceSelectionBox->currentIndex()])
            {
                long long idAddon = tmp.get("idAddon");
                for(unsigned i=0 ; i<mapAddonsIdSboxRow.size(); i++)
                {
                    if(idAddon == mapAddonsIdSboxRow[i])
                    {
                        comboAddon->setCurrentIndex(i); 
                        createFormSourceState = "Modify";
                        createFormSourceParameters();
                    }
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[selectedSource] Problems parsing JSON:" << strJSON;
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[selectedSource] Problems parsing JSON.:" << strJSON;
    }   
}

void PluginEditionWidget::selectedSearch()
{
    periodeLE->setText(boost::lexical_cast<std::string>(mapSearchPeriode[searchSelectionBox->currentIndex()]));
    staticCB->setChecked(mapSearchStatic[searchSelectionBox->currentIndex()]);
    pKValueLE->setText(boost::lexical_cast<std::string>(mapSearchPKValue[searchSelectionBox->currentIndex()]));
    nbValueLE->setText(boost::lexical_cast<std::string>(mapSearchNbValue[searchSelectionBox->currentIndex()]));
    for(unsigned i=0 ; i<mapSeaTypeIdSboxRow.size(); i++)
    {
        if(mapSearchSTYId[searchSelectionBox->currentIndex()] == mapSeaTypeIdSboxRow[i])
        {
//            std::cerr << mapSeaTypeIdSboxRow.size() << "\n i= "<< i << "\n\n";
//            std::cerr << mapSearchSTYId[searchSelectionBox->currentIndex()] << "\n i= "<< mapSeaTypeIdSboxRow[i] << "\n\n";
            //if() pour eviter boucle infini au cas ou un addon ne soit pas relié (dans TJ_STY_ADO) a un search type 
            if(mapSeaTypeIdSboxRow[i] != 0)
            {
                comboSeaType->setCurrentIndex(i); 
                createFormSearchState = "Modify";
                createFormSearchParameters();
                break;
            }
            else
            {
               // std::cerr << "test";
                i = mapSeaTypeIdSboxRow.size()*2;
            }
        }
        else // meme commentaire 
        {
            if(mapSeaTypeIdSboxRow[i] == 0)
            {
                 i = mapSeaTypeIdSboxRow.size()*2;
            }
           // std::cerr << "test1";
        }
    }
}

void PluginEditionWidget::completFormSource()
{
    Wt::Json::Object result ;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    try
    {                  
        Wt::Json::parse(strJSON, result);
        result1 = result.get("sources");
//      descriptif
        for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
        {
            Wt::Json::Object tmp = (*idx1);
            long long sourceID = tmp.get("id");

            if(sourceID == mapSourceId[sourceSelectionBox->currentIndex()])
            {
                Wt::Json::Object tmp1 = tmp.get("params");
                for(unsigned idx = 0; idx < mapAddonParameterIdSboxRow.size(); idx++)
                {
                     //au cas ou il y ait des backslash dans le nom
                    Wt::WString tmp2 = tmp1.get(boost::lexical_cast<std::string>(mapAddonParameterNameSBoxRow[idx]));
                    std::string strTmp = boost::lexical_cast<std::string>(tmp2);
//                    formatValue(strTmp, 1);
                    mapAddonParameterValueSBoxRow[idx] = strTmp;
                    mapLEAddonParam[mapAddonParameterNameSBoxRow[idx]]->setText(mapAddonParameterValueSBoxRow[idx]); 
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[completFormSource] Problems parsing JSON:" << strJSON;
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[completFormSource] Problems parsing JSON.:" << strJSON;
    }    

}

void PluginEditionWidget::completFormSearch()
{
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchParameters, this, _1, _2));

    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "/searches/" + boost::lexical_cast<std::string>(mapSearchId[searchSelectionBox->currentIndex()])
            + "/parameters" // "/parameters/" 
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    
    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::handleHttpResponseInformationsList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {      
//        formatValue(response.body(), 1);//-1
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {          
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                int valueNb = tmp1.get("sub_search_number");
                if(valueNb>0 && valueNb<= boost::lexical_cast<int>(nbValueLE->text()))
                {
                    int unitId = tmp1.get("unit_id");
                    for(unsigned i=0 ; i<mapUnitIdSboxRow.size(); i++)
                    {
                        if(unitId == mapUnitIdSboxRow[i])
                        {
                            mapCBUnit[valueNb-1]->setCurrentIndex(i);
                        }
                    }
                    if(tmp.contains("calculate"))
                    {
                        mapCBCalculateInformation[valueNb-1]->setChecked();
                        mapLECalculateInformation[valueNb-1]->setText(tmp.get("calculate"));
                        mapLECalculateInformation[valueNb-1]->show();
                    }
                    mapLENameInformation[valueNb-1]->setText(tmp.get("name"));
                    mapCBDisplayInformation[valueNb-1]->setChecked(tmp.get("display"));
                }
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseInformationsList] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseInformationsList] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
        Wt::log("warning") << "[ERROR] : fct handleHttpResponseInformationsList" << response.body();
    }
    refresh1();
}

void PluginEditionWidget::handleHttpResponseSearchParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
     Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {      
//        formatValue(response.body(), 1);//-1
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {          
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                long long seaParamID = tmp1.get("search_parameter_id");
                Wt::WString paramValue = tmp.get("value");
                for(unsigned i=0 ; i<mapSeaTypeParameterIdSboxRow.size(); i++)
                {
                    if(seaParamID == mapSeaTypeParameterIdSboxRow[i])
                    {
                         //au cas ou il y ait des backslash dans le nom
                        //////////////////////////////////
                        std::string strTmp = boost::lexical_cast<std::string>(paramValue);
//                        formatValue(strTmp, 1);
                        mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[i]]->setValidator(validatorRegExp);
                        mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[i]]->setText(strTmp);
                    }
                }
            }
            tableInformationState = "Modify";
            createTableUnit();
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchParameters] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchParameters] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << " fct handleHttpResponseSearchParameters" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseSourceList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    sourceSelectionBox->clear();
    mapSourceId.clear();
    int idx = 0;
    
    if(response.status()==200)
    {     
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {                  
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                long long sourceID = tmp1.get("source_id");
                sourceSelectionBox->addItem("source " + boost::lexical_cast<std::string>(sourceID));
                mapSourceId[idx] = sourceID;
                idx ++;
            }                
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSourceList] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSourceList] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseSourceList" << response.body();
    }
    refresh1();
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));

    formJSON = "string";
    
    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::handleHttpResponseSearchList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    searchSelectionBox->clear();
    mapSearchPeriode.clear();
    mapSearchStatic.clear();
    mapSearchPKValue.clear();
    mapSearchNbValue.clear();
    mapSearchSTYId.clear();
    mapSearchId.clear();
    int idx = 0;
    
    if(response.status()==200)
    {      
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {       
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                long long seaID = tmp1.get("search_id");
                
                Wt::Json::Object tmp2 = tmp.get("search_type");
                mapSearchSTYId[idx] = tmp2.get("id");
                
                searchSelectionBox->addItem("search " + boost::lexical_cast<std::string>(seaID));
                mapSearchId[idx] = seaID;
                mapSearchPeriode[idx] = tmp.get("period");

                mapSearchStatic[idx] = tmp.get("is_static");
                mapSearchNbValue[idx] = tmp.get("nb_value");
                mapSearchPKValue[idx] = tmp.get("pos_key_value");
                idx ++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchList] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchList] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseSearchList" << response.body();
    }
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchTypeList, this, _1, _2));
    
    std::string urlAdd = this->getApiUrl() + "/addons/"
            + boost::lexical_cast<std::string>(mapAddonsIdSboxRow[comboAddon->currentIndex()])
            + "/search_types" 
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    
    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }  
    else
        Wt::log("error") << "Error Client Http";
}



void PluginEditionWidget::handleHttpResponseUnits(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering(); 
    delete client1;
    int idx = 0;
    mapUnitIdSboxRow.clear();
    delete slmUnit;
    slmUnit = new Wt::WStringListModel();

    if(response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {            
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long unitId = tmp.get("id");
                Wt::WString name = tmp.get("name");
                mapUnitIdSboxRow[idx] = unitId;
                slmUnit->insertString(idx,name);

                idx++;
            }
            createTableInformation();
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseUnits] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseUnits] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
        Wt::log("warning") << "[ERROR] : fct handleHttpResponseUnits" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseAddInformation(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {      
        currentInformation++;
        if(currentInformation < boost::lexical_cast<int>(nbValueLE->text()))
        {
            addInformation();
        }
        else
        {
            refresh1();
            selectedSource();
        }
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseAddInformation" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseAddSearch(boost::system::error_code err, const Wt::Http::Message& response)
{
      Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {      
        Wt::Json::Object result ;
        try
        {              
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            //descriptif
            Wt::Json::Object result1 = result.get("id");
            mapSearchId[mapSearchId.size()] = result1.get("search_id");
            searchSelectionBox->addItem("search " + boost::lexical_cast<std::string>(mapSearchId[mapSearchId.size()-1]));
            searchSelectionBox->setCurrentIndex(mapSearchId.size()-1);
            if(mapLENameInformation.size() > 0)
            {
                currentInformation = 0;
                addInformation();
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddSearch] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddSearch] Problems parsing JSON.:" << response.body();
        }   

    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseAddSearch" << response.body();
    }
    refresh1();
}

void PluginEditionWidget::handleHttpResponseModifySource(boost::system::error_code err, const Wt::Http::Message& response)
{
     Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {    
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));

        formJSON = "string";
    
        std::string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
        if(client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseModifySource" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseModifySearch(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {      
        selectedSource();
        Wt::log("warning") << "fct handleHttpResponseModifySearch" << response.body();
//        Wt::Json::Object result ;
//        try
//        {                  
//            Wt::Json::parse(response.body(), result);
//            //descriptif
////            Wt::Json::Object result1 = result.get("id");   
////            mapSourceId[mapSourceId.size()] = result1.get("source_id");
////            sourceSelectionBox->addItem("source " + boost::lexical_cast<std::string>(mapSourceId[mapSourceId.size()-1]));
////            sourceSelectionBox->setCurrentIndex(mapSourceId.size()-1);
//        }
//        catch (Wt::Json::ParseError const& e)
//        {
//            Wt::log("warning") << "[handleHttpResponseModifySearch] Problems parsing JSON:" << response.body();
//        }
//        catch (Wt::Json::TypeException const& e)
//        {
//            Wt::log("warning") << "[handleHttpResponseModifySearch] Problems parsing JSON.:" << response.body();
//        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseModifySearch" << response.body();
    }
    refresh1();
    
}

void PluginEditionWidget::handleHttpResponseAddSource(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {      
        buttonModifySource->enable();
        Wt::Json::Object result ;
        try
        {            
            std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            //descriptif
            Wt::Json::Object result1 = result.get("id");   
            mapSourceId[mapSourceId.size()] = result1.get("source_id");
            sourceSelectionBox->addItem("source " + boost::lexical_cast<std::string>(mapSourceId[mapSourceId.size()-1]));
            sourceSelectionBox->setCurrentIndex(-1);//utile lorsqu'on crée la premiére source sinon elle ne se selectionne pas automatiqement
            sourceSelectionBox->setCurrentIndex(mapSourceId[mapSourceId.size()-1]);
            std::cerr << sourceSelectionBox->currentIndex() << std::endl;
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddSource] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddSource] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseAddSource" << response.body();
    }
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));

    formJSON = "string";
    
    std::string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    
    if(client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::handleHttpResponseAddPlg(boost::system::error_code err, const Wt::Http::Message& response)
{
    plgNameEdit->setText("");
    plgDescEdit->setText("");
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if(response.status()==200)
    {
        Wt::Json::Object result ;
        try
        {         
            std::string strTmp1 = response.body();
                        formatValue(strTmp1, 1);//-1
            Wt::Json::parse(strTmp1, result);
//            Wt::Json::parse(response.body(), result);
            //descriptif
            
            long long pluginId = result.get("id");
            Wt::WString name = result.get("name");
            Wt::WString pluginDesc = result.get("desc");
            
            //si il y a des backslash dans le nom
            std::string strTmp = boost::lexical_cast<std::string>(name);
//            formatValue(strTmp, 1);
            pluginSelectionBox->addItem(strTmp);
            mapPluginsIdSboxRow[mapPluginsIdSboxRow.size()] = pluginId;
            pluginSelectionBox->setCurrentIndex(-1);//utile lorsqu'on crée le premier plugin sinon elle ne se selectionne pas automatiqement
            pluginSelectionBox->setCurrentIndex(mapPluginsIdSboxRow.size());
            mapPluginsDescrition[mapPluginsIdSboxRow.size()-1] = pluginDesc;
            Wt::WPushButton *deletePlugin = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-plugin-button"));
            deletePlugin->addStyleClass("btn");
            deletePlugin->addStyleClass("btn-danger");
            deletePlugin->setTextFormat(Wt::XHTMLUnsafeText);
            int row = tablePlugin->rowCount() - 1;
            deletePlugin->clicked().connect( boost::bind(&PluginEditionWidget::deletePlugin,this, row-1));
            tablePlugin->insertRow(row);
            tablePlugin->elementAt(row,0)->addWidget(new Wt::WText(strTmp));
            tablePlugin->elementAt(row,1)->addWidget(new Wt::WText(pluginDesc));
            tablePlugin->elementAt(row,2)->addWidget(deletePlugin);   
            
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddPlg] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddPlg] Problems parsing JSON.:" << response.body();
        } 
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseAddPlg" << response.body();
    }
    refresh1();
}



void PluginEditionWidget::handleHttpResponseSeaTypeParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    int idx = 0;
    for(unsigned idx1 = 0 ; idx1 < mapSeaTypeParameterNameSBoxRow.size() ; idx1++)
    {
       delete mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[idx1]];
    }
    mapSeaTypeParameterIdSboxRow.clear();
    mapSeaTypeParameterNameSBoxRow.clear();
    mapSeaTypeParameterValueSBoxRow.clear();
    mapLESeaTypeParam.clear();
    tableSeaTypeParam->clear();
    tableSeaTypeParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSeaTypeParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSeaTypeParam);
    
    if(response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {                  
std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long parameterId = tmp.get("id");
                Wt::WString name = tmp.get("name");
                mapSeaTypeParameterIdSboxRow[idx] = parameterId;
                mapSeaTypeParameterNameSBoxRow[idx] = name;
                mapLESeaTypeParam[name] = new Wt::WLineEdit();
               // mapLESeaTypeParam[name]->setStyleClass("input-xxlarge");
                mapLESeaTypeParam[name]->setValidator(validatorRegExp);
                tableSeaTypeParam->elementAt(idx + 1, 0)
                    ->addWidget(new Wt::WText(name));
                tableSeaTypeParam->elementAt(idx + 1, 1)
                    ->addWidget(mapLESeaTypeParam[name]);
       
                idx++;
            }
            if(createFormSearchState == "Modify")
            {
                completFormSearch();
                createFormSearchState = "";
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSeaTypeParameters] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSeaTypeParameters] Problems parsing JSON.:" << response.body();
        }   
    }
    else if(response.status() == 404 && createFormSearchState == "Modify")
    {
        createFormSearchState = "";
        tableInformationState = "Modify";
        createTableUnit();
    }
    else
    {
        Wt::log("warning") << "fct handleHttpResponseSeaTypeParameters" << response.body();
        createFormSearchState = "";
    }
    refresh1();
}

void PluginEditionWidget::handleHttpResponseAddonParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();

    delete client1;
    int idx = 0;
    
    for(unsigned idx = 0 ; idx < mapLEAddonParam.size() ; idx++)
    {
       delete mapLEAddonParam[mapAddonParameterNameSBoxRow[idx]];
    }
    
    mapAddonParameterIdSboxRow.clear();
    mapAddonParameterNameSBoxRow.clear();
    mapAddonParameterValueSBoxRow.clear();
    mapLEAddonParam.clear();
    tableSrcParam->clear();
    tableSrcParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSrcParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSrcParam);
    if(response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {        
std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long parameterId = tmp.get("id");
                Wt::WString name = tmp.get("name");
                             
                mapAddonParameterIdSboxRow[idx] = parameterId;
                mapAddonParameterNameSBoxRow[idx] = name;
                mapLEAddonParam[name] = new Wt::WLineEdit();
                mapLEAddonParam[name]->setValidator(validatorRegExp);
                /////////////// Attention méthode qui ne marche que pour l'affichage dans le gui (en base le passwd sera en claire.
                if (name == "passwd")
                {
                    mapLEAddonParam[name]->setEchoMode(Wt::WLineEdit::Password);
                }
                /////////////// If temporaire
                
                tableSrcParam->elementAt(idx + 1, 0)
                    ->addWidget(new Wt::WText(name));
                tableSrcParam->elementAt(idx + 1, 1)
                    ->addWidget(mapLEAddonParam[name]);
                idx++;
            }
            
            if(createFormSourceState == "Modify")
            {
                completFormSource();
            }

        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddonParameters] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddonParameters] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseAddonParameters" << response.body();
    }
    
    if(createFormSourceState == "Modify")
    {
        client1 = new Wt::Http::Client();

        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchList, this, _1, _2));
    
        std::string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<std::string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "/sources/" + boost::lexical_cast<std::string>(mapSourceId[sourceSelectionBox->currentIndex()])
                + "/searches"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

        if(client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    else
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchTypeList, this, _1, _2));
    
        std::string urlAdd = this->getApiUrl() + "/addons/"
                + boost::lexical_cast<std::string>(mapAddonsIdSboxRow[comboAddon->currentIndex()])
                + "/search_types"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

        if(client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
     createFormSourceState = "";
     refresh1();
}

void PluginEditionWidget::handleHttpResponsePlgList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    
    int countTmp = tablePlugin->rowCount() -1;
    for(int i=1; i<countTmp; i++)
    {
        tablePlugin->deleteRow(1);
    }
    
    
    delete client1;
    pluginSelectionBox->clear();
    mapPluginsIdSboxRow.clear();
    mapPluginsDescrition.clear();
    int idx = 0;
    if (response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {      
std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);          
//            Wt::Json::parse(response.body(), result);
            result1 = result;
            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                int pluginId = tmp.get("id");
                Wt::WString name = tmp.get("name");
                Wt::WString pluginDesc = tmp.get("desc");
                
                //si il y a des backslash dans le nom
                std::string strTmp = boost::lexical_cast<std::string>(name); 
//                formatValue(strTmp, 1);
                pluginSelectionBox->addItem(strTmp);
                mapPluginsIdSboxRow[idx] = pluginId;
                mapPluginsDescrition[idx] = pluginDesc;
                
                int row = tablePlugin->rowCount() - 1;
       
                Wt::WPushButton *deletePlugin = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-plugin-button"));
                deletePlugin->addStyleClass("btn");
                deletePlugin->addStyleClass("btn-danger");
                deletePlugin->setTextFormat(Wt::XHTMLUnsafeText);
                deletePlugin->clicked().connect( boost::bind(&PluginEditionWidget::deletePlugin,this, row-1));

                tablePlugin->insertRow(row);
                tablePlugin->elementAt(row,0)->addWidget(new Wt::WText(strTmp));
                tablePlugin->elementAt(row,1)->addWidget(new Wt::WText(pluginDesc));
                tablePlugin->elementAt(row,2)->addWidget(deletePlugin);            
                idx ++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponsePlgList] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponsePlgList] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponsePlgList" << response.body();
    }
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddonList, this, _1, _2));
    
    std::string urlAdd = this->getApiUrl() + "/addons"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if(client1->get(urlAdd))
    {
         Wt::WApplication::instance()->deferRendering();
    } 
    else
        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::handleHttpResponseAddonList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    this->mapAddonsIdSboxRow.clear();
    comboAddon->clear();
    comboAddon->setModel(slmAddons);
    int idx = 0;
    if (response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {      
std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long addonId = tmp.get("id");
                Wt::WString name = tmp.get("name");

                slmAddons->insertString(idx,name);
                this->mapAddonsIdSboxRow[idx] = addonId;
                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddonList] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddonList] Problems parsing JSON.:" << response.body();
        }   
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseAddonList" << response.body();
    }
    refresh1();
}

void PluginEditionWidget::handleHttpResponseSearchTypeList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    int idx = 0;
    comboSeaType->clear();

    
    if (response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {              
std::string strTmp = response.body();
                        formatValue(strTmp, 1);//-1
            Wt::Json::parse(strTmp, result);
//            Wt::Json::parse(response.body(), result);
            result1 = result;
//            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long seaTypeId = tmp.get("id");
                Wt::WString name = tmp.get("name");

                slmSeaType->insertString(idx,name);
                this->mapSeaTypeIdSboxRow[idx] = seaTypeId;
                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchTypeList] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchTypeList] Problems parsing JSON.:" << response.body();
        }          
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponseSearchTypeList" << response.body();
    }
    comboSeaType->setModel(slmSeaType);
    comboSeaType->setCurrentIndex(0);
    createFormSearchParameters();
    
}

void PluginEditionWidget::handleHttpResponsePlgJSON(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;    
    if (response.status() == 200)
    {
        if(formJSON == "string")
        {
            strJSON = response.body(); 
                        formatValue(strJSON, 1);//-1
            formJSON = "";
            createFormSourceParameters();
        }
        else
        {
            
            char *tmpname = strdup("/tmp/echoes-tmp-plugin-fileXXXXXX");
            int mkstempRes = mkstemp(tmpname);
            Wt::log("debug") << "[AssetManagementWidget] " << "Res temp file creation : " << mkstempRes;
    
            std::ofstream fileJSON(tmpname); 
            //fileJSON.open( boost::lexical_cast<std::string>(pluginSelectionBox->currentText()) + ".json", std::ios::out);
            fileJSON << "/*\n\
 * " + pluginSelectionBox->currentText().toUTF8() + "\n\
 * \n\
 * Static file for ECHOES Alert Probe generated by ECHOES Alert API.\n\
 * DO NOT EDIT THIS FILE BY HAND WITHOUT SUPPORT AUTHORIZATION.\n\
 * \n\
 */\n\n";
            fileJSON << response.body();
            fileJSON.close();
            
            
           
    
            std::string pluginNameSpacesReplaced = pluginSelectionBox->currentText().toUTF8();
            boost::replace_all(pluginNameSpacesReplaced, " ", "_");

            // creating resource to send to the client
            Wt::WFileResource *res = new Wt::WFileResource();
            res->setFileName(tmpname);
            
            res->suggestFileName("ea-plugin_" + pluginNameSpacesReplaced + ".json",Wt::WResource::Attachment);
            res->setMimeType("application/x-json");

            createJSONAnchor->setLink(res);
            
            createJSONAnchor->enable();
        }
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponsePlgJSON" << response.body();
    }   
    refresh1();
}

void PluginEditionWidget::handleHttpResponseDeletePlg(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 204)
    {
        resetSearch();
        resetSource();
        pluginSelectionBox->setCurrentIndex(-1);
        plgDescEdit->setText("");
        plgNameEdit->setText("");
        
        
        client1 = new Wt::Http::Client();
        client1->setTimeout(30);
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgList, this, _1, _2));
        
        std::string urlAdd = this->getApiUrl() + "/plugins"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

        if(client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
      
    }
    else if (response.status() == 409)
    {
       Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body() ,Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body() ,Wt::Ok);
         Wt::log("warning") << "fct handleHttpResponseDeletePlg" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseDeleteSource(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 204)
    {
        selectedPlugin();
    }
    else if (response.status() == 409) 
    {
       Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body() ,Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body() ,Wt::Ok);
         Wt::log("warning") << "fct handleHttpResponseDeleteSource" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseDeleteSearch(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();  
    delete client1;
    if (response.status() == 204)
    {
        selectedSource();
    }
    else if (response.status() == 409)
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body() ,Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body() ,Wt::Ok);
         Wt::log("warning") << "[ERROR] : fct handleHttpResponseDeleteSearch" << response.body();
    }
}


void PluginEditionWidget::searchToken()
{
    try
    {      
        Wt::Dbo::Transaction transaction(*(this->session));
        
        //on prend le token de la ligne avec le plus grand id (le token le plus récemment créé)
        std::string queryStr = "SELECT \"USR_TOKEN\" FROM \"T_USER_USR\" "
                               " WHERE \"USR_MAIL\"  = '" + boost::lexical_cast<std::string>(this->session->user().get()->eMail.toUTF8()) +
                               "' ORDER BY \"USR_TOKEN\" DESC "
                               " LIMIT 1";

        token = session->query<Wt::WString> (queryStr);
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
}

void PluginEditionWidget::formatValue(std::string &value, int nbBackslash) const
{
    int compt = 0;
    while( compt != -1)
    {
        if(value.find("\\",compt) < value.size() && value.find("\\",compt) >= 0 )
        {
            for(int i=0; i<nbBackslash; i++)
            {
                value = value.replace(value.find("\\", compt), 1, "\\\\");
            }
            compt = value.find("\\",compt); //compt = l'indexe ou se trouve le premier caractére "\" dans la string.
                                            // recherche dans la string à partir de l'indexe compt;
            compt = compt + nbBackslash*2;//value.find_first_not_of("\\", compt+1); 
        }
        else 
        {
            compt=-1;
        }
    }
}

void PluginEditionWidget::setApiUrl(std::string apiUrl)
{
    _apiUrl = apiUrl;
    return;
}

std::string PluginEditionWidget::getApiUrl() const
{
    return _apiUrl;
}

