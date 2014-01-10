/* 
 * File:   RoleCustomizationWidget.cpp
 * Author: tsa
 * 
 * Created on 21 avril 2013, 11:41
 */

#include "RoleCustomizationWidget.h"

RoleCustomizationWidget::RoleCustomizationWidget(Echoes::Dbo::Session *session, const std::string &apiUrl) 
: Wt::WContainerWidget()
{
    //init
    this->session = session;
    mainTemplate = new Wt::WTemplate(Wt::WString::tr("Alert.role.customization.template"));
    mainTemplate->addFunction("tr", &Wt::WTemplate::Functions::tr);
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("role-customization",false);
    this->addWidget(mainTemplate);
    
    
    mediasComboBox = new Wt::WComboBox(this);
    //selected?
    mediasComboBox->clicked().connect(this, &RoleCustomizationWidget::selectMedia);
    
    rolesComboBox = new Wt::WComboBox(this);
    //selected?
    rolesComboBox->clicked().connect(this, &RoleCustomizationWidget::selectRole);
    
    pluginsComboBox = new Wt::WComboBox(this);
    //selected?
//    pluginsComboBox->clicked().connect(this, &RoleCustomizationWidget::selectPlugin);
//    Wt::WContainerWidget *pluginLoadButtonContainer = new Wt::WContainerWidget(this);
 
    assetsContainer = new Wt::WContainerWidget(this);
    assetsContainer->setStyleClass("container-fluid");
    pluginsContainer = new Wt::WContainerWidget(this);
    pluginsContainer->setStyleClass("container-fluid no-padding");
    informationsContainer = new Wt::WContainerWidget(this);
    informationsContainer->setStyleClass("container-fluid no-padding");
        
    
    pluginRow = new Wt::WContainerWidget(this);
    pluginRow->setStyleClass("row-fluid control-group");
    
    Wt::WContainerWidget *pluginNameContainer = new Wt::WContainerWidget();
    pluginName = new Wt::WText(this);
    pluginName->setText(tr("Alert.admin.plugin-select"));
    pluginNameContainer->setStyleClass("span2");
    pluginNameContainer->addWidget(pluginName);
    
    Wt::WContainerWidget *pluginEditLineContainer = new Wt::WContainerWidget();
    pluginEditLine = new Wt::WLineEdit(this);
    pluginEditLineContainer->setStyleClass("span3 control-group");
    pluginEditLineContainer->addWidget(pluginEditLine);
    
    Wt::WContainerWidget *pluginSaveButtonContainer = new Wt::WContainerWidget();
    pluginSaveButton = new Wt::WPushButton(this);
    pluginSaveButton->setTextFormat(Wt::XHTMLUnsafeText);
    pluginSaveButton->setText("<i class='icon-white icon-ok-sign'></i>");
    pluginSaveButton->setStyleClass("span1");
    pluginSaveButton->addStyleClass("btn-primary");
    pluginSaveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putPluginAlias, this));
    pluginSaveButtonContainer->addWidget(pluginSaveButton);
    
    
    pluginLoadButton = new Wt::WPushButton(this);
    pluginLoadButton->setTextFormat(Wt::XHTMLUnsafeText);
    pluginLoadButton->setText("</ br><i class='icon-white icon-time'></i> " + tr("Alert.role.load"));
    pluginLoadButton->setStyleClass("btn-success");
    pluginLoadButton->clicked().connect(boost::bind(&RoleCustomizationWidget::selectPlugin, this));
//    pluginLoadButtonContainer->addWidget(pluginLoadButton);
//    pluginLoadButtonContainer->setStyleClass("row-fluid");
    
    pluginRow->addWidget(pluginNameContainer);
    pluginRow->addWidget(pluginEditLineContainer);
    pluginRow->addWidget(pluginSaveButtonContainer);
    
    pluginsContainer->addWidget(pluginRow);
    

    this->setApiUrl(apiUrl);
    this->setCredentials("?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8()) +
            "&token=" + session->user()->token.toUTF8());

    mediasSet = false;
    rolesSet = false;
    
    bindWidgets();
    
    fillMediaSelector();
    fillRoleSelector();
    
    

    fillPluginSelector();
    

    
