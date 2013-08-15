/* 
 * File:   RoleCustomizationWidget.cpp
 * Author: tsa
 * 
 * Created on 21 avril 2013, 11:41
 */

#include "RoleCustomizationWidget.h"

RoleCustomizationWidget::RoleCustomizationWidget(Session *session, const std::string &apiUrl) 
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
    assetsContainer->setStyleClass("container");
    pluginsContainer = new Wt::WContainerWidget(this);
    pluginsContainer->setStyleClass("container no-padding");
    informationsContainer = new Wt::WContainerWidget(this);
    informationsContainer->setStyleClass("container no-padding");
        
    
    pluginRow = new Wt::WContainerWidget(this);
    pluginRow->setStyleClass("row control-group");
    
    Wt::WContainerWidget *pluginNameContainer = new Wt::WContainerWidget();
    pluginName = new Wt::WText(this);
    pluginName->setText(tr("Alert.admin.plugin-select"));
    pluginNameContainer->setStyleClass("col-lg-2");
    pluginNameContainer->addWidget(pluginName);
    
    Wt::WContainerWidget *pluginEditLineContainer = new Wt::WContainerWidget();
    pluginEditLine = new Wt::WLineEdit(this);
    pluginEditLineContainer->setStyleClass("col-lg-3 control-group");
    pluginEditLineContainer->addWidget(pluginEditLine);
    
    Wt::WContainerWidget *pluginSaveButtonContainer = new Wt::WContainerWidget();
    pluginSaveButton = new Wt::WPushButton(this);
    pluginSaveButton->setTextFormat(Wt::XHTMLUnsafeText);
    pluginSaveButton->setText("<i class='icon-white glyphicon glyphicon-ok-sign'></i>");
    pluginSaveButton->setStyleClass("col-lg-1");
    pluginSaveButton->addStyleClass("btn-primary");
    pluginSaveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putPluginAlias, this));
    pluginSaveButtonContainer->addWidget(pluginSaveButton);
    
    
    pluginLoadButton = new Wt::WPushButton(this);
    pluginLoadButton->setTextFormat(Wt::XHTMLUnsafeText);
    pluginLoadButton->setText("</ br><i class='icon-white glyphicon glyphicon-time'></i> " + tr("Alert.role.load"));
    pluginLoadButton->setStyleClass("btn-success");
    pluginLoadButton->clicked().connect(boost::bind(&RoleCustomizationWidget::selectPlugin, this));
//    pluginLoadButtonContainer->addWidget(pluginLoadButton);
//    pluginLoadButtonContainer->setStyleClass("row");
    
    pluginRow->addWidget(pluginNameContainer);
    pluginRow->addWidget(pluginEditLineContainer);
    pluginRow->addWidget(pluginSaveButtonContainer);
    
    pluginsContainer->addWidget(pluginRow);
    

    this->setApiUrl(apiUrl);
    this->setCredentials("?login=" + session->user()->eMail.toUTF8() +
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
        rolesComboBox->setCurrentIndex(0);
        rolesSet = true;
        if (mediasSet)
        {
            createAssetsWidgets();
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
    mapEditInformations.clear();
    mapIdInformations.clear();
    pluginEditLine->setText("");
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
        
        
        
        std::string urlToCall2 = this->getApiUrl() 
                + "/plugins/" + boost::lexical_cast<std::string>(pluginId)
                + "/alias";
        Wt::Http::Client *client2 = new Wt::Http::Client(this);
        client2->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, pluginEditLine));

        std::string apiAddress2 = urlToCall2 + this->getCredentials();
        
        apiAddress2 += "&role=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);

        std::cout << "address to call : " << apiAddress << std::endl;

        if (client2->get(apiAddress2))
        {
            Wt::WApplication::instance()->deferRendering();
        } 
        
        
        
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

