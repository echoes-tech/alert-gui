/* 
 * File:   PluginEditionWidget.cpp
 * Author: gdr
 *
 * Created on 18 mars 2013, 11:59
 */

#include <Wt/Utils>

#include "PluginEditionWidget.h"

Wt::Json::Array Wt::Json::Array::Empty;

using namespace std;

PluginEditionWidget::PluginEditionWidget(Echoes::Dbo::Session* session, const string &apiUrl)
: Wt::WTemplateFormView(Wt::WString::tr("Alert.plugins.Edition.template"))
{
    this->session = session;
    setApiUrl(apiUrl);

    if (this->session->user().id() != -1)
    {
        Wt::WApplication *app = Wt::WApplication::instance();
        app->enableUpdates(true);
        app->messageResourceBundle().use("plugin", false);
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
    validatorRegExp->setMandatory(true);

    // creation plugin
    pluginSelectionBox = new Wt::WComboBox();

    pluginSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedPlugin);
    bindWidget("plugin-selection-box", pluginSelectionBox);

    plgNameEdit = new Wt::WLineEdit();
    plgNameEdit->setValidator(validatorRegExp);

    plgDescEdit = new Wt::WLineEdit();
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

    // SOURCE
    sourceSelectionBox = new Wt::WSelectionBox();
    bindWidget("source-selection-box", sourceSelectionBox);
    sourceSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedSource);

    comboAddon = new Wt::WComboBox();
    slmAddons = new Wt::WStringListModel();
//    comboAddon->setModel(slmAddons);
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

    buttonModifySource = new Wt::WPushButton(tr("Alert.plugins.modify-source-button"));
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

    // SEARCH
    searchSelectionBox = new Wt::WSelectionBox();
    bindWidget("plugin-search-selection-box", searchSelectionBox);
    searchSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedSearch);

    comboSearchType = new Wt::WComboBox();
    slmSearchTypes = new Wt::WStringListModel();
    comboSearchType->setModel(slmSearchTypes);
    comboSearchType->clicked().connect(this, &PluginEditionWidget::createFormSearchParameters);
    bindWidget("plugin-combo-search-type", comboSearchType);

    tableSeaParam = new Wt::WTable();
    bindWidget("plugin-table-search-param", tableSeaParam);
    fixStyleTable(tableSeaParam);

    tableSeaParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSeaParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));

    Wt::WPushButton *addSearch = new Wt::WPushButton("<span class='icon'><i class='icon-plus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.add-search-button"));
    addSearch->addStyleClass("btn");
    addSearch->addStyleClass("btn-info");
    addSearch->setTextFormat(Wt::XHTMLUnsafeText);
    addSearch->clicked().connect(this, &PluginEditionWidget::addSearch);
    bindWidget("plugin-add-search-button", addSearch);

    buttonModifySearch = new Wt::WPushButton(tr("Alert.plugins.modify-search-button"));
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

    // FILTER
    filterSelectionBox = new Wt::WSelectionBox();
    filterSelectionBox->clicked().connect(this, &PluginEditionWidget::selectedFilter);
    bindWidget("plugin-filter-selection-box", filterSelectionBox);

    comboFilType = new Wt::WComboBox();
    slmFilType = new Wt::WStringListModel();
    comboFilType->setModel(slmFilType);
    comboFilType->clicked().connect(this, &PluginEditionWidget::createFormFilterParameters);
    bindWidget("plugin-combo-filter-type", comboFilType);

    tableFilTypeParam = new Wt::WTable();
    fixStyleTable(tableFilTypeParam);
    tableFilTypeParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableFilTypeParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    bindWidget("plugin-table-filter-type-param", tableFilTypeParam);

    periodeLE = new Wt::WLineEdit();
    periodeLE->setValidator(validatorInt);
    bindWidget("plugin-search-period", periodeLE);

    staticCB = new Wt::WCheckBox("Static");
    bindWidget("plugin-filter-static", staticCB);

    pKValueLE = new Wt::WLineEdit();
    pKValueLE->setText("0");
    pKValueLE->setValidator(validatorInt);
    bindWidget("plugin-filter-key-value", pKValueLE);

    nbValueLE = new Wt::WLineEdit();
    nbValueLE->setValidator(validatorInt);
    bindWidget("plugin-filter-nomber-value", nbValueLE);

    Wt::WPushButton *addFilter = new Wt::WPushButton("<span class='icon'><i class='icon-plus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.add-filter-button"));
    addFilter->addStyleClass("btn");
    addFilter->addStyleClass("btn-info");
    addFilter->setTextFormat(Wt::XHTMLUnsafeText);
    addFilter->clicked().connect(this, &PluginEditionWidget::addFilter);
    bindWidget("plugin-add-filter-button", addFilter);

    buttonModifyFilter = new Wt::WPushButton(tr("Alert.plugins.modify-filter-button"));
    buttonModifyFilter->addStyleClass("btn");
    buttonModifyFilter->addStyleClass("btn-primary");
    buttonModifyFilter->setTextFormat(Wt::XHTMLUnsafeText);
    buttonModifyFilter->clicked().connect(this, &PluginEditionWidget::modifyFilter);
    bindWidget("plugin-modify-filter-button", buttonModifyFilter);

    Wt::WPushButton *deleteFilter = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-filter-button"));
    deleteFilter->addStyleClass("btn");
    deleteFilter->addStyleClass("btn-danger");
    deleteFilter->setTextFormat(Wt::XHTMLUnsafeText);
    deleteFilter->clicked().connect(this, &PluginEditionWidget::deleteFilter);
    bindWidget("plugin-delete-filter-button", deleteFilter);

    // JSON BUTTONS
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
    
    buttonModifySource->disable();
    buttonModifySearch->disable();
    buttonModifyFilter->disable();
    
    // Once everything is created, plugin list update
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgList, this, _1, _2));

    string urlPlugins = this->getApiUrl() + "/plugins"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlPlugins;
    if (client1->get(urlPlugins))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
    
}

void PluginEditionWidget::handleHttpResponsePlgList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();

    int countTmp = tablePlugin->rowCount() - 1;
    for (int i = 1; i < countTmp; i++)
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
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            //            Wt::Json::parse(response.body(), result);
            result1 = result;
            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                int pluginId = tmp.get("id");
                Wt::WString name = tmp.get("name");
                Wt::WString pluginDesc = tmp.get("desc");

                //si il y a des backslash dans le nom
                string strTmp = boost::lexical_cast<string>(name);
                //                formatValue(strTmp, 1);
                pluginSelectionBox->addItem(strTmp);
                mapPluginsIdSboxRow[idx] = pluginId;
                mapPluginsDescrition[idx] = pluginDesc;

                int row = tablePlugin->rowCount() - 1;

                Wt::WPushButton *deletePlugin = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-plugin-button"));
                deletePlugin->addStyleClass("btn");
                deletePlugin->addStyleClass("btn-danger");
                deletePlugin->setTextFormat(Wt::XHTMLUnsafeText);
                deletePlugin->clicked().connect(boost::bind(&PluginEditionWidget::deletePlugin, this, row - 1));

                tablePlugin->insertRow(row);
                tablePlugin->elementAt(row, 0)->addWidget(new Wt::WText(strTmp));
                tablePlugin->elementAt(row, 1)->addWidget(new Wt::WText(pluginDesc));
                tablePlugin->elementAt(row, 2)->addWidget(deletePlugin);
                idx++;
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

    //setup addons
    fillAddonComboBox();
    
}