//    createAssetsWidgets();
}

RoleCustomizationWidget::RoleCustomizationWidget(const RoleCustomizationWidget& orig)
{
}

RoleCustomizationWidget::~RoleCustomizationWidget()
{
}

void RoleCustomizationWidget::bindWidgets()
{
    mainTemplate->bindWidget("medias-combo-box",mediasComboBox);
    mainTemplate->bindWidget("roles-combo-box",rolesComboBox);
    mainTemplate->bindWidget("assets-container",assetsContainer);
    
    mainTemplate->bindWidget("plugins-load-button",pluginLoadButton);
    mainTemplate->bindWidget("plugins-combo-box",pluginsComboBox);
    mainTemplate->bindWidget("plugins-container",pluginsContainer);
    mainTemplate->bindWidget("informations-container",informationsContainer);
}

void RoleCustomizationWidget::setApiUrl(std::string apiUrl)
{
    this->apiUrl = apiUrl;
}

std::string RoleCustomizationWidget::getApiUrl() const
{
    return apiUrl;
}

void RoleCustomizationWidget::getRoles(boost::system::error_code err, const Wt::Http::Message& response) 
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    //ToDo: clear all I have to clean
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::Json::Value result ;
            Wt::Json::Array& result1 = Wt::Json::Array::Empty;
            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;
                  //descriptif
                for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
                {
                    Wt::Json::Object tmp = (*idx1);
                    rolesComboBox->insertItem(idx,tmp.get("name"));
                    mapIdRolesComboBox[idx] = tmp.get("id");
                    idx++;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }
            rolesComboBox->setCurrentIndex(0);
            rolesSet = true;
            if (mediasSet)
            {
                createAssetsWidgets();
            }
        }
    }
    else
    {
//         Wt::log("warning") << "fct handleHttpResponse" << response.body();
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
    }
}

void RoleCustomizationWidget::setCredentials(std::string credentials)
{
    this->credentials = credentials;
}

std::string RoleCustomizationWidget::getCredentials() const
{
    return credentials;
}

void RoleCustomizationWidget::selectPlugin()
{
    mapEditInformations.clear();
    mapIdInformations.clear();
    pluginEditLine->setText("");
    if (pluginsComboBox->currentIndex() != -1)
    {
        pluginName->setText(pluginsComboBox->currentText());
        long long pluginId = mapIdPluginsComboBox[pluginsComboBox->currentIndex()];
        
        std::string urlToCall = this->getApiUrl() 
                + "/plugins/" + boost::lexical_cast<std::string>(pluginId)
                + "/informations" + this->getCredentials();
        
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getInformations, this, _1, _2));

        Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;
        if (client->get(urlToCall))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
        
        
        urlToCall = this->getApiUrl() 
                + "/plugins/" + boost::lexical_cast<std::string>(pluginId)
                + "/alias" + this->getCredentials()
                + "&user_role_id=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                + "&media_type_id=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);

        Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;

        Wt::Http::Client *client2 = new Wt::Http::Client(this);
        client2->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, pluginEditLine));
        if (client2->get(urlToCall))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
            Wt::log("error") << "Error Client Http";
    }
    
}

void RoleCustomizationWidget::selectMedia()
{
    createAssetsWidgets();
//    selectPlugin();
}

void RoleCustomizationWidget::selectRole()
{
    createAssetsWidgets();
//    selectPlugin();
}

void RoleCustomizationWidget::fillMediaSelector()
{
    int idx = 0;
    mediasComboBox->addItem("email");
    mapIdMediasComboBox[idx] = Enums::EMedia::email;
    idx++;
    mediasComboBox->addItem("sms");
    mapIdMediasComboBox[idx] = Enums::EMedia::sms;
    idx++;
    mediasComboBox->addItem("mobile");
    mapIdMediasComboBox[idx] = Enums::EMedia::mobileapp;

    mediasComboBox->setCurrentIndex(0);
    mediasSet = true;
    if (rolesSet)
    {
        createAssetsWidgets();
    }

}