void RoleCustomizationWidget::createCriteriaWidgets()
{
    mapEditInformationCriteria.clear();
//    unsigned int idx = 0;
    for(std::map<int,Wt::Json::Object>::const_iterator i = mapIdInformations.begin() ; i != mapIdInformations.end() ; i ++)
    {
        Wt::Json::Object infIdJson = i->second;
        std::string urlToCall = this->getApiUrl() 
                + "/plugins/" + boost::lexical_cast<std::string>((long long)infIdJson.get("plugin_id"))
                + "/sources/" + boost::lexical_cast<std::string>((long long)infIdJson.get("source_id"))
                + "/searches/" + boost::lexical_cast<std::string>((long long)infIdJson.get("search_id"))
                + "/inf_values/" + boost::lexical_cast<std::string>((long long)infIdJson.get("sub_search_number"))
                + "/units/" + boost::lexical_cast<std::string>((long long)infIdJson.get("unit_id"))
                + "/criteria";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getCriteria, this, _1, _2, i->first, mapRowCriteriaInformations[i->first]));

        std::string apiAddress = urlToCall + this->getCredentials();

        std::cout << "address to call : " << apiAddress << std::endl;

        if (client->get(apiAddress))
        {
            Wt::WApplication::instance()->deferRendering();
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
                row->setStyleClass("row");
                Wt::Json::Object tmp = (*idx1);
                
                Wt::WContainerWidget *labelAssetContainer = new Wt::WContainerWidget();
                labelAssetContainer->setStyleClass("col-lg-2");
                Wt::WText *labelAsset = new Wt::WText();
                labelAsset->setText(tmp.get("name"));
                labelAssetContainer->addWidget(labelAsset);
                
                Wt::WContainerWidget *lineEditAssetContainer = new Wt::WContainerWidget();
                lineEditAssetContainer->setStyleClass("col-lg-3 control-group");
                Wt::WLineEdit *lineEditAsset = new Wt::WLineEdit();
                lineEditAssetContainer->addWidget(lineEditAsset);
                mapIdAssets[idx] = tmp.get("id");
                mapEditAssets[idx] = lineEditAsset;
                
                Wt::WContainerWidget *saveButtonContainer = new Wt::WContainerWidget();
                Wt::WPushButton *saveButton = new Wt::WPushButton();
                saveButton->setTextFormat(Wt::XHTMLUnsafeText);
                saveButton->setText("<i class='icon-white glyphicon glyphicon-ok-sign'></i>");
                saveButton->setStyleClass("col-lg-1");
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
        mediasComboBox->setCurrentIndex(0);
        mediasSet = true;
        if (rolesSet)
        {
            createAssetsWidgets();
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

void RoleCustomizationWidget::getCriteria(boost::system::error_code err, const Wt::Http::Message& response, int idForInfMap, Wt::WContainerWidget *row)
{
    Wt::WApplication::instance()->resumeRendering();
    int idx = 0;
    
    
    if (response.status() >= 200 && response.status() < 400)
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
                labelContainer->setStyleClass("col-lg-2");
                Wt::WText *label = new Wt::WText();
                label->setText(Wt::WString::tr("Alert.alert.operator." + (std::string)tmp.get("name")));
                labelContainer->addWidget(label);
                
                
                Wt::WContainerWidget *lineEditContainer = new Wt::WContainerWidget();
                lineEditContainer->setStyleClass("col-lg-2 control-group");
                Wt::WLineEdit *lineEdit = new Wt::WLineEdit();
                long long critId = tmp.get("id");
                lineEditContainer->addWidget(lineEdit);
                mapIdCritEdit[critId] = lineEdit;
                
                Wt::WContainerWidget *saveButtonContainer = new Wt::WContainerWidget();
                saveButtonContainer->setStyleClass("col-lg-1");
                Wt::WPushButton *saveButton = new Wt::WPushButton();
                saveButton->setTextFormat(Wt::XHTMLUnsafeText);
                saveButton->setText("<i class='icon-white glyphicon glyphicon-ok-sign'></i>");
                saveButton->setStyleClass("col-lg-12");
                saveButton->addStyleClass("btn-primary");
                saveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putCriterionAlias, this, idForInfMap, critId, lineEdit));
                saveButtonContainer->addWidget(saveButton);
                
                Wt::WContainerWidget *exampleButtonContainer = new Wt::WContainerWidget();
                exampleButtonContainer->setStyleClass("col-lg-1");
                Wt::WPushButton *exampleButton = new Wt::WPushButton();
                exampleButton->setTextFormat(Wt::XHTMLUnsafeText);
                exampleButton->setText("<i class='icon-white glyphicon glyphicon-envelope'></i>");
                exampleButton->setStyleClass("col-lg-12");
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
                    changeRow->setText("</div><div class='row'>");
                    row->addWidget(changeRow);
                }
                
                idx++;
            }
            