// Quand on selectionne un plugin
void PluginEditionWidget::selectedPlugin()
{
    createJSONAnchor->disable();

    resetSource();
    resetSearch();
    resetFilter();

    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSourceList, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/sources/"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&plugin_id=" + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

}

// on récupère la liste des sources
void PluginEditionWidget::handleHttpResponseSourceList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    sourceSelectionBox->clear();
    mapSourceId.clear();
    int idx = 0;

    if (response.status() == 200)
    {
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            result1 = result;
            //            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long sourceID = tmp.get("id");
                sourceSelectionBox->addItem("source " + boost::lexical_cast<string>(sourceID));
                mapSourceId[idx] = sourceID;
                idx++;
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
    
    //setup addons
    fillAddonComboBox();
    
    
    
    refresh1();

}


void PluginEditionWidget::displayButtonCreateJSON()
{
    createJSONAnchor->disable();
}

void PluginEditionWidget::createJSON()
{
    if (pluginSelectionBox->currentIndex() != -1)
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));

        string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();


        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
        if (client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-no-selected-plugin"), Wt::Ok);
    }
}

void PluginEditionWidget::createFormSearchParameters()
{
    if (createFormSearchState == "Modify")
    {
//        buttonModifySearch->enable();
        buttonModifySearch->disable();
    }
    else
    {
        buttonModifySearch->disable();
    }

    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSeaTypeParameters, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/searches/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&type_id=" + boost::lexical_cast<string>(mapSeaTypeIdSboxRow[comboSearchType->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::createFormFilterParameters()
{
    if (createFormFilterState == "Modify")
    {
//        buttonModifyFilter->enable();
        buttonModifyFilter->disable();
    }
    else
    {
        buttonModifyFilter->disable();
    }

    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseFilTypeParameters, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/filters/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&type_id=" + boost::lexical_cast<string>(mapFilTypeIdSboxRow[comboFilType->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::createFormSourceParameters()
{
    resetSearch();
    resetFilter();
    if (createFormSourceState != "Modify")
    {
        buttonModifySource->disable();
    }
    else
    {
//        buttonModifySource->enable();
        buttonModifySource->disable();
    }
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddonList, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/sources/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&addon_id=" + boost::lexical_cast<string>(mapAddonsIdSboxRow[comboAddon->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

}

bool PluginEditionWidget::validatePlugin(string &badField)
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
    string badField = "";
    resetSearch();
    resetSource();
    if (validatePlugin(badField))
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddPlg, this, _1, _2));

        //si il y a des backslash dans le nom
        string strTmp = boost::lexical_cast<string>(plgNameEdit->text());
        formatValue(strTmp, 1); //
        string strTmp2 = boost::lexical_cast<string>(plgDescEdit->text());
        formatValue(strTmp2, 1); //
        string messAdd("{\n\
            \"name\" : \"" + strTmp + "\",\n\
            \"desc\" : \"" + strTmp2 + "\"\n\
     }\n");

        Wt::Http::Message message;
        message.addBodyText(messAdd);

        string urlAdd = this->getApiUrl() + "/plugins"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

        if (client1->post(urlAdd, message))
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
                              tr("Alert.plugins.message-cause"), Wt::Ok);
        cerr << "[addPlugin] : Mal remplit" << endl;
    }
}

bool PluginEditionWidget::validateSource(string &badField)
{
    if (pluginSelectionBox->currentIndex() != -1)
    {
        for (unsigned i = 0; i < mapAddonParameterNameSBoxRow.size(); i++)
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
    string badField = "";
    if (validateSource(badField))
    {
//        buttonModifySource->enable();
        for (int row = 1; row < tableSrcParam->rowCount(); row++)
        {
            mapAddonParameterValueSBoxRow[row - 1] = mapLEAddonParam[mapAddonParameterNameSBoxRow[row - 1]]->text();
        }

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddSource, this, _1, _2));
        string messAdd = "{\n\"addon_id\": " + boost::lexical_cast<string>(mapAddonsIdSboxRow[comboAddon->currentIndex()]) + ","
                + "\n\"plugin_id\": " + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()]);

        if (mapAddonParameterNameSBoxRow.size() == mapAddonParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapAddonParameterNameSBoxRow.size(); idx++)
            {
                //au cas ou il y ait des backslash dans le nom
                string strTmp = boost::lexical_cast<string>(mapAddonParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1); //
                messAdd += ",\n\"" + boost::lexical_cast<string>(mapAddonParameterNameSBoxRow[idx]) + "\": \"" + strTmp + "\"";
            }
            messAdd += "\n}\n";

            Wt::Http::Message message;
            message.addBodyText(messAdd);

            string urlAdd = this->getApiUrl() + "/sources"
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

            if (client1->post(urlAdd, message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
                Wt::log("error") << "Error Client Http";
        }
        else
        {
            cerr << "[PluginEditionWidget] : mapAddonParameterNameSBoxRow.size() =" << mapAddonParameterNameSBoxRow.size()
                    << " != mapAddonParameterValueSBoxRow.size() =" << mapAddonParameterValueSBoxRow.size() << endl;
        }
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") +
                              tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                              tr("Alert.plugins.message-cause"), Wt::Ok);
        cerr << "[addSource] : Mal remplit" << endl;
    }
}

bool PluginEditionWidget::validateSearch(string &badField)
{
    if (sourceSelectionBox->currentIndex() == -1 || pluginSelectionBox->currentIndex() == -1)
    {
        return false;
    }
    for (unsigned i = 0; i < mapSeaTypeParameterNameSBoxRow.size(); i++)
    {
        if (mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[i]]->validate() != Wt::WValidator::State::Valid)
        {
            badField = "Alert.plugins.parameter-title";
            return false;
        }
    }
    if (periodeLE->validate() != Wt::WValidator::State::Valid)
    {
        badField = "Alert.plugins.search-periode-label";
        return false;
    }

    return true;
}

bool PluginEditionWidget::validateFilter(string &badField)
{
    if (sourceSelectionBox->currentIndex() == -1 || pluginSelectionBox->currentIndex() == -1 || searchSelectionBox->currentIndex() == -1)
    {
        return false;
    }

    cout << "VALID 1" << endl;
    for (unsigned i = 0; i < mapFilTypeParameterNameSBoxRow.size(); i++)
    {
        cout << "VALID 1-" << i << endl;
        if (mapLEFilTypeParam[mapFilTypeParameterNameSBoxRow[i]]->validate() != Wt::WValidator::State::Valid)
        {
            badField = "Alert.plugins.parameter-title";
            return false;
        }
    }
    cout << "VALID 2" << endl;
    if (pKValueLE->validate() != Wt::WValidator::State::Valid)
    {
        badField = "Alert.plugins.filter-key-value";
        return false;
    }
    cout << "VALID 3" << endl;
    if (nbValueLE->validate() != Wt::WValidator::State::Valid)
    {
        badField = "Alert.plugins.filter-nomber-value";
        return false;
    }
    return true;
}