void RoleCustomizationWidget::fillRoleSelector()
{

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getRoles, this, _1, _2));

    std::string urlToCall = this->getApiUrl()
            + "/roles" + this->getCredentials();
    
    Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;
    
    if (client->get(urlToCall))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void RoleCustomizationWidget::fillPluginSelector()
{
    std::string urlToCall = this->getApiUrl()
            + "/plugins" + this->getCredentials();
    Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getPlugins, this, _1, _2));
    if (client->get(urlToCall))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
    else
        Wt::log("error") << "Error Client Http";
}

void RoleCustomizationWidget::createAssetsWidgets()
{
    std::string urlToCall = this->getApiUrl()
            + "/assets" + this->getCredentials();
    Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getAssets, this, _1, _2));
    if (client->get(urlToCall))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void RoleCustomizationWidget::createCriteriaWidgets()
{
    mapEditInformationCriteria.clear();
//    unsigned int idx = 0;
    for(std::map<int,long long>::const_iterator i = mapIdInformations.begin() ; i != mapIdInformations.end() ; i ++)
    {
        std::string urlToCall = this->getApiUrl() 
                + "/informations/" + boost::lexical_cast<std::string>((long long)i->second)
                + "/criteria";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getCriteria, this, _1, _2, i->first, mapRowCriteriaInformations[i->first]));

        std::string apiAddress = urlToCall + this->getCredentials();

        Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << apiAddress;

        if (client->get(apiAddress))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
    }
    
}

void RoleCustomizationWidget::getAssets(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    assetsContainer->clear();
    mapIdAssets.clear();
    mapEditAssets.clear();
    
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::Json::Value result ;
            Wt::Json::Array& result1 = Wt::Json::Array::Empty;
            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;
                  //descriptif
                for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
                {
                    Wt::WContainerWidget *row = new Wt::WContainerWidget();
                    row->setStyleClass("row-fluid");
                    Wt::Json::Object tmp = (*idx1);

                    Wt::WContainerWidget *labelAssetContainer = new Wt::WContainerWidget();
                    labelAssetContainer->setStyleClass("span2");
                    Wt::WText *labelAsset = new Wt::WText();
                    labelAsset->setText(tmp.get("name"));
                    labelAssetContainer->addWidget(labelAsset);

                    Wt::WContainerWidget *lineEditAssetContainer = new Wt::WContainerWidget();
                    lineEditAssetContainer->setStyleClass("span3 control-group");
                    Wt::WLineEdit *lineEditAsset = new Wt::WLineEdit();
                    lineEditAssetContainer->addWidget(lineEditAsset);
                    mapIdAssets[idx] = tmp.get("id");
                    mapEditAssets[idx] = lineEditAsset;

                    Wt::WContainerWidget *saveButtonContainer = new Wt::WContainerWidget();
                    Wt::WPushButton *saveButton = new Wt::WPushButton();
                    saveButton->setTextFormat(Wt::XHTMLUnsafeText);
                    saveButton->setText("<i class='icon-white icon-ok-sign'></i>");
                    saveButton->setStyleClass("span1");
                    saveButton->addStyleClass("btn-primary");
                    saveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putAssetAlias, this, idx));
                    saveButtonContainer->addWidget(saveButton);


                    row->addWidget(labelAssetContainer);
                    row->addWidget(lineEditAssetContainer);
                    row->addWidget(saveButtonContainer);
                    assetsContainer->addWidget(row);
                    idx++;

                    fillAssetsFields();
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }        
        }
    }
    else
    {
//         Wt::log("warning") << "fct handleHttpResponse" << response.body();
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
    }
}

