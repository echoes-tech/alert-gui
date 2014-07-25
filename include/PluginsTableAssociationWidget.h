/* 
 * GUI PluginsTableAssociationWidget.h
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

#ifndef PLUGINSTABLEASSOCIATIONWIDGET_H
#define	PLUGINSTABLEASSOCIATIONWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "PluginsTableFilterWidget.h"

class AbstractPage;

class PluginsTableAssociationWidget :
public AbstractPage
{
    struct SourceData
    {
        long long addonID;
        std::map<long long, Wt::WString> parametersValue;
    };

public:
                                                PluginsTableAssociationWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    PluginsTableFilterWidget* pluginsTableFilterWidget);
    void                                        updatePage();
    
protected:    
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long sourceID);
    void                                        addResourcePopup();
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    void                                        fillHostComboBox(Wt::Json::Value result, boost::function<void()> functorSendRequestInfoAndFilterComboBox,
                                                    Wt::WDialog* dialog);
    void                                        sendRequestInfoAndFilterIndexComboBox(boost::function<void (vectors_Json)> functorFillInfoAndFilterComboBox,
                                                    Wt::WComboBox* assetComboBox);
    void                                        fillInfoAndFilterIndexComboBox(vectors_Json jsonResources, Wt::WDialog* dialog);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    PluginsTableFilterWidget*                   m_pluginsTableFilterWidget;
    long long                                   m_selectedFilterID;
    std::map<long long, SourceData>             m_sourcesData;
    
    Wt::WStandardItemModel*                     m_assetStandardItemModel;
    Wt::WStandardItemModel*                     m_infoStandardItemModel;
    Wt::WStandardItemModel*                     m_filterIndexStandardItemModel;
};



#endif	/* PLUGINSTABLEASSOCIATIONWIDGET_H */