void PluginEditionWidget::addSearch()
{
    createJSONAnchor->disable();
    string badField = "";
    if (validateSearch(badField))
    {
        for (int row = 1; row < tableSeaParam->rowCount(); row++)
        {
            mapSeaTypeParameterValueSBoxRow[row - 1] = mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[row - 1]]->text();
        }

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddSearch, this, _1, _2));
        string messAdd =
                "{\n"
                "\"source_id\" : " + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()]) + ",\n"
                "\"type_id\" : " + boost::lexical_cast<string>(mapSeaTypeIdSboxRow[comboSearchType->currentIndex()]) + ",\n"
                "\"period\" : " + boost::lexical_cast<string>(periodeLE->text()) + ",\n";

        if (mapSeaTypeParameterNameSBoxRow.size() == mapSeaTypeParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapSeaTypeParameterNameSBoxRow.size(); idx++)
            {

                //si il y a des backslash dans le nom
                //////////////////////////////////
                string strTmp = boost::lexical_cast<string>(mapSeaTypeParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1); //
                messAdd += "\"" + boost::lexical_cast<string>(mapSeaTypeParameterNameSBoxRow[idx]) + "\": \"" + strTmp + "\"";
                if (idx != mapSeaTypeParameterNameSBoxRow.size() - 1)
                {
                    messAdd += ",\n";
                }

            }

            messAdd += "\n}\n";

            Wt::Http::Message message;
            message.addBodyText(messAdd);

            string urlAdd = this->getApiUrl() + "/searches"
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;
            Wt::log("debug") << "PluginEditionWidget : [POST] message : " << messAdd;

            if (client1->post(urlAdd, message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
            {
                Wt::log("error") << "Error Client Http";
            }

        }
        else
        {
            cerr << "[PluginEditionWidget] : mapSeaTypeParameterNameSBoxRow.size() =" << mapSeaTypeParameterNameSBoxRow.size()
                    << " != mapSeaTypeParameterValueSBoxRow.size() =" << mapSeaTypeParameterValueSBoxRow.size() << endl;
        }
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") +
                              tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                              tr("Alert.plugins.message-cause"), Wt::Ok);
        cerr << "[addSearch] : Mal remplit" << endl;
    }
}

void PluginEditionWidget::addFilter()
{
    cout << "STEP 0" << endl;
    createJSONAnchor->disable();
    string badField = "";
    if (validateFilter(badField))
    {
        cout << "STEP 1" << endl;
        for (int row = 1; row < tableFilTypeParam->rowCount(); row++)
        {
            mapFilTypeParameterValueSBoxRow[row - 1] = mapLEFilTypeParam[mapFilTypeParameterNameSBoxRow[row - 1]]->text();
        }

        cout << "STEP 2" << endl;
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddFilter, this, _1, _2));
        string messAdd =
                "{\n"
                "\"search_id\": " + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()]) + ",\n"
                "\"type_id\" : " + boost::lexical_cast<string>(mapFilTypeIdSboxRow[comboFilType->currentIndex()]) + ",\n"
                "\"position_key_value\" : " + boost::lexical_cast<string>(pKValueLE->text()) + ",\n"
                "\"nb_value\" : " + boost::lexical_cast<string>(nbValueLE->text()) + ",\n";

        cout << "STEP 3" << endl;
        if (mapFilTypeParameterNameSBoxRow.size() == mapFilTypeParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapFilTypeParameterNameSBoxRow.size(); idx++)
            {

                //si il y a des backslash dans le nom
                //////////////////////////////////
                string strTmp = boost::lexical_cast<string>(mapFilTypeParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1); //
                messAdd += "\"" + boost::lexical_cast<string>(mapFilTypeParameterNameSBoxRow[idx]) + "\": \"" + strTmp + "\"";
                if (idx != mapFilTypeParameterNameSBoxRow.size() - 1)
                {
                    messAdd += ",\n";
                }

            }
            messAdd += "\n}\n";
            Wt::Http::Message message;
            message.addBodyText(messAdd);

            string urlAdd = this->getApiUrl() + "/filters"
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;
            Wt::log("debug") << "PluginEditionWidget : [POST] message : " << messAdd;
            
            cout << "STEP 4" << endl;
            if (client1->post(urlAdd, message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
            {
                Wt::log("error") << "Error Client Http";
            }

        }
        else
        {
            cerr << "[PluginEditionWidget] : mapFilTypeParameterNameSBoxRow.size() =" << mapFilTypeParameterNameSBoxRow.size()
                    << " != mapFilTypeParameterValueSBoxRow.size() =" << mapFilTypeParameterValueSBoxRow.size() << endl;
        }
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") +
                              tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                              tr("Alert.plugins.message-cause"), Wt::Ok);
        cerr << "[addFilter] : Mal rempli" << endl;
    }
}

