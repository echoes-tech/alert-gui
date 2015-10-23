/* 
 * GUI MessagesTableHistoricWidget.h
 * 
 * @author ECHOES Technologies (CDE)
 * 
 * @date 06/08/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef MESSAGESTABLEHISTORICWIDGET_H
#define	MESSAGESTABLEHISTORICWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"

#include "MessagesTableMessageWidget.h"

class AbstractPage;

class MessagesTableHistoricWidget :
public AbstractPage
{    
    struct HistoricData
    {
        long long stateID;
        Wt::WString date;
    };
    
public:
    MessagesTableHistoricWidget(Echoes::Dbo::Session *session, std::string apiUrl, MessagesTableMessageWidget* MessageWidget);
    void                                        updatePage();
    long long                                   getSelectedSourceAddonID();
    
protected:    
    //void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    //void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    /*void                                        addResourcePopup(long long searchID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long searchID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* searchTypeComboBox,
                                                    long long searchID);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long searchID);
    void                                        fillModel(Wt::Json::Value result, Wt::WComboBox* searchTypeComboBox, long long searchID,
                                                    boost::function<void (Wt::Json::Value)> functorHandleRequestPopupAdd);
    Wt::WString                                 getSearchParameterName(long long searchParameterID);
    */
    Wt::WContainerWidget*                      createTableFirstHeader();
    Wt::WString                                 getStateName(long long searchTypeID);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    MessagesTableMessageWidget*                               m_messagesTableMessageWidget;
    long long                                   m_selectedSourceID;
    Wt::WStandardItemModel*                     m_searchTypeStandardItemModel;
    std::map<long long, HistoricData>             m_messageHistoricData;
};



#endif	/* MESSAGESTABLEHISTORICWIDGET_H */