//void RoleCustomizationWidget::getMedias(boost::system::error_code err, const Wt::Http::Message& response)
//{
//    Wt::WApplication::instance()->resumeRendering();
//    int idx = 0;
//    mediasComboBox->clear();
//
//    if (!err)
//    {
//        if (response.status() >= 200 && response.status() < 300)
//        {
//            Wt::Json::Value result ;
//            Wt::Json::Array& result1 = Wt::Json::Array::Empty;
//            try
//            {                  
//                Wt::Json::parse(response.body(), result);
//                result1 = result;
//                  //descriptif
//                for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
//                {
//                    Wt::Json::Object tmp = (*idx1);
//                    mediasComboBox->addItem(tmp.get("name"));
//                    mapIdMediasComboBox[idx] = tmp.get("id");
//                    idx++;
//                }
//            }
//            }
//            catch (Wt::Json::ParseError const& e)
//            {
//                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
//                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
//            }
//            catch (Wt::Json::TypeException const& e)
//            {
//                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
//                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
//            }
//            mediasComboBox->setCurrentIndex(0);
//            mediasSet = true;
//            if (rolesSet)
//            {
//                createAssetsWidgets();
//            }
//        }
//    }
//    else
//    {
//        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
//        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
////         Wt::log("warning") << "fct handleHttpResponse " << "response status: " << response.status() << " Body : "<< response.body();
////         Wt::log("warning") << "Boost error code: " << err.message();
//    }
//}

void RoleCustomizationWidget::getPlugins(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    pluginsComboBox->clear();
    
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::Json::Value result ;
            Wt::Json::Array& result1 = Wt::Json::Array::Empty;
            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;
                  //descriptif
                for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
                {
                    Wt::Json::Object tmp = (*idx1);
                    pluginsComboBox->addItem(tmp.get("name"));
                    mapIdPluginsComboBox[idx] = tmp.get("id");
                    idx++;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }          
        }
        else
        {
            Wt::log("debug") << "[Role Customization Widget] no plugin";
        }
    }
    else
    {
        Wt::log("error") << "[Role Customization Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
//         Wt::log("warning") << "fct handleHttpResponse" << response.body();
    }
}

void RoleCustomizationWidget::getCriteria(boost::system::error_code err, const Wt::Http::Message& response, int idForInfMap, Wt::WContainerWidget *row)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    
    
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::Json::Value result ;
            Wt::Json::Array& result1 = Wt::Json::Array::Empty;

            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;

                std::map<long long, Wt::WLineEdit*> mapIdCritEdit;

                for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
                {

                    Wt::Json::Object tmp = (*idx1);

                    Wt::WContainerWidget *labelContainer = new Wt::WContainerWidget();
                    labelContainer->setStyleClass("span2");
                    Wt::WText *label = new Wt::WText();
                    label->setText(Wt::WString::tr("Alert.alert.operator." + (std::string)tmp.get("name")));
                    labelContainer->addWidget(label);


                    Wt::WContainerWidget *lineEditContainer = new Wt::WContainerWidget();
                    lineEditContainer->setStyleClass("span2 control-group");
                    Wt::WLineEdit *lineEdit = new Wt::WLineEdit();
                    long long critId = tmp.get("id");
                    lineEditContainer->addWidget(lineEdit);
                    mapIdCritEdit[critId] = lineEdit;

                    Wt::WContainerWidget *saveButtonContainer = new Wt::WContainerWidget();
                    saveButtonContainer->setStyleClass("span1");
                    Wt::WPushButton *saveButton = new Wt::WPushButton();
                    saveButton->setTextFormat(Wt::XHTMLUnsafeText);
                    saveButton->setText("<i class='icon-white icon-ok-sign'></i>");
                    saveButton->setStyleClass("span12");
                    saveButton->addStyleClass("btn-primary");
                    saveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putCriterionAlias, this, idForInfMap, critId, lineEdit));
                    saveButtonContainer->addWidget(saveButton);

                    Wt::WContainerWidget *exampleButtonContainer = new Wt::WContainerWidget();
                    exampleButtonContainer->setStyleClass("span1");
                    Wt::WPushButton *exampleButton = new Wt::WPushButton();
                    exampleButton->setTextFormat(Wt::XHTMLUnsafeText);
                    exampleButton->setText("<i class='icon-white icon-envelope'></i>");
                    exampleButton->setStyleClass("span12");
                    exampleButton->addStyleClass("btn-info");
                    exampleButton->clicked().connect(boost::bind(&RoleCustomizationWidget::displayMessageExample, this, idForInfMap, critId, lineEdit));
                    exampleButtonContainer->addWidget(exampleButton);

                    row->addWidget(labelContainer);
                    row->addWidget(lineEditContainer);
                    row->addWidget(saveButtonContainer);
                    row->addWidget(exampleButtonContainer);

                    if ((idx > 0) && (idx%2 == 1))
                    {
                        Wt::WText *changeRow = new Wt::WText();
                        changeRow->setTextFormat(Wt::XHTMLUnsafeText);
                        changeRow->setText("</div><div class='row-fluid'>");
                        row->addWidget(changeRow);
                    }

                    idx++;
                }

    //            mapEditInformationCriteria[idForInfMap] = mapIdCritEdit;
                fillCriteriaFields(idForInfMap, mapIdCritEdit);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
    }
}