void PluginEditionWidget::deletePlugin(int pluginId)
{
    createJSONAnchor->disable();
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseDeletePlg, this, _1, _2));
    Wt::Http::Message message;
    message.addBodyText("");

    string urlAdd = this->getApiUrl() + "/plugins/"
            + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginId])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

    if (client1->deleteRequest(urlAdd, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

}

void PluginEditionWidget::modifySource()
{
    createJSONAnchor->disable();
    string badField = "";
    if (validateSource(badField))
    {
        for (int row = 1; row < tableSrcParam->rowCount(); row++)
        {
            mapAddonParameterValueSBoxRow[row - 1] = mapLEAddonParam[mapAddonParameterNameSBoxRow[row - 1]]->text();
        }

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseModifySource, this, _1, _2));

        string messAdd = "{\n\"addon_id\" : " + boost::lexical_cast<string>(mapAddonsIdSboxRow[comboAddon->currentIndex()]) + ",\n";

        messAdd += "\"parameters\" : [\n";
        for (long unsigned idx1 = 0; idx1 < mapAddonParameterIdSboxRow.size(); idx1++)
        {
            messAdd += "\t{\"srp_id\" : " + boost::lexical_cast<string>(mapAddonParameterIdSboxRow[idx1]) + ",\n";
            //si il y a des backslash dans le nom
            //////////////////////////////////
            string strTmp = boost::lexical_cast<string>(mapAddonParameterValueSBoxRow[idx1]);
            formatValue(strTmp, 1); //
            messAdd += "\t\"value\" : \"" + strTmp + "\"},\n";
        }
        if (mapAddonParameterIdSboxRow.size() > 0)
        {
            messAdd.replace(messAdd.size() - 2, 1, "");
        }
        messAdd += "]\n}\n";

        Wt::Http::Message message;
        message.addBodyText(messAdd);

        string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "/sources/" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
                + "/parameters"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

        if (client1->post(urlAdd, message))
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
                              tr("Alert.plugins.message-cause"), Wt::Ok);
        cerr << "[addSource] : Mal remplit" << endl;
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

    string urlAdd = this->getApiUrl() + "/sources/"
            + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;

    if (client1->deleteRequest(urlAdd, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::modifySearch()
{
    string badField = "";
    if (validateSearch(badField))
    {
        for (int row = 1; row < tableSeaParam->rowCount(); row++)
        {
            mapSeaTypeParameterValueSBoxRow[row - 1] = mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[row - 1]]->text();
        }
        string tmpBool;
        if (boost::lexical_cast<int>(staticCB->isChecked()))
            tmpBool = "true";
        else
            tmpBool = "false";

        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseModifySearch, this, _1, _2));

        string messAdd = "{\n    \"sea_period\" : " + boost::lexical_cast<string>(periodeLE->text()) + ",\n\
    \"sea_is_static\" : " + tmpBool + ",\n\
    \"pos_key_value\" : " + boost::lexical_cast<string>(pKValueLE->text()) + ",\n";
        if (mapSeaTypeParameterNameSBoxRow.size() == mapSeaTypeParameterValueSBoxRow.size())
        {
            for (long unsigned idx = 0; idx < mapSeaTypeParameterNameSBoxRow.size(); idx++)
            {

                //si il y a des backslash dans le nom
                //////////////////////////////////
                string strTmp = boost::lexical_cast<string>(mapSeaTypeParameterValueSBoxRow[idx]);
                formatValue(strTmp, 1); //
                messAdd += "\"" + boost::lexical_cast<string>(mapSeaTypeParameterNameSBoxRow[idx]) + "\": \"" + strTmp + "\"\n";
            }
            messAdd += "}\n";

            Wt::Http::Message message;
            message.addBodyText(messAdd);

            string urlAdd = this->getApiUrl() + "/plugins/"
                    + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                    + "/sources/" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
                    + "/searches/" + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()])
                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                    + "&token=" + session->user()->token.toUTF8();

            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;

            if (client1->post(urlAdd, message))
            {
                Wt::WApplication::instance()->deferRendering();
            }
            else
                Wt::log("error") << "Error Client Http";

        }
        else
        {
            cerr << "[PluginEditionWidget] : mapSeaTypeParameterNameSBoxRow.size() =" << mapSeaTypeParameterNameSBoxRow.size()
                    << " != mapSeaTypeParameterValueSBoxRow.size() =" << mapSeaTypeParameterValueSBoxRow.size() << endl;
            //     cerr << "post message" << messAdd << endl;
        }
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") +
                              tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
                              tr("Alert.plugins.message-cause"), Wt::Ok);
        cerr << "[addSource] : Mal remplit" << endl;
    }
}

void PluginEditionWidget::modifyFilter()
{
    //    string badField = "";
    //    if(validateSearch(badField))
    //    {
    //        for(int row = 1; row < tableSeaParam->rowCount(); row ++)
    //        {
    //            mapSeaTypeParameterValueSBoxRow[row-1] = mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[row - 1]]->text();
    //        }
    //        string tmpBool;
    //        if(boost::lexical_cast<int>(staticCB->isChecked()))
    //            tmpBool = "true";
    //        else
    //            tmpBool = "false";
    //
    //        client1 = new Wt::Http::Client();
    //        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseModifySearch, this, _1, _2));
    //        
    //        string messAdd ="{\n    \"sea_period\" : " + boost::lexical_cast<string>(periodeLE->text()) +",\n\
//    \"sea_is_static\" : " + tmpBool + ",\n\
//    \"pos_key_value\" : " + boost::lexical_cast<string>(pKValueLE->text()) +",\n";
    //        if(mapSeaTypeParameterNameSBoxRow.size() == mapSeaTypeParameterValueSBoxRow.size())
    //        {
    //            for (long unsigned idx = 0; idx < mapSeaTypeParameterNameSBoxRow.size(); idx++)
    //            {
    //                
    //                 //si il y a des backslash dans le nom
    //                //////////////////////////////////
    //                string strTmp = boost::lexical_cast<string>(mapSeaTypeParameterValueSBoxRow[idx]);
    //                formatValue(strTmp, 1);//
    //                messAdd += "\"" + boost::lexical_cast<string>(mapSeaTypeParameterNameSBoxRow[idx]) + "\": \""+ strTmp +"\"\n";
    //            }
    //            messAdd += "}\n";
    //
    //            Wt::Http::Message message;
    //            message.addBodyText(messAdd);
    //            
    //            string urlAdd = this->getApiUrl() + "/plugins/"
    //                    + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
    //                    + "/sources/" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
    //                    + "/searches/" + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()])
    //                    + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
    //                    + "&token=" + session->user()->token.toUTF8();
    //
    //            Wt::log("debug") << "PluginEditionWidget : [POST] address to call : " << urlAdd;
    //
    //            if(client1->post(urlAdd ,message))
    //            {
    //                Wt::WApplication::instance()->deferRendering();
    //            }
    //            else
    //                Wt::log("error") << "Error Client Http";
    //
    //        }
    //        else
    //        {
    //            cerr << "[PluginEditionWidget] : mapSeaTypeParameterNameSBoxRow.size() =" << mapSeaTypeParameterNameSBoxRow.size() 
    //                      <<" != mapSeaTypeParameterValueSBoxRow.size() =" << mapSeaTypeParameterValueSBoxRow.size() <<endl;
    //       //     cerr << "post message" << messAdd << endl;
    //        }
    //    }
    //    else
    //    {
    //         Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), tr("Alert.plugins.message-field") + 
    //                                tr(badField) + tr("Alert.plugins.message-is-bad-fills") + "\n" +
    //                                tr("Alert.plugins.message-cause"),Wt::Ok);
    //        cerr << "[addSource] : Mal remplit"<< endl;
    //    }
}

void PluginEditionWidget::deleteSearch()
{
    createJSONAnchor->disable();
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseDeleteSearch, this, _1, _2));
    Wt::Http::Message message;
    message.addBodyText("");

    string urlAdd = this->getApiUrl() + "/searches/" + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;


    if (client1->deleteRequest(urlAdd, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::deleteFilter()
{
    createJSONAnchor->disable();
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseDeleteFilter, this, _1, _2));
    Wt::Http::Message message;
    message.addBodyText("");

    string urlAdd = this->getApiUrl() + "/filters/"
            + boost::lexical_cast<string>(mapFilterId[filterSelectionBox->currentIndex()])
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [DELETE] address to call : " << urlAdd;


    if (client1->deleteRequest(urlAdd, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::resetSource()
{
    for (unsigned idx = 0; idx < mapLEAddonParam.size(); idx++)
    {
        delete mapLEAddonParam[mapAddonParameterNameSBoxRow[idx]];
        //       tableSrcParam->deleteRow(idx+1);
    }

    sourceSelectionBox->clear();

    mapAddonParameterIdSboxRow.clear();
    mapAddonParameterNameSBoxRow.clear();
    mapAddonParameterValueSBoxRow.clear();
//    mapLEAddonParam.clear();
    tableSrcParam->clear();
    tableSrcParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSrcParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSrcParam);
}

void PluginEditionWidget::resetSearch()
{
    comboSearchType->clear();
    for (unsigned idx = 0; idx < mapSeaTypeParameterNameSBoxRow.size(); idx++)
    {
        delete mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[idx]];
    }
    searchSelectionBox->clear();
    mapSeaTypeParameterIdSboxRow.clear();
    mapSeaTypeParameterNameSBoxRow.clear();
    mapSeaTypeParameterValueSBoxRow.clear();
    mapLESeaTypeParam.clear();
    tableSeaParam->clear();
    tableSeaParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSeaParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSeaParam);
    periodeLE->setText("");
    staticCB->setChecked(false);
    pKValueLE->setText("0");
    nbValueLE->setText("");

}