//            mapEditInformationCriteria[idForInfMap] = mapIdCritEdit;
            fillCriteriaFields(idForInfMap, mapIdCritEdit);
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
                "       <span class='icon'><i class='glyphicon glyphicon-th-list'></i></span>"
                "       <h5>Information : " + (std::string)tmp.get("name") + "</h5>"
                "   </div>"
                "   <div class='widget-content'>"
                "       <div class='container no-padding'>");

                Wt::WText *infoBoxClose = new Wt::WText();
                infoBoxClose->setTextFormat(Wt::XHTMLUnsafeText);
                infoBoxClose->setText(
                "       </div>"
                "   </div>"
                "</div>"
                "<div>");
                informationsContainer->addWidget(infoBoxOpen);
                
                Wt::WContainerWidget *row = new Wt::WContainerWidget();
                row->setStyleClass("row");
                
                Wt::WContainerWidget *labelContainer = new Wt::WContainerWidget();
                Wt::WText *label = new Wt::WText();
                label->setText(tmp.get("name"));
                labelContainer->setStyleClass("col-lg-2");
                labelContainer->addWidget(label);
                
                Wt::WContainerWidget *lineEditContainer = new Wt::WContainerWidget();
                Wt::WLineEdit *lineEdit = new Wt::WLineEdit();
                lineEditContainer->setStyleClass("col-lg-2 control-group");
                lineEditContainer->addWidget(lineEdit);
                
                mapEditInformations[idx] = lineEdit;
                mapIdInformations[idx] = tmp.get("id");
                
                Wt::WContainerWidget *saveButtonContainer = new Wt::WContainerWidget();
                saveButtonContainer->setStyleClass("col-lg-1");
                Wt::WPushButton *saveButton = new Wt::WPushButton();
                saveButton->setTextFormat(Wt::XHTMLUnsafeText);
                saveButton->setText("<i class='icon-white glyphicon glyphicon-ok-sign'></i>");
                saveButton->setStyleClass("col-lg-12");
                saveButton->addStyleClass("btn-primary");
                saveButton->clicked().connect(boost::bind(&RoleCustomizationWidget::putInformationAlias, this, idx));
                saveButtonContainer->addWidget(saveButton);
                
                row->addWidget(labelContainer);
                row->addWidget(lineEditContainer);
                row->addWidget(saveButtonContainer);
                informationsContainer->addWidget(row);
                
                Wt::WContainerWidget *rowCriteria = new Wt::WContainerWidget();
                rowCriteria->setStyleClass("row");
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

void RoleCustomizationWidget::putAssetAlias(int idx)
{
    std::string urlToCall = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string>(mapIdAssets[idx])
                + "/" + "alias";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutAssetAlias, this, _1, _2, mapEditAssets[idx]));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::string strMessage = "{\n"
            "\"alias\" : {\n\
            \"role\" : \""+ boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +"\",\n"
            "\"media\" : \""+ boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +"\",\n"
            "\"value\" : \""+ boost::lexical_cast<std::string>(mapEditAssets[idx]->text()) +"\"\n}\n"
            "}\n";
    
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    if (client->put(apiAddress, message))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void RoleCustomizationWidget::putPluginAlias()
{
    std::string urlToCall = this->getApiUrl() + "/plugins/" + boost::lexical_cast<std::string>(mapIdPluginsComboBox[pluginsComboBox->currentIndex()])
                + "/" + "alias";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutPluginAlias, this, _1, _2, pluginEditLine));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::string strMessage = "{\n"
            "\"alias\" : {\n\
            \"role\" : \""+ boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +"\",\n"
            "\"media\" : \""+ boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +"\",\n"
            "\"value\" : \""+ boost::lexical_cast<std::string>(pluginEditLine->text()) +"\"\n}\n"
            "}\n";
    
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    if (client->put(apiAddress, message))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}


void RoleCustomizationWidget::putInformationAlias(int idx)
{
    Wt::Json::Object infIdJson = mapIdInformations[idx];
    std::string urlToCall = this->getApiUrl() 
            + "/plugins/" + boost::lexical_cast<std::string>(mapIdPluginsComboBox[pluginsComboBox->currentIndex()])
            + "/sources/" + boost::lexical_cast<std::string>((long long)infIdJson.get("source_id"))
            + "/searches/" + boost::lexical_cast<std::string>((long long)infIdJson.get("search_id"))
            + "/inf_values/" + boost::lexical_cast<std::string>((long long)infIdJson.get("sub_search_number"))
            + "/units/" + boost::lexical_cast<std::string>((long long)infIdJson.get("unit_id"))
            + "/alias";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutAssetAlias, this, _1, _2, mapEditInformations[idx]));

    std::string apiAddress = urlToCall + this->getCredentials();
    
    std::cout << apiAddress << std::endl;

    std::string strMessage = "{\n"
            "\"alias\" : {\n\
            \"role\" : \""+ boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +"\",\n"
            "\"media\" : \""+ boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +"\",\n"
            "\"value\" : \""+ boost::lexical_cast<std::string>(mapEditInformations[idx]->text()) +"\"\n}\n"
            "}\n";
    
    Wt::Http::Message message;
    message.addBodyText(strMessage);

    if (client->put(apiAddress, message))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void RoleCustomizationWidget::putCriterionAlias(int idForInfMap, long long idCrit, Wt::WLineEdit *critEdit)
{
    Wt::Json::Object infIdJson = mapIdInformations[idForInfMap];
 
    std::string urlToCall = this->getApiUrl() 
            + "/plugins/" + boost::lexical_cast<std::string>(mapIdPluginsComboBox[pluginsComboBox->currentIndex()])
            + "/sources/" + boost::lexical_cast<std::string>((long long)infIdJson.get("source_id"))
            + "/searches/" + boost::lexical_cast<std::string>((long long)infIdJson.get("search_id"))
            + "/inf_values/" + boost::lexical_cast<std::string>((long long)infIdJson.get("sub_search_number"))
            + "/units/" + boost::lexical_cast<std::string>((long long)infIdJson.get("unit_id"))
            + "/criteria/" + boost::lexical_cast<std::string>(idCrit)
            + "/alias";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&RoleCustomizationWidget::resPutCritAlias, this, _1, _2, critEdit));

    std::string apiAddress = urlToCall + this->getCredentials();

    std::cout << apiAddress << std::endl;

    std::string strMessage = "{\n"
            "\"alias\" : {\n\
            \"role\" : \""+ boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()]) 
            +"\",\n"
            "\"media\" : \""+ boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]) 
            +"\",\n"
            "\"value\" : \""+ boost::lexical_cast<std::string>(critEdit->text()) +"\"\n}\n"
            "}\n";

    Wt::Http::Message message;
    message.addBodyText(strMessage);

    if (client->put(apiAddress, message))
    {
        Wt::WApplication::instance()->deferRendering();
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
        std::string urlToCall = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string>(i->second)
                + "/" + "alias";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, mapEditAssets[i->first]));

        std::string apiAddress = urlToCall + this->getCredentials();
        
        apiAddress += "&role=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);

        std::cout << "address to call : " << apiAddress << std::endl;

        if (client->get(apiAddress))
        {
            Wt::WApplication::instance()->deferRendering();
        } 
    }
}

