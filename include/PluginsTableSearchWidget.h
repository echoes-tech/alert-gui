/* 
 * GUI PluginsTableSearchWidget.h
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

#ifndef PLUGINSTABLESEARCHWIDGET_H
#define	PLUGINSTABLESEARCHWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "PluginsTableSourceWidget.h"


class AbstractPage;

class PluginsTableSearchWidget :
public AbstractPage
{    
    struct SearchData
    {
        long long searchTypeID;
        std::map<long long, Wt::WString> parametersValue;
        int period;
    };
    
public:
                                                PluginsTableSearchWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    PluginsTableSourceWidget* pluginsTableSourceWidget);
    void                                        updatePage();
    
protected:    
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        addResourcePopup(long long searchID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long searchID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* searchTypeComboBox);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long searchID);
    void                                        fillModel(Wt::Json::Value result, Wt::WComboBox* searchTypeComboBox, long long searchID,
                                                    boost::function<void (Wt::Json::Value)> functorSendRequestPopupAdd);
    Wt::WString                                 getSearchParameterName(long long searchParameterID);
    Wt::WString                                 getSearchTypeName(long long searchTypeID);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    PluginsTableSourceWidget*                   m_pluginsTableSourceWidget;
    long long                                   m_selectedSourceID;
    Wt::WStandardItemModel*                     m_searchTypeStandardItemModel;
    std::map<long long, SearchData>             m_searchesData;
};



#endif	/* PLUGINSTABLESEARCHWIDGET_H */