void PluginEditionWidget::resetFilter()
{
        comboFilType->clear();
    
        for(unsigned idx = 0 ; idx < mapFilTypeParameterNameSBoxRow.size() ; idx++)
        {
           delete mapLEFilTypeParam[mapFilTypeParameterNameSBoxRow[idx]];
        }
        filterSelectionBox->clear();
        mapFilTypeParameterIdSboxRow.clear();
        mapFilTypeParameterNameSBoxRow.clear();
        mapFilTypeParameterValueSBoxRow.clear();
        mapLEFilTypeParam.clear();
        tableFilTypeParam->clear();
        tableFilTypeParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
        tableFilTypeParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
        fixStyleTable(tableFilTypeParam);
        pKValueLE->setText("0");
        nbValueLE->setText("");
}



void PluginEditionWidget::selectedSource()
{
    resetSearch();
    resetFilter();
    
    bool refreshSearches = true;

    for (unsigned i = 0; i < mapAddonsIdSboxRow.size(); i++)
    {
        cout << "mapAddonParameterIdSboxRow" << mapAddonParameterIdSboxRow[comboAddon->currentIndex()] << endl;
        cout << "mapAddonsIdSboxRow" << mapAddonsIdSboxRow[i] << endl;
        if (mapAddonParameterIdSboxRow[comboAddon->currentIndex()] == mapAddonsIdSboxRow[i])
        {
            
            if (mapAddonsIdSboxRow[i] != 0)
            {
                refreshSearches = false;
                comboAddon->setCurrentIndex(i);
                createFormSourceState = "Modify";
                createFormSourceParameters();
                break;
            }
            else
            {
                // cerr << "test";
                i = mapAddonsIdSboxRow.size()*2;
            }
        }
        else // meme commentaire 
        {
            if (mapAddonsIdSboxRow[i] == 0)
            {
                i = mapAddonsIdSboxRow.size()*2;
            }
            // cerr << "test1";
        }
    }
    

    if (refreshSearches)
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchList, this, _1, _2));

        string urlAdd = this->getApiUrl() + "/searches"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8()
                + "&plugin_id=" + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "&source_id=" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()]);

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

        if (client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
    }
    
}

void PluginEditionWidget::selectedSearch()
{
    resetFilter();

    periodeLE->setText(boost::lexical_cast<string>(mapSearchPeriode[searchSelectionBox->currentIndex()]));

    for (unsigned i = 0; i < mapSeaTypeIdSboxRow.size(); i++)
    {
        if (mapSearchSTYId[searchSelectionBox->currentIndex()] == mapSeaTypeIdSboxRow[i])
        {
            //            cerr << mapSeaTypeIdSboxRow.size() << "\n i= "<< i << "\n\n";
            //            cerr << mapSearchSTYId[searchSelectionBox->currentIndex()] << "\n i= "<< mapSeaTypeIdSboxRow[i] << "\n\n";
            //if() pour eviter boucle infini au cas ou un addon ne soit pas relié (dans TJ_STY_ADO) a un search type 
            if (mapSeaTypeIdSboxRow[i] != 0)
            {
                comboSearchType->setCurrentIndex(i);
                createFormSearchState = "Modify";
                createFormSearchParameters();
                break;
            }
            else
            {
                // cerr << "test";
                i = mapSeaTypeIdSboxRow.size()*2;
            }
        }
        else // meme commentaire 
        {
            if (mapSeaTypeIdSboxRow[i] == 0)
            {
                i = mapSeaTypeIdSboxRow.size()*2;
            }
            // cerr << "test1";
        }
    }
    
}

void PluginEditionWidget::selectedFilter()
{
    nbValueLE->setText(boost::lexical_cast<string>(mapFilterNbValue[filterSelectionBox->currentIndex()]));
    pKValueLE->setText(boost::lexical_cast<string>(mapFilterPKValue[filterSelectionBox->currentIndex()]));
    
    for (unsigned i = 0; i < mapFilTypeIdSboxRow.size(); i++)
    {
        if (mapFilterSTYId[filterSelectionBox->currentIndex()] == mapFilTypeIdSboxRow[i])
        {
            if (mapFilTypeIdSboxRow[i] != 0)
            {
                comboFilType->setCurrentIndex(i);
                createFormFilterState = "Modify";
                createFormFilterParameters();
                break;
            }
            else
            {
                i = mapFilTypeIdSboxRow.size()*2;
            }
        }
        else
        {
            if (mapFilTypeIdSboxRow[i] == 0)
            {
                i = mapFilTypeIdSboxRow.size()*2;
            }
        }
    }
}

void PluginEditionWidget::completFormSource()
{
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseAddonParameters, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/sources/"
            + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

}

void PluginEditionWidget::completFormSearch()
{
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchParameters, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/searches/"
            + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()])
            + "/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::completFormFilter()
{
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseFilterParameters, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/filters/"
            + boost::lexical_cast<string>(mapFilterId[filterSelectionBox->currentIndex()])
            + "/parameters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&plugin_id=" + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "&source_id=" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "&search_id=" + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::handleHttpResponseSearchParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        //        formatValue(response.body(), 1);//-1
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            result1 = result;
            //            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                long long seaParamID = tmp1.get("search_parameter_id");
                Wt::WString paramValue = tmp.get("value");
                for (unsigned i = 0; i < mapSeaTypeParameterIdSboxRow.size(); i++)
                {
                    if (seaParamID == mapSeaTypeParameterIdSboxRow[i])
                    {
                        //au cas ou il y ait des backslash dans le nom
                        //////////////////////////////////
                        string strTmp = boost::lexical_cast<string>(paramValue);
                        //                        formatValue(strTmp, 1);
                        mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[i]]->setValidator(validatorRegExp);
                        mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[i]]->setText(strTmp);
                    }
                }
            }

        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchParameters] " <<  e.what() << " " << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchParameters] " <<  e.what() << " " << response.body();
        }
    }
    else
    {
        Wt::log("warning") << " fct handleHttpResponseSearchParameters" << response.body();
    }
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseFilterList, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/filters"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&plugin_id=" + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "&source_id=" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()])
            + "&search_id=" + boost::lexical_cast<string>(mapSearchId[searchSelectionBox->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void PluginEditionWidget::handleHttpResponseFilterParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            result1 = result;
            //            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                long long filParamID = tmp1.get("filter_parameter_id");
                Wt::WString paramValue = tmp.get("value");
                for (unsigned i = 0; i < mapFilTypeParameterIdSboxRow.size(); i++)
                {
                    if (filParamID == mapFilTypeParameterIdSboxRow[i])
                    {
                        //au cas ou il y ait des backslash dans le nom
                        //////////////////////////////////
                        string strTmp = boost::lexical_cast<string>(paramValue);
                        //                        formatValue(strTmp, 1);
                        mapLEFilTypeParam[mapFilTypeParameterNameSBoxRow[i]]->setValidator(validatorRegExp);
                        mapLEFilTypeParam[mapFilTypeParameterNameSBoxRow[i]]->setText(strTmp);
                    }
                }
            }

        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseFilterParameters] " <<  e.what() << " " << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseFilterParameters] " <<  e.what() << " " << response.body();
        }
    }
    else
    {
        Wt::log("warning") << " fct handleHttpResponseFilterParameters" << response.body();
    }
    
    refresh1();
}



