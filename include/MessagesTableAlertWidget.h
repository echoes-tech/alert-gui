/* 
 * Gui MessagesTableAlertWidget.h
 * 
 * @author ECHOES Technologies (CDE)
 * @author Cyprien DEGEORGE
 * 
 * @date 04/08/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef MESSAGESTABLEALERTWIDGET_H
#define	MESSAGESTABLEALERTWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>

#include <tools/Enums.h>

#include "AbstractPage.h"

class AbstractPage;

class MessagesTableAlertWidget :
public AbstractPage
{
    struct AlertData
    {
        long long alertID;
        Wt::WString name;
        long long stateID;
        Wt::WString user;
    };
    
public:
    MessagesTableAlertWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    void                                        updatePage();
    long long                                   getSelectedAlertID();
    Wt::WString                                 getSelectedAlertName();
protected:
    //virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    std::vector<Wt::WInteractWidget*>           initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    Wt::WString                                 getStateName(long long stateID);
    
    
private:
    void                        takeAssignement(long long id);
    void                        setResolved(long long id);
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    Wt::WStandardItemModel      *m_assetsStandardItemModel;
    Wt::WComboBox               *m_assetComboBox;
    virtual int                 addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable);
    std::map<long long, AlertData>             m_alertsData;
    void                        assignementCallBack(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    void                        resolveCallBack(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    //void fillModel(Wt::Json::Value result);
    //virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    //virtual void setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
};



#endif	/* MESSAGESTABLEALERTWIDGET_H */

