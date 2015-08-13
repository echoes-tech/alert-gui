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

#include "AbstractPage.h"

class AbstractPage;

class MessagesTableAlertWidget :
public AbstractPage
{
public:
    MessagesTableAlertWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    
protected:
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    
private:
    void                        takeAssignement();
    void                        setResolved();
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    Wt::WStandardItemModel      *m_assetsStandardItemModel;
    Wt::WComboBox               *m_assetComboBox;
      virtual int                            addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable);

    void fillModel(Wt::Json::Value result);
    //virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    //virtual void setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
};



#endif	/* MESSAGESTABLEALERTWIDGET_H */