void RoleCustomizationWidget::getInformations(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    informationsContainer->clear();
    
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::Json::Value result ;
            Wt::Json::Array& result1 = Wt::Json::Array::Empty;
            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;
                  //descriptif

                for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
                {
                    Wt::Json::Object tmp = (*idx1);

                    // do not show "not to display" inf
                    bool display = tmp.get("display");
                    if (!display)
                    {
                        continue;
                    }

                    Wt::WText *infoBoxOpen = new Wt::WText();
                    infoBoxOpen->setTextFormat(Wt::XHTMLUnsafeText);
                    infoBoxOpen->setText(
                    "</div>"
                    "<div class='widget-box'>"
                    "   <div class='widget-title'>"
                    "       <span class='icon'><i class='icon-th-list'></i></span>"
                    "       <h5>Information : " + (std::string)tmp.get("name") + "</h5>"
                    "   </div>"
                    "   <div class='widget-content'>"
                    "       <div class='container-fluid no-padding'>");

                    Wt::WText *infoBoxClose = new Wt::WText();
                    infoBoxClose->setTextFormat(Wt::XHTMLUnsafeText);
                    infoBoxClose->setText(
                    "       </div>"
                    "   </div>"
                    "</div>"
                    "<div>");
                    informationsContainer->addWidget(infoBoxOpen);

                    Wt::WContainerWidget *row = new Wt::WContainerWidget();
                    row->setStyleClass("row-fluid");

                    Wt::WContainerWidget *labelContainer = new Wt::WContainerWidget();
                    Wt::WText *label = new Wt::WText();
                    label->setText(tmp.get("name"));
                    labelContainer->setStyleClass("span2");
                    labelContainer->addWidget(label);

                    Wt::WContainerWidget *lineEditContainer = new Wt::WContainerWidget();
                    Wt::WLineEdit *lineEdit = new Wt::WLineEdit();
                    lineEditContainer->setStyleClass("span2 control-group");
                    lineEditContainer->addWidget(lineEdit);

                    mapEditInformations[idx] = lineEdit;
                    mapIdInformations[idx] = tmp.get("id");

                    Wt::WContainerWidget *saveButtonContainer = new Wt::WContainerWidget();
                    saveButtonContainer->setStyleClass("span1");
                    Wt::WPushButton *saveButton = new Wt::WPushButton();
                    saveButton->setTextFormat(Wt::XHTMLUnsafeText);
                    saveButton->setText("<i class='icon-white icon-ok-sign'></i>");
                    saveButton->setStyleClass("span12");
                    saveButton->addStyleClass("btn-primary");
                    saveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putInformationAlias, this, idx));
                    saveButtonContainer->addWidget(saveButton);

                    row->addWidget(labelContainer);
                    row->addWidget(lineEditContainer);
                    row->addWidget(saveButtonContainer);
                    informationsContainer->addWidget(row);

                    Wt::WContainerWidget *rowCriteria = new Wt::WContainerWidget();
                    rowCriteria->setStyleClass("row-fluid");
                    mapRowCriteriaInformations[idx] = rowCriteria;
                    informationsContainer->addWidget(rowCriteria);

                    idx++;
                    informationsContainer->addWidget(infoBoxClose);
                }
                fillInformationsFields();
                createCriteriaWidgets();

            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }         
        }
    }
    else
    {
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
//         Wt::log("warning") << "fct handleHttpResponse" << response.body();
    }
}

