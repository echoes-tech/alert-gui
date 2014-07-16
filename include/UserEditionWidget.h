/* 
 * Gui UserEditionWidget.h
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 15/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef USEREDITIONWIDGET_H
#define	USEREDITIONWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>

#include "AbstractPage.h"

typedef std::map<long long, std::string>        MapLongString;

class AbstractPage;

class UserEditionWidget :
public AbstractPage
{
public:
    UserEditionWidget(Echoes::Dbo::Session *session, std::string apiUrl, int type);

    Wt::WValidator              *editValidator(int who);
    
    void                        addResource(std::vector<Wt::WInteractWidget*>* argument);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    virtual std::string         addParameter();
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);

protected:
    virtual void                handleJsonGet(vectors_Json jsonResources);

private:

    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    Wt::Json::Value             result_;
    int                         type_;  
    MapLongString               mediasTokens;
    Wt::WStandardItemModel      *usersModel_;
};



#endif	/* USEREDITIONWIDGET_H */

