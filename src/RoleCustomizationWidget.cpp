/* 
 * File:   RoleCustomizationWidget.cpp
 * Author: tsa
 * 
 * Created on 21 avril 2013, 11:41
 */

#include "RoleCustomizationWidget.h"

RoleCustomizationWidget::RoleCustomizationWidget(Session *session) 
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
    pluginsComboBox->clicked().connect(this, &RoleCustomizationWidget::selectPlugin);
    
 
    assetsContainer = new Wt::WContainerWidget(this);
    assetsContainer->setStyleClass("container-fluid");
    pluginsContainer = new Wt::WContainerWidget(this);
    pluginsContainer->setStyleClass("container-fluid");
    informationsContainer = new Wt::WContainerWidget(this);
    informationsContainer->setStyleClass("container-fluid");
    
    
    pluginRow = new Wt::WContainerWidget(this);
    pluginRow->setStyleClass("row-fluid");
    pluginName = new Wt::WText(this);
    pluginName->setText("ToTr TBD");
    pluginName->setStyleClass("span2");
    pluginEditLine = new Wt::WLineEdit(this);
    pluginEditLine->setStyleClass("span3");
    pluginSaveButton = new Wt::WPushButton(this);
    pluginSaveButton->setText("ToTrSave");
    pluginSaveButton->setStyleClass("span2");
    pluginSaveButton->addStyleClass("btn-info");
    
    pluginRow->addWidget(pluginName);
    pluginRow->addWidget(pluginEditLine);
    pluginRow->addWidget(pluginSaveButton);
    
    pluginsContainer->addWidget(pluginRow);
    
    
    
    std::string apiUrl = "http://127.0.0.1:8081";
    Wt::WApplication::readConfigurationProperty("api-url", apiUrl);
    this->setApiUrl(apiUrl);
    this->setCredentials("?login=thomas.saquet@echoes-tech.com"
            "&token=" + session->user()->token.toUTF8());
    
    
    bindWidgets();
    
    fillMediaSelector();
    fillRoleSelector();
    fillPluginSelector();
    
    createAssetsWidgets();
    createInformationsWidgets();
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

    if (response.status() >= 200 && response.status() < 400)
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
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }

        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
        }          
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponse" << response.body();
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
    if (pluginsComboBox->currentIndex() != -1)
    {
        pluginName->setText(pluginsComboBox->currentText());
        long long pluginId = mapIdPluginsComboBox[pluginsComboBox->currentIndex()];
        std::string urlToCall = this->getApiUrl() 
                + "/plugins/" + boost::lexical_cast<std::string>(pluginId)
                + "/informations";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getInformations, this, _1, _2));

        std::string apiAddress = urlToCall + this->getCredentials();

        std::cout << "address to call : " << apiAddress << std::endl;

        if (client->get(apiAddress))
        {
            Wt::WApplication::instance()->deferRendering();
        } 
        
    }
    
}

void RoleCustomizationWidget::selectMedia()
{
    
}

void RoleCustomizationWidget::selectRole()
{
    
}

void RoleCustomizationWidget::fillMediaSelector()
{
    std::string urlToCall = this->getApiUrl() + "/medias";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getMedias, this, _1, _2));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::cout << "address to call : " << apiAddress << std::endl;
    
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void RoleCustomizationWidget::fillRoleSelector()
{
    std::string urlToCall = this->getApiUrl() + "/organizations/roles";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getRoles, this, _1, _2));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::cout << "address to call : " << apiAddress << std::endl;
    
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void RoleCustomizationWidget::fillPluginSelector()
{
    std::string urlToCall = this->getApiUrl() + "/plugins";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getPlugins, this, _1, _2));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::cout << "address to call : " << apiAddress << std::endl;
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void RoleCustomizationWidget::createAssetsWidgets()
{
    std::string urlToCall = this->getApiUrl() + "/assets";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getAssets, this, _1, _2));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::cout << "address to call : " << apiAddress << std::endl;
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void RoleCustomizationWidget::createInformationsWidgets()
{
    std::string urlToCall1 = this->getApiUrl() + "/plugins/1/informations" + this->getCredentials();
    std::string urlToCall2 = this->getApiUrl() + "/plugins/2/informations" + this->getCredentials();
    std::string urlToCall3 = this->getApiUrl() + "/plugins/3/informations" + this->getCredentials();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::getInformations, this, _1, _2));
    
    if (client->get(urlToCall1))
    {
      Wt::WApplication::instance()->deferRendering();
    } 
//    client->get(urlToCall2);
//    client->get(urlToCall3);
}

void RoleCustomizationWidget::getAssets(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    assetsContainer->clear();
    
    if (response.status() >= 200 && response.status() < 400)
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
                Wt::WText *labelAsset = new Wt::WText();
                labelAsset->setText(tmp.get("name"));
                labelAsset->setStyleClass("span2");
                
                
                Wt::WLineEdit *lineEditAsset = new Wt::WLineEdit();
                lineEditAsset->setStyleClass("span3");
                mapIdAssets[idx] = tmp.get("id");
                mapEditAssets[idx] = lineEditAsset;
                
                Wt::WPushButton *saveButton = new Wt::WPushButton();
                saveButton->setText("ToTrSave");
                saveButton->setStyleClass("span2");
                saveButton->addStyleClass("btn-info");
                
                row->addWidget(labelAsset);
                row->addWidget(lineEditAsset);
                row->addWidget(saveButton);
                assetsContainer->addWidget(row);
                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }

        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
        }          
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponse" << response.body();
    }
}

void RoleCustomizationWidget::getMedias(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    mediasComboBox->clear();
    
    if (response.status() >= 200 && response.status() < 400)
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
                mediasComboBox->addItem(tmp.get("name"));
                mapIdMediasComboBox[idx] = tmp.get("id");
                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }

        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
        }          
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponse" << response.body();
    }
}

void RoleCustomizationWidget::getPlugins(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    pluginsComboBox->clear();
    
    if (response.status() >= 200 && response.status() < 400)
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
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }

        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
        }          
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponse" << response.body();
    }
}

void RoleCustomizationWidget::getInformations(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    informationsContainer->clear();
    
    if (response.status() >= 200 && response.status() < 400)
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
                Wt::WText *label = new Wt::WText();
                label->setText(tmp.get("name"));
                label->setStyleClass("span2");
                
                
                Wt::WLineEdit *lineEdit = new Wt::WLineEdit();
                lineEdit->setStyleClass("span3");
                mapEditPlugins[idx] = lineEdit;
                mapIdPlugins[idx] = tmp.get("id");
                
                Wt::WPushButton *saveButton = new Wt::WPushButton();
                saveButton->setText("ToTrSave");
                saveButton->setStyleClass("span2");
                saveButton->addStyleClass("btn-info");
                
                row->addWidget(label);
                row->addWidget(lineEdit);
                row->addWidget(saveButton);
                informationsContainer->addWidget(row);
                idx++;
            }
        }
        catch (Wt::Json::ParseError const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }

        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
        }          
    }
    else
    {
         Wt::log("warning") << "fct handleHttpResponse" << response.body();
    }
}