void PluginEditionWidget::handleHttpResponseSearchList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    searchSelectionBox->clear();
    mapSearchPeriode.clear();

    mapSearchSTYId.clear();
    mapSearchId.clear();
    int idx = 0;

    if (response.status() == 200)
    {
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            //            Wt::Json::parse(response.body(), result);
            result1 = result;
            //            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long seaID = tmp.get("id");

                Wt::Json::Object tmp2 = tmp.get("search_type");
                mapSearchSTYId[idx] = tmp2.get("id");

                searchSelectionBox->addItem("search " + boost::lexical_cast<string>(seaID));
                mapSearchId[idx] = seaID;
                mapSearchPeriode[idx] = tmp.get("period");

                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchList] " << e.what() << " " << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchList] " << e.what() << " " << response.body();
        }
    }
    else
    {
        Wt::log("warning") << "fct handleHttpResponseSearchList" << response.body();
    }

    fillSearchTypeComboBox();
}

void PluginEditionWidget::handleHttpResponseFilterList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    filterSelectionBox->clear();

    mapFilterSTYId.clear();
    mapFilterId.clear();
    int idx = 0;

    if (response.status() == 200)
    {
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            result1 = result;
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long filId = tmp.get("id");

                Wt::Json::Object tmp2 = tmp.get("filter_type");
                mapFilterSTYId[idx] = tmp2.get("id");

                filterSelectionBox->addItem("filter " + boost::lexical_cast<string>(filId));
                mapFilterId[idx] = filId;
                mapFilterNbValue[idx] = tmp.get("nb_value");
                mapFilterPKValue[idx] = tmp.get("pos_key_value");

                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchList] " << e.what() << " " << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseSearchList] " << e.what() << " " << response.body();
        }
    }
    else
    {
        Wt::log("warning") << "fct handleHttpResponseSearchList" << response.body();
    }

    fillFilterTypeComboBox();
}

void PluginEditionWidget::handleHttpResponseAddSearch(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        Wt::Json::Object result;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            //descriptif
            Wt::Json::Object result1 = result.get("id");
            mapSearchId[mapSearchId.size()] = result1.get("search_id");
            searchSelectionBox->addItem("search " + boost::lexical_cast<string>(mapSearchId[mapSearchId.size() - 1]));
            searchSelectionBox->setCurrentIndex(mapSearchId.size() - 1);

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

void PluginEditionWidget::handleHttpResponseAddFilter(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
        if(response.status()==200)
        {      
            Wt::Json::Object result ;
            try
            {              
                string strTmp = response.body();
                            formatValue(strTmp, 1);//-1
                Wt::Json::parse(strTmp, result);
    //            Wt::Json::parse(response.body(), result);
                //descriptif
                mapFilterId[mapFilterId.size()] = result.get("id");
                filterSelectionBox->addItem("filter " + boost::lexical_cast<string>(mapFilterId[mapFilterId.size()-1]));
                filterSelectionBox->setCurrentIndex(mapFilterId.size()-1);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[handleHttpResponseAddFilter] " << e.what() << " " << response.body();
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[handleHttpResponseAddFilter] " << e.what() << " " << response.body();
            }   
    
        }
        else
        {
             Wt::log("warning") << "fct handleHttpResponseAddFilter" << response.body();
        }
    refresh1();
}

void PluginEditionWidget::handleHttpResponseModifySource(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        client1 = new Wt::Http::Client();
        client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));

        formJSON = "string";

        string urlAdd = this->getApiUrl() + "/plugins/"
                + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
        if (client1->get(urlAdd))
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
    if (response.status() == 200)
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
        ////            sourceSelectionBox->addItem("source " + boost::lexical_cast<string>(mapSourceId[mapSourceId.size()-1]));
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
    if (response.status() == 200)
    {
//        buttonModifySource->enable();
        Wt::Json::Object result;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            //            Wt::Json::parse(response.body(), result);
            //descriptif
            Wt::Json::Object result1 = result.get("id");
            mapSourceId[mapSourceId.size()] = result1.get("source_id");
            sourceSelectionBox->addItem("source " + boost::lexical_cast<string>(mapSourceId[mapSourceId.size() - 1]));
            sourceSelectionBox->setCurrentIndex(-1); //utile lorsqu'on crée la premiére source sinon elle ne se selectionne pas automatiqement
            sourceSelectionBox->setCurrentIndex(mapSourceId[mapSourceId.size() - 1]);
            cerr << sourceSelectionBox->currentIndex() << endl;
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
//    client1 = new Wt::Http::Client();
//    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponsePlgJSON, this, _1, _2));
//
//    formJSON = "string";
//
//    string urlAdd = this->getApiUrl() + "/plugins/"
//            + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
//            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
//            + "&token=" + session->user()->token.toUTF8();
//
//    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;
//
//    if (client1->get(urlAdd))
//    {
//        Wt::WApplication::instance()->deferRendering();
//    }
//    else
//        Wt::log("error") << "Error Client Http";
}

