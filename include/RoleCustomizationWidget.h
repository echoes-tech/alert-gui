/* 
 * File:   RoleCustomizationWidget.h
 * Author: tsa
 *
 * Created on 21 avril 2013, 11:41
 */

#ifndef ROLECUSTOMIZATIONWIDGET_H
#define	ROLECUSTOMIZATIONWIDGET_H

#include "GlobalIncludeFile.h"
#include <Wt/WContainerWidget>
#include <Wt/WIOService>

#include <tools/ApiCaller.h>

#include <Wt/Json/Array>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Json/Value>
#include<pthread.h>


class RoleCustomizationWidget : public Wt::WContainerWidget {
public:
    RoleCustomizationWidget(Session *session);
    RoleCustomizationWidget(const RoleCustomizationWidget& orig);
    virtual ~RoleCustomizationWidget();
    std::string getApiUrl() const;
    std::string getCredentials() const;
    
    void getRoles(boost::system::error_code err, const Wt::Http::Message& response);
    void getAssets(boost::system::error_code err, const Wt::Http::Message& response);
    void getMedias(boost::system::error_code err, const Wt::Http::Message& response);
    void getPlugins(boost::system::error_code err, const Wt::Http::Message& response);
    void getInformations(boost::system::error_code err, const Wt::Http::Message& response);
private:
    Session * session;
    bool created_;
    Wt::WTemplate *mainTemplate;
    std::string credentials;
    std::string apiUrl;
//    Wt::Http::Client *client;
    
    Wt::WComboBox *mediasComboBox;
    std::map<int,long long> mapIdMediasComboBox;
    
    Wt::WComboBox *rolesComboBox;
    std::map<int,long long> mapIdRolesComboBox;
    
    Wt::WComboBox *pluginsComboBox;
    std::map<int,long long> mapIdPluginsComboBox;
    
    std::map<int,long long> mapIdAssets;
    std::map<int,Wt::WLineEdit*> mapEditAssets;
    
    std::map<int,long long> mapIdPlugins;
    std::map<int,Wt::WLineEdit*> mapEditPlugins;
    
    std::map<int,InformationId> mapIdInformations;
    std::map<int,Wt::WLineEdit*> mapEditInformations;
    
    Wt::WContainerWidget *assetsContainer;
    Wt::WContainerWidget *pluginsContainer;
    Wt::WContainerWidget *informationsContainer;
    
    Wt::WContainerWidget *pluginRow;
    Wt::WText *pluginName;
    Wt::WLineEdit *pluginEditLine;
    Wt::WPushButton *pluginSaveButton;
    
    pthread_mutex_t callMutex;
    
    void bindWidgets();
    
    void setCredentials(std::string credentials);
    void setApiUrl(std::string apiUrl);
    
    void selectMedia();
    void selectRole();
    void selectPlugin();
    
    void fillMediaSelector();
    void fillRoleSelector();
    void fillPluginSelector();
    
    void createAssetsWidgets();
    void createInformationsWidgets();
    
    

};

#endif	/* ROLECUSTOMIZATIONWIDGET_H */

