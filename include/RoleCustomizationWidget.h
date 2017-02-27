/**
 * @file RoleCustomizationWidget.h
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#ifndef ROLECUSTOMIZATIONWIDGET_H
#define	ROLECUSTOMIZATIONWIDGET_H

#include "GlobalIncludeFile.h"
#include <Wt/WContainerWidget>
#include <Wt/WIOService>

#include <Wt/Json/Array>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>
#include <Wt/Json/Value>

#include <Wt/Http/Client>

#include<pthread.h>


class RoleCustomizationWidget : public Wt::WContainerWidget {
public:
    RoleCustomizationWidget(Echoes::Dbo::Session *session, const std::string &apiUrl);
    RoleCustomizationWidget(const RoleCustomizationWidget& orig);
    virtual ~RoleCustomizationWidget();
    std::string getApiUrl() const;
    std::string getCredentials() const;
    
    void getRoles(boost::system::error_code err, const Wt::Http::Message& response);
    void getAssets(boost::system::error_code err, const Wt::Http::Message& response);
//    void getMedias(boost::system::error_code err, const Wt::Http::Message& response);
    void getPlugins(boost::system::error_code err, const Wt::Http::Message& response);
    void getCriteria(int idForInfMap, Wt::WContainerWidget *row);
    void addCriterionRow(int idx, std::string critName, long long critId, Wt::WContainerWidget *row, std::map<long long, Wt::WLineEdit*> *mapIdCritEdit, int idForInfMap);
    void getInformations(boost::system::error_code err, const Wt::Http::Message& response);
    void getAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit);
    void putAssetAlias(int idx);
    void putInformationAlias(int idx);
    void putCriterionAlias(int idForInfMap, long long idCrit, Wt::WLineEdit *critEdit);
    void displayMessageExample(int idForInfMap, long long idCrit, Wt::WLineEdit *critEdit);
    void putPluginAlias();
    void resPutAssetAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit);
    void resPutPluginAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit);
    void resPutCritAlias(boost::system::error_code err, const Wt::Http::Message& response, Wt::WLineEdit *edit);
    
private:
    Echoes::Dbo::Session * session;
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
    std::map<int,Wt::WLineEdit*> mapEditPlugins;
    
    std::map<int,long long> mapIdAssets;
    std::map<int,Wt::WLineEdit*> mapEditAssets;
    
    std::map<int,std::map<long long,Wt::WLineEdit*>> mapEditInformationCriteria;
    
    std::map<int,long long> mapIdInformations;
    std::map<int,Wt::WLineEdit*> mapEditInformations;
    std::map<int,Wt::WContainerWidget*> mapRowCriteriaInformations;
    
    Wt::WContainerWidget *assetsContainer;
    Wt::WContainerWidget *pluginsContainer;
    Wt::WContainerWidget *informationsContainer;
    
    Wt::WContainerWidget *pluginRow;
    Wt::WText *pluginName;
    Wt::WLineEdit *pluginEditLine;
    Wt::WPushButton *pluginSaveButton;
    Wt::WPushButton *pluginLoadButton;
    
    pthread_mutex_t callMutex;
    
    void bindWidgets();
    
    void setCredentials(std::string credentials);
    void setApiUrl(std::string apiUrl);
    
    void selectMedia();
    void selectRole();
    void selectPlugin();
    
    void createAssetsWidgets();
    void createCriteriaWidgets();
    
    void fillMediaSelector();
    void fillRoleSelector();
    void fillPluginSelector();
    
    void fillAssetsFields();
    void fillInformationsFields();
    void fillCriteriaFields(int idForInfMap, std::map<long long, Wt::WLineEdit*> *mapIdCritEdit);
    
    bool mediasSet;
    bool rolesSet;
    
    

};

#endif	/* ROLECUSTOMIZATIONWIDGET_H */