void RoleCustomizationWidget::fillCriteriaFields(int idForInfMap, std::map<long long, Wt::WLineEdit*> mapIdCritEdit)
{

    for(std::map<long long, Wt::WLineEdit*>::const_iterator j = mapIdCritEdit.begin() ; j != mapIdCritEdit.end() ; j++)
    {
        Wt::Json::Object infIdJson = mapIdInformations[idForInfMap];
        std::string urlToCall = this->getApiUrl() 
            + "/plugins/" + boost::lexical_cast<std::string>((long long)infIdJson.get("plugin_id"))
            + "/sources/" + boost::lexical_cast<std::string>((long long)infIdJson.get("source_id"))
            + "/searches/" + boost::lexical_cast<std::string>((long long)infIdJson.get("search_id"))
            + "/inf_values/" + boost::lexical_cast<std::string>((long long)infIdJson.get("sub_search_number"))
            + "/units/" + boost::lexical_cast<std::string>((long long)infIdJson.get("unit_id"))
            + "/criteria/" + boost::lexical_cast<std::string>(j->first)
            + "/alias";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, j->second));
        std::string apiAddress = urlToCall + this->getCredentials();

        apiAddress += "&role=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);

        std::cout << "address to call : " << apiAddress << std::endl;

        if (client->get(apiAddress))
        {
            Wt::WApplication::instance()->deferRendering();
        } 
    }

}

void RoleCustomizationWidget::fillInformationsFields()
{
    for(std::map<int,Wt::Json::Object>::const_iterator i = mapIdInformations.begin() ; i != mapIdInformations.end() ; i ++)
    {
        Wt::Json::Object infIdJson = i->second;
        std::string urlToCall = this->getApiUrl() 
                + "/plugins/" + boost::lexical_cast<std::string>((long long)infIdJson.get("plugin_id"))
                + "/sources/" + boost::lexical_cast<std::string>((long long)infIdJson.get("source_id"))
                + "/searches/" + boost::lexical_cast<std::string>((long long)infIdJson.get("search_id"))
                + "/inf_values/" + boost::lexical_cast<std::string>((long long)infIdJson.get("sub_search_number"))
                + "/units/" + boost::lexical_cast<std::string>((long long)infIdJson.get("unit_id"))
                + "/alias";
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&RoleCustomizationWidget::getAlias, this, _1, _2, mapEditInformations[i->first]));

        std::string apiAddress = urlToCall + this->getCredentials();
        
        apiAddress += "&role=" + boost::lexical_cast<std::string>(mapIdRolesComboBox[rolesComboBox->currentIndex()])
                +"&media=" + boost::lexical_cast<std::string>(mapIdMediasComboBox[mediasComboBox->currentIndex()]);

        std::cout << "address to call : " << apiAddress << std::endl;

        if (client->get(apiAddress))
        {
            Wt::WApplication::instance()->deferRendering();
        } 
    }
}


void RoleCustomizationWidget::getAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit)
{
    Wt::WApplication::instance()->resumeRendering();
    
    if (response.status() >= 200 && response.status() < 400)
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
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }

        catch (Wt::Json::TypeException const& e)
        {
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
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
            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
        }
//        Wt::log("warning") << "fct handleHttpResponse" << response.body();
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