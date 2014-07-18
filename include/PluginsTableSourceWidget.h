/* 
 * GUI PluginsTableSourceWidget.h
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 11/07/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef PLUGINSTABLESOURCEWIDGET_H
#define	PLUGINSTABLESOURCEWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"

class AbstractPage;

class PluginsTableSourceWidget :
public AbstractPage
{
    struct SourceData
    {
        long long addonID;
        std::map<long long, Wt::WString> parametersValue;
    };

public:
                                                PluginsTableSourceWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    AbstractPage* abstractPage);
    void                                        updatePage();
    
protected:    
    std::string                                 addParameter();
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        addResourcePopup(long long sourceID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long sourceID);
    void                                        sendRequestPopupAdd(Wt::WComboBox* addonComboBox, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long sourceID);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long sourceID);
    void                                        fillModel();
    Wt::WString                                 getSourceParameterName(long long sourceParameterID);
    Wt::WString                                 getAddonName(long long addonID);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    AbstractPage*                               m_pluginsTablePluginWidget;
    long long                                   m_selectedPluginID;
    Wt::WStandardItemModel*                     m_addonStandardItemModel;
    std::map<long long, SourceData>             m_sourcesData;
};



#endif	/* PLUGINSTABLESOURCEWIDGET_H */

