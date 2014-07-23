/* 
 * GUI PluginsTableFilterWidget.h
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

#ifndef PLUGINSTABLEFILTERWIDGET_H
#define	PLUGINSTABLEFILTERWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "PluginsTableSearchWidget.h"


class AbstractPage;

class PluginsTableFilterWidget :
public AbstractPage
{    
    struct FilterData
    {
        long long filterTypeID;
        std::map<long long, Wt::WString> parametersValue;
        int nbValue, posKeyValue;
    };
    
public:
                                                PluginsTableFilterWidget(Echoes::Dbo::Session *session, std::string apiUrl,
                                                    PluginsTableSearchWidget* pluginsTableSearchWidget);
    void                                        updatePage();
    
protected:                                 
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    Wt::WString                                 getFilterParameterName(long long searchParameterID);
    Wt::WString                                 getFilterTypeName(long long searchTypeID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long typeID);
    void                                        addResourcePopup(long long filterID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* filterTypeComboBox);    
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long filterID);
    void                                        fillModel();
    void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    PluginsTableSearchWidget*                   m_pluginsTableSearchWidget;
    long long                                   m_selectedSearchID;
    Wt::WStandardItemModel*                     m_filterTypeStandardItemModel;
    std::map<long long, FilterData>             m_filtersData;
};



#endif	/* PLUGINSTABLEFILTERWIDGET_H */