void RoleCustomizationWidget::putAssetAlias(int idx)
{    
    
    std::string strMessage = "{\n"
            "\"user_role_id\" : "+ boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +",\n"
            "\"media_type_id\" : "+ boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +",\n"
            "\"value\" : \""+ boost::lexical_cast<std::string>(mapEditAssets[idx]->text()) +"\"\n}";
    
    std::cout << "Message : " << strMessage << std::endl;
    
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    std::string urlToCall = this->getApiUrl()
            + "/assets/" + boost::lexical_cast<std::string>(mapIdAssets[idx])
            + "/alias"  + this->getCredentials();
    Wt::log("debug") << "RoleCustomizationWidget : [PUT] address to call : " << urlToCall;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutAssetAlias, this, _1, _2, mapEditAssets[idx]));
    if (client->put(urlToCall, message))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void RoleCustomizationWidget::putPluginAlias()
{
    std::string strMessage = "{\n"
            "\"user_role_id\" : " + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +",\n"
            "\"media_type_id\" : " + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +",\n"
            "\"value\" : \"" + boost::lexical_cast<std::string>(pluginEditLine->text()) + "\"\n}";
    
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    std::string urlToCall = this->getApiUrl()
            + "/plugins/" + boost::lexical_cast<std::string>(mapIdPluginsComboBox[pluginsComboBox->currentIndex()])
            + "/alias" + this->getCredentials();
    Wt::log("debug") << "RoleCustomizationWidget : [PUT] address to call : " << urlToCall;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutPluginAlias, this, _1, _2, pluginEditLine));
    if (client->put(urlToCall, message))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}


void RoleCustomizationWidget::putInformationAlias(int idx)
{
    std::string strMessage = "{\n"
            "\"user_role_id\" : " + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +",\n"
            "\"media_type_id\" : " + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +",\n"
            "\"value\" : \"" + boost::lexical_cast<std::string>(mapEditInformations[idx]->text()) +"\"\n}\n";
      
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    std::string urlToCall = this->getApiUrl()
            + "/informations/" + boost::lexical_cast<std::string>(mapIdInformations[idx])
            + "/alias" + this->getCredentials();
    Wt::log("debug") << "RoleCustomizationWidget : [PUT] address to call : " << urlToCall;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutAssetAlias, this, _1, _2, mapEditInformations[idx]));
    if (client->put(urlToCall, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void RoleCustomizationWidget::putCriterionAlias(int idForInfMap, long long idCrit, Wt::WLineEdit *critEdit)
{
    std::string strMessage = "{\n"
            "\"user_role_id\" : " + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +",\n"
            "\"media_type_id\" : " + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +",\n"
            "\"value\" : \"" + boost::lexical_cast<std::string>(critEdit->text()) +"\"\n}\n";
     
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    std::string urlToCall = this->getApiUrl() 
            + "/informations/" + boost::lexical_cast<std::string>(mapIdInformations[idForInfMap])
            + "/criteria/" + boost::lexical_cast<std::string>(idCrit)
            + "/alias" + this->getCredentials();
    Wt::log("debug") << "RoleCustomizationWidget : [PUT] address to call : " << urlToCall;

    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutCritAlias, this, _1, _2, critEdit));
    if (client->post(urlToCall, message))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void RoleCustomizationWidget::displayMessageExample(int idForInfMap, long long idCrit, Wt::WLineEdit *critEdit)
{
    std::string res = "Pour le role : " + rolesComboBox->currentText().toUTF8() + "<br />"
            "Pour le media : " + mediasComboBox->currentText().toUTF8() + "<br />"
            "L'alerte sera : <br />"
            "Serveur %nom_serveur% : <br />"
            "Information : " + mapEditInformations[idForInfMap]->text().toUTF8() + "<br />"
            "Alerte : " + critEdit->text().toUTF8();
    Wt::WString test(res);
    Wt::WMessageBox::show("Exemple", test, Wt::Ok);
}

void RoleCustomizationWidget::fillAssetsFields()
{
    
    for(std::map<int,long long>::const_iterator i = mapIdAssets.begin() ; i != mapIdAssets.end() ; i ++)
    {
        std::string urlToCall = this->getApiUrl()
                + "/assets/" + boost::lexical_cast<std::string>(i->second)
                + "/alias" + this->getCredentials()
                + "&user_role_id=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media_type_id=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);
        Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;

        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, mapEditAssets[i->first]));
        if (client->get(urlToCall))
        {
            Wt::WApplication::instance()->deferRendering();
        } 
        else
        {
            Wt::log("error") << "Error Client Http";
        }
    }
}

