/* 
 * GUI MessagesTableMessageWidget.h
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 05/08/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef MESSAGESTABLEMESSAGEWIDGET_H
#define	MESSAGESTABLEMESSAGEWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"

class AbstractPage;

class MessagesTableMessageWidget :
public AbstractPage
{
    struct MessageData
    {
        long long messageID;
        long long stateID;
        Wt::WString date;
        Wt::WString dest;
    };

public:
    MessagesTableMessageWidget(Echoes::Dbo::Session *session, std::string apiUrl, AbstractPage* abstractPage);
    void                                        updatePage();
    long long                                   getSelectedMessageAddonID();
    
protected:    
    //void                                        setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    //void                                        setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    /*void                                        addResourcePopup(long long sourceID);
    void                                        sendRequestPopupAdd(boost::function<void (Wt::Json::Value)> functor, Wt::WComboBox* addonComboBox,
                                                    long long sourceID);
    void                                        addPopupAddHandler(Wt::WInteractWidget* widget);
    void                                        addPopupModifHandler(Wt::WInteractWidget* widget, long long sourceID);
    void                                        handleRequestPopupAdd(Wt::Json::Value result, Wt::WContainerWidget* paramsContainer,
                                                    std::vector<Wt::WInteractWidget*>* inputName, long long sourceID);
    Wt::WString                                 getSourceParameterName(long long sourceParameterID);
    */Wt::WString                                 getStateName(long long stateID);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    
private:
    Echoes::Dbo::Session                        *session_;
    std::string                                 apiUrl_;
    AbstractPage*                               m_messagesTableAlertWidget;
    long long                                   m_selectedAlertID;
    Wt::WStandardItemModel*                     m_addonStandardItemModel;
    std::map<long long, MessageData>             m_messagesData;
};



#endif	/* MESSAGESTABLEMESSAGEWIDGET_H */