void PluginEditionWidget::handleHttpResponseAddPlg(boost::system::error_code err, const Wt::Http::Message& response)
{
    plgNameEdit->setText("");
    plgDescEdit->setText("");
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        Wt::Json::Object result;
        try
        {
            string strTmp1 = response.body();
            formatValue(strTmp1, 1); //-1
            Wt::Json::parse(strTmp1, result);
            //descriptif

            long long pluginId = result.get("id");
            Wt::WString name = result.get("name");
            Wt::WString pluginDesc = result.get("desc");

            //si il y a des backslash dans le nom
            string strTmp = boost::lexical_cast<string>(name);
            //            formatValue(strTmp, 1);
            pluginSelectionBox->addItem(strTmp);
            mapPluginsIdSboxRow[mapPluginsIdSboxRow.size()] = pluginId;
            pluginSelectionBox->setCurrentIndex(-1); //utile lorsqu'on crée le premier plugin sinon elle ne se selectionne pas automatiqement
            pluginSelectionBox->setCurrentIndex(mapPluginsIdSboxRow.size());
            mapPluginsDescrition[mapPluginsIdSboxRow.size() - 1] = pluginDesc;
            Wt::WPushButton *deletePlugin = new Wt::WPushButton("<span class='icon'><i class='icon-minus icon-white'></i></span>&nbsp;" + tr("Alert.plugins.delete-plugin-button"));
            deletePlugin->addStyleClass("btn");
            deletePlugin->addStyleClass("btn-danger");
            deletePlugin->setTextFormat(Wt::XHTMLUnsafeText);
            int row = tablePlugin->rowCount() - 1;
            deletePlugin->clicked().connect(boost::bind(&PluginEditionWidget::deletePlugin, this, row - 1));
            tablePlugin->insertRow(row);
            tablePlugin->elementAt(row, 0)->addWidget(new Wt::WText(strTmp));
            tablePlugin->elementAt(row, 1)->addWidget(new Wt::WText(pluginDesc));
            tablePlugin->elementAt(row, 2)->addWidget(deletePlugin);

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
    for (unsigned idx1 = 0; idx1 < mapSeaTypeParameterNameSBoxRow.size(); idx1++)
    {
        delete mapLESeaTypeParam[mapSeaTypeParameterNameSBoxRow[idx1]];
    }
    mapSeaTypeParameterIdSboxRow.clear();
    mapSeaTypeParameterNameSBoxRow.clear();
    mapSeaTypeParameterValueSBoxRow.clear();
    mapLESeaTypeParam.clear();
    tableSeaParam->clear();
    tableSeaParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableSeaParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableSeaParam);

    if (response.status() == 200)
    {
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            //            Wt::Json::parse(response.body(), result);
            result1 = result;
            //            descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                long long parameterId = tmp.get("id");
                Wt::WString name = tmp.get("name");
                mapSeaTypeParameterIdSboxRow[idx] = parameterId;
                mapSeaTypeParameterNameSBoxRow[idx] = name;
                mapLESeaTypeParam[name] = new Wt::WLineEdit();
                // mapLESeaTypeParam[name]->setStyleClass("input-xxlarge");
                mapLESeaTypeParam[name]->setValidator(validatorRegExp);
                tableSeaParam->elementAt(idx + 1, 0)->addWidget(new Wt::WText(name));
                tableSeaParam->elementAt(idx + 1, 1)->addWidget(mapLESeaTypeParam[name]);

                idx++;
            }
            if (createFormSearchState == "Modify")
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
    else if (response.status() == 404 && createFormSearchState == "Modify")
    {
        createFormSearchState = "";
    }
    else
    {
        Wt::log("warning") << "fct handleHttpResponseSeaTypeParameters" << response.body();
        createFormSearchState = "";
    }
    
   
    refresh1();
}

void PluginEditionWidget::handleHttpResponseFilTypeParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    int idx = 0;
    for(unsigned idx1 = 0 ; idx1 < mapFilTypeParameterNameSBoxRow.size() ; idx1++)
    {
       delete mapLEFilTypeParam[mapFilTypeParameterNameSBoxRow[idx1]];
    }
    mapFilTypeParameterIdSboxRow.clear();
    mapFilTypeParameterNameSBoxRow.clear();
    mapFilTypeParameterValueSBoxRow.clear();
    mapLEFilTypeParam.clear();
    tableFilTypeParam->clear();
    tableFilTypeParam->elementAt(0, 0)->addWidget(new Wt::WText(tr("Alert.plugins.parameter-title")));
    tableFilTypeParam->elementAt(0, 1)->addWidget(new Wt::WText(tr("Alert.plugins.value-title")));
    fixStyleTable(tableFilTypeParam);

    if(response.status() == 200)
    {
        Wt::Json::Value result ;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {                  
            string strTmp = response.body();
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
                mapFilTypeParameterIdSboxRow[idx] = parameterId;
                mapFilTypeParameterNameSBoxRow[idx] = name;
                mapLEFilTypeParam[name] = new Wt::WLineEdit();
               // mapLEFilTypeParam[name]->setStyleClass("input-xxlarge");
                mapLEFilTypeParam[name]->setValidator(validatorRegExp);
                tableFilTypeParam->elementAt(idx + 1, 0)->addWidget(new Wt::WText(name));
                tableFilTypeParam->elementAt(idx + 1, 1)->addWidget(mapLEFilTypeParam[name]);

                idx++;
            }
            if(createFormFilterState == "Modify")
            {
                completFormFilter();
                createFormFilterState = "";
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseFilTypeParameters] Problems parsing JSON:" << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseFilTypeParameters] Problems parsing JSON.:" << response.body();
        }   
    }
    else if(response.status() == 404 && createFormFilterState == "Modify")
    {
        createFormFilterState = "";
    }
    else
    {
        Wt::log("warning") << "fct handleHttpResponseFil" << response.body();
        createFormFilterState = "";
    }
    refresh1();
}

void PluginEditionWidget::handleHttpResponseAddonParameters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        //        formatValue(response.body(), 1);//-1
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            result1 = result;
            //            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
            {
                Wt::Json::Object tmp = (*idx1);
                Wt::Json::Object tmp1 = tmp.get("id");
                long long srcParamID = tmp1.get("source_parameter_id");
                Wt::WString paramValue = tmp.get("value");
                for (unsigned i = 0; i < mapAddonParameterIdSboxRow.size(); i++)
                {
                    if (srcParamID == mapAddonParameterIdSboxRow[i])
                    {
                        //au cas ou il y ait des backslash dans le nom
                        //////////////////////////////////
                        string strTmp = boost::lexical_cast<string>(paramValue);
                        //                        formatValue(strTmp, 1);
                        mapLEAddonParam[mapAddonParameterNameSBoxRow[i]]->setValidator(validatorRegExp);
                        mapLEAddonParam[mapAddonParameterNameSBoxRow[i]]->setText(strTmp);
                    }
                }
            }

        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddonParameters] " <<  e.what() << " " << response.body();
        }
        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponseAddonParameters] " <<  e.what() << " " << response.body();
        }
    }
    else
    {
        Wt::log("warning") << " fct handleHttpResponseAddonParameters" << response.body();
    }
    
    client1 = new Wt::Http::Client();
    client1->done().connect(boost::bind(&PluginEditionWidget::handleHttpResponseSearchList, this, _1, _2));

    string urlAdd = this->getApiUrl() + "/searches"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8()
            + "&plugin_id=" + boost::lexical_cast<string>(mapPluginsIdSboxRow[pluginSelectionBox->currentIndex()])
            + "&source_id=" + boost::lexical_cast<string>(mapSourceId[sourceSelectionBox->currentIndex()]);

    Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

    if (client1->get(urlAdd))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
    
}