void RoleCustomizationWidget::fillCriteriaFields(int idForInfMap, std::map<long long, Wt::WLineEdit*> mapIdCritEdit)
{

    for(std::map<long long, Wt::WLineEdit*>::const_iterator j = mapIdCritEdit.begin() ; j != mapIdCritEdit.end() ; j++)
    {
        std::string urlToCall = this->getApiUrl() 
                + "/informations/" + boost::lexical_cast<std::string>(mapIdInformations[idForInfMap])
                + "/criteria/" + boost::lexical_cast<std::string>(j->first)
                + "/alias" + this->getCredentials()
                +"&user_role_id=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media_type_id=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);
        Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;

        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, j->second));
        if (client->get(urlToCall))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
    }

}

void RoleCustomizationWidget::fillInformationsFields()
{
    for(std::map<int,long long>::const_iterator i = mapIdInformations.begin() ; i != mapIdInformations.end() ; i ++)
    {
        std::string urlToCall = this->getApiUrl() 
                + "/informations/" + boost::lexical_cast<std::string>(i->second)
                + this->getCredentials()
                + "&user_role_id=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media_type_id=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);

        Wt::log("debug") << "RoleCustomizationWidget : [GET] address to call : " << urlToCall;

        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, mapEditInformations[i->first]));
        if (client->get(urlToCall))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
    }
}


void RoleCustomizationWidget::getAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit)
{
    Wt::WApplication::instance()->resumeRendering();
    
    if (!err)
    {
        if (response.status() >= 200 && response.status() < 300)
        {
            Wt::Json::Value result ;
            Wt::Json::Object& result1 = Wt::Json::Object::Empty;
            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;
                std::string res = result1.get("alias");
                edit->setText(res);

            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }         
        }
        else
        {
            Wt::Json::Value result ;
            Wt::Json::Object& result1 = Wt::Json::Object::Empty;
            try
            {                  
                Wt::Json::parse(response.body(), result);
                result1 = result;
                std::string res = result1.get("message");
                edit->setText(res);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[RoleCustomizationWidget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
    }
}


// Todo : gestion d'erreurs
void RoleCustomizationWidget::resPutAssetAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit)
{
    Wt::WApplication::instance()->resumeRendering();
    doJavaScript("$('#" + boost::lexical_cast<std::string>(edit->parent()->id()) + "').toggleClass('success',true)");
    Wt::log("warning") << response.body();
}

void RoleCustomizationWidget::resPutPluginAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit)
{
    Wt::WApplication::instance()->resumeRendering();
    doJavaScript("$('#" + boost::lexical_cast<std::string>(edit->parent()->id()) + "').toggleClass('success',true)");
    Wt::log("warning") << response.body();
}

void RoleCustomizationWidget::resPutCritAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit)
{
    Wt::WApplication::instance()->resumeRendering();
    doJavaScript("$('#" + boost::lexical_cast<std::string>(edit->parent()->id()) + "').toggleClass('success',true)");
    Wt::log("warning") << response.body();
}