void PluginEditionWidget::handleHttpResponseAddonList(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();

    delete client1;
    int idx = 0;

    for (unsigned idx = 0; idx < mapLEAddonParam.size(); idx++)
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
    if (response.status() == 200)
    {
        Wt::Json::Value result;
        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
        try
        {
            string strTmp = response.body();
            formatValue(strTmp, 1); //-1
            Wt::Json::parse(strTmp, result);
            //            Wt::Json::parse(response.body(), result);
            result1 = result;
            //            //descriptif
            for (Wt::Json::Array::const_iterator idx1 = result1.begin(); idx1 < result1.end(); idx1++)
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

            if (createFormSourceState == "Modify")
            {
                completFormSource();
                createFormSourceState = "";
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

    refresh1();
}


void PluginEditionWidget::fillAddonComboBox()
{
    comboAddon->clear();

    int idx = 0;
    
    map<int, Wt::WString> mapEAddonName;
    mapEAddonName[Echoes::Dbo::EAddon::FILESYSTEM] = tr("Alert.plugins.filesystem");
    mapEAddonName[Echoes::Dbo::EAddon::FILE] = tr("Alert.plugins.file");
    mapEAddonName[Echoes::Dbo::EAddon::LOG] = tr("Alert.plugins.log");
    mapEAddonName[Echoes::Dbo::EAddon::ODBC] = tr("Alert.plugins.odbc");
    mapEAddonName[Echoes::Dbo::EAddon::SNMP] = tr("Alert.plugins.snmp");

    for (int idx1 = Echoes::Dbo::EAddon::FILESYSTEM; idx1 <= Echoes::Dbo::EAddon::SNMP; idx1++)
    {
        slmAddons->insertString(idx, mapEAddonName[idx1]);
//        this->mapAddonParameterIdSboxRow[idx] = idx1;
        this->mapAddonsIdSboxRow[idx] = idx1;
        idx++;
    }

    comboAddon->setModel(slmAddons);
    comboAddon->setCurrentIndex(0);
    createFormSourceParameters();
}


void PluginEditionWidget::fillSearchTypeComboBox()
{
    comboSearchType->clear();

    int idx = 0;

    map<int, Wt::WString> mapESearchTypeName;
    mapESearchTypeName[Echoes::Dbo::ESearchType::OID] = tr("Alert.plugins.oid");
    mapESearchTypeName[Echoes::Dbo::ESearchType::PATH_LINE] = tr("Alert.plugins.path_line");
    mapESearchTypeName[Echoes::Dbo::ESearchType::PATH] = tr("Alert.plugins.path");
    mapESearchTypeName[Echoes::Dbo::ESearchType::PATH_LAST_LINE] = tr("Alert.plugins.path_last_line");
    mapESearchTypeName[Echoes::Dbo::ESearchType::QUERY] = tr("Alert.plugins.query");

    for (int idx1 = Echoes::Dbo::ESearchType::OID; idx1 <= Echoes::Dbo::ESearchType::QUERY; idx1++)
    {
        slmSearchTypes->insertString(idx, mapESearchTypeName[idx1]);
        this->mapSeaTypeIdSboxRow[idx] = idx1;
    }

    comboSearchType->setModel(slmSearchTypes);
    comboSearchType->setCurrentIndex(0);
    createFormSearchParameters();
}

void PluginEditionWidget::fillFilterTypeComboBox()
{
    comboFilType->clear();

    int idx = 0;
    
    map<int, Wt::WString> mapEFilterTypeName;
    mapEFilterTypeName[Echoes::Dbo::EFilterType::ALL] = tr("Alert.plugins.all");
    mapEFilterTypeName[Echoes::Dbo::EFilterType::REGEX] = tr("Alert.plugins.regex");
    mapEFilterTypeName[Echoes::Dbo::EFilterType::LOCATION] = tr("Alert.plugins.location");
   
    for (int idx1 = Echoes::Dbo::EFilterType::ALL; idx1 <= Echoes::Dbo::EFilterType::LOCATION; idx1++)
    {
        slmFilType->insertString(idx, mapEFilterTypeName[idx1]);
        this->mapFilTypeIdSboxRow[idx] = idx1;
    }

    comboFilType->setModel(slmFilType);
    comboFilType->setCurrentIndex(0);
    createFormFilterParameters();
}

void PluginEditionWidget::handleHttpResponsePlgJSON(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 200)
    {
        if (formJSON == "string")
        {
            strJSON = response.body();
            formatValue(strJSON, 1); //-1
            formJSON = "";
            createFormSourceParameters();
        }
        else
        {

            char *tmpname = strdup("/tmp/echoes-tmp-plugin-fileXXXXXX");
            int mkstempRes = mkstemp(tmpname);
            Wt::log("debug") << "[AssetManagementWidget] " << "Res temp file creation : " << mkstempRes;

            ofstream fileJSON(tmpname);
            //fileJSON.open( boost::lexical_cast<string>(pluginSelectionBox->currentText()) + ".json", ios::out);
            fileJSON << "/*\n\
 * " + pluginSelectionBox->currentText().toUTF8() + "\n\
 * \n\
 * Static file for ECHOES Alert Probe generated by ECHOES Alert API.\n\
 * DO NOT EDIT THIS FILE BY HAND WITHOUT SUPPORT AUTHORIZATION.\n\
 * \n\
 */\n\n";
            fileJSON << response.body();
            fileJSON.close();




            string pluginNameSpacesReplaced = pluginSelectionBox->currentText().toUTF8();
            boost::replace_all(pluginNameSpacesReplaced, " ", "_");

            // creating resource to send to the client
            Wt::WFileResource *res = new Wt::WFileResource();
            res->setFileName(tmpname);

            res->suggestFileName("ea-plugin_" + pluginNameSpacesReplaced + ".json", Wt::WResource::Attachment);
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

        string urlAdd = this->getApiUrl() + "/plugins"
                + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
                + "&token=" + session->user()->token.toUTF8();

        Wt::log("debug") << "PluginEditionWidget : [GET] address to call : " << urlAdd;

        if (client1->get(urlAdd))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }

    }
    else if (response.status() == 409)
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
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
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
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
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
        Wt::log("warning") << "[ERROR] : fct handleHttpResponseDeleteSearch" << response.body();
    }
}

void PluginEditionWidget::handleHttpResponseDeleteFilter(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    delete client1;
    if (response.status() == 204)
    {
        selectedSearch();
    }
    else if (response.status() == 409)
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
    }
    else
    {
        Wt::WMessageBox::show(tr("Alert.plugins.message-box-error"), response.body(), Wt::Ok);
        Wt::log("warning") << "[ERROR] : fct handleHttpResponseDeleteFilter" << response.body();
    }
}

void PluginEditionWidget::searchToken()
{
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));

        //on prend le token de la ligne avec le plus grand id (le token le plus récemment créé)
        string queryStr = "SELECT \"USR_TOKEN\" FROM \"T_USER_USR\" "
                " WHERE \"USR_MAIL\"  = '" + boost::lexical_cast<string>(this->session->user().get()->eMail.toUTF8()) +
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

void PluginEditionWidget::formatValue(string &value, int nbBackslash) const
{
    int compt = 0;
    while (compt != -1)
    {
        if (value.find("\\", compt) < value.size() && value.find("\\", compt) >= 0)
        {
            for (int i = 0; i < nbBackslash; i++)
            {
                value = value.replace(value.find("\\", compt), 1, "\\\\");
            }
            compt = value.find("\\", compt); //compt = l'indexe ou se trouve le premier caractére "\" dans la string.
            // recherche dans la string à partir de l'indexe compt;
            compt = compt + nbBackslash * 2; //value.find_first_not_of("\\", compt+1); 
        }
        else
        {
            compt = -1;
        }
    }
}

void PluginEditionWidget::setApiUrl(string apiUrl)
{
    _apiUrl = apiUrl;
    return;
}

string PluginEditionWidget::getApiUrl() const
{
    return _apiUrl;
}